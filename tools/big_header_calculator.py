"""
Requires: PyQt5
"""
import os
import sys
from collections import defaultdict
from PyQt5 import QtWidgets, QtCore

LIMIT_BYTES = 2 * 1024 * 1024  # 2 MiB


def compute_header_size_for_folder(folder_path: str):
    """Return (total_bytes, file_entries)

    file_entries: list of tuples (relative_path, filename_byte_len, contribution_bytes)
    Recursively walks the folder (recursion always enabled).
    """
    entries = []
    total = 24  # general data

    for root, dirs, files in os.walk(folder_path):
        for fn in files:
            full = os.path.join(root, fn)
            rel = os.path.relpath(full, folder_path)
            rel = rel.replace(os.path.sep, '/')
            filename_bytes = len(rel.encode('utf-8')) + 1  # null-terminated
            contribution = 8 + filename_bytes
            entries.append((rel, filename_bytes, contribution))
            total += contribution

    # Sort files by contribution (largest first) — useful if later we want to expand a folder
    entries.sort(key=lambda e: e[2], reverse=True)
    return total, entries


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('BIG Header Size Calculator — Folder View')
        self.resize(900, 700)

        container = QtWidgets.QWidget()
        self.setCentralWidget(container)
        layout = QtWidgets.QVBoxLayout(container)

        # Controls row
        row = QtWidgets.QHBoxLayout()
        self.btn_select = QtWidgets.QPushButton('Select Folder')
        self.btn_select.clicked.connect(self.on_select_folder)
        row.addWidget(self.btn_select)

        self.lbl_path = QtWidgets.QLabel('(no folder selected)')
        self.lbl_path.setTextInteractionFlags(QtCore.Qt.TextSelectableByMouse)
        row.addWidget(self.lbl_path, 1)

        self.btn_recalc = QtWidgets.QPushButton('Recalculate')
        self.btn_recalc.clicked.connect(self.on_recalculate)
        row.addWidget(self.btn_recalc)

        layout.addLayout(row)

        # Result display
        info_layout = QtWidgets.QHBoxLayout()

        left = QtWidgets.QVBoxLayout()
        self.lbl_total = QtWidgets.QLabel('Header size: —')
        self.lbl_total.setStyleSheet('font-size: 14pt;')
        left.addWidget(self.lbl_total)

        self.lbl_breakdown = QtWidgets.QLabel('Folders: —    Limit: 2 MiB (2 * 1024 * 1024 bytes)')
        left.addWidget(self.lbl_breakdown)

        note = QtWidgets.QLabel('Note: filenames counted as UTF-8 bytes + 1 null byte. Recursion is always enabled. 1 MiB = 1024*1024 bytes.')
        note.setWordWrap(True)
        left.addWidget(note)

        info_layout.addLayout(left, 1)

        right = QtWidgets.QVBoxLayout()
        self.lbl_status = QtWidgets.QLabel('Status: idle')
        right.addWidget(self.lbl_status)
        right.addStretch(1)
        info_layout.addLayout(right)

        layout.addLayout(info_layout)

        # Table with folder contributions (no limit)
        self.table = QtWidgets.QTableWidget(0, 4)
        self.table.setHorizontalHeaderLabels(['Folder (relative)', 'Files', 'Contribution bytes', 'Contribution (MiB)'])
        self.table.horizontalHeader().setStretchLastSection(False)
        self.table.horizontalHeader().setSectionResizeMode(0, QtWidgets.QHeaderView.Stretch)
        self.table.horizontalHeader().setSectionResizeMode(1, QtWidgets.QHeaderView.ResizeToContents)
        self.table.horizontalHeader().setSectionResizeMode(2, QtWidgets.QHeaderView.ResizeToContents)
        self.table.horizontalHeader().setSectionResizeMode(3, QtWidgets.QHeaderView.ResizeToContents)
        layout.addWidget(self.table, 1)

        # Footer: tip
        footer = QtWidgets.QHBoxLayout()
        footer.addWidget(QtWidgets.QLabel('Tip: select a folder and press Recalculate. The table shows all folders sorted by their total header contribution (descending).'))
        layout.addLayout(footer)

        # State
        self.current_folder = None
        self.current_entries = []

    def on_select_folder(self):
        folder = QtWidgets.QFileDialog.getExistingDirectory(self, 'Select folder to archive')
        if folder:
            self.current_folder = folder
            self.lbl_path.setText(folder)
            self.on_recalculate()

    def on_recalculate(self):
        if not self.current_folder:
            QtWidgets.QMessageBox.warning(self, 'No folder', 'Please select a folder first.')
            return
        self.lbl_status.setText('Calculating...')
        QtWidgets.QApplication.processEvents()

        try:
            total, entries = compute_header_size_for_folder(self.current_folder)
        except Exception as e:
            QtWidgets.QMessageBox.critical(self, 'Error', f'Error while reading folder: {e}')
            self.lbl_status.setText('Error')
            return

        self.current_entries = entries
        # Aggregate per-folder
        folder_map = defaultdict(lambda: {'files': 0, 'bytes': 0})
        for rel, fname_bytes, contrib in entries:
            folder = os.path.dirname(rel)
            if folder == '':
                folder = '.'
            # Normalize to forward slashes already done in compute
            folder_map[folder]['files'] += 1
            folder_map[folder]['bytes'] += contrib

        folder_list = [(folder, data['files'], data['bytes']) for folder, data in folder_map.items()]
        # Sort by bytes descending
        folder_list.sort(key=lambda x: x[2], reverse=True)

        num_folders = len(folder_list)
        self._update_display(total, num_folders, folder_list)
        self.lbl_status.setText('Done')

    def _update_display(self, total_bytes: int, num_folders: int, folder_list):
        mb = total_bytes / (1024 * 1024)
        limit_mb = LIMIT_BYTES / (1024 * 1024)
        formatted = f'{total_bytes:,} bytes ({mb:.4f} MiB)'

        # Color: green if <= limit, red otherwise
        color = 'green' if total_bytes <= LIMIT_BYTES else 'red'
        self.lbl_total.setText(f'Header size: <span style="color:{color};">{formatted}</span>')

        self.lbl_breakdown.setText(f'Folders: {num_folders}    Limit: {limit_mb:.2f} MiB ({LIMIT_BYTES:,} bytes)')

        # Populate table with all folder rows
        self.table.setRowCount(len(folder_list))
        for i, (folder, files_count, bytes_contrib) in enumerate(folder_list):
            item0 = QtWidgets.QTableWidgetItem(folder)
            item0.setFlags(item0.flags() ^ QtCore.Qt.ItemIsEditable)
            item1 = QtWidgets.QTableWidgetItem(str(files_count))
            item1.setFlags(item1.flags() ^ QtCore.Qt.ItemIsEditable)
            item2 = QtWidgets.QTableWidgetItem(str(bytes_contrib))
            item2.setFlags(item2.flags() ^ QtCore.Qt.ItemIsEditable)
            item3 = QtWidgets.QTableWidgetItem(f'{bytes_contrib / (1024*1024):.6f}')
            item3.setFlags(item3.flags() ^ QtCore.Qt.ItemIsEditable)
            self.table.setItem(i, 0, item0)
            self.table.setItem(i, 1, item1)
            self.table.setItem(i, 2, item2)
            self.table.setItem(i, 3, item3)

        self.table.resizeColumnsToContents()


def main():
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
