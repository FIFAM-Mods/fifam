import sys
import math
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QPushButton, 
                             QLabel, QFileDialog, QMessageBox)
from PyQt5.QtCore import Qt

class MapMetadataConverter(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('PGW to TXT Map Metadata Converter')
        self.setFixedSize(400, 200)

        layout = QVBoxLayout()

        self.info_label = QLabel("Select a .pgw file to convert to .txt format.\nAssumes standard 1024x1024 map dimensions.", self)
        self.info_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.info_label)

        self.convert_btn = QPushButton('Browse and Convert .pgw', self)
        self.convert_btn.clicked.connect(self.select_and_convert)
        layout.addWidget(self.convert_btn)

        self.setLayout(layout)

    def convert_3857_to_4326(self, x_meters, y_meters):
        """Replicates the C++ Convert3857to4326 function."""
        R = 6378137.0
        out_lon = (x_meters / R) * (180.0 / math.pi)
        out_lat = (2.0 * math.atan(math.exp(y_meters / R)) - (math.pi / 2.0)) * (180.0 / math.pi)
        return out_lon, out_lat

    def select_and_convert(self):
        # 1. Open File Dialog for PGW
        pgw_file, _ = QFileDialog.getOpenFileName(self, "Open PGW File", "", "World Files (*.pgw);;All Files (*)")
        
        if not pgw_file:
            return  # User canceled

        try:
            # 2. Read PGW data
            with open(pgw_file, 'r') as f:
                lines = f.readlines()
            
            if len(lines) < 6:
                raise ValueError("Invalid PGW file. It must contain at least 6 lines.")

            pixel_width = float(lines[0].strip())
            y_rotation = float(lines[1].strip())
            x_rotation = float(lines[2].strip())
            pixel_height = float(lines[3].strip())
            x_center = float(lines[4].strip())
            y_center = float(lines[5].strip())

            # Image dimensions from C++ code
            W = 1024.0
            H = 1024.0

            # 3. Calculate bounds in meters
            x_min_meters = x_center - (pixel_width / 2.0)
            x_max_meters = x_center + (pixel_width * (W - 0.5))
            
            y_max_meters = y_center - (pixel_height / 2.0)
            y_min_meters = y_center + (pixel_height * (H - 0.5))

            # 4. Convert to WGS84
            lon_min, lat_min = self.convert_3857_to_4326(x_min_meters, y_min_meters)
            lon_max, lat_max = self.convert_3857_to_4326(x_max_meters, y_max_meters)

            # 5. Open Save File Dialog for TXT
            default_txt_name = pgw_file.rsplit('.', 1)[0] + '.txt'
            txt_file, _ = QFileDialog.getSaveFileName(self, "Save TXT File", default_txt_name, "Text Files (*.txt)")
            
            if not txt_file:
                return  # User canceled saving

            # 6. Write output
            with open(txt_file, 'w') as f:
                # Writing exactly in the order the C++ code reads it:
                # lon_min, lon_max, lat_min, lat_max
                f.write(f"{lon_min}\n")
                f.write(f"{lon_max}\n")
                f.write(f"{lat_min}\n")
                f.write(f"{lat_max}\n")

            QMessageBox.information(self, "Success", f"Successfully converted and saved to:\n{txt_file}")

        except Exception as e:
            QMessageBox.critical(self, "Error", f"An error occurred during conversion:\n{str(e)}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MapMetadataConverter()
    ex.show()
    sys.exit(app.exec_())