import os
from PIL import Image, ImageChops

BASE_DIR = r"D:\Games\fc26_files\content\character\balls"
SIZE = (512, 512)

for folder in os.listdir(BASE_DIR):
    folder_path = os.path.join(BASE_DIR, folder)

    if not os.path.isdir(folder_path):
        continue

    if not folder.startswith("ball_"):
        continue

    try:
        ball_id = folder.split("_", 1)[1]
        int(ball_id)  # validate it's an integer
    except ValueError:
        continue

    color_path = os.path.join(folder_path, f"ball_{ball_id}_color.png")
    cavity_path = os.path.join(folder_path, "cavity.bmp")

    if not os.path.isfile(color_path) or not os.path.isfile(cavity_path):
        continue

    # Load images
    color = Image.open(color_path).convert("RGB")
    cavity = Image.open(cavity_path).convert("RGB")

    # Resize
    color = color.resize(SIZE, Image.LANCZOS)
    cavity = cavity.resize(SIZE, Image.LANCZOS)

    # Multiply blend
    result = ImageChops.multiply(color, cavity)

    # Save
    output_path = os.path.join(folder_path, f"ball@t99__{ball_id}.png")
    result.save(output_path, "PNG")

    print(f"Processed ball_{ball_id}")
