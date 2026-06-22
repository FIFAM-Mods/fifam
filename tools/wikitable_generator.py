import sys
import pandas as pd
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, 
                             QTabWidget, QPlainTextEdit, QPushButton, 
                             QFileDialog, QLabel, QSplitter)
from PyQt5.QtCore import Qt

class WikiTableConverter(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Excel to Wikitable Converter')
        self.resize(800, 600)

        # Main layout
        layout = QVBoxLayout(self)

        # QSplitter for dynamic resizing
        splitter = QSplitter(Qt.Vertical)

        # ----------------------------------------------------
        # TOP PART: Input Section
        # ----------------------------------------------------
        self.tabs = QTabWidget()
        
        # Tab 1: Raw Text Input
        self.tab_text = QWidget()
        tab_text_layout = QVBoxLayout()
        # Using QPlainTextEdit ensures only raw, unformatted text is pasted
        self.text_input = QPlainTextEdit()
        self.text_input.setPlaceholderText("Paste your tab-separated text directly from Excel here...")
        self.text_input.textChanged.connect(self.process_text_input)
        tab_text_layout.addWidget(self.text_input)
        self.tab_text.setLayout(tab_text_layout)

        # Tab 2: Excel File Input
        self.tab_file = QWidget()
        tab_file_layout = QVBoxLayout()
        self.file_btn = QPushButton("Select Excel File (.xlsx / .xls)")
        self.file_btn.clicked.connect(self.process_file_input)
        self.file_label = QLabel("No file selected.")
        self.file_label.setAlignment(Qt.AlignCenter)
        
        tab_file_layout.addStretch()
        tab_file_layout.addWidget(self.file_btn)
        tab_file_layout.addWidget(self.file_label)
        tab_file_layout.addStretch()
        self.tab_file.setLayout(tab_file_layout)

        self.tabs.addTab(self.tab_text, "Paste Tab-Separated Text")
        self.tabs.addTab(self.tab_file, "Select Excel File")

        splitter.addWidget(self.tabs)

        # ----------------------------------------------------
        # BOTTOM PART: Output Section
        # ----------------------------------------------------
        # Using QPlainTextEdit for raw text output as well
        self.output_text = QPlainTextEdit()
        self.output_text.setReadOnly(True)
        self.output_text.setPlaceholderText("Your Wikitable syntax will appear here...")
        
        splitter.addWidget(self.output_text)
        
        splitter.setSizes([300, 300])

        layout.addWidget(splitter)
        self.setLayout(layout)

    def generate_wikitable(self, headers, rows):
        """Helper method to format data into MediaWiki table syntax."""
        if not headers and not rows:
            return ""

        wiki_str = '{| class="wikitable sortable"\n|+\n'

        if headers:
            for header in headers:
                wiki_str += f'!{header}\n'

        for row in rows:
            wiki_str += '|-\n'
            for cell in row:
                wiki_str += f'|{cell}\n'

        wiki_str += '|}'
        return wiki_str

    def process_text_input(self):
        """Processes raw text pasted from Excel."""
        raw_text = self.text_input.toPlainText().strip('\r\n')
        
        if not raw_text:
            self.output_text.clear()
            return

        lines = raw_text.split('\n')
        
        headers = lines[0].split('\t')
        rows = [line.split('\t') for line in lines[1:]]
        
        wiki_output = self.generate_wikitable(headers, rows)
        self.output_text.setPlainText(wiki_output)

    def process_file_input(self):
        """Processes an imported Excel file using pandas."""
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getOpenFileName(
            self, 
            "Open Excel File", 
            "", 
            "Excel Files (*.xlsx *.xls);;All Files (*)", 
            options=options
        )
        
        if file_path:
            self.file_label.setText(f"Loaded: {file_path}")
            try:
                df = pd.read_excel(file_path).fillna('')
                headers = [str(col) for col in df.columns]
                rows = df.values.tolist()
                
                wiki_output = self.generate_wikitable(headers, rows)
                self.output_text.setPlainText(wiki_output)
                
            except Exception as e:
                self.output_text.setPlainText(f"Error reading Excel file:\n{str(e)}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = WikiTableConverter()
    window.show()
    sys.exit(app.exec_())