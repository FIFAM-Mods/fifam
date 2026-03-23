// ============================================================
//  struct_updater.idc
//  IDA Pro 7.0 IDC Plugin — Struct Updater from Pseudocode
//
//  Usage:
//    File -> Script File -> select this .idc
//    When prompted, select the .txt / .c / .cpp file that
//    contains the decompiled function body.
//
//  What it does:
//    1. Reads the first non-empty line and extracts the class
//       name (the token before "::").
//    2. Finds or creates a struct with that name in IDA.
//       A freshly-created struct gets a "base" member at
//       offset 0 with type CXgFMPanel.
//    3. Scans every subsequent line for two patterns:
//       a) CXgFMPanel::GetXxx(...)  -> adds/updates a 4-byte
//          pointer member named m_p<Name>, type CXg<Xxx> *
//       b) CFMListBox::Create(...)  -> adds/updates an
//          embedded struct member named m_<Name>,
//          type CFMListBox
//    4. If the existing member name already starts with "m_"
//       the name is preserved; only the type is (re)applied.
//
//  Offset extraction supports all IDA pseudocode variants:
//    *(_DWORD *)(var + DEC)          plain decimal
//    *(_DWORD *)(var + 0xHEX)        hex with 0x prefix
//    var->field_HEX                  bare-hex IDA field name
//    var->dwordHEX                   bare-hex IDA dword name
//    var->cfmlistboxHEX              bare-hex CFMListBox name
// ============================================================

#include <idc.idc>

// ============================================================
//  STRING UTILITIES
// ============================================================

// Remove leading and trailing whitespace (space, tab, CR, LF).
static Trim(s) {
    auto i, j, len, ch;
    len = strlen(s);
    i = 0;
    while (i < len) {
        ch = substr(s, i, i + 1);
        if (ch != " " && ch != "\t" && ch != "\r" && ch != "\n") break;
        i++;
    }
    j = len;
    while (j > i) {
        ch = substr(s, j - 1, j);
        if (ch != " " && ch != "\t" && ch != "\r" && ch != "\n") break;
        j--;
    }
    if (i >= j) return "";
    return substr(s, i, j);
}

// Return 1 if ch (single-char string) is a decimal digit.
static IsDigit(ch) {
    return ch=="0"||ch=="1"||ch=="2"||ch=="3"||ch=="4"||
           ch=="5"||ch=="6"||ch=="7"||ch=="8"||ch=="9";
}

// Return 1 if ch is a hexadecimal digit (upper or lower case).
static IsHexDigit(ch) {
    return IsDigit(ch) ||
           ch=="a"||ch=="b"||ch=="c"||ch=="d"||ch=="e"||ch=="f"||
           ch=="A"||ch=="B"||ch=="C"||ch=="D"||ch=="E"||ch=="F";
}

// Return the integer value 0-15 for a single hex-digit char, or -1.
static HexCharVal(ch) {
    if (ch >= "0" && ch <= "9") return atol(ch);
    if (ch == "a" || ch == "A") return 10;
    if (ch == "b" || ch == "B") return 11;
    if (ch == "c" || ch == "C") return 12;
    if (ch == "d" || ch == "D") return 13;
    if (ch == "e" || ch == "E") return 14;
    if (ch == "f" || ch == "F") return 15;
    return -1;
}

// Convert a bare hex string (no "0x" prefix) to an integer.
// Stops at the first non-hex character.
static XtoL(s) {
    auto r, i, v;
    r = 0;
    for (i = 0; i < strlen(s); i++) {
        v = HexCharVal(substr(s, i, i + 1));
        if (v < 0) break;
        r = r * 16 + v;
    }
    return r;
}

// Read a run of hexadecimal characters starting at startPos.
// Returns the digit string (may be empty).
static ReadHexSeq(line, startPos) {
    auto r, i, len;
    r = ""; i = startPos; len = strlen(line);
    while (i < len && IsHexDigit(substr(line, i, i + 1))) {
        r = r + substr(line, i, i + 1);
        i++;
    }
    return r;
}

// Read a run of decimal characters starting at startPos.
static ReadDecSeq(line, startPos) {
    auto r, i, len;
    r = ""; i = startPos; len = strlen(line);
    while (i < len && IsDigit(substr(line, i, i + 1))) {
        r = r + substr(line, i, i + 1);
        i++;
    }
    return r;
}

// Advance pos past any space characters; return updated pos.
static SkipSpaces(line, pos) {
    auto len;
    len = strlen(line);
    while (pos < len && substr(line, pos, pos + 1) == " ") pos++;
    return pos;
}

