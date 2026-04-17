import csv
from decimal import Decimal, InvalidOperation
from collections import OrderedDict
from pathlib import Path

INPUT_FILE = Path("RegionLanguages.txt")
TEMP_FILE = Path("RegionLanguages.txt.tmp")

def parse_weight(value):
    value = value.strip()
    if not value:
        return Decimal("0")
    try:
        return Decimal(value)
    except InvalidOperation:
        raise ValueError("Invalid Weight value: {!r}".format(value))

def format_weight(value):
    # Write integers without trailing ".0"
    if value == value.to_integral_value():
        return str(int(value))
    return format(value.normalize(), "f").rstrip("0").rstrip(".")

def main():
    if not INPUT_FILE.exists():
        raise FileNotFoundError("File not found: {}".format(INPUT_FILE))

    with INPUT_FILE.open("r", encoding="utf-16", newline="") as f:
        reader = csv.DictReader(f, delimiter="\t")
        if reader.fieldnames is None:
            raise ValueError("Input file has no header row")

        # Keep rows grouped by (RegionID, LanguageID)
        grouped = OrderedDict()

        for row in reader:
            region_id = row["RegionID"]
            language_id = row["LanguageID"]
            key = (region_id, language_id)

            if key not in grouped:
                grouped[key] = row.copy()
                grouped[key]["Weight"] = parse_weight(row["Weight"])
            else:
                grouped[key]["Weight"] += parse_weight(row["Weight"])

                # Keep the first non-empty values for the other columns
                for col in ("Country", "Name", "LanguageName"):
                    if not grouped[key].get(col) and row.get(col):
                        grouped[key][col] = row[col]

    fieldnames = ["RegionID", "Country", "Name", "Weight", "LanguageID", "LanguageName"]

    with TEMP_FILE.open("w", encoding="utf-16", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames, delimiter="\t", lineterminator="\r\n")
        writer.writeheader()

        for row in grouped.values():
            out_row = row.copy()
            out_row["Weight"] = format_weight(out_row["Weight"])
            writer.writerow(out_row)

    TEMP_FILE.replace(INPUT_FILE)
    print("Done. Merged {} unique RegionID+LanguageID rows.".format(len(grouped)))

if __name__ == "__main__":
    main()