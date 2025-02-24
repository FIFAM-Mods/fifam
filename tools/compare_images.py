import os
import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QScrollArea, QPushButton, QCheckBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

# Paths to compare
FOLDER1 = r"D:\Projects\fifam\content\fm13\_oe_export\badges\256x256"
FOLDER2 = r"D:\Projects\fifam\output\tests\fifam"

# Get common images in both folders
files1 = set(os.listdir(FOLDER1))
files2 = set(os.listdir(FOLDER2))
common_files = sorted(files1 & files2)  # Only files existing in both folders

selected_files = set()

class ImageComparisonApp(QWidget):
    def __init__(self):
        super().__init__()
        self.init_ui()
    
    def init_ui(self):
        self.setWindowTitle("Image Comparison Tool")
        self.setGeometry(100, 100, 600, 800)  # Adjusted window size for better layout
        
        layout = QVBoxLayout(self)
        
        # Scroll Area Setup
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        
        self.scroll_widget = QWidget()
        self.scroll_layout = QVBoxLayout(self.scroll_widget)
        
        self.scroll_area.setWidget(self.scroll_widget)
        layout.addWidget(self.scroll_area)
        
        self.load_images()
        
        # Save button
        save_button = QPushButton("Save Selected")
        save_button.clicked.connect(self.save_selected_files)
        layout.addWidget(save_button)
    
    def load_images(self):
        """Load images into scrollable area."""
        for filename in common_files:
            path1 = os.path.join(FOLDER1, filename)
            path2 = os.path.join(FOLDER2, filename)
            
            row_layout = QHBoxLayout()
            
            img1_label = QLabel()
            img2_label = QLabel()
            
            pixmap1 = QPixmap(path1)  # No scaling, original 256x256 size
            pixmap2 = QPixmap(path2)  # No scaling, original 256x256 size
            
            img1_label.setPixmap(pixmap1)
            img2_label.setPixmap(pixmap2)
            
            checkbox = QCheckBox(filename)
            checkbox.stateChanged.connect(lambda state, f=filename: self.toggle_selection(f, state))
            
            row_layout.addWidget(img1_label)
            row_layout.addWidget(img2_label)
            row_layout.addWidget(checkbox)
            
            row_layout.setSpacing(10)  # Reduce spacing between items
            
            self.scroll_layout.addLayout(row_layout)
    
    def toggle_selection(self, filename, state):
        if state == Qt.Checked:
            selected_files.add(filename)
        else:
            selected_files.discard(filename)
    
    def save_selected_files(self):
        with open("selected_images.txt", "w") as f:
            for filename in selected_files:
                f.write(filename + "\n")
        print("Saved selected filenames to selected_images.txt")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ImageComparisonApp()
    window.show()
    sys.exit(app.exec_())