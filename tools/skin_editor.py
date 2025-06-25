import os
import glob
import tkinter as tk
import re
from tkinter import Label, Button
from PIL import Image, ImageTk
from PIL import UnidentifiedImageError

# Folder paths
IMAGE_FOLDER = r"D:\Projects\FIFA\heads\inter\23"
SKIN_FOLDER  = r"D:\Projects\FIFA\heads\in\skin"

def natural_sort_key(s):
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(r'(\d+)', os.path.basename(s))]

def sorted_file_list(folder):
    return sorted(glob.glob(os.path.join(folder, "*.tga")), key=natural_sort_key)

class TGAViewer:
    def __init__(self, root):
        self.root = root
        self.root.title("TGA Viewer")

        self.image_files = sorted_file_list(IMAGE_FOLDER)
        self.current_index = 0
        self.skin = None          # currently selected skin ID (int or None)
        self.skin_colors = self._load_skin_colors()
        self.auto_detect_skin = True

        if not self.image_files:
            print("No TGA images found.")
            return

        # ---------- UI ----------
        self.label = Label(root)               # image preview
        self.label.pack()

        self.filename_label = Label(root, text="", font=("Arial", 12))
        self.filename_label.pack()

        self.btn_left  = Button(root, text="<", command=self.prev_image)
        self.btn_left.pack(side=tk.LEFT)

        self.btn_right = Button(root, text=">", command=self.next_image)
        self.btn_right.pack(side=tk.RIGHT)

        self.btn_plus  = Button(root, text="+", command=self.increase_skin)
        self.btn_plus.pack()

        self.btn_minus = Button(root, text="-", command=self.decrease_skin)
        self.btn_minus.pack()

        # **NEW** — live skin indicator
        self.skin_label = Label(root, text="Skin: –", font=("Arial", 12))
        self.skin_label.pack()     # directly above “Apply”

        self.btn_apply = Button(root, text="Apply", command=self.apply_changes)
        self.btn_apply.pack()

        # ---------- key bindings ----------
        self.root.bind("<Left>",  lambda _: self.prev_image())
        self.root.bind("<Right>", lambda _: self.next_image())
        self.root.bind("<Return>",lambda _: self.apply_changes())

        # **NEW** — up / down arrows to change skin
        self.root.bind("<Up>",    lambda _: self.increase_skin())
        self.root.bind("<Down>",  lambda _: self.decrease_skin())

        self.display_image()
        
    def _load_skin_colors(self):
        colors = {}
        for i in range(1, 11):
            skin_path = os.path.join(SKIN_FOLDER, f"{i}.tga")
            if os.path.exists(skin_path):
                try:
                    with Image.open(skin_path).convert("RGB") as img:
                        top_left_pixel = img.getpixel((0, 0))
                        colors[i] = top_left_pixel
                except UnidentifiedImageError:
                    print(f"Warning: Couldn't read skin {i}.tga")
        return colors

    # ---------- helpers ----------
    def _update_skin_label(self):
        txt = f"Skin: {self.skin}" if self.skin else "Skin: –"
        self.skin_label.config(text=txt)

    def display_image(self):
        img_path = self.image_files[self.current_index]
        img = Image.open(img_path).convert("RGBA")
    
        if self.auto_detect_skin:
            top_left_rgb = img.getpixel((0, 0))[:3]
            matched_skin = None
            for skin_id, skin_rgb in self.skin_colors.items():
                if top_left_rgb == skin_rgb:
                    matched_skin = skin_id
                    break
            self.skin = matched_skin
            self.auto_detect_skin = False  # don't auto-detect again
    
        if self.skin is not None:
            skin_path = os.path.join(SKIN_FOLDER, f"{self.skin}.tga")
            if os.path.exists(skin_path):
                skin_img = Image.open(skin_path).convert("RGBA")
                skin_img = skin_img.crop((0, 0, 512, 512))
                img.paste(skin_img, (0, 0), skin_img)
    
        self.tk_image = ImageTk.PhotoImage(img)
        self.label.config(image=self.tk_image)
        self.filename_label.config(text=os.path.basename(img_path))
        self._update_skin_label()

    # ---------- navigation ----------
    def prev_image(self):
        self.current_index = (self.current_index - 1) % len(self.image_files)
        self.skin = None
        self.auto_detect_skin = True
        self.display_image()
    
    def next_image(self):
        self.current_index = (self.current_index + 1) % len(self.image_files)
        self.skin = None
        self.auto_detect_skin = True
        self.display_image()

    # ---------- skin selection ----------
    def increase_skin(self):
        if self.skin is None:
            self.skin = 1
        elif self.skin < 10:
            self.skin += 1
        self.display_image()

    def decrease_skin(self):
        if self.skin is None:
            self.skin = 1  # start at 1 if user presses Down first
        elif self.skin > 1:
            self.skin -= 1
        self.display_image()

    # ---------- save ----------
    def apply_changes(self):
        img_path = self.image_files[self.current_index]
        img = Image.open(img_path).convert("RGBA")

        if self.skin is not None:
            skin_path = os.path.join(SKIN_FOLDER, f"{self.skin}.tga")
            if os.path.exists(skin_path):
                skin_img = Image.open(skin_path).convert("RGBA")
                skin_img = skin_img.crop((0, 0, 512, 512))
                img.paste(skin_img, (0, 0), skin_img)

        img.save(img_path)
        print(f"Image saved: {img_path}")

if __name__ == "__main__":
    root = tk.Tk()
    viewer = TGAViewer(root)
    root.mainloop()