// ============================================================
//  EXTRACTION HELPERS
// ============================================================

// Return the text between the last pair of double-quotes on
// a line, or "" if none found.
//   "foo bar, \"TbRating\")"  ->  "TbRating"
static ExtractLastString(line) {
    auto len, closePos, i;
    len = strlen(line);
    closePos = -1;
    for (i = len - 1; i >= 0; i--) {
        if (substr(line, i, i + 1) == "\"") { closePos = i; break; }
    }
    if (closePos < 1) return "";
    for (i = closePos - 1; i >= 0; i--) {
        if (substr(line, i, i + 1) == "\"")
            return substr(line, i + 1, closePos);
    }
    return "";
}

// Return the identifier that precedes the first "::" on a line.
// Handles return types, calling conventions, and cast operators.
//   "void __thiscall CMyClass::Foo(..."  ->  "CMyClass"
static ExtractClassName(line) {
    auto pos, i, ch;
    pos = strstr(line, "::");
    if (pos == -1) return "";
    // Walk backwards until we hit a non-identifier character.
    i = pos - 1;
    while (i >= 0) {
        ch = substr(line, i, i + 1);
        if (ch==" "||ch=="\t"||ch=="*"||ch=="("||ch=="&"||ch==",") break;
        i--;
    }
    i++;  // step past the delimiter
    if (i >= pos) return "";
    return substr(line, i, pos);
}

// Extract the "Xxx" from "CXgFMPanel::GetXxx(".
// Returns "" on failure.
static ExtractGetType(line) {
    auto TAG, pos, after, rest, ppos;
    TAG = "CXgFMPanel::Get";
    pos = strstr(line, TAG);
    if (pos == -1) return "";
    after = pos + strlen(TAG);           // points just past "Get"
    rest  = substr(line, after, strlen(line));
    ppos  = strstr(rest, "(");           // distance to first "("
    if (ppos == -1) return "";
    return substr(line, after, after + ppos);
}

// Extract the numeric struct offset from a line.
//
// Priority order (earliest position wins):
//   "->field_HEX"     — bare hex follows the marker
//   "->dwordHEX"      — bare hex follows the marker
//   "->cfmlistboxHEX" — bare hex follows the marker (cfm=1 only)
//   "+ DEC/0xHEX"     — decimal or 0x-prefixed hex follows "+"
//
// Returns the offset as an integer, or -1 on failure.
static ExtractOffset(line, cfm) {
    auto fP, dP, cP, pP;
    auto best, type, numStart, pfx, h, d;

    fP = strstr(line, "->field_");
    dP = strstr(line, "->dword");
    cP = cfm ? strstr(line, "->cfmlistbox") : -1;
    pP = strstr(line, "+");

    best = -1; type = "";

    if (fP != -1 && (best == -1 || fP < best)) { best = fP; type = "field"; }
    if (dP != -1 && (best == -1 || dP < best)) { best = dP; type = "dword"; }
    if (cP != -1 && (best == -1 || cP < best)) { best = cP; type = "cfmlistbox"; }
    if (pP != -1 && (best == -1 || pP < best)) { best = pP; type = "plus"; }

    if (best == -1) return -1;

    // Compute start of the number token.
    //   strlen("->field_")    = 8
    //   strlen("->dword")     = 7
    //   strlen("->cfmlistbox")= 12
    if      (type == "field")      numStart = best + 8;
    else if (type == "dword")      numStart = best + 7;
    else if (type == "cfmlistbox") numStart = best + 12;
    else                           numStart = best + 1;   // past "+"

    numStart = SkipSpaces(line, numStart);
    if (numStart >= strlen(line)) return -1;

    // 0x / 0X prefix -> hex regardless of origin marker
    if (numStart + 1 < strlen(line)) {
        pfx = substr(line, numStart, numStart + 2);
        if (pfx == "0x" || pfx == "0X") {
            h = ReadHexSeq(line, numStart + 2);
            return strlen(h) > 0 ? XtoL(h) : -1;
        }
    }

    // field/dword/cfmlistbox always use bare hex (no prefix)
    if (type != "plus") {
        h = ReadHexSeq(line, numStart);
        return strlen(h) > 0 ? XtoL(h) : -1;
    }

    // "+" variant: the literal after + is decimal
    d = ReadDecSeq(line, numStart);
    return strlen(d) > 0 ? atol(d) : -1;
}

// ============================================================
//  IDA STRUCT OPERATIONS
// ============================================================

