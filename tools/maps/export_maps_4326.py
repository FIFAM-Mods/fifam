import os
import csv
import struct
import numpy as np
import math
from qgis.core import *
from qgis.PyQt.QtCore import QSize
from qgis.PyQt.QtGui import QImage

# ==============================================================================
# 1. CONFIGURATION
# ==============================================================================

OUTPUT_FOLDER = r"C:\Users\User\Desktop\Maps\4326"
CSV_PATH      = r"D:\Projects\fifam\tools\maps\feat.csv"
IMAGE_SIZE    = 1024
WORLD_WIDTH_3857 = 20037508.3427892 * 2   # full equatorial width in EPSG:3857

PADDING_SMALL = 1.10
PADDING_LARGE = 1.02
SIZE_SMALL_KM = 500     # km
SIZE_LARGE_KM = 4000    # km

EXPORT_FILTER = [ "Russia", "Fiji", "New Zealand" ]

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
            
        # 1. Parse custom Padding if the column exists and has a value
        pad_str = row.get("Padding", "").strip()
        try:
            custom_padding = float(pad_str) if pad_str else None
        except ValueError:
            custom_padding = None

        entries.append({
            "name": name,
            "id":   row["Id"].strip(),
            "adm0": row["ADM0"].strip(),
            "adm2": row["ADM2"].strip() == "1",
            "padding": custom_padding
        })

print(f"Loaded {len(entries)} entr{'y' if len(entries) == 1 else 'ies'} to process.")


# ==============================================================================
# 3. PROJECT / CRS SETUP
# ==============================================================================

project   = QgsProject.instance()
tree_root = project.layerTreeRoot()

crs_3857 = QgsCoordinateReferenceSystem("EPSG:3857")
crs_4326 = QgsCoordinateReferenceSystem("EPSG:4326")

def circular_mean_longitude(geom_4326):
    """
    Circular mean of all vertex longitudes — correctly handles the ±180° wrap.
    A straight average would give ~0° for {+178°, −178°}; this gives ~±180°.
    """
    sins, coss = [], []
    for v in geom_4326.vertices():
        rad = math.radians(v.x())
        sins.append(math.sin(rad))
        coss.append(math.cos(rad))
    if not sins:
        return 0.0
    return math.degrees(math.atan2(
        sum(sins) / len(sins),
        sum(coss) / len(coss)
    ))


