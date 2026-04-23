import os
import csv
import struct
import numpy as np
from qgis.core import *
from qgis.PyQt.QtCore import QSize
from qgis.PyQt.QtGui import QImage

# ==============================================================================
# 1. CONFIGURATION
# ==============================================================================

OUTPUT_FOLDER = r"C:\Users\User\Desktop\Maps"
CSV_PATH      = r"D:\Projects\fifam\tools\maps\feat.csv"
IMAGE_SIZE    = 1024

# --- Dynamic padding ---
# The padding factor is linearly interpolated based on the country's longest
# bounding-box axis (in km, measured in EPSG:3857).
#   • Countries whose longest axis is ≤ SIZE_SMALL_KM  → PADDING_SMALL
#   • Countries whose longest axis is ≥ SIZE_LARGE_KM  → PADDING_LARGE
#   • Anything in between                              → linear interpolation
PADDING_SMALL = 1.10    # e.g. San Marino, Monaco
PADDING_LARGE = 1.02    # e.g. Russia, Canada
SIZE_SMALL_KM = 500     # km
SIZE_LARGE_KM = 4000    # km

# --- Export filter ---
# List of Name values (from CSV) to export. Leave empty to export everything.
EXPORT_FILTER = []      # e.g. ["Ukraine", "France", "Germany"]

# --- QGIS layer-group names ---
LIGHT_GROUP_NAME = "Light Mode"
DARK_GROUP_NAME  = "Dark Mode"

# Layers inside each group that receive a per-country "shapeGroup" SQL filter
FILTERED_LAYER_NAMES = {"border_active", "border_1", "border_2", "country_active"}

# Which layer to use for bounding-box calculation (resolved from the Light group)
BBOX_SOURCE_LAYER = "country_active"


# ==============================================================================
# 2. READ CSV  (UTF-8 BOM, comma-separated)
# ==============================================================================

entries = []
with open(CSV_PATH, encoding="utf-8-sig", newline="") as csv_file:
    reader = csv.DictReader(csv_file)
    for row in reader:
        name = row["Name"].strip()
        if EXPORT_FILTER and name not in EXPORT_FILTER:
            continue
        entries.append({
            "name": name,
            "id":   row["Id"].strip(),
            "adm0": row["ADM0"].strip(),
            "adm2": row["ADM2"].strip() == "1",
        })

print(f"Loaded {len(entries)} entr{'y' if len(entries) == 1 else 'ies'} to process.")


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
    """Return (QgsLayerTreeLayer, QgsMapLayer) for the first match inside *group*."""
    for tl in group.findLayers():
        lyr = tl.layer()
        if lyr and lyr.name() == layer_name:
            return tl, lyr
    return None, None


light_group = get_group(LIGHT_GROUP_NAME)
dark_group  = get_group(DARK_GROUP_NAME)

# Resolve the bbox layer (geometry is the same in both groups, use Light)
_, bbox_layer = find_tree_layer(light_group, BBOX_SOURCE_LAYER)
if not bbox_layer:
    raise Exception(
        f"Bounding-box source layer '{BBOX_SOURCE_LAYER}' "
        f"not found inside '{LIGHT_GROUP_NAME}'."
    )

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
# 6. DYNAMIC PADDING
# ==============================================================================

def compute_padding(max_dim_meters):
    """
    Linearly interpolate the padding factor from PADDING_SMALL to PADDING_LARGE
    based on the country's longest bounding-box axis expressed in kilometres.
    """
    size_km = max_dim_meters / 1000.0
    if size_km <= SIZE_SMALL_KM:
        return PADDING_SMALL
    if size_km >= SIZE_LARGE_KM:
        return PADDING_LARGE
    t = (size_km - SIZE_SMALL_KM) / (SIZE_LARGE_KM - SIZE_SMALL_KM)
    return PADDING_SMALL + t * (PADDING_LARGE - PADDING_SMALL)


# ==============================================================================
# 7. TGA WRITER  (bypasses Qt's optional TGA image-format plugin)
# ==============================================================================

