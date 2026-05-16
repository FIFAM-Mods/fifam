import csv
import io

def remove_men_exact(input_file: str, output_file: str):
    # Read entire file as raw text (preserves BOM)
    with open(input_file, "r", encoding="utf-8-sig") as infile:
        lines = infile.readlines()

    if not lines:
        print("Empty file.")
        return

    # Write back using normal UTF-8
    with open(output_file, "w", encoding="utf-8-sig", newline="") as outfile:
        # --- Process header ---
        header_line = lines[0]
        outfile.write(header_line)

        # Parse header to locate Gender column
        header_fields = next(csv.reader([header_line]))
        try:
            gender_index = header_fields.index("Gender")
        except ValueError:
            raise RuntimeError("Gender column not found in CSV header.")

        # --- Process remaining lines ---
        for line in lines[1:]:
            if not line.strip():
                continue  # skip empty lines

            # Parse this row WITHOUT altering the line
            fields = next(csv.reader([line]))

            # Keep only rows where Gender == "0"
            gender_value = fields[gender_index].strip()

            if gender_value == "1":
                outfile.write(line)

if __name__ == "__main__":
    remove_men_exact("fm_players.csv", "fm_players_filtered.csv")
    remove_men_exact("fm_clubs.csv", "fm_clubs_filtered.csv")
    remove_men_exact("fm_comps.csv", "fm_comps_filtered.csv")
    print("Done!")
