import os
import re
import shutil
from pathlib import Path
from PIL import Image

def main():
    # 1. Define paths and create the output folder
    current_dir = Path.cwd()
    out_dir = current_dir / "out"
    
    # Create "out" folder if it doesn't exist
    out_dir.mkdir(exist_ok=True)
    print(f"Output directory ready at: {out_dir}")

    # Regex pattern to match folders like 'ball_1', 'ball_42', etc.
    folder_pattern = re.compile(r"^ball_(\d+)$")

    # 2. Scan folders in the current directory
    for item in current_dir.iterdir():
        if item.is_dir():
            match = folder_pattern.match(item.name)
            if match:
                ball_id = match.group(1)
                print(f"\nProcessing {item.name} (ID: {ball_id})...")
                
                # Define source files
                color_src = item / f"ball_{ball_id}_color.png"
                coeff_src = item / f"ball_{ball_id}_coeff.png"
                normal_src = item / f"ball_{ball_id}_normal.png"
                mesh_src = item / f"ball_{ball_id}_mesh.fbx"

                # --- STEP A: Copy color texture ---
                if color_src.exists():
                    color_dst = out_dir / f"ball@t99__{ball_id}.png"
                    shutil.copy2(color_src, color_dst)
                    print(f"  Copied: {color_src.name} -> {color_dst.name}")
                else:
                    print(f"  [!] Missing: {color_src.name}")

                # --- STEP B: Combine coeff and normal textures ---
                if coeff_src.exists() and normal_src.exists():
                    try:
                        # Open images and ensure they have RGBA channels
                        coeff_img = Image.open(coeff_src).convert("RGBA")
                        normal_img = Image.open(normal_src).convert("RGBA")

                        # Ensure both images are the same size before merging
                        if coeff_img.size == normal_img.size:
                            # Split into individual channels
                            c_R, c_G, c_B, c_A = coeff_img.split()
                            n_R, n_G, n_B, n_A = normal_img.split()

                            # Combine channels as requested:
                            # R - coeff's G, G - normal's G, B - coeff's B, A - normal's R
                            new_img = Image.merge("RGBA", (c_G, n_G, c_B, n_R))
                            
                            # Save the new composite image
                            baln_dst = out_dir / f"baln@t99__{ball_id}.png"
                            new_img.save(baln_dst)
                            print(f"  Merged: {coeff_src.name} & {normal_src.name} -> {baln_dst.name}")
                        else:
                            print(f"  [!] Size mismatch: {coeff_src.name} and {normal_src.name} must be the same resolution.")
                    except Exception as e:
                        print(f"  [!] Error processing images for ID {ball_id}: {e}")
                else:
                    print(f"  [!] Missing coeff or normal texture for ID {ball_id}, skipping merge.")

                # --- STEP C: Copy and rename FBX files ---
                if mesh_src.exists():
                    fbx_dst1 = out_dir / f"m222__{ball_id}.fbx"
                    fbx_dst2 = out_dir / f"m316__{ball_id}.fbx"
                    
                    shutil.copy2(mesh_src, fbx_dst1)
                    shutil.copy2(mesh_src, fbx_dst2)
                    print(f"  Copied: {mesh_src.name} -> {fbx_dst1.name}")
                    print(f"  Copied: {mesh_src.name} -> {fbx_dst2.name}")
                else:
                    print(f"  [!] Missing: {mesh_src.name}")

    print("\nProcessing complete!")

if __name__ == "__main__":
    main()