def save_tga_24bit(qimage, output_path):
    """
    Write *qimage* to *output_path* as a 24-bit uncompressed TGA.

    Why not QImage.save(..., "tga")?
    Qt's TGA support lives in an optional imageformats plugin (qtga.dll on
    Windows) that many QGIS distributions do not ship.  When the plugin is
    absent, save() returns False silently and no file is written.

    This function writes the binary directly, relying only on struct and numpy
    (numpy is always available in QGIS).

    TGA format notes used here:
      - Image type 2  = uncompressed true-colour (no RLE, no palette)
      - 24 bpp        = BGR byte order (TGA convention)
      - Descriptor 0x20 = bit 5 set -> pixel (0,0) is top-left, matching
                          QImage's natural memory layout; no row-flip needed
    """
    img = qimage.convertToFormat(QImage.Format.Format_RGB888)
    w   = img.width()
    h   = img.height()
    bpl = img.bytesPerLine()   # may be > w*3 due to 4-byte row alignment

    # ------------------------------------------------------------------ #
    # Extract raw pixel bytes via the sip.voidptr returned by bits().     #
    # setsize() is required to tell sip how many bytes are addressable    #
    # before numpy can wrap the pointer without a copy.                   #
    # ------------------------------------------------------------------ #
    ptr = img.bits()
    ptr.setsize(h * bpl)
    raw = np.frombuffer(ptr, dtype=np.uint8).reshape(h, bpl)

    # Strip any row-padding bytes, then swap R<->B (RGB -> BGR for TGA)
    bgr = raw[:, : w * 3].reshape(h, w, 3)[:, :, ::-1].copy()

    # 18-byte TGA header
    header = struct.pack(
        "<BBBHHBHHHHBB",
        0,    # ID field length  (no ID string)
        0,    # colour-map type  (no palette)
        2,    # image type       (uncompressed true-colour)
        0,    # colour-map first entry index
        0,    # colour-map length
        0,    # colour-map entry size
        0,    # X origin
        0,    # Y origin
        w,    # image width
        h,    # image height
        24,   # bits per pixel
        0x20  # image descriptor: top-left origin
    )

    with open(output_path, "wb") as f:
        f.write(header)
        f.write(bgr.tobytes())


# ==============================================================================
# 8. FILTER / VISIBILITY HELPERS
# ==============================================================================

def apply_country_filters(group, sql_adm0, adm2_visible):
    """
    For layers whose name is in FILTERED_LAYER_NAMES, apply:
        "shapeGroup" = '<sql_adm0>'
    Also toggle the border_2 layer's individual visibility according to adm2_visible.
    """
    filter_expr = f'"shapeGroup"=\'{sql_adm0}\''
    for tl in group.findLayers():
        lyr = tl.layer()
        if not lyr:
            continue
        lname = lyr.name()
        if lname in FILTERED_LAYER_NAMES:
            lyr.setSubsetString(filter_expr)
        if lname == "border_2":
            tl.setItemVisibilityChecked(adm2_visible)


def clear_country_filters(group):
    """Remove shapeGroup filters from all relevant layers in *group*."""
    for tl in group.findLayers():
        lyr = tl.layer()
        if lyr and lyr.name() in FILTERED_LAYER_NAMES:
            lyr.setSubsetString("")


# ==============================================================================
# 9. RENDER HELPER
# ==============================================================================

def render_group_to_tga(group, extent_3857, output_path):
    """
    Render all individually-visible layers in *group* to a 24-bit TGA file.

    Uses tl.itemVisibilityChecked() rather than tl.isVisible() so that the
    render succeeds even when the group itself is toggled off in the project UI.
    Layer order from findLayers() (top -> bottom in the panel) matches the order
    expected by QgsMapSettings.setLayers() (index 0 = topmost / drawn last).
    """
    render_layers = [
        tl.layer()
        for tl in group.findLayers()
        if tl.itemVisibilityChecked() and tl.layer()
    ]

    settings = QgsMapSettings()
    settings.setLayers(render_layers)
    settings.setDestinationCrs(crs_3857)
    settings.setExtent(extent_3857)
    settings.setOutputSize(QSize(IMAGE_SIZE, IMAGE_SIZE))

    job = QgsMapRendererSequentialJob(settings)
    job.start()
    job.waitForFinished()

    image = job.renderedImage()
    if image.isNull():
        print(f"  ERROR: Rendered image is null -- skipping {os.path.basename(output_path)}")
        return False

    save_tga_24bit(image, output_path)
    return True


