import os
import csv
import struct
import re
import numpy as np
from qgis.core import *
from qgis.PyQt.QtCore import QSize, QRect
from qgis.PyQt.QtGui import QImage, QPainter, QPen, QColor

# ==============================================================================
# 1. CONFIGURATION
# ==============================================================================

OUTPUT_FOLDER = r"C:\Users\User\Desktop\Maps"
CSV_PATH      = r"D:\Projects\fifam\tools\maps\feat.csv"
IMAGE_SIZE    = 1024

# --- Dynamic padding ---
PADDING_SMALL = 1.10    # e.g. San Marino, Monaco
PADDING_LARGE = 1.02    # e.g. Russia, Canada
SIZE_SMALL_KM = 500     # km
SIZE_LARGE_KM = 4000    # km

# --- Export filter ---
# List of Name values (from CSV) to export. Leave empty to export everything.
EXPORT_FILTER = [ "Fiji", "Rotuma" ]

# --- QGIS layer-group names ---
LIGHT_GROUP_NAME = "Light Mode"
DARK_GROUP_NAME  = "Dark Mode"

# Layers inside each group that receive a per-country "shapeGroup"='X' SQL filter
FILTERED_LAYER_NAMES = {"border_active", "border_1", "border_2", "country_active"}

# Which layer to use for bounding-box calculation (resolved from the Light group)
BBOX_SOURCE_LAYER = "country_active"


# ==============================================================================
# 2. READ CSV  (UTF-8 BOM, comma-separated)
# ==============================================================================

entries_by_main = {}

with open(CSV_PATH, encoding="utf-8-sig", newline="") as csv_file:
    reader = csv.DictReader(csv_file)
    for row in reader:
        name = row["Name"].strip()
        if EXPORT_FILTER and name not in EXPORT_FILTER:
            continue
            
        adm0 = row["ADM0"].strip()

        # Parse new visibility columns
        adm1   = row.get("ADM1", "0").strip() == "1"
        adm2   = row.get("ADM2", "0").strip() == "1"
        relief = row.get("Relief", "0").strip() == "1"
        
        # Parse overrides
        padding_str = row.get("Padding", "").strip()
        padding = float(padding_str) if padding_str else None
        
        res_str = row.get("Resolution", "").strip()
        if res_str:
            if "x" in res_str.lower():
                res_w, res_h = map(int, res_str.lower().split("x"))
            else:
                res_w = res_h = int(res_str)
        else:
            res_w = res_h = IMAGE_SIZE
            
        offset_x = float(row.get("OffsetX", 0) or 0)
        offset_y = float(row.get("OffsetY", 0) or 0)
        
        emb_rect_str = row.get("EmbeddedRect", "").strip()
        emb_rect = tuple(map(int, emb_rect_str.split(","))) if emb_rect_str else None

        entry = {
            "name": name,
            "id":   row["Id"].strip(),
            "adm0": adm0,
            "adm1": adm1,
            "adm2": adm2,
            "relief": relief,
            "padding": padding,
            "res_w": res_w,
            "res_h": res_h,
            "offset_x": offset_x,
            "offset_y": offset_y,
            "emb_rect": emb_rect
        }

        # Identify if this is an embedded map (e.g. "USA_1")
        match = re.match(r"^(.*)_([1-4])$", adm0)
        if match:
            main_adm0 = match.group(1)
            if main_adm0 not in entries_by_main:
                entries_by_main[main_adm0] = {'main': None, 'embedded': []}
            entries_by_main[main_adm0]['embedded'].append(entry)
        else:
            if adm0 not in entries_by_main:
                entries_by_main[adm0] = {'main': None, 'embedded': []}
            entries_by_main[adm0]['main'] = entry

# Filter out groups where the main map was ignored by EXPORT_FILTER
valid_groups = [g for g in entries_by_main.values() if g['main'] is not None]

print(f"Loaded {len(valid_groups)} main map(s) to process.")


# ==============================================================================
# 3. PROJECT / CRS SETUP
# ==============================================================================

project   = QgsProject.instance()
tree_root = project.layerTreeRoot()

crs_3857 = QgsCoordinateReferenceSystem("EPSG:3857")
crs_4326 = QgsCoordinateReferenceSystem("EPSG:4326")


# ==============================================================================
# 4. LAYER-GROUP HELPERS
# ==============================================================================

