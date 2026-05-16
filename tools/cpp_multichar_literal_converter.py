import sys
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont, QIntValidator
from PyQt5.QtWidgets import (
    QApplication,
    QWidget,
    QLabel,
    QLineEdit,
    QGridLayout,
    QMessageBox,
)


def multichar_to_u32(text: str) -> int:
    """Convert exactly 4 characters to a 32-bit value in MSVC-style order.

    C++ multi-character literals are implementation-defined. This function
    mirrors the common Windows/MSVC packing used for 4-character literals:

        'ABCD' -> 0x41424344

    The displayed hex bytes below are shown in little-endian order, matching
    default Windows 32-bit memory layout.
    """
    if len(text) != 4:
        raise ValueError("Please enter exactly 4 characters.")

    try:
        data = text.encode("latin-1")
    except UnicodeEncodeError:
        raise ValueError("Only Latin-1 / ASCII characters are supported.")

    value = (
        (data[0] << 24)
        | (data[1] << 16)
        | (data[2] << 8)
        | data[3]
    )
    return value & 0xFFFFFFFF


def u32_to_signed(value: int) -> int:
    """Interpret a 32-bit unsigned value as signed 32-bit integer."""
    return value if value < 0x80000000 else value - 0x100000000


class Converter(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("C++ Multi-Character Literal Converter")
        self.setMinimumWidth(520)
        self._build_ui()
        self._connect_signals()
        self._update_outputs()

    def _build_ui(self):
        layout = QGridLayout(self)
        layout.setHorizontalSpacing(12)
        layout.setVerticalSpacing(10)

        title = QLabel("Enter a 4-character C++ multi-character literal")
        title_font = QFont()
        title_font.setPointSize(11)
        title.setFont(title_font)

        self.input_edit = QLineEdit()
        self.input_edit.setMaxLength(4)
        self.input_edit.setPlaceholderText("e.g. Pwhj")

        self.integer_edit = QLineEdit()
        self.integer_edit.setReadOnly(True)

        self.hex_edit = QLineEdit()
        self.hex_edit.setReadOnly(True)

        # Make output fields visually distinct.
        for edit in (self.integer_edit, self.hex_edit):
            edit.setAlignment(Qt.AlignLeft)
            edit.setStyleSheet("QLineEdit { background: #f5f5f5; }")

        layout.addWidget(title, 0, 0, 1, 2)
        layout.addWidget(QLabel("Literal:"), 1, 0)
        layout.addWidget(self.input_edit, 1, 1)
        layout.addWidget(QLabel("Integer (signed 32-bit):"), 2, 0)
        layout.addWidget(self.integer_edit, 2, 1)
        layout.addWidget(QLabel("4 bytes (hex, little-endian):"), 3, 0)
        layout.addWidget(self.hex_edit, 3, 1)

    def _connect_signals(self):
        self.input_edit.textChanged.connect(self._update_outputs)

    def _update_outputs(self):
        text = self.input_edit.text()

        if not text:
            self.integer_edit.clear()
            self.hex_edit.clear()
            return

        try:
            value_u32 = multichar_to_u32(text)
            value_i32 = u32_to_signed(value_u32)
            bytes_le = value_u32.to_bytes(4, byteorder="little", signed=False)
            hex_bytes = " ".join(f"{b:02X}" for b in bytes_le)

            self.integer_edit.setText(str(value_i32))
            self.hex_edit.setText(hex_bytes)
        except ValueError as e:
            self.integer_edit.clear()
            self.hex_edit.clear()


def main():
    app = QApplication(sys.argv)
    window = Converter()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