# ==============================================================================
# 10. MAIN EXPORT LOOP
# ==============================================================================

for i, entry in enumerate(entries, 1):
    name = entry["name"]
    fid  = entry["id"]
    adm0 = entry["adm0"]
    adm2 = entry["adm2"]

    # Escape single quotes for SQL (e.g. "Cote d'Ivoire" -> "Cote d''Ivoire")
    sql_adm0 = adm0.replace("'", "''")

    print(f"[{i}/{len(entries)}] {name}  (id={fid}, ADM0={adm0}, ADM2={int(adm2)})")

    # ------------------------------------------------------------------
    # 10a. Compute bounding box from bbox_layer filtered by shapeGroup
    # ------------------------------------------------------------------
    bbox_layer.setSubsetString(f'"shapeGroup"=\'{sql_adm0}\'')
    geoms = [feat.geometry() for feat in bbox_layer.getFeatures()]
    bbox_layer.setSubsetString("")   # Reset immediately after reading

    if not geoms:
        print(f"  WARNING: No geometry found for shapeGroup='{adm0}'. Skipping.")
        continue

    # Merge all features (handles multi-part countries / island groups)
    country_geom = QgsGeometry.unaryUnion(geoms)
    country_geom.transform(transform_to_3857)
    raw_extent = country_geom.boundingBox()

    max_dim = max(raw_extent.width(), raw_extent.height())
    padding = compute_padding(max_dim)
    half    = (max_dim * padding) / 2.0
    center  = raw_extent.center()

    extent_3857 = QgsRectangle(
        center.x() - half, center.y() - half,
        center.x() + half, center.y() + half,
    )
    print(f"  Longest axis: {max_dim / 1000:.0f} km  ->  padding factor: {padding:.4f}")

    # ------------------------------------------------------------------
    # 10b. Apply filters and border_2 visibility in both groups
    # ------------------------------------------------------------------
    apply_country_filters(light_group, sql_adm0, adm2)
    apply_country_filters(dark_group,  sql_adm0, adm2)

    # ------------------------------------------------------------------
    # 10c. Render Light and Dark modes
    # ------------------------------------------------------------------
    render_group_to_tga(light_group, extent_3857, os.path.join(light_dir, f"{fid}.tga"))
    render_group_to_tga(dark_group,  extent_3857, os.path.join(dark_dir,  f"{fid}.tga"))

    # ------------------------------------------------------------------
    # 10d. Write metadata file  (lon_min / lon_max / lat_min / lat_max)
    #      Re-project the square extent back to EPSG:4326 degrees.
    # ------------------------------------------------------------------
    sq_geom = QgsGeometry.fromRect(extent_3857)
    sq_geom.transform(transform_to_4326)
    ext_4326 = sq_geom.boundingBox()

    txt_path = os.path.join(OUTPUT_FOLDER, f"{fid}.txt")
    with open(txt_path, "w", encoding="utf-8") as txt:
        txt.write(f"{round(ext_4326.xMinimum(), 6)}\n")
        txt.write(f"{round(ext_4326.xMaximum(), 6)}\n")
        txt.write(f"{round(ext_4326.yMinimum(), 6)}\n")
        txt.write(f"{round(ext_4326.yMaximum(), 6)}\n")

    print(f"  -> light/{fid}.tga   dark/{fid}.tga   {fid}.txt")


# ==============================================================================
# 11. CLEANUP
# ==============================================================================

clear_country_filters(light_group)
clear_country_filters(dark_group)

# Restore border_2 visibility to ON in both groups
for grp in (light_group, dark_group):
    tl, _ = find_tree_layer(grp, "border_2")
    if tl:
        tl.setItemVisibilityChecked(True)

print(f"\nFinished! {len(entries)} countries exported to: {OUTPUT_FOLDER}")
