# =============================================================================
#  struct_updater.py
#  IDA Pro 7.0+ IDAPython Plugin — Struct Updater from Pseudocode
#
#  Compatible with Python 2.7 (IDA 7.0) and Python 3.x (IDA 7.4+).
#
#  Installation:
#    Copy this file into  <IDA install dir>/plugins/
#    Restart IDA (or use File > Script File once to load it on the fly).
#
#  Invocation:
#    Edit > Plugins > Struct Updater   (hotkey: Ctrl-Alt-U)
#
#  What it does:
#    1. Reads the first non-empty line and extracts the class name
#       (identifier before "::").
#    2. Finds or creates an IDA struct with that name.
#       A freshly-created struct gets a "base" member at offset 0
#       with type CXgFMPanel.
#    3. Scans every subsequent line for two patterns:
#         a) CXgFMPanel::GetXxx(...)  -> member  m_p<n>  type  CXg<Xxx> *
#         b) CFMListBox::Create(...)  -> member  m_<n>   type  CFMListBox
#    4. If the existing member name already starts with "m_" the name is
#       kept; only the type is (re)applied.
#    5. If a quoted string contains "|" (pipe) the LAST segment is used
#       as the member name suffix — e.g. "Stadium|StadiumImage|ImgStadium"
#       gives ImgStadium.
#
#  Input source (tried in order):
#    a) The currently focused Hex-Rays pseudocode window (automatic).
#    b) A multi-line paste dialog (fallback when no decompiler view is
#       active or Hex-Rays is not available).
#
#  Debug tip:
#    Set DEBUG = True below to dump the raw pseudocode lines that reach
#    the parser — useful when "no members found" is reported.
# =============================================================================

from __future__ import print_function   # Py2/3 neutral print()

import re
import idaapi
import idc
import ida_kernwin
import ida_bytes

# Set to True to print every pseudocode line as it is scanned.
DEBUG = True

try:
    import ida_hexrays
    _HAS_HEXRAYS = True
except ImportError:
    _HAS_HEXRAYS = False

# ── Plugin metadata ────────────────────────────────────────────────────────────
_PLUGIN_NAME    = "Struct Updater"
_PLUGIN_HOTKEY  = "Ctrl-Alt-U"
_PLUGIN_COMMENT = "Update IDA struct members from Hex-Rays pseudocode"


# =============================================================================
#  STRING / EXTRACTION HELPERS
# =============================================================================

def _extract_last_string(line):
    """Return the text between the *last* pair of double-quotes on a line."""
    matches = re.findall(r'"([^"]*)"', line)
    return matches[-1] if matches else ""


def _pick_last_pipe(name):
    """
    If *name* contains '|' return the last '|'-delimited segment;
    otherwise return *name* unchanged.
      "Stadium|StadiumImage|ImgStadium"  ->  "ImgStadium"
      "TbRating"                          ->  "TbRating"
    """
    if "|" in name:
        return name.rsplit("|", 1)[-1]
    return name


def _extract_class_name(line):
    """Return the identifier immediately before '::' on the first code line."""
    m = re.search(r'(\w+)::', line)
    return m.group(1) if m else ""


def _extract_get_type(line):
    """Extract 'Xxx' from a 'CXgFMPanel::GetXxx(' token."""
    m = re.search(r'CXgFMPanel::Get(\w+)\s*\(', line)
    return m.group(1) if m else ""


