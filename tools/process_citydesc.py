#!/usr/bin/env python3
"""
process_citydesc.py

For each citydescriptions_X.txt file in the current folder:
  1. Read UTF-16 LE BOM, tab-separated rows (key -> text), skip header.
  2. Resolve redirections: if a text value looks like an IDS_CITYDESC_* key,
     follow the reference (up to 2 hops, drop if unresolvable).
  3. Load ids.csv (UTF-8 BOM): hex club ID -> decimal city ID + priority.
  4. Re-key every row from IDS_CITYDESC_<clubHex> -> <cityDecimal>.
     Rows whose club ID is not in ids.csv are dropped.
     When multiple clubs share a city ID, the highest-priority one wins.
  5. Log only cases where texts genuinely differ between clubs for the same city.
  6. Write result as X.txt (UTF-16 LE BOM, tab-separated, no header).
     Output key is the plain decimal city ID (no IDS_CITYDESC_ prefix).
"""

import csv
import glob
import os
import re
import sys

IDS_KEY_RE     = re.compile(r'^IDS_CITYDESC_([0-9A-Fa-f]+)$')
IDS_IN_TEXT_RE = re.compile(r'^(IDS_CITYDESC_[0-9A-Fa-f]+)$')

CITYDESC_ENCODING = "utf-16"    # auto-handles LE/BE BOM
IDS_CSV_ENCODING  = "utf-8-sig" # strips UTF-8 BOM


# ---------------------------------------------------------------------------
# ids.csv loader
# ---------------------------------------------------------------------------

def load_ids_csv(path: str) -> dict[str, tuple[int, int]]:
    """
    Return {hex_club_id_upper -> (city_id, priority)} from ids.csv.
    Columns: hex_club_id, city_id, priority  (no header row).
    """
    mapping: dict[str, tuple[int, int]] = {}
    with open(path, newline="", encoding=IDS_CSV_ENCODING) as fh:
        reader = csv.reader(fh)
        for lineno, row in enumerate(reader, 1):
            if len(row) < 3:
                continue
            hex_str, city_str, prio_str = row[0].strip(), row[1].strip(), row[2].strip()
            try:
                club_id  = int(hex_str, 16)  # accepts "0x..." prefix
                city_id  = int(city_str)
                priority = int(prio_str)
            except ValueError:
                print(f"  [ids.csv] Skipping bad row {lineno}: {row}", file=sys.stderr)
                continue
            mapping[f"{club_id:08X}"] = (city_id, priority)
    return mapping


# ---------------------------------------------------------------------------
# Language-file reader
# ---------------------------------------------------------------------------

def read_citydesc(path: str) -> dict[str, str]:
    """
    Read citydescriptions_X.txt.
    Returns {IDS_CITYDESC_XXXXXXXX -> raw_text}, header row dropped.
    """
    entries: dict[str, str] = {}
    with open(path, newline="", encoding=CITYDESC_ENCODING) as fh:
        reader = csv.reader(fh, delimiter="\t")
        for lineno, row in enumerate(reader):
            if lineno == 0:       # header
                continue
            if len(row) < 2:
                continue
            key, text = row[0].strip(), row[1]
            if IDS_KEY_RE.match(key):
                entries[key] = text
    return entries


# ---------------------------------------------------------------------------
# Redirect resolver
# ---------------------------------------------------------------------------

def resolve_redirects(entries: dict[str, str]) -> dict[str, str]:
    """
    Follow IDS_CITYDESC_* redirects in text values (up to 2 hops).
    Rows that cannot be resolved are dropped.
    """
    resolved: dict[str, str] = {}
    for key, text in entries.items():
        current = text.strip()
        for _ in range(2):                          # allow up to 2 hops
            m = IDS_IN_TEXT_RE.match(current)
            if not m:
                break                               # plain text - done
            target = m.group(1)
            if target not in entries:
                current = None                      # unresolvable
                break
            current = entries[target].strip()
        if current is None or IDS_IN_TEXT_RE.match(current):
            continue                                # still a redirect - drop
        resolved[key] = current
    return resolved


# ---------------------------------------------------------------------------
# Re-key by city ID with priority-based deduplication
# ---------------------------------------------------------------------------

