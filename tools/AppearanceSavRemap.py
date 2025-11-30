import glob
import os
import sys
import xml.etree.ElementTree as ET
from xml.dom import minidom

# --- CONFIGURE CATEGORIES HERE ---
FaceCategories = [30, 40, 50]
HairCategories = [10, 20, 30, 40]
# ---------------------------------

PARAM_TARGETS = {
    'Face': FaceCategories,
    'Hair': HairCategories,
}


def find_input_xml():
    # prefer exact name, otherwise pick first match
    if os.path.exists('AppearanceDefs.xml'):
        return 'AppearanceDefs.xml'
    matches = sorted(glob.glob('AppearanceDefs*.xml'))
    if matches:
        return matches[0]
    return None


def prettify_without_xml_decl(elem):
    rough_str = ET.tostring(elem, encoding='utf-8')
    reparsed = minidom.parseString(rough_str)
    pretty = reparsed.toprettyxml(indent='    ')
    lines = pretty.splitlines()
    if lines and lines[0].startswith('<?xml'):
        lines = lines[1:]
    # drop leading blank lines
    while lines and lines[0].strip() == '':
        lines = lines[1:]
    return '\n'.join(lines) + '\n'


def remap_weights_in_parameter(param_elem, categories):
    # collect value nodes
    value_nodes = [v for v in param_elem.findall('Value')]
    if not value_nodes:
        return 0

    weights = []
    for v in value_nodes:
        w = v.get('weight')
        if w is None:
            # skip nodes without weight
            continue
        try:
            weights.append(int(w))
        except ValueError:
            # skip invalid integer
            continue

    if not weights:
        return 0

    min_w = min(weights)
    max_w = max(weights)
    ncat = len(categories)

    # if range is zero, map all to first category
    if max_w == min_w or ncat == 1:
        mapped = [categories[0]] * len(weights)
    else:
        size = (max_w - min_w) / float(ncat)
        mapped = []
        for w in weights:
            # compute bucket index
            idx = int((w - min_w) / size)
            if idx < 0:
                idx = 0
            if idx >= ncat:
                idx = ncat - 1
            mapped.append(categories[idx])

    # apply mapped weights back to nodes in order (only to nodes that had a valid integer weight)
    mi = 0
    for v in value_nodes:
        w = v.get('weight')
        if w is None:
            continue
        try:
            int(w)
        except ValueError:
            continue
        new_w = mapped[mi]
        v.set('weight', str(new_w))
        mi += 1

    return mi


def main():
    input_path = find_input_xml()
    if not input_path:
        print("No AppearanceDefs.xml or AppearanceDefs*.xml found in the current folder.")
        sys.exit(2)

    print(f"Loading '{input_path}'...")
    tree = ET.parse(input_path)
    root = tree.getroot()

    if root.tag != 'Definitions':
        print(f"Warning: root tag is '{root.tag}', expected 'Definitions' — continuing anyway.")

    total_changed = 0
    for def_elem in root.findall('Definition'):
        for param_elem in def_elem.findall('Parameter'):
            pname = param_elem.get('name')
            if pname in PARAM_TARGETS:
                cats = PARAM_TARGETS[pname]
                changed = remap_weights_in_parameter(param_elem, cats)
                if changed:
                    print(f"Definition id={def_elem.get('id')} Parameter='{pname}': remapped {changed} weights -> categories {cats}")
                total_changed += changed

    out_path = 'AppearanceDefs_remapped.xml'
    pretty = prettify_without_xml_decl(root)
    with open(out_path, 'w', encoding='utf-8', newline='') as f:
        f.write(pretty)

    print(f"Done. Wrote {out_path}. Total remapped values: {total_changed}")


if __name__ == '__main__':
    main()
