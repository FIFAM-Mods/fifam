import glob
import os
import sys
import xml.etree.ElementTree as ET

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


class ConvertError(Exception):
    pass


def collect_definitions_from_xml(path):
    tree = ET.parse(path)
    root = tree.getroot()
    if root.tag != 'Definitions':
        raise ConvertError(f"Root element is '{root.tag}', expected 'Definitions'")

    defs = []
    for def_elem in root.findall('Definition'):
        # id attribute
        id_attr = def_elem.get('id')
        if id_attr is None:
            raise ConvertError("Definition element missing 'id' attribute")
        try:
            def_id = int(id_attr)
        except ValueError:
            raise ConvertError(f"Invalid Definition id '{id_attr}'")

        # translations: attributes nameX
        translations = {}
        for k, v in def_elem.items():
            if k.startswith('name') and k[4:].isdigit():
                translations[int(k[4:])] = v

        # numeric attributes
        def_attr = {}
        for attr in ('unknown', 'height', 'weight'):
            val = def_elem.get(attr)
            if val is None:
                raise ConvertError(f"Definition id={def_id} missing '{attr}' attribute")
            try:
                def_attr[attr] = int(val)
            except ValueError:
                raise ConvertError(f"Invalid integer for {attr} in Definition id={def_id}: '{val}'")

        # parameters: collect by expected name order; if parameter not present, empty list
        params = []
        for pname in PARAM_NAMES:
            pnode = None
            for candidate in def_elem.findall('Parameter'):
                if candidate.get('name') == pname:
                    pnode = candidate
                    break
            if pnode is None:
                # empty parameter
                params.append([])
                continue

            values = []
            for vnode in pnode.findall('Value'):
                id_attr = vnode.get('id')
                wt_attr = vnode.get('weight')
                if id_attr is None or wt_attr is None:
                    raise ConvertError(f"Value node in Definition id={def_id}, Parameter='{pname}' missing 'id' or 'weight'")
                try:
                    vid = int(id_attr)
                    w = int(wt_attr)
                except ValueError:
                    raise ConvertError(f"Invalid integer in Value node (Definition id={def_id}, Parameter='{pname}') id='{id_attr}' weight='{wt_attr}'")
                if not (0 <= w <= 255):
                    raise ConvertError(f"Weight out of uchar range (0..255) in Definition id={def_id}, Parameter='{pname}': {w}")
                values.append((vid, w))

            params.append(values)

        defs.append({
            'id': def_id,
            'translations': translations,
            'unknown': def_attr['unknown'],
            'height': def_attr['height'],
            'weight': def_attr['weight'],
            'parameters': params,
        })

    return defs


def write_sav_for_definitions(defs, out_path):
    # nextAppearanceDefId = max id + 1
    max_id = max((d['id'] for d in defs), default=0)
    next_id = max_id + 1
    lines = []
    lines.append(str(next_id))
    lines.append(str(len(defs)))

    for d in defs:
        lines.append(str(d['id']))
        # translations: write count and entries sorted by translation id
        trans_items = sorted(d['translations'].items(), key=lambda x: x[0])
        lines.append(str(len(trans_items)))
        for tid, tname in trans_items:
            lines.append(str(tid))
            # write the translation name as-is on its own line
            lines.append(tname)

        lines.append(str(d['unknown']))
        lines.append(str(d['height']))
        lines.append(str(d['weight']))

        # numAppearanceParameters (always 9)
        lines.append(str(len(d['parameters'])))

        for param in d['parameters']:
            num_values = len(param)
            lines.append(str(num_values))
            for vid, _ in param:
                lines.append(str(vid))

            # numWeights (same as num_values)
            lines.append(str(num_values))
            for _, wt in param:
                lines.append(str(wt))

    # write file with BOM (utf-8-sig) so that the file begins with BOM
    with open(out_path, 'w', encoding='utf-8-sig', newline="") as f:
        f.write("\r\n".join(lines) + "\r\n")


def convert_file(xml_path):
    base = os.path.splitext(xml_path)[0]
    out_path = base + '.sav'
    defs = collect_definitions_from_xml(xml_path)
    write_sav_for_definitions(defs, out_path)
    print(f"Converted '{xml_path}' -> '{out_path}' (definitions: {len(defs)})")


def main():
    files = sorted(glob.glob('AppearanceDefs*.xml'))
    if not files:
        print("No files matching 'AppearanceDefs*.xml' found in current directory.")
        return

    for xml in files:
        try:
            convert_file(xml)
        except Exception as e:
            print(f"Error converting '{xml}': {e}", file=sys.stderr)


if __name__ == '__main__':
    main()