// Apply a C-type string to the struct member at (sid, offset).
// Reports success or failure to the Output window.
static SetMemberTypeStr(sid, offset, typeStr) {
    auto membId;
    membId = GetMemberId(sid, offset);
    if (membId == BADADDR) {
        Message("    [!] GetMemberId failed at offset 0x%X\n", offset);
        return;
    }
    if (SetType(membId, typeStr))
        Message("    [T] Type '%s' applied at offset 0x%X\n", typeStr, offset);
    else
        Message("    [!] SetType('%s') failed at offset 0x%X\n", typeStr, offset);
}

// Find struct by name; if absent, create it and add a "base"
// member at offset 0 with type CXgFMPanel.
// Returns the struct ID (sid), or BADADDR on error.
static EnsureStruct(name) {
    auto sid, baseSid, r;

    sid = GetStrucIdByName(name);
    if (sid != BADADDR) {
        Message("[*] Struct '%s' already exists (id=0x%X)\n", name, sid);
        return sid;
    }

    sid = AddStrucEx(-1, name, 0);
    if (sid == BADADDR) {
        Warning("Failed to create struct '%s'!", name);
        return BADADDR;
    }
    Message("[+] Created new struct '%s' (id=0x%X)\n", name, sid);

    // Add "base" member at offset 0.
    // If CXgFMPanel is already defined in IDA, embed it as a
    // proper struct member so the size is tracked correctly.
    // Otherwise add a DWORD placeholder and set the type string.
    baseSid = GetStrucIdByName("CXgFMPanel");
    if (baseSid != BADADDR && baseSid != -1) {
        r = AddStrucMember(sid, "base", 0,
                           FF_DATA | FF_STRU, baseSid, GetStrucSize(baseSid));
    } else {
        r = AddStrucMember(sid, "base", 0, FF_DATA | FF_DWRD, -1, 4);
    }

    if (r == 0) {
        // If CXgFMPanel wasn't found, we still want the type annotation.
        if (baseSid == BADADDR || baseSid == -1)
            SetMemberTypeStr(sid, 0, "CXgFMPanel");
        Message("    [+] Added 'base' member at offset 0 (type CXgFMPanel)\n");
    } else {
        Message("    [!] Failed to add 'base' member (AddStrucMember code %d)\n", r);
    }

    return sid;
}

// Create or update the struct member at (sid, offset).
//
//   newName   – desired member name (e.g. "m_pTbRating")
//   typeName  – base type identifier:
//                 isPointer=1 -> "TextBox"   => type  "CXgTextBox *"
//                 isPointer=0 -> "CFMListBox"=> type  "CFMListBox"
//   isPointer – 1 for CXg* pointer, 0 for embedded struct
//
// Rules:
//   • If member absent   -> create it, then apply type.
//   • If member present  -> rename only if current name does
//                           NOT start with "m_"; always apply type.
static ApplyUpdate(sid, offset, newName, typeName, isPointer) {
    auto existing, exists, addOk, typeStr, embSid, r;

    existing = GetMemberName(sid, offset);
    exists   = (strlen(existing) > 0);

    if (!exists) {
        // ---- Create ----
        addOk = 0;

        // For embedded structs try to use FF_STRU so IDA tracks
        // the size correctly (important for CFMListBox).
        if (!isPointer) {
            embSid = GetStrucIdByName(typeName);
            if (embSid != BADADDR && embSid != -1) {
                r = AddStrucMember(sid, newName, offset,
                                   FF_DATA | FF_STRU, embSid, GetStrucSize(embSid));
                addOk = (r == 0);
            }
        }

        // Fall back to a plain 4-byte DWORD slot for pointers
        // or when the embedded type is not yet defined.
        if (!addOk) {
            r = AddStrucMember(sid, newName, offset, FF_DATA | FF_DWRD, -1, 4);
            addOk = (r == 0);
        }

        if (!addOk) {
            Message("    [!] Failed to create member '%s' at 0x%X (code %d)\n",
                    newName, offset, r);
            return;
        }
        Message("    [+] Created member '%s' at offset 0x%X\n", newName, offset);

    } else {
        // ---- Update name (conditionally) ----
        if (strlen(existing) < 2 || substr(existing, 0, 2) != "m_") {
            if (SetMemberName(sid, offset, newName))
                Message("    [~] Renamed '%s' -> '%s' at offset 0x%X\n",
                        existing, newName, offset);
            else
                Message("    [!] Rename failed at offset 0x%X\n", offset);
        } else {
            Message("    [=] Keeping '%s' (m_ prefix present), only updating type\n",
                    existing);
        }
    }

    // ---- Apply type annotation ----
    typeStr = isPointer ? ("CXg" + typeName + " *") : typeName;
    SetMemberTypeStr(sid, offset, typeStr);
}