def _extract_offset(line, cfm=False):
    """
    Extract the numeric struct offset from a pseudocode line.

    Priority (earliest character position wins):
      ->field_HEX        bare hex after the marker
      ->dwordHEX         bare hex after the marker
      ->cfmlistboxHEX    bare hex (only when cfm=True)
      + DEC / + 0xHEX   decimal or 0x-prefixed hex after '+'

    Returns the integer offset, or -1 if nothing is found.
    """
    candidates = []   # list of (match_start, value)

    def _add(pattern, base):
        for m in re.finditer(pattern, line):
            candidates.append((m.start(), int(m.group(1), base)))

    _add(r'->field_([0-9A-Fa-f]+)',  16)
    _add(r'->dword([0-9A-Fa-f]+)',   16)
    if cfm:
        _add(r'->cfmlistbox([0-9A-Fa-f]+)', 16)

    # "+ 0xHEX"  or  "+ DEC"
    for m in re.finditer(r'\+\s*(0[xX][0-9A-Fa-f]+|\d+)', line):
        raw = m.group(1)
        val = int(raw, 16) if raw.lower().startswith("0x") else int(raw)
        candidates.append((m.start(), val))

    if not candidates:
        return -1
    candidates.sort(key=lambda t: t[0])
    return candidates[0][1]


# =============================================================================
#  IDA COLOUR-TAG REMOVAL
# =============================================================================

def _strip_tags(line):
    """
    Remove IDA colour-tag escape sequences from *line* so that plain-text
    regex matching works correctly.

    IDA embeds colour codes as  0x01 <colour-byte> ... 0x02  pairs.
    We try idaapi.tag_remove() first (the official API); if that is absent
    or returns an unexpected type we fall back to a regex that strips all
    such escape bytes directly.
    """
    # Official API path (works in IDA 7.x / IDAPython 2 & 3)
    try:
        result = idaapi.tag_remove(line)
        # In some builds tag_remove() returns the length rather than the
        # cleaned string — detect that and fall through to the manual path.
        if isinstance(result, str) and result != line:
            return result
        if isinstance(result, bytes):
            try:
                return result.decode("utf-8", errors="replace")
            except Exception:
                pass
    except Exception:
        pass

    # Manual fallback: strip  0x01 XX  ...  0x02  escape sequences.
    # Also strip any stray 0x01 / 0x02 bytes left over.
    cleaned = re.sub(r'\x01.\x02', '', line)      # colour spans
    cleaned = re.sub(r'[\x01\x02]', '', cleaned)  # lone delimiters
    return cleaned


# =============================================================================
#  IDA STRUCT OPERATIONS
# =============================================================================

def _set_member_type(sid, offset, type_str):
    """Apply a C-type string to the struct member at (sid, offset)."""
    mid = idc.get_member_id(sid, offset)
    if mid == idc.BADADDR:
        print("    [!] get_member_id failed at offset 0x%X" % offset)
        return
    if idc.SetType(mid, type_str):
        print("    [T] Type '%s' applied at offset 0x%X" % (type_str, offset))
    else:
        print("    [!] SetType('%s') failed at offset 0x%X" % (type_str, offset))


def _ensure_struct(name):
    """
    Return the SID for *name*, creating the struct (with a 'base' member)
    if it does not yet exist.  Returns idc.BADADDR on failure.
    """
    sid = idc.get_struc_id(name)
    if sid != idc.BADADDR:
        print("[*] Struct '%s' already exists  (id=0x%X)" % (name, sid))
        return sid

    sid = idc.add_struc(-1, name, 0)
    if sid == idc.BADADDR:
        idaapi.warning("Struct Updater: failed to create struct '%s'!" % name)
        return idc.BADADDR
    print("[+] Created new struct '%s'  (id=0x%X)" % (name, sid))

    # ── Add "base" member at offset 0 ──────────────────────────────────────
    base_sid = idc.get_struc_id("CXgFMPanel")
    r = -1

    if base_sid != idc.BADADDR and base_sid != -1:
        base_size = idc.get_struc_size(base_sid)
        r = idc.add_struc_member(
            sid, "base", 0,
            ida_bytes.FF_DATA | ida_bytes.FF_STRUCT,
            base_sid, base_size,
        )
    else:
        r = idc.add_struc_member(
            sid, "base", 0,
            ida_bytes.FF_DATA | ida_bytes.FF_DWORD,
            -1, 4,
        )

    if r == 0:
        if base_sid == idc.BADADDR or base_sid == -1:
            _set_member_type(sid, 0, "CXgFMPanel")
        print("    [+] Added 'base' member at offset 0  (type CXgFMPanel)")
    else:
        print("    [!] Failed to add 'base' member  (AddStrucMember code %d)" % r)

    return sid


