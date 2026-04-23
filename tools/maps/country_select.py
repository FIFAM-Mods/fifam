from qgis.core import QgsProject, QgsGeometry, QgsCoordinateTransform
from qgis.utils import iface

# ==============================================================================
# 1. CONFIGURATION
# ==============================================================================

COUNTRY_TO_SELECT = "ITA"  # Set your ADM0 value here

LIGHT_GROUP_NAME = "Light Mode"
DARK_GROUP_NAME  = "Dark Mode"

# Layers inside each group that receive a per-country "shapeGroup" SQL filter
FILTERED_LAYER_NAMES = {"border_active", "border_1", "border_2", "country_active"}

# Which layer to use for bounding-box calculation (resolved from the Light group)
BBOX_SOURCE_LAYER = "country_active"

# ==============================================================================
# 2. SETUP & HELPERS
# ==============================================================================

project = QgsProject.instance()
tree_root = project.layerTreeRoot()

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

def apply_country_filters(group, sql_adm0):
    """Apply the 'shapeGroup' filter to the defined layers in the group."""
    filter_expr = f'"shapeGroup"=\'{sql_adm0}\''
    for tl in group.findLayers():
        lyr = tl.layer()
        if lyr and lyr.name() in FILTERED_LAYER_NAMES:
            lyr.setSubsetString(filter_expr)

# ==============================================================================
# 3. APPLY FILTERS & ZOOM
# ==============================================================================

light_group = get_group(LIGHT_GROUP_NAME)
dark_group  = get_group(DARK_GROUP_NAME)

# Escape single quotes for SQL (e.g. "Cote d'Ivoire" -> "Cote d''Ivoire")
sql_adm0 = COUNTRY_TO_SELECT.replace("'", "''")

# Apply the subset string filters to both groups
apply_country_filters(light_group, sql_adm0)
apply_country_filters(dark_group, sql_adm0)

# Resolve the bbox layer to figure out where to zoom
_, bbox_layer = find_tree_layer(light_group, BBOX_SOURCE_LAYER)

if bbox_layer:
    # Gather geometries for the selected country
    geoms = [feat.geometry() for feat in bbox_layer.getFeatures()]
    
    if geoms:
        # Merge all features (handles multi-part countries / island groups)
        country_geom = QgsGeometry.unaryUnion(geoms)
        
        # Access the main QGIS map canvas
        canvas = iface.mapCanvas()
        
        # --- NEW: Handle CRS Transformation ---
        layer_crs = bbox_layer.crs()
        canvas_crs = canvas.mapSettings().destinationCrs()
        
        # If the layer and the canvas are using different coordinate systems, transform the geometry
        if layer_crs != canvas_crs:
            transform = QgsCoordinateTransform(layer_crs, canvas_crs, project)
            country_geom.transform(transform)
        # --------------------------------------
        
        # Zoom to the bounding box of the transformed geometry
        canvas.setExtent(country_geom.boundingBox())
        
        # Apply a slight 10% zoom out so the borders don't touch the absolute edges of the screen
        canvas.zoomScale(canvas.scale() * 1.1)
        canvas.refresh()
        
        print(f"Successfully filtered and zoomed to {COUNTRY_TO_SELECT}.")
    else:
        print(f"WARNING: No geometry found for shapeGroup='{COUNTRY_TO_SELECT}'. Filters applied, but could not zoom.")
else:
    print(f"WARNING: Bounding-box source layer '{BBOX_SOURCE_LAYER}' not found. Filters applied, but could not zoom.")