// ============================================================
//  LINE PARSING
// ============================================================

// Inspect one (already-trimmed) line of pseudocode.
// Detects the two supported patterns and calls ApplyUpdate.
static ProcessLine(line, sid) {
    auto isCFM, isGet, memberName, offset, typeName;

    if (strlen(line) == 0) return;
    // Both patterns require a quoted string literal.
    if (strstr(line, "\"") == -1) return;

    isCFM = (strstr(line, "CFMListBox::Create") != -1);
    isGet = (strstr(line, "CXgFMPanel::Get")    != -1);

    if (!isCFM && !isGet) return;

    // ---- Extract member name (last quoted literal) ----
    memberName = ExtractLastString(line);
    if (strlen(memberName) == 0) {
        Message("  [?] Member name not found in: %.80s\n", line);
        return;
    }

    // ---- Extract numeric offset ----
    offset = ExtractOffset(line, isCFM);
    if (offset == -1) {
        Message("  [?] Offset not found in: %.80s\n", line);
        return;
    }

    if (isGet) {
        // --- CXgFMPanel::GetXxx pattern ---
        typeName = ExtractGetType(line);
        if (strlen(typeName) == 0) {
            Message("  [?] Get-type not found in: %.80s\n", line);
            return;
        }
        Message("  >> CXgFMPanel::Get%s  offset=0x%X  "
                "member='m_p%s'  type='CXg%s *'\n",
                typeName, offset, memberName, typeName);
        ApplyUpdate(sid, offset, "m_p" + memberName, typeName, 1);

    } else {
        // --- CFMListBox::Create pattern ---
        Message("  >> CFMListBox::Create  offset=0x%X  "
                "member='m_%s'  type='CFMListBox'\n",
                offset, memberName);
        ApplyUpdate(sid, offset, "m_" + memberName, "CFMListBox", 0);
    }
}

// ============================================================
//  FILE READING
// ============================================================

// Read the file line by line (character-by-character via fgetc),
// process the first non-empty line as the function signature,
// and pass all remaining non-empty lines to ProcessLine.
static ProcessFile(filename) {
    auto h, c, line, trimmed, firstLine, sid, structName;

    h = fopen(filename, "r");
    if (h == 0) {
        Warning("Cannot open file: %s", filename);
        return;
    }

    line       = "";
    firstLine  = 1;
    sid        = BADADDR;
    structName = "";

    while (1) {
        c = fgetc(h);

        if (c == 10 || c == -1) {   // newline (LF=10) or EOF (-1)
            trimmed = Trim(line);

            if (strlen(trimmed) > 0) {

                if (firstLine) {
                    // ---- Signature line: extract class name ----
                    structName = ExtractClassName(trimmed);
                    if (strlen(structName) == 0) {
                        Message("[!] Cannot extract class name from:\n    %s\n", trimmed);
                        fclose(h);
                        return;
                    }
                    Message("[*] Class name : '%s'\n", structName);
                    sid = EnsureStruct(structName);
                    if (sid == BADADDR) { fclose(h); return; }
                    firstLine = 0;

                } else {
                    // ---- Body line: inspect for struct members ----
                    if (sid != BADADDR)
                        ProcessLine(trimmed, sid);
                }
            }

            line = "";
            if (c == -1) break;   // EOF reached

        } else if (c != 13) {     // skip CR (=13) from CRLF line endings
            line = line + form("%c", c);
        }
    }

    fclose(h);
    Message("[*] Processing complete. Struct '%s' updated.\n", structName);
}

// ============================================================
//  ENTRY POINT
// ============================================================

static main() {
    auto fname;
    Message("\n=================================================\n");
    Message(" IDA Struct Updater v1.0  (IDA Pro 7.0)\n");
    Message("=================================================\n");

    fname = AskFile(0, "*.c;*.cpp;*.txt;*.idc",
                    "Select decompiled function pseudocode file");
    if (fname == "" || fname == 0) {
        Message("No file selected — aborted.\n");
        return;
    }

    Message("[*] Input file : %s\n\n", fname);
    ProcessFile(fname);
    Message("\n=================================================\n");
    Message(" Done.\n");
    Message("=================================================\n\n");
}