def _apply_update(sid, offset, new_name, type_name, is_pointer):
    """
    Create or update the struct member at (sid, offset).

    Rules
    -----
    Member absent  ->  create it, then apply the type.
    Member present ->  rename only when the current name does NOT start
                       with "m_"; always (re)apply the type.
    """
    existing = idc.get_member_name(sid, offset) or ""
    exists   = bool(existing)

    if not exists:
        # ── Create ────────────────────────────────────────────────────────
        add_ok = False
        r      = -1

        if not is_pointer:
            emb_sid = idc.get_struc_id(type_name)
            if emb_sid not in (idc.BADADDR, -1):
                emb_size = idc.get_struc_size(emb_sid)
                r = idc.add_struc_member(
                    sid, new_name, offset,
                    ida_bytes.FF_DATA | ida_bytes.FF_STRUCT,
                    emb_sid, emb_size,
                )
                add_ok = (r == 0)

        if not add_ok:
            r = idc.add_struc_member(
                sid, new_name, offset,
                ida_bytes.FF_DATA | ida_bytes.FF_DWORD,
                -1, 4,
            )
            add_ok = (r == 0)

        if not add_ok:
            print("    [!] Failed to create member '%s' at 0x%X  (code %d)"
                  % (new_name, offset, r))
            return

        print("    [+] Created member '%s' at offset 0x%X" % (new_name, offset))

    else:
        # ── Conditionally rename ───────────────────────────────────────────
        if not existing.startswith("m_"):
            if idc.set_member_name(sid, offset, new_name):
                print("    [~] Renamed '%s' -> '%s' at offset 0x%X"
                      % (existing, new_name, offset))
            else:
                print("    [!] Rename failed at offset 0x%X" % offset)
        else:
            print("    [=] Keeping '%s' (m_ prefix present), only updating type"
                  % existing)

    # ── Apply type ────────────────────────────────────────────────────────
    type_str = ("CXg" + type_name + " *") if is_pointer else type_name
    _set_member_type(sid, offset, type_str)


# =============================================================================
#  LINE PARSING
# =============================================================================

def _process_line(line, sid):
    """
    Inspect one stripped pseudocode line (colour tags already removed).
    Recognises the two supported patterns and calls _apply_update.
    """
    line = line.strip()
    if not line or '"' not in line:
        return

    is_cfm = "CFMListBox::Create" in line
    is_get = "CXgFMPanel::Get"    in line

    if not is_cfm and not is_get:
        return

    # ── Member name (last quoted literal, pipe-resolved) ──────────────────
    raw_name = _extract_last_string(line)
    if not raw_name:
        print("  [?] Member name not found in: %.80s" % line)
        return
    member_name = _pick_last_pipe(raw_name)

    # ── Numeric offset ────────────────────────────────────────────────────
    offset = _extract_offset(line, cfm=is_cfm)
    if offset == -1:
        print("  [?] Offset not found in: %.80s" % line)
        return

    if is_get:
        type_name = _extract_get_type(line)
        if not type_name:
            print("  [?] Get-type not found in: %.80s" % line)
            return
        print("  >> CXgFMPanel::Get%s  offset=0x%X  member='m_p%s'  type='CXg%s *'"
              % (type_name, offset, member_name, type_name))
        _apply_update(sid, offset, "m_p" + member_name, type_name, True)

    else:   # CFMListBox::Create
        print("  >> CFMListBox::Create  offset=0x%X  member='m_%s'  type='CFMListBox'"
              % (offset, member_name))
        _apply_update(sid, offset, "m_" + member_name, "CFMListBox", False)


