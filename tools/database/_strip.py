#!/usr/bin/env python3
"""
Simplified filter script for FM CSVs.
Assumes first column is an integer and there are no quoted newlines.
Preserves full lines; only removes data lines whose first-column ID is not
present in the corresponding base file.
"""

from pathlib import Path
import glob
import argparse
import sys

BASE_TO_PATTERN = {
    "fm_clubs.csv": "fm_club_*.csv",
    "fm_comps.csv": "fm_comp_*.csv",
    "fm_players.csv": "fm_player_*.csv",
}

def extract_first_field_id_simple(line: str):
    """Get int from first CSV field by splitting at the first comma.
    Handles optional surrounding double-quotes like \"123\"."""
    if not line:
        return None
    # remove trailing newline for safety when checking
    raw = line.rstrip('\r\n')
    idx = raw.find(',')
    if idx == -1:
        field = raw.strip()
    else:
        field = raw[:idx].strip()
    # strip surrounding quotes if present
    if len(field) >= 2 and field[0] == '"' and field[-1] == '"':
        field = field[1:-1].strip()
    try:
        return int(field)
    except Exception:
        return None

def collect_ids_from_base(base_path: Path):
    ids = set()
    if not base_path.exists():
        print(f"Warning: base file {base_path} not found. Skipping.")
        return ids
    with base_path.open('r', encoding='utf-8-sig', newline='') as f:
        header = f.readline()
        if header == '':
            print(f"Warning: base file {base_path} is empty.")
            return ids
        for lineno, line in enumerate(f, start=2):
            if line.strip() == '':
                continue
            idv = extract_first_field_id_simple(line)
            if idv is None:
                print(f"Warning: could not parse ID in {base_path.name} at line {lineno}; skipping.")
                continue
            ids.add(idv)
    return ids

def process_additional_file(path: Path, valid_ids: set):
    with path.open('r', encoding='utf-8-sig', newline='') as f:
        lines = f.readlines()
    if not lines:
        print(f"Note: {path.name} is empty — skipping.")
        return 0, 0
    header = lines[0]
    data_lines = lines[1:]
    kept = [header]
    removed = 0
    kept_count = 0
    for idx, line in enumerate(data_lines, start=2):
        if line.strip() == '':
            kept.append(line)
            kept_count += 1
            continue
        idv = extract_first_field_id_simple(line)
        if idv is None:
            # safer: keep unparsable lines
            kept.append(line)
            kept_count += 1
            print(f"Warning: couldn't parse ID in {path.name} at line {idx}; keeping line.")
            continue
        if idv in valid_ids:
            kept.append(line)
            kept_count += 1
        else:
            removed += 1
    if removed == 0:
        return kept_count, removed
    with path.open('w', encoding='utf-8-sig', newline='') as f:
        f.writelines(kept)
    return kept_count, removed

def main(directory: Path):
    total_removed = 0
    total_processed_files = 0
    for base_name, pattern in BASE_TO_PATTERN.items():
        base_path = directory / base_name
        valid_ids = collect_ids_from_base(base_path)
        if not valid_ids:
            print(f"Info: no IDs collected from {base_name} (file missing or empty). Skipping matching files.")
            continue
        pattern_path = str(directory / pattern)
        matched = sorted(glob.glob(pattern_path))
        if not matched:
            print(f"Info: no files matching pattern {pattern} in {directory}")
            continue
        for filepath in matched:
            if Path(filepath).name == base_name:
                continue
            p = Path(filepath)
            total_processed_files += 1
            kept, removed = process_additional_file(p, valid_ids)
            total_removed += removed
            print(f"Processed {p.name}: kept {kept} data lines, removed {removed} lines.")
    print(f"\nDone. Processed {total_processed_files} files — total removed lines: {total_removed}.")

if __name__ == '__main__':
    # Use current working directory
    base_dir = Path(".").resolve()
    main(base_dir)