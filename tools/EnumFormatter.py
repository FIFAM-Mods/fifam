import sys
import re
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, 
    QCheckBox, QLabel, QLineEdit, QTextEdit
)

class EnumFormatterApp(QWidget):
    def __init__(self):
        super().__init__()
        self.init_ui()

    def init_ui(self):
        self.setWindowTitle("C++ Enum Formatter")
        self.resize(800, 500)

        main_layout = QVBoxLayout()

        # --- 1. Checkboxes (Top) ---
        chk_layout = QHBoxLayout()
        
        self.chk_enum = QCheckBox("Format as enum")
        self.chk_enum.setChecked(True)
        
        self.chk_numbers = QCheckBox("Add numbers")
        self.chk_numbers.setChecked(True)
        
        self.chk_hex = QCheckBox("Hexadecimal numbers")
        self.chk_hex.setChecked(False)
        
        self.chk_upper = QCheckBox("Uppercase")
        self.chk_upper.setChecked(True)

        chk_layout.addWidget(self.chk_enum)
        chk_layout.addWidget(self.chk_numbers)
        chk_layout.addWidget(self.chk_hex)
        chk_layout.addWidget(self.chk_upper)
        main_layout.addLayout(chk_layout)

        # --- 2. Single-line Textboxes (Name & Prefix) ---
        param_layout = QHBoxLayout()
        
        self.edit_name = QLineEdit()
        self.edit_prefix = QLineEdit()
        
        param_layout.addWidget(QLabel("Name:"))
        param_layout.addWidget(self.edit_name)
        param_layout.addWidget(QLabel("Prefix:"))
        param_layout.addWidget(self.edit_prefix)
        main_layout.addLayout(param_layout)

        # --- 3. Multiline Input/Output (Horizontally Stacked) ---
        text_layout = QHBoxLayout()
        
        self.text_in = QTextEdit()
        self.text_in.setPlaceholderText("Paste your list here...\nExample:\nRelegationClinchedBeforeLastMatchday\nFIFAWorldPlayerReigning")
        
        self.text_out = QTextEdit()
        self.text_out.setReadOnly(True)
        self.text_out.setStyleSheet("background-color: #f4f4f4; font-family: monospace;")
        
        text_layout.addWidget(self.text_in)
        text_layout.addWidget(self.text_out)
        main_layout.addLayout(text_layout)

        self.setLayout(main_layout)

        # --- 4. Connect Signals to Slot ---
        self.chk_enum.stateChanged.connect(self.update_output)
        self.chk_numbers.stateChanged.connect(self.update_output)
        self.chk_hex.stateChanged.connect(self.update_output)
        self.chk_upper.stateChanged.connect(self.update_output)
        
        self.edit_name.textChanged.connect(self.update_output)
        self.edit_prefix.textChanged.connect(self.update_output)
        
        self.text_in.textChanged.connect(self.update_output)

    def update_output(self):
        input_text = self.text_in.toPlainText()
        
        # If there's no input, clear output and stop
        if not input_text.strip():
            self.text_out.setPlainText("")
            return

        lines = input_text.split('\n')
        formatted_lines = []

        # Read UI states
        format_enum = self.chk_enum.isChecked()
        add_numbers = self.chk_numbers.isChecked()
        hex_numbers = self.chk_hex.isChecked()
        uppercase = self.chk_upper.isChecked()
        prefix = self.edit_prefix.text()
        name = self.edit_name.text().strip()

        enum_index = 0
        
        for line in lines:
            line = line.strip()
            
            # Skip empty lines so we don't format them or increment numbers
            if not line:
                continue

            # Step 1: Replace spaces with underscores
            line = line.replace(' ', '_')

            # Step 2: Add underscores between CamelCase words
            # Rule A: Lowercase followed by Uppercase (e.g., Matchday -> Match_day)
            line = re.sub(r'([a-z])([A-Z])', r'\1_\2', line)
            
            # Rule B: Multiple Capitals followed by Lowercase (e.g., FIFAWorld -> FIFA_World)
            line = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', line)

            # Step 3: Remove consecutive underscores
            line = re.sub(r'_+', '_', line)

            # Step 4: Apply Prefix
            if prefix:
                line = prefix + line

            # Step 5: Apply Uppercase
            if uppercase:
                line = line.upper()

            # Step 6: Append numbers and C++ formatting
            line_suffix = ""
            if add_numbers:
                if hex_numbers:
                    # Format as uppercase hexadecimal
                    line_suffix += f" = 0x{enum_index:X}"
                else:
                    line_suffix += f" = {enum_index}"
                enum_index += 1

            if format_enum:
                line_suffix += ","
                line = "    " + line # 4 spaces for indentation

            formatted_lines.append(line + line_suffix)

        # --- Final Assembly ---
        if format_enum:
            header = f"enum {name} {{" if name else "enum {"
            footer = "};"
            result = header + "\n" + "\n".join(formatted_lines) + "\n" + footer
        else:
            result = "\n".join(formatted_lines)

        self.text_out.setPlainText(result)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = EnumFormatterApp()
    window.show()
    sys.exit(app.exec())