def get_group(name):
    group = tree_root.findGroup(name)
    if not group:
        raise Exception(f"Layer group '{name}' not found in the project.")
    return group

def find_tree_layer(group, layer_name):
    for tl in group.findLayers():
        lyr = tl.layer()
        if lyr and lyr.name() == layer_name:
            return tl, lyr
    return None, None

light_group = get_group(LIGHT_GROUP_NAME)
dark_group  = get_group(DARK_GROUP_NAME)

_, bbox_layer = find_tree_layer(light_group, BBOX_SOURCE_LAYER)
if not bbox_layer:
    raise Exception(f"Layer '{BBOX_SOURCE_LAYER}' not found in '{LIGHT_GROUP_NAME}'.")

transform_to_3857 = QgsCoordinateTransform(bbox_layer.crs(), crs_3857, project)
transform_to_4326 = QgsCoordinateTransform(crs_3857, crs_4326, project)


# ==============================================================================
# 5. OUTPUT DIRECTORIES
# ==============================================================================

light_dir = os.path.join(OUTPUT_FOLDER, "light")
dark_dir  = os.path.join(OUTPUT_FOLDER, "dark")
for d in (OUTPUT_FOLDER, light_dir, dark_dir):
    os.makedirs(d, exist_ok=True)


# ==============================================================================
# 6. DYNAMIC PADDING & EXTENT CALCULATOR
# ==============================================================================

def compute_padding(max_dim_meters):
    size_km = max_dim_meters / 1000.0
    if size_km <= SIZE_SMALL_KM: return PADDING_SMALL
    if size_km >= SIZE_LARGE_KM: return PADDING_LARGE
    t = (size_km - SIZE_SMALL_KM) / (SIZE_LARGE_KM - SIZE_SMALL_KM)
    return PADDING_SMALL + t * (PADDING_LARGE - PADDING_SMALL)

def get_render_extent(sql_adm0, res_w, res_h, padding_override, offset_x, offset_y):
    """Calculates the exact EPSG:3857 rectangle keeping aspect ratio and offsets."""
    bbox_layer.setSubsetString(f'"shapeGroup"=\'{sql_adm0}\'')
    geoms = [feat.geometry() for feat in bbox_layer.getFeatures()]
    bbox_layer.setSubsetString("")

    if not geoms:
        return None

    country_geom = QgsGeometry.unaryUnion(geoms)
    country_geom.transform(transform_to_3857)
    raw_extent = country_geom.boundingBox()

    raw_w, raw_h = raw_extent.width(), raw_extent.height()
    pad_factor = padding_override if padding_override is not None else compute_padding(max(raw_w, raw_h))
    
    padded_w = raw_w * pad_factor
    padded_h = raw_h * pad_factor
    target_aspect = res_w / res_h

    # Conform bounding box strictly to target aspect ratio
    if padded_w / padded_h > target_aspect:
        final_w = padded_w
        final_h = padded_w / target_aspect
    else:
        final_h = padded_h
        final_w = padded_h * target_aspect

    # Camera panning math (1 pixel = final_w / res_w meters)
    shift_x_m = offset_x * (final_w / res_w)
    shift_y_m = offset_y * (final_h / res_h)

    center = raw_extent.center()
    center_x = center.x() - shift_x_m
    center_y = center.y() + shift_y_m

    half_w, half_h = final_w / 2.0, final_h / 2.0

    return QgsRectangle(
        center_x - half_w, center_y - half_h,
        center_x + half_w, center_y + half_h
    )


# ==============================================================================
# 7. TGA & COMPOSITING WRITER
# ==============================================================================

def save_tga_24bit(qimage, output_path):
    img = qimage.convertToFormat(QImage.Format.Format_RGB888)
    w, h = img.width(), img.height()
    bpl = img.bytesPerLine()
    ptr = img.bits()
    ptr.setsize(h * bpl)
    raw = np.frombuffer(ptr, dtype=np.uint8).reshape(h, bpl)
    bgr = raw[:, : w * 3].reshape(h, w, 3)[:, :, ::-1].copy()
    
    header = struct.pack("<BBBHHBHHHHBB", 0, 0, 2, 0, 0, 0, 0, 0, w, h, 24, 0x20)
    with open(output_path, "wb") as f:
        f.write(header)
        f.write(bgr.tobytes())

