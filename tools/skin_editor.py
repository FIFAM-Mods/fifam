import os
import glob
import tkinter as tk
import re
from tkinter import Label, Button
from PIL import Image, ImageTk

# Folder paths
IMAGE_FOLDER = r"D:\Projects\FIFA\heads\inter\23"
SKIN_FOLDER = r"D:\Projects\FIFA\heads\in\skin"

def natural_sort_key(s):
    """Sort filenames in a way that mimics Windows Explorer ordering."""
    return [int(text) if text.isdigit() else text.lower() for text in re.split(r'(\d+)', os.path.basename(s))]

def sorted_file_list(folder):
    """Returns a naturally sorted list of files as displayed by Windows Explorer."""
    return sorted(glob.glob(os.path.join(folder, "*.tga")), key=natural_sort_key)

class TGAViewer:
    def __init__(self, root):
        self.root = root
        self.root.title("TGA Viewer")
        
        # Find all .tga images and sort them naturally
        self.image_files = sorted_file_list(IMAGE_FOLDER)
        self.current_index = 0
        self.skin = None  # Initially, no skin overlay
        
        if not self.image_files:
            print("No TGA images found.")
            return
        
        self.label = Label(root)
        self.label.pack()
        
        self.filename_label = Label(root, text="", font=("Arial", 12))
        self.filename_label.pack()
        
        self.btn_left = Button(root, text="<", command=self.prev_image)
        self.btn_left.pack(side=tk.LEFT)
        
        self.btn_right = Button(root, text=">", command=self.next_image)
        self.btn_right.pack(side=tk.RIGHT)
        
        self.btn_plus = Button(root, text="+", command=self.increase_skin)
        self.btn_plus.pack()
        
        self.btn_minus = Button(root, text="-", command=self.decrease_skin)
        self.btn_minus.pack()
        
        self.btn_apply = Button(root, text="Apply", command=self.apply_changes)
        self.btn_apply.pack()
        
        self.root.bind("<Left>", lambda e: self.prev_image())
        self.root.bind("<Right>", lambda e: self.next_image())
        self.root.bind("<Return>", lambda e: self.apply_changes())
        
        self.display_image()
    
    def display_image(self):
        """Load and display the current image with the skin overlay if applicable."""
        img_path = self.image_files[self.current_index]
        img = Image.open(img_path).convert("RGBA")
        
        if self.skin is not None:
            skin_path = os.path.join(SKIN_FOLDER, f"{self.skin}.tga")
            if os.path.exists(skin_path):
                skin_img = Image.open(skin_path).convert("RGBA")
                skin_img = skin_img.crop((0, 0, 512, 512))
                img.paste(skin_img, (0, 0), skin_img)
        
        self.tk_image = ImageTk.PhotoImage(img)
        self.label.config(image=self.tk_image)
        self.filename_label.config(text=os.path.basename(img_path))
    
    def prev_image(self):
        """Go to the previous image."""
        self.current_index = (self.current_index - 1) % len(self.image_files)
        self.skin = None  # Reset skin overlay
        self.display_image()
    
    def next_image(self):
        """Go to the next image."""
        self.current_index = (self.current_index + 1) % len(self.image_files)
        self.skin = None  # Reset skin overlay
        self.display_image()
    
    def increase_skin(self):
        """Increase skin value."""
        if self.skin is None:
            self.skin = 1
        elif self.skin < 10:
            self.skin += 1
        self.display_image()
    
    def decrease_skin(self):
        """Decrease skin value."""
        if self.skin is None:
            self.skin = 1
        elif self.skin > 1:
            self.skin -= 1
        self.display_image()
    
    def apply_changes(self):
        """Save the displayed image back to its original location."""
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