# =============================================================================
#  TOP-LEVEL TEXT PROCESSING
# =============================================================================

def process_text(text):
    """
    Drive the whole update from a multi-line string of pseudocode.
    The first non-empty line must contain the 'ClassName::' signature.
    Colour tags are stripped from every line before processing.
    """
    sid         = idc.BADADDR
    struct_name = ""
    first_line  = True

    for raw_line in text.splitlines():
        clean = _strip_tags(raw_line).strip()

        if DEBUG:
            print("  [DBG] %r" % clean)

        if not clean:
            continue

        if first_line:
            struct_name = _extract_class_name(clean)
            if not struct_name:
                print("[!] Cannot extract class name from:\n    %s" % clean)
                return
            print("[*] Class name: '%s'" % struct_name)
            sid = _ensure_struct(struct_name)
            if sid == idc.BADADDR:
                return
            first_line = False
        else:
            if sid != idc.BADADDR:
                _process_line(clean, sid)

    if struct_name:
        print("[*] Processing complete. Struct '%s' updated." % struct_name)


# =============================================================================
#  PSEUDOCODE RETRIEVAL FROM ACTIVE DECOMPILER WINDOW
# =============================================================================

def _get_current_pseudocode():
    """
    Return the raw (colour-tagged) text of the currently focused Hex-Rays
    pseudocode view, or None if the active widget is not a decompiler view
    (or Hex-Rays is unavailable).

    Tags are intentionally preserved here; process_text() strips them via
    _strip_tags() so the same path handles pasted text too.
    """
    if not _HAS_HEXRAYS:
        return None
    try:
        widget = ida_kernwin.get_current_widget()
        if widget is None:
            return None
        vu = ida_hexrays.get_widget_vdui(widget)
        if vu is None:
            return None

        # Ensure the pseudocode is up to date.
        vu.cfunc.refresh_func_ctext()

        sv    = vu.cfunc.get_pseudocode()
        lines = []
        for i in range(len(sv)):
            lines.append(sv[i].line)   # raw tagged line

        return "\n".join(lines)

    except Exception as exc:
        print("[Struct Updater] Could not read pseudocode view: %s" % exc)
        return None


# =============================================================================
#  PLUGIN CLASS
# =============================================================================

class _StructUpdaterPlugin(idaapi.plugin_t):
    flags         = idaapi.PLUGIN_UNL
    comment       = _PLUGIN_COMMENT
    help          = _PLUGIN_COMMENT
    wanted_name   = _PLUGIN_NAME
    wanted_hotkey = _PLUGIN_HOTKEY

    def init(self):
        print("[Struct Updater] Plugin loaded — "
              "Edit > Plugins > %s  (%s)" % (_PLUGIN_NAME, _PLUGIN_HOTKEY))
        return idaapi.PLUGIN_OK

    def run(self, _arg):
        print()
        print("=" * 57)
        print("  %s" % _PLUGIN_NAME)
        print("=" * 57)

        # 1 — Try the active Hex-Rays pseudocode window first.
        text   = _get_current_pseudocode()
        source = "Hex-Rays pseudocode window"

        # 2 — Fall back to a multi-line paste dialog.
        if not text:
            text = ida_kernwin.ask_text(
                0,    # max_size; 0 = unlimited
                "",   # default value
                "%s: paste decompiled function pseudocode below:" % _PLUGIN_NAME,
            )
            source = "pasted text"

        if not text or not text.strip():
            print("No input — aborted.")
            return

        print("[*] Source : %s" % source)
        if DEBUG:
            print("[DBG] First 300 chars of raw input:\n%r\n" % text[:300])
        print()
        process_text(text)
        print()
        print("=" * 57)
        print("  Done.")
        print("=" * 57)
        print()

    def term(self):
        pass


# =============================================================================
#  REQUIRED ENTRY POINT
# =============================================================================

def PLUGIN_ENTRY():
    return _StructUpdaterPlugin()
