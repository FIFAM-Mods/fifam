from pathlib import Path
import xml.etree.ElementTree as ET
import sys
import glob

PARAM_NAMES = [
    "Face",
    "Hair",
    "SkinColor",
    "Variation",
    "BeardColor",
    "EyeColor",
    "Beard",
    "Sideburns",
    "HairColor",
]


def indent(elem, level=0):
    i = "\n" + level * "    "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "    "
        children = list(elem)
        for idx, child in enumerate(children):
            indent(child, level + 1)
            # if not last child -> indent between siblings; else -> newline before parent close
            if not child.tail or not child.tail.strip():
                child.tail = i if idx == len(children) - 1 else (i + "    ")
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = "\n" + (level - 1) * "    "

class ParseError(Exception):
    pass


def parse_appearance_defs(path: Path):
    text = path.read_text(encoding="utf-8-sig")
    lines = text.splitlines()
    p = 0
    n_lines = len(lines)

    def eof():
        return p >= n_lines

    def peek():
        return None if eof() else lines[p]

    def read_line():
        nonlocal p
        if eof():
            raise ParseError(f"Unexpected end of file while parsing {path}")
        line = lines[p]
        p += 1
        return line

    def read_non_empty_strip():
        """Read next non-empty (after strip) line and return stripped value."""
        nonlocal p
        while not eof():
            s = lines[p].strip()
            p += 1
            if s != "":
                return s
        raise ParseError(f"Unexpected EOF while expecting a numeric value in {path}")

    def read_int():
        s = read_non_empty_strip()
        try:
            return int(s)
        except Exception as e:
            raise ParseError(f"Failed to parse int from '{s}' at line {p} in {path}: {e}")

    def read_string_line():
        # Take the next physical line (allowing empty string as a translation name)
        return read_line()

    # Begin parsing according to provided layout
    root = ET.Element("Definitions")

    # nextAppearanceDefId (we ignore except to advance)
    nextAppearanceDefId = read_int()

    # numAppearanceDefs
    numAppearanceDefs = read_int()

    for _ in range(numAppearanceDefs):
        def_id = read_int()
        num_trans_entries = read_int()

        definition = ET.SubElement(root, "Definition")
        definition.set("id", str(def_id))

        # translations
        for _ in range(num_trans_entries):
            translation_id = read_int()
            translation_name = read_string_line()
            # XML attribute name: name<translationId>
            attr_name = f"name{translation_id}"
            # If the same attr exists, append a suffix to avoid overwriting (rare)
            if attr_name in definition.attrib:
                # fallback: name{translationId}_N
                i = 1
                while f"{attr_name}_{i}" in definition.attrib:
                    i += 1
                attr_name = f"{attr_name}_{i}"
            definition.set(attr_name, translation_name)

        unknown = read_int()
        height = read_int()
        weight = read_int()
        # set these as attributes
        definition.set("unknown", str(unknown))
        definition.set("height", str(height))
        definition.set("weight", str(weight))

        num_appearance_parameters = read_int()

        # For each parameter (expected 9)
        for param_index in range(num_appearance_parameters):
            # read numValues
            num_values = read_int()
            values = []
            for _v in range(num_values):
                values.append(read_int())
            num_weights = read_int()
            weights = []
            for _w in range(num_weights):
                # weights are uchar but stored as integers in text
                weights.append(read_int())

            param_name = PARAM_NAMES[param_index] if param_index < len(PARAM_NAMES) else f"Param{param_index}"
            param_el = ET.SubElement(definition, "Parameter")
            param_el.set("name", param_name)

            # pair values and weights; if lengths differ, pair until shortest and ignore extras
            for vid, w in zip(values, weights):
                val_el = ET.SubElement(param_el, "Value")
                val_el.set("id", str(vid))
                val_el.set("weight", str(w))

    return root


def convert_all_in_cwd():
    files = sorted(Path('.').glob('AppearanceDefs*.sav'))
    if not files:
        print("No AppearanceDefs*.sav files found in the current directory.")
        return 1

    for f in files:
        try:
            root = parse_appearance_defs(f)
            indent(root)
            out_path = f.with_suffix('.xml')
            tree = ET.ElementTree(root)
            # write without XML declaration
            tree.write(out_path, encoding='utf-8', xml_declaration=False)
            print(f"Converted: {f} -> {out_path}")
        except Exception as e:
            print(f"Failed to convert {f}: {e}", file=sys.stderr)
    return 0


if __name__ == '__main__':
    sys.exit(convert_all_in_cwd())
