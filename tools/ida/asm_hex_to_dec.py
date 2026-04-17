#!/usr/bin/env python3
# hex2dec_asm.py
# Reads asm.txt, converts occurrences like 8090h -> 0x8090 (if >= 16) or -> 15 (if < 16)
# Writes result to asm_out.txt

import re
import sys

INFILE = "asm.txt"
OUTFILE = "asm_out.txt"

# Match a run of hex digits followed by 'h' (case-insensitive).
# Ensure the match is NOT preceded by a letter/digit/underscore so we don't convert identifiers like var_123h.
# \b after 'h' helps to avoid partial matches inside words.
pattern = re.compile(r'(?<![0-9A-Za-z_])([0-9A-Fa-f]+)h\b', re.IGNORECASE)

def replace_match(m, counter):
    hex_digits = m.group(1)
    try:
        val = int(hex_digits, 16)
    except ValueError:
        return m.group(0)  # shouldn't happen, but be safe

    counter[0] += 1
    if val < 10:
        return str(val)
    else:
        # use Python's hex() to produce 0x-prefixed lower-case hex (e.g. 0x1a)
        # if you prefer uppercase hex digits, use format(val, '#X') instead.
        return '0x' + hex_digits.upper()

def convert_file(inpath: str, outpath: str) -> int:
    counter = [0]
    try:
        with open(inpath, 'r', encoding='utf-8') as fin:
            text = fin.read()
    except FileNotFoundError:
        print(f"Input file '{inpath}' not found.", file=sys.stderr)
        return -1

    new_text = pattern.sub(lambda m: replace_match(m, counter), text)

    with open(outpath, 'w', encoding='utf-8') as fout:
        fout.write(new_text)

    return counter[0]

if __name__ == "__main__":
    changed = convert_file(INFILE, OUTFILE)
    if changed == -1:
        sys.exit(1)
    print(f"Done — wrote '{OUTFILE}'. Replaced {changed} hex literal(s).")