def composite_embedded(main_img, emb_img, rect_tuple, border_color):
    """Draws the embedded image onto the main image with a 2px inner border."""
    x, y, w, h = rect_tuple
    
    if main_img.format() != QImage.Format.Format_RGB888:
        main_img = main_img.convertToFormat(QImage.Format.Format_RGB888)

    painter = QPainter(main_img)
    painter.setRenderHint(QPainter.RenderHint.Antialiasing, True)
    painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, True)

    # Draw scaled embedded overlay
    painter.drawImage(QRect(x, y, w, h), emb_img)
    
    border = 2
    c = border_color
    
    # Top and bottom strips
    painter.fillRect(x, y, w, border, c)
    painter.fillRect(x, y + h - border, w, border, c)
    
    # Left and right strips, excluding top/bottom overlap
    middle_h = h - 2 * border
    if middle_h > 0:
        painter.fillRect(x, y + border, border, middle_h, c)
        painter.fillRect(x + w - border, y + border, border, middle_h, c)

    return main_img


# ==============================================================================
# 8. FILTER / VISIBILITY HELPERS
# ==============================================================================

def apply_country_filters(group, sql_adm0, adm1_vis, adm2_vis, relief_vis):
    filter_expr = f'"shapeGroup"=\'{sql_adm0}\''
    inv_filter_expr = f'"shapeGroup"!=\'{sql_adm0}\''
    
    for tl in group.findLayers():
        lyr = tl.layer()
        if not lyr: continue
        lname = lyr.name()
        
        if lname in FILTERED_LAYER_NAMES:
            lyr.setSubsetString(filter_expr)
        elif lname == "border":
            lyr.setSubsetString(inv_filter_expr)
            
        if lname == "border_1":
            tl.setItemVisibilityChecked(adm1_vis)
        elif lname == "border_2":
            tl.setItemVisibilityChecked(adm2_vis)
        elif lname == "terrain_relief":
            tl.setItemVisibilityChecked(relief_vis)

def clear_country_filters(group):
    for tl in group.findLayers():
        lyr = tl.layer()
        if lyr and (lyr.name() in FILTERED_LAYER_NAMES or lyr.name() == "border"):
            lyr.setSubsetString("")


# ==============================================================================
# 9. RENDER HELPER
# ==============================================================================

def render_group_to_image(group, extent_3857, res_w, res_h):
    render_layers = [tl.layer() for tl in group.findLayers() if tl.itemVisibilityChecked() and tl.layer()]
    settings = QgsMapSettings()
    settings.setLayers(render_layers)
    settings.setDestinationCrs(crs_3857)
    settings.setExtent(extent_3857)
    settings.setOutputSize(QSize(res_w, res_h))

    job = QgsMapRendererSequentialJob(settings)
    job.start()
    job.waitForFinished()
    return job.renderedImage()


# ==============================================================================
# 10. MAIN EXPORT LOOP
# ==============================================================================