def rekey_to_city(
    entries: dict[str, str],
    club_to_city: dict[str, tuple[int, int]],
    conflicts: list[str],
) -> dict[int, str]:
    """
    Map IDS_CITYDESC_<hexClub> -> city_id (int).
    Keeps the text of the club with the highest priority.
    Appends conflict descriptions to `conflicts` when texts differ.
    Returns {city_id -> winning_text}.
    """
    # city_id -> (text, priority, winning_club_hex)
    city_best: dict[int, tuple[str, int, str]] = {}

    for key, text in entries.items():
        m = IDS_KEY_RE.match(key)
        if not m:
            continue
        club_hex = m.group(1).upper().zfill(8)
        entry = club_to_city.get(club_hex)
        if entry is None:
            continue
        city_id, priority = entry

        if city_id not in city_best:
            city_best[city_id] = (text, priority, club_hex)
        else:
            prev_text, prev_prio, prev_club = city_best[city_id]
            if text != prev_text:
                # Texts differ - log the conflict, then apply priority
                if priority > prev_prio:
                    winner_club, winner_text, winner_prio = club_hex,  text,      priority
                    loser_club,  loser_text               = prev_club, prev_text
                else:
                    winner_club, winner_text, winner_prio = prev_club, prev_text, prev_prio
                    loser_club,  loser_text               = club_hex,  text

                conflicts.append(
                    f"CONFLICT city_id={city_id}: "
                    f"club {winner_club} (prio {winner_prio}) wins.\n"
                    f"  used   [{winner_club}]: {winner_text!r}\n"
                    f"  dropped[{loser_club}]:  {loser_text!r}"
                )

                if priority > prev_prio:
                    city_best[city_id] = (text, priority, club_hex)
            elif priority > prev_prio:
                # Same text, higher-priority record - update silently
                city_best[city_id] = (text, priority, club_hex)

    return {city_id: data[0] for city_id, data in city_best.items()}


# ---------------------------------------------------------------------------
# Output writer
# ---------------------------------------------------------------------------

def write_output(city_entries: dict[int, str], out_path: str) -> None:
    """Write UTF-16 LE BOM tab-separated file. Key = plain decimal city ID."""
    with open(out_path, "w", newline="", encoding="utf-16") as fh:
        writer = csv.writer(fh, delimiter="\t", lineterminator="\r\n")
        for city_id in sorted(city_entries):
            writer.writerow([city_id, city_entries[city_id]])


# ---------------------------------------------------------------------------
# Per-language orchestration
# ---------------------------------------------------------------------------

def process_language_file(
    txt_path: str,
    club_to_city: dict[str, tuple[int, int]],
) -> None:
    basename = os.path.basename(txt_path)
    lang     = basename[len("citydescriptions_"):-len(".txt")]
    out_path = f"{lang}.txt"
    log_path = f"{lang}.log"

    print(f"  Processing {basename} -> {out_path}")

    entries = read_citydesc(txt_path)
    entries = resolve_redirects(entries)

    conflicts: list[str] = []
    city_entries = rekey_to_city(entries, club_to_city, conflicts)

    # Write log only when there are real text conflicts
    if conflicts:
        with open(log_path, "w", encoding="utf-8") as lf:
            lf.write("\n\n".join(conflicts) + "\n")
        print(f"    ! {len(conflicts)} conflict(s) logged to {log_path}")
    elif os.path.exists(log_path):
        os.remove(log_path)   # clean up stale log from a previous run

    write_output(city_entries, out_path)
    print(f"    -> {len(city_entries)} city entries written.")


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main() -> None:
    ids_csv = "ids.csv"
    if not os.path.isfile(ids_csv):
        sys.exit(f"ERROR: {ids_csv} not found in current directory.")

    print(f"Loading {ids_csv} ...")
    club_to_city = load_ids_csv(ids_csv)
    print(f"  {len(club_to_city)} club->city mappings loaded.\n")

    txt_files = sorted(glob.glob("citydescriptions_???.txt"))
    if not txt_files:
        sys.exit("No citydescriptions_XXX.txt files found in current directory.")

    print(f"Found {len(txt_files)} language file(s).")
    for txt_path in txt_files:
        process_language_file(txt_path, club_to_city)

    print("\nDone.")


if __name__ == "__main__":
    main()