def resolve_extent_and_crs(geoms):
    """
    Return (extent_in_render_crs, render_crs).

    For most countries: render_crs == crs_3857 (standard Web Mercator).
    For antimeridian-crossing countries (Fiji, Kiribati, …) the EPSG:3857
    bbox spans > half the world width, so we fall back to a custom Mercator
    CRS centred on the country's true circular-mean longitude.
    """
    country_geom = QgsGeometry.unaryUnion(geoms)

    # ── Standard path ────────────────────────────────────────────────────
    geom_3857 = QgsGeometry(country_geom)
    geom_3857.transform(QgsCoordinateTransform(bbox_layer.crs(), crs_3857, project))
    raw_extent = geom_3857.boundingBox()

    if raw_extent.width() <= WORLD_WIDTH_3857 * 0.5:
        return raw_extent, crs_3857

    # ── Antimeridian crossing detected ───────────────────────────────────
    # 1. Find the true centre longitude via the circular mean of all vertices
    geom_4326 = QgsGeometry(country_geom)
    geom_4326.transform(QgsCoordinateTransform(bbox_layer.crs(), crs_4326, project))
    center_lon = circular_mean_longitude(geom_4326)

    # 2. Build a Mercator CRS shifted to that meridian
    proj_str = (
        f"+proj=merc +lon_0={center_lon:.4f} "
        "+x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"
    )
    render_crs = QgsCoordinateReferenceSystem()
    render_crs.createFromProj(proj_str)

    # 3. Recompute the bounding box in the new CRS — now geographically correct
    geom_custom = QgsGeometry(country_geom)
    geom_custom.transform(QgsCoordinateTransform(bbox_layer.crs(), render_crs, project))
    raw_extent = geom_custom.boundingBox()

    print(f"  Antimeridian crossing detected — using custom CRS (lon_0={center_lon:.2f}°)")
    return raw_extent, render_crs


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
    """
    img = qimage.convertToFormat(QImage.Format.Format_RGB888)
    w   = img.width()
    h   = img.height()
    bpl = img.bytesPerLine()   # may be > w*3 due to 4-byte row alignment
    ptr = img.bits()
    ptr.setsize(h * bpl)
    raw = np.frombuffer(ptr, dtype=np.uint8).reshape(h, bpl)

    # Strip any row-padding bytes, then swap R<->B (RGB -> BGR for TGA)
    bgr = raw[:, : w * 3].reshape(h, w, 3)[:, :, ::-1].copy()

    # 18-byte TGA header
    header = struct.pack(
        "<BBBHHBHHHHBB",
        0,
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        w,
        h,
        24,
        0x20
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

def render_group_to_tga(group, extent, render_crs, output_path):
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
    settings.setDestinationCrs(render_crs)
    settings.setExtent(extent)
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
    # 10a. Compute bounding box (antimeridian-safe)
    # ------------------------------------------------------------------
    bbox_layer.setSubsetString(f'"shapeGroup"=\'{sql_adm0}\'')
    geoms = [feat.geometry() for feat in bbox_layer.getFeatures()]
    bbox_layer.setSubsetString("")

    if not geoms:
        print(f"  WARNING: No geometry found for shapeGroup='{adm0}'. Skipping.")
        continue

    raw_extent, render_crs = resolve_extent_and_crs(geoms) 

    max_dim = max(raw_extent.width(), raw_extent.height())
    
    # Use custom padding from CSV if it exists, otherwise compute dynamically
    padding = entry["padding"] if entry["padding"] else compute_padding(max_dim)
    
    half    = (max_dim * padding) / 2.0
    center  = raw_extent.center()

    extent_render = QgsRectangle(
        center.x() - half, center.y() - half,
        center.x() + half, center.y() + half,
    )
    print(f"  Longest axis: {max_dim / 1000:.0f} km  ->  padding factor: {padding:.4f} {'(Override)' if entry['padding'] else ''}")

    # ------------------------------------------------------------------
    # 10b. Calculate and save the QGIS Scale parameter
    # ------------------------------------------------------------------
    # We use a dummy QgsMapSettings initialized exactly like the render to get the scale
    tmp_settings = QgsMapSettings()
    tmp_settings.setDestinationCrs(render_crs)
    tmp_settings.setExtent(extent_render)
    tmp_settings.setOutputSize(QSize(IMAGE_SIZE, IMAGE_SIZE))
    
    scale_denominator = int(round(tmp_settings.scale()))
    scale_str = f"1:{scale_denominator}"
    
    scale_path = os.path.join(OUTPUT_FOLDER, f"scale_{fid}.txt")
    with open(scale_path, "w", encoding="utf-8") as scale_file:
        scale_file.write(scale_str)

    # ------------------------------------------------------------------
    # 10c. Apply filters and border_2 visibility in both groups
    # ------------------------------------------------------------------
    apply_country_filters(light_group, sql_adm0, adm2)
    apply_country_filters(dark_group,  sql_adm0, adm2)

    # ------------------------------------------------------------------
    # 10d. Render — pass render_crs through
    # ------------------------------------------------------------------
    render_group_to_tga(light_group, extent_render, render_crs,
                        os.path.join(light_dir, f"{fid}.tga"))
    render_group_to_tga(dark_group,  extent_render, render_crs,
                        os.path.join(dark_dir,  f"{fid}.tga"))

    # ------------------------------------------------------------------
    # 10e. Metadata — reproject the square back to EPSG:4326
    # ------------------------------------------------------------------
    to_4326 = QgsCoordinateTransform(render_crs, crs_4326, project)
    sq_geom = QgsGeometry.fromRect(extent_render)
    sq_geom.transform(to_4326)
    ext_4326 = sq_geom.boundingBox()

    lon_min_out = ext_4326.xMinimum()
    lon_max_out = ext_4326.xMaximum()
    
    # When the box crosses the antimeridian, xMinimum/xMaximum are swapped
    # and wrapped. Restore the correct left→right order, with the right edge
    # allowed to exceed 180° rather than wrapping to a negative value.
    if lon_max_out - lon_min_out > 180.0:
        lon_min_out, lon_max_out = lon_max_out, lon_min_out + 360.0
    
    txt_path = os.path.join(OUTPUT_FOLDER, f"{fid}.txt")
    with open(txt_path, "w", encoding="utf-8") as txt:
        txt.write(f"{round(lon_min_out, 6)}\n")   # always the LEFT  edge
        txt.write(f"{round(lon_max_out, 6)}\n")   # always the RIGHT edge (may exceed 180)
        txt.write(f"{round(ext_4326.yMinimum(), 6)}\n")
        txt.write(f"{round(ext_4326.yMaximum(), 6)}\n")

    print(f"  -> light/{fid}.tga   dark/{fid}.tga   {fid}.txt   scale_{fid}.txt [{scale_str}]")


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