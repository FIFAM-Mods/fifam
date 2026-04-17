import sys
import os
import re
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QHBoxLayout,
                             QPushButton, QLineEdit, QFileDialog, QTextBrowser,
                             QLabel, QMessageBox)

class ScannerApp(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('__fastcall Hook Argument Scanner')
        self.resize(900, 600)

        # Main layout
        layout = QVBoxLayout()

        # Top bar layout
        top_layout = QHBoxLayout()
        
        self.dir_input = QLineEdit(os.getcwd())
        
        browse_btn = QPushButton("Browse...")
        browse_btn.clicked.connect(self.browse_folder)
        
        self.scan_btn = QPushButton("Scan")
        self.scan_btn.clicked.connect(self.scan_folder)

        top_layout.addWidget(QLabel("Folder:"))
        top_layout.addWidget(self.dir_input)
        top_layout.addWidget(browse_btn)
        top_layout.addWidget(self.scan_btn)

        # Results area
        self.results_browser = QTextBrowser()
        self.results_browser.setOpenExternalLinks(False)
        self.results_browser.setStyleSheet("font-family: Consolas, monospace; font-size: 13px;")

        # Add to main layout
        layout.addLayout(top_layout)
        layout.addWidget(self.results_browser)

        self.setLayout(layout)

    def browse_folder(self):
        folder = QFileDialog.getExistingDirectory(self, "Select Directory", self.dir_input.text())
        if folder:
            # Convert separators to standard slashes
            self.dir_input.setText(os.path.normpath(folder))

    def scan_folder(self):
        folder_path = self.dir_input.text()
        
        if not os.path.isdir(folder_path):
            QMessageBox.warning(self, "Error", "Invalid directory selected.")
            return

        self.scan_btn.setEnabled(False)
        self.results_browser.clear()
        self.results_browser.append(f"<b>Scanning directory:</b> {folder_path}<br>")
        QApplication.processEvents() # Flush GUI events so it updates immediately

        # \b matches word boundaries (spaces, brackets, etc.)
        method_pattern = re.compile(r'\bMETHOD\b')
        dummy_pattern = re.compile(r'\bDUMMY_ARG\b')
        valid_extensions = {'.cpp', '.hpp', '.h'}

        error_count = 0
        scanned_files = 0

        for root, _, files in os.walk(folder_path):
            for file in files:
                ext = os.path.splitext(file)[1].lower()
                if ext in valid_extensions:
                    filepath = os.path.join(root, file)
                    scanned_files += 1
                    
                    # errors='ignore' safely handles older Windows-1252/latin-1 encodings 
                    # without crashing, since we only care about ASCII macro text anyway.
                    try:
                        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                            for line_num, line in enumerate(f, 1):
                                # 1. Check for METHOD
                                if method_pattern.search(line):
                                    # 2. Check for comma (>1 argument)
                                    if ',' in line:
                                        # 3. Check for missing DUMMY_ARG
                                        if not dummy_pattern.search(line):
                                            error_count += 1
                                            
                                            display_line = line.strip()
                                            # Basic HTML escaping so < > brackets in C++ don't break QTextBrowser
                                            display_line = display_line.replace('<', '&lt;').replace('>', '&gt;')
                                            
                                            self.results_browser.append(
                                                f"<span style='color: #0055ff;'>{filepath}</span> "
                                                f"(Line {line_num}):<br>"
                                                f"<span style='color: #d90000; font-weight: bold;'>{display_line}</span><br>"
                                            )
                    except Exception as e:
                        self.results_browser.append(f"<span style='color: orange;'>Could not read {filepath}: {e}</span><br>")

        self.results_browser.append(f"<br><b>Scan complete.</b> Scanned {scanned_files} files. Found {error_count} potential errors.")
        self.scan_btn.setEnabled(True)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = ScannerApp()
    ex.show()
    sys.exit(app.exec_())