import sys
import os
import glob
import csv
from PyQt5.QtWidgets import (
    QApplication, QWidget, QLabel, QVBoxLayout, QHBoxLayout,
    QComboBox, QListWidget, QListWidgetItem, QScrollArea, QAbstractItemView
)
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtCore import Qt
from PIL import Image
import pandas as pd
from color import Color, ColorPair
from PyQt5.QtWidgets import QLineEdit
from PyQt5.QtGui import QColor, QPalette

KIT_FOLDER = r"D:\Projects\fifam\content\fm13\art_04\ucp_popups\colors\vert"
CSV_FILE = r"D:\Projects\fifam\db\fifam-uids.csv"

KIT_TYPE_MAP = {'_h': 'Home', '_a': 'Away', '_t': 'Third'}

class Kit:
    def __init__(self, team_id, kit_type, image_path, team_name):
        self.team_id = team_id
        self.kit_type = kit_type
        self.image_path = image_path
        self.team_name = team_name
        self.image = Image.open(image_path)
        self.first_color = self.get_pixel_color(0, 0)
        self.second_color = self.get_pixel_color(0, 32)
        self.color_pair = ColorPair(Color(*self.first_color), Color(*self.second_color))

    def get_pixel_color(self, x, y):
        r, g, b = self.image.getpixel((x, y))[:3]
        return r, g, b

    def display_name(self):
        return f"{self.team_name} ({KIT_TYPE_MAP.get(self.kit_type, 'Unknown')})"

class KitComparator(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Football Kit Color Comparator")
        self.resize(900, 600)
        self.kits = self.load_kits()
        self.init_ui()

    def load_kits(self):
        df = pd.read_csv(CSV_FILE, encoding='utf-8-sig')
    
        team_name_map = {}
        for _, row in df.iterrows():
            raw_id = row.get('FIFAManagerID', None)
            name = row.get('Name', "Unknown")
            try:
                if isinstance(raw_id, str) and raw_id.startswith("0x"):
                    team_id = raw_id[2:].upper().zfill(8)
                elif isinstance(raw_id, float) and not pd.isna(raw_id):
                    team_id = f"{int(raw_id):08X}"
                else:
                    continue
                team_name_map[team_id] = name
            except Exception as e:
                print(f"Skipping row due to error: {e}")
                continue

        kit_files = glob.glob(os.path.join(KIT_FOLDER, "*.png"))
        kits = []
        for file_path in kit_files:
            filename = os.path.basename(file_path)
            if len(filename) < 10:
                continue
            base, _ = os.path.splitext(filename)
            team_id = base[:8].upper()
            
            if team_id.endswith("FFFF"):
                continue
            
            kit_type = base[8:]
            team_name = team_name_map.get(team_id, f"Unknown ({team_id})")
            if kit_type in KIT_TYPE_MAP:
                try:
                    kits.append(Kit(team_id, kit_type, file_path, team_name))
                except Exception as e:
                    print(f"Failed to load kit {file_path}: {e}")
        return kits

    def init_ui(self):
        layout = QVBoxLayout()
        self.combo = QComboBox()
        self.combo.addItems([kit.display_name() for kit in self.kits])
        self.combo.currentIndexChanged.connect(self.update_comparison)
        layout.addWidget(self.combo)
        
        self.threshold = 35.0
        self.threshold_input = QLineEdit(str(self.threshold))
        self.threshold_input.setPlaceholderText("Distance threshold")
        self.threshold_input.editingFinished.connect(self.on_threshold_changed)
        layout.addWidget(self.threshold_input)

        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.scroll_content = QWidget()
        self.scroll_layout = QVBoxLayout()
        self.scroll_content.setLayout(self.scroll_layout)
        self.scroll_area.setWidget(self.scroll_content)
        layout.addWidget(self.scroll_area)

        self.setLayout(layout)
        self.update_comparison(0)
        
    def on_threshold_changed(self):
        try:
            value = float(self.threshold_input.text())
            self.threshold = value
        except ValueError:
            self.threshold_input.setText(str(self.threshold))
        self.update_comparison(self.combo.currentIndex())

    def update_comparison(self, index):
        while self.scroll_layout.count():
            child = self.scroll_layout.takeAt(0)
            if child.widget():
                child.widget().deleteLater()
    
        base_kit = self.kits[index]
    
        for other_kit in self.kits:
            if other_kit == base_kit:
                continue
            distance = ColorPair.distance(base_kit.color_pair, other_kit.color_pair)
            item_widget = self.create_kit_comparison_widget(base_kit, other_kit, distance)
            self.scroll_layout.addWidget(item_widget)

    def create_kit_comparison_widget(self, base_kit, other_kit, distance):
        widget = QWidget()
        layout = QHBoxLayout()
    
        pix1 = QPixmap(base_kit.image_path).scaled(64, 64)
        pix2 = QPixmap(other_kit.image_path).scaled(64, 64)
        label1 = QLabel()
        label2 = QLabel()
        label1.setPixmap(pix1)
        label2.setPixmap(pix2)
    
        name_label = QLabel(other_kit.display_name())
        distance_label = QLabel(f"{distance:.2f}")
        distance_label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
    
        layout.addWidget(label1)
        layout.addWidget(label2)
        layout.addWidget(name_label, 1)
        layout.addWidget(distance_label)
        widget.setLayout(layout)

        if distance < self.threshold:
            palette = widget.palette()
            palette.setColor(QPalette.Window, QColor("#FFC0CB"))
            widget.setAutoFillBackground(True)
            widget.setPalette(palette)
    
        return widget

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = KitComparator()
    window.show()
    sys.exit(app.exec_())