for i, group_data in enumerate(valid_groups, 1):
    m_entry = group_data['main']
    embedded_entries = group_data['embedded']

    m_name     = m_entry["name"]
    m_fid      = m_entry["id"]
    m_adm0     = m_entry["adm0"]
    m_sql_adm0 = m_adm0.replace("'", "''")

    print(f"[{i}/{len(valid_groups)}] {m_name} (id={m_fid}) + {len(embedded_entries)} embedded map(s)")

    # 10a. MAIN MAP Extent
    m_ext = get_render_extent(m_sql_adm0, m_entry["res_w"], m_entry["res_h"],
                              m_entry["padding"], m_entry["offset_x"], m_entry["offset_y"])
    if not m_ext:
        print(f"  WARNING: No geometry found for {m_adm0}. Skipping.")
        continue

    # 10b. RENDER MAIN MAP Images
    apply_country_filters(light_group, m_sql_adm0, m_entry["adm1"], m_entry["adm2"], m_entry["relief"])
    light_img = render_group_to_image(light_group, m_ext, m_entry["res_w"], m_entry["res_h"])

    apply_country_filters(dark_group, m_sql_adm0, m_entry["adm1"], m_entry["adm2"], m_entry["relief"])
    dark_img = render_group_to_image(dark_group, m_ext, m_entry["res_w"], m_entry["res_h"])

    if light_img.isNull() or dark_img.isNull():
        print(f"  ERROR: Rendered main image is null for {m_adm0}.")
        continue

    # 10c. RENDER & COMPOSITE EMBEDDED MAPS
    embedded_regions_data = []  # Added to store metadata for regions
    
    for emb in embedded_entries:
        e_adm0     = emb["adm0"]
        e_sql_adm0 = e_adm0.replace("'", "''")
        e_rect     = emb["emb_rect"]
        
        if not e_rect:
            print(f"  WARNING: Embedded map {e_adm0} missing 'EmbeddedRect'. Skipping.")
            continue

        # If resolution is untouched from IMAGE_SIZE, inherit from EmbeddedRect
        e_res_w, e_res_h = emb["res_w"], emb["res_h"]
        if e_res_w == IMAGE_SIZE and e_res_h == IMAGE_SIZE:
            e_res_w, e_res_h = e_rect[2], e_rect[3]

        e_ext = get_render_extent(e_sql_adm0, e_res_w, e_res_h,
                                  emb["padding"], emb["offset_x"], emb["offset_y"])
        if not e_ext:
            continue
            
        # Extract geographic extent to save in metadata later
        e_sq_geom = QgsGeometry.fromRect(e_ext)
        e_sq_geom.transform(transform_to_4326)
        e_ext_4326 = e_sq_geom.boundingBox()

        # Calculate relative values [0; 1] using main image resolution
        rel_x = e_rect[0] / m_entry["res_w"]
        rel_y = e_rect[1] / m_entry["res_h"]
        rel_w = e_rect[2] / m_entry["res_w"]
        rel_h = e_rect[3] / m_entry["res_h"]
        
        embedded_regions_data.append({
            'rel_rect': (rel_x, rel_y, rel_w, rel_h),
            'ext': e_ext_4326
        })

        # Render Overlays
        apply_country_filters(light_group, e_sql_adm0, emb["adm1"], emb["adm2"], emb["relief"])
        e_light_img = render_group_to_image(light_group, e_ext, e_res_w, e_res_h)

        apply_country_filters(dark_group, e_sql_adm0, emb["adm1"], emb["adm2"], emb["relief"])
        e_dark_img = render_group_to_image(dark_group, e_ext, e_res_w, e_res_h)

        # Composite Overlays directly onto main image memories
        if not e_light_img.isNull():
            light_img = composite_embedded(light_img, e_light_img, e_rect, QColor(0, 0, 0, 128))
        if not e_dark_img.isNull():
            dark_img = composite_embedded(dark_img, e_dark_img, e_rect, QColor(255, 255, 255, 64))

    # 10d. SAVE COMBINED TGA
    save_tga_24bit(light_img, os.path.join(light_dir, f"{m_fid}.tga"))
    save_tga_24bit(dark_img,  os.path.join(dark_dir,  f"{m_fid}.tga"))

    # 10e. Write Metadata file (EPSG:4326 coords for the main map extent)
    sq_geom = QgsGeometry.fromRect(m_ext)
    sq_geom.transform(transform_to_4326)
    ext_4326 = sq_geom.boundingBox()

    txt_path = os.path.join(OUTPUT_FOLDER, f"{m_fid}.txt")
    with open(txt_path, "w", encoding="utf-8") as txt:
        # Main Map Coordinates
        txt.write(f"{ext_4326.xMinimum()}\n")
        txt.write(f"{ext_4326.xMaximum()}\n")
        txt.write(f"{ext_4326.yMinimum()}\n")
        txt.write(f"{ext_4326.yMaximum()}\n")
        
        # Embedded Maps Coordinates
        for r_data in embedded_regions_data:
            txt.write("REGION\n")
            rx, ry, rw, rh = r_data['rel_rect']
            txt.write(f"{rx},{ry},{rw},{rh}\n")
            
            r_ext = r_data['ext']
            txt.write(f"{r_ext.xMinimum()}\n")
            txt.write(f"{r_ext.xMaximum()}\n")
            txt.write(f"{r_ext.yMinimum()}\n")
            txt.write(f"{r_ext.yMaximum()}\n")
            
            txt.write("END\n")

    print(f"  -> light/{m_fid}.tga   dark/{m_fid}.tga   {m_fid}.txt")


# ==============================================================================
# 11. CLEANUP
# ==============================================================================

clear_country_filters(light_group)
clear_country_filters(dark_group)

# Restore default visibilities
for grp in (light_group, dark_group):
    for lname in ["border_1", "border_2", "terrain_relief"]:
        tl, _ = find_tree_layer(grp, lname)
        if tl:
            tl.setItemVisibilityChecked(True)

print(f"\nFinished! Processed {len(valid_groups)} top-level records.")