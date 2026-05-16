import sys
import os
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QLineEdit, QPushButton, QGraphicsView, 
                             QGraphicsScene, QGraphicsObject, QScrollArea, QGroupBox, 
                             QFormLayout, QFileDialog, QGraphicsItem)
from PyQt5.QtCore import Qt, QRectF, QPointF, pyqtSignal, QPoint
from PyQt5.QtGui import QPixmap, QPen, QPainter, QCursor, QFont, QImage

from PIL import Image

class AspectRatioWidget(QWidget):
    """A container that forces its child widget to maintain a 1:1 aspect ratio."""
    def __init__(self, widget):
        super().__init__()
        self.widget = widget
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(widget, 0, Qt.AlignCenter)

    def resizeEvent(self, event):
        size = min(self.width(), self.height())
        self.widget.setFixedSize(size, size)
        super().resizeEvent(event)


class RenderView(QGraphicsView):
    """Custom view to handle panning the main image item with Ctrl + LMB."""
    offsetChanged = pyqtSignal()

    def __init__(self):
        super().__init__()
        self.setDragMode(QGraphicsView.NoDrag)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        
        # 1px border around the render zone
        self.setStyleSheet("border: 1px solid #777; background-color: #2b2b2b;")
        self.setResizeAnchor(QGraphicsView.AnchorViewCenter)

        self._is_panning = False
        self._pan_start_pos = QPoint()
        self.main_item = None # Reference to the movable image item

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton and (event.modifiers() & Qt.ControlModifier):
            self._is_panning = True
            self._pan_start_pos = event.pos()
            self.setCursor(Qt.ClosedHandCursor)
            event.accept()
        else:
            super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if self._is_panning and self.main_item:
            delta = event.pos() - self._pan_start_pos
            
            # Map screen pixel delta to scene coordinate delta (accounts for view scaling)
            p1 = self.mapToScene(0, 0)
            p2 = self.mapToScene(delta.x(), delta.y())
            scene_delta = p2 - p1
            
            # Move the main image itself
            self.main_item.setPos(self.main_item.pos() + scene_delta)
            
            self._pan_start_pos = event.pos()
            self.offsetChanged.emit()
            event.accept()
        else:
            super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        if self._is_panning:
            self._is_panning = False
            self.setCursor(Qt.ArrowCursor)
            event.accept()
        else:
            super().mouseReleaseEvent(event)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        if self.scene() and not self.scene().sceneRect().isEmpty():
            # Scale scene to fill the 1:1 view based on the ORIGINAL image size
            scale_w = self.viewport().width() / self.scene().sceneRect().width()
            scale_h = self.viewport().height() / self.scene().sceneRect().height()
            scale = max(scale_w, scale_h)
            self.resetTransform()
            self.scale(scale, scale)


class OverlayWidget(QGraphicsObject):
    """Custom Graphics Item for sub-images, handling own scale and crop."""
    changed = pyqtSignal()

    def __init__(self, pixmap, filename, index):
        super().__init__()
        self.pixmap = pixmap
        self.filename = filename

        self.setFlags(QGraphicsItem.ItemIsMovable | QGraphicsItem.ItemSendsGeometryChanges)
        self.setAcceptHoverEvents(True)

        self.crop_rect = QRectF(0, 0, pixmap.width(), pixmap.height())
        self.scale_factor = 0.3
        self.setPos(10, 30 * index)

        self.handle_size = 12
        self.active_handle = None

    def boundingRect(self):
        w = self.crop_rect.width() * self.scale_factor
        h = self.crop_rect.height() * self.scale_factor
        return QRectF(0, 0, w, h)

    def paint(self, painter, option, widget):
        w = self.crop_rect.width() * self.scale_factor
        h = self.crop_rect.height() * self.scale_factor
        target_rect = QRectF(0, 0, w, h)

        painter.drawPixmap(target_rect, self.pixmap, self.crop_rect)

        painter.setPen(QPen(Qt.black, 2))
        painter.drawRect(QRectF(1, 1, w - 2, h - 2))

        font = QFont()
        font.setBold(True)
        font.setPixelSize(max(30, int(34 * self.scale_factor)))
        painter.setFont(font)

        text_rect = QRectF(5, 5, w - 10, h - 10)
        painter.setPen(QPen(Qt.black))
        painter.drawText(text_rect.translated(1, 1), Qt.AlignTop | Qt.AlignLeft, self.filename)
        painter.setPen(QPen(Qt.white))
        painter.drawText(text_rect, Qt.AlignTop | Qt.AlignLeft, self.filename)

    def get_handle_rects(self):
        w = self.boundingRect().width()
        h = self.boundingRect().height()
        s = self.handle_size
        return {
            'TL': QRectF(0, 0, s, s), 'TR': QRectF(w - s, 0, s, s),
            'BL': QRectF(0, h - s, s, s), 'BR': QRectF(w - s, h - s, s, s),
            'T': QRectF(s, 0, w - 2*s, s), 'B': QRectF(s, h - s, w - 2*s, s),
            'L': QRectF(0, s, s, h - 2*s), 'R': QRectF(w - s, s, s, h - 2*s)
        }

    def hoverMoveEvent(self, event):
        pos = event.pos()
        rects = self.get_handle_rects()
        if rects['TL'].contains(pos) or rects['BR'].contains(pos): self.setCursor(Qt.SizeFDiagCursor)
        elif rects['TR'].contains(pos) or rects['BL'].contains(pos): self.setCursor(Qt.SizeBDiagCursor)
        elif rects['T'].contains(pos) or rects['B'].contains(pos): self.setCursor(Qt.SizeVerCursor)
        elif rects['L'].contains(pos) or rects['R'].contains(pos): self.setCursor(Qt.SizeHorCursor)
        else: self.setCursor(Qt.ArrowCursor)
        super().hoverMoveEvent(event)

    def mousePressEvent(self, event):
        if event.modifiers() & Qt.ControlModifier:
            event.ignore()
            return

        pos = event.pos()
        rects = self.get_handle_rects()
        self.active_handle = None
        for key, rect in rects.items():
            if rect.contains(pos):
                self.active_handle = key
                break

        if self.active_handle:
            self.start_mouse_pos = event.scenePos()
            self.start_crop = QRectF(self.crop_rect)
            self.start_scale = self.scale_factor
            self.start_item_pos = self.pos()
            event.accept()
        else:
            super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if self.active_handle:
            delta = event.scenePos() - self.start_mouse_pos
            dx, dy = delta.x(), delta.y()

            new_crop = QRectF(self.start_crop)
            new_scale = self.start_scale
            new_item_pos = QPointF(self.start_item_pos)

            if self.active_handle in ['L', 'R', 'T', 'B']:
                if self.active_handle == 'L':
                    crop_dx = max(-self.start_crop.left(), min(self.start_crop.width() - 1, dx / self.start_scale))
                    new_crop.setLeft(self.start_crop.left() + crop_dx)
                    new_item_pos.setX(self.start_item_pos.x() + crop_dx * self.start_scale)
                elif self.active_handle == 'R':
                    crop_dx = max(-(self.start_crop.width() - 1), min(self.pixmap.width() - self.start_crop.right(), dx / self.start_scale))
                    new_crop.setRight(self.start_crop.right() + crop_dx)
                elif self.active_handle == 'T':
                    crop_dy = max(-self.start_crop.top(), min(self.start_crop.height() - 1, dy / self.start_scale))
                    new_crop.setTop(self.start_crop.top() + crop_dy)
                    new_item_pos.setY(self.start_item_pos.y() + crop_dy * self.start_scale)
                elif self.active_handle == 'B':
                    crop_dy = max(-(self.start_crop.height() - 1), min(self.pixmap.height() - self.start_crop.bottom(), dy / self.start_scale))
                    new_crop.setBottom(self.start_crop.bottom() + crop_dy)

            elif self.active_handle in ['TL', 'TR', 'BL', 'BR']:
                start_w = self.start_crop.width() * self.start_scale
                start_h = self.start_crop.height() * self.start_scale

                if self.active_handle == 'BR': scale_mult = max((start_w + dx) / start_w, (start_h + dy) / start_h)
                elif self.active_handle == 'TR': scale_mult = max((start_w + dx) / start_w, (start_h - dy) / start_h)
                elif self.active_handle == 'BL': scale_mult = max((start_w - dx) / start_w, (start_h + dy) / start_h)
                elif self.active_handle == 'TL': scale_mult = max((start_w - dx) / start_w, (start_h - dy) / start_h)

                new_scale = max(0.05, self.start_scale * scale_mult)
                w_diff = self.start_crop.width() * new_scale - start_w
                h_diff = self.start_crop.height() * new_scale - start_h

                if self.active_handle == 'TL':
                    new_item_pos.setX(self.start_item_pos.x() - w_diff)
                    new_item_pos.setY(self.start_item_pos.y() - h_diff)
                elif self.active_handle == 'TR':
                    new_item_pos.setY(self.start_item_pos.y() - h_diff)
                elif self.active_handle == 'BL':
                    new_item_pos.setX(self.start_item_pos.x() - w_diff)

            self.prepareGeometryChange()
            self.crop_rect = new_crop
            self.scale_factor = new_scale
            self.setPos(new_item_pos)
            self.changed.emit()

        else:
            super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        if self.active_handle:
            self.active_handle = None
            event.accept()
        else:
            super().mouseReleaseEvent(event)

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionHasChanged:
            self.changed.emit()
        return super().itemChange(change, value)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Embedded Map Editor")
        self.resize(1000, 750)
        self.items = []

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        # Top Bar
        top_layout = QHBoxLayout()
        self.path_edit = QLineEdit()
        self.path_edit.setReadOnly(True)
        self.path_edit.setPlaceholderText("No file selected...")
        btn_browse = QPushButton("Choose file")
        btn_browse.clicked.connect(self.browse_file)
        top_layout.addWidget(self.path_edit)
        top_layout.addWidget(btn_browse)
        main_layout.addLayout(top_layout)

        # Content Area
        content_layout = QHBoxLayout()
        
        self.scene = QGraphicsScene()
        self.view = RenderView()
        self.view.setScene(self.scene)
        self.view.offsetChanged.connect(self.update_offset_info)
        
        self.render_zone = AspectRatioWidget(self.view)
        self.render_zone.setMinimumSize(400, 400) 
        
        # Sidebar
        self.sidebar = QScrollArea()
        self.sidebar.setWidgetResizable(True)
        self.sidebar.setMinimumWidth(320)
        self.sidebar_content = QWidget()
        self.info_layout = QVBoxLayout(self.sidebar_content)
        self.info_layout.setAlignment(Qt.AlignTop)
        self.sidebar.setWidget(self.sidebar_content)

        content_layout.addWidget(self.render_zone, 1)
        content_layout.addWidget(self.sidebar, 0)
        main_layout.addLayout(content_layout)

        # Bottom Bar
        bottom_layout = QHBoxLayout()
        self.offset_edit = QLineEdit()
        self.offset_edit.setReadOnly(True)
        self.offset_edit.setPlaceholderText("Main Image Offset: +0.0, +0.0")
        bottom_layout.addWidget(self.offset_edit)
        main_layout.addLayout(bottom_layout)

    def browse_file(self):
        path, _ = QFileDialog.getOpenFileName(self, "Select TGA Image", "", "TGA Images (*.tga)")
        if path:
            self.path_edit.setText(path)
            self.load_images(path)

    def pil_to_pixmap(self, path):
        try:
            img = Image.open(path).convert("RGBA")
            data = img.tobytes("raw", "RGBA")
            qimg = QImage(data, img.width, img.height, QImage.Format_RGBA8888)
            return QPixmap.fromImage(qimg)
        except Exception as e:
            print(f"Error loading {path}: {e}")
            return None

    def load_images(self, main_path):
        self.scene.clear()
        self.items.clear()

        for i in reversed(range(self.info_layout.count())):
            widget = self.info_layout.itemAt(i).widget()
            if widget:
                widget.deleteLater()

        main_pixmap = self.pil_to_pixmap(main_path)
        if not main_pixmap: return

        # Keep scene bounding exactly as original image so scaling calculates correctly
        self.scene.setSceneRect(0, 0, main_pixmap.width(), main_pixmap.height())
        
        self.view.main_item = self.scene.addPixmap(main_pixmap)
        # Ensure main item is at the bottom (Z = 0)
        self.view.main_item.setZValue(0)

        self.view.resizeEvent(None)

        base_dir = os.path.dirname(main_path)
        base_name = os.path.splitext(os.path.basename(main_path))[0]

        for i in range(1, 5):
            sub_name = f"{base_name}_{i}.tga"
            sub_path = os.path.join(base_dir, sub_name)
            if os.path.exists(sub_path):
                pixmap = self.pil_to_pixmap(sub_path)
                if pixmap:
                    item = OverlayWidget(pixmap, sub_name, i)
                    
                    # FIX: Add directly to the scene instead of making it a child of main_item
                    self.scene.addItem(item)
                    # FIX: Force them to always render on top of the main image
                    item.setZValue(1) 
                    
                    self.items.append(item)
                    item.changed.connect(self.update_widgets_info)
                    self.create_info_panel(item)

        self.update_widgets_info()

    def create_info_panel(self, item):
        group = QGroupBox(item.filename)
        layout = QFormLayout(group)
        aspect_edit = QLineEdit(); aspect_edit.setReadOnly(True)
        size_edit = QLineEdit(); size_edit.setReadOnly(True)
        rect_edit = QLineEdit(); rect_edit.setReadOnly(True)

        layout.addRow("Aspect Ratio:", aspect_edit)
        layout.addRow("Crop Size:", size_edit)
        layout.addRow("Rect (x,y,w,h):", rect_edit)
        self.info_layout.addWidget(group)
        item.info_widgets = (aspect_edit, size_edit, rect_edit)

    def update_widgets_info(self):
        for item in self.items:
            aspect_edit, size_edit, rect_edit = item.info_widgets
            cw, ch = item.crop_rect.width(), item.crop_rect.height()
            
            aspect = cw / ch if ch != 0 else 0.0
            aspect_edit.setText(f"{aspect:.2f}")
            size_edit.setText(f"{int(cw)}x{int(ch)}")

            # FIX: Use scenePos() which explicitly grabs coordinates relative to the original unshifted render zone
            x, y = item.scenePos().x(), item.scenePos().y()
            w, h = item.boundingRect().width(), item.boundingRect().height()
            rect_edit.setText(f"{int(x)}, {int(y)}, {int(w)}, {int(h)}")

        self.update_offset_info()

    def update_offset_info(self):
        if self.view.main_item:
            offset_x = self.view.main_item.pos().x()
            offset_y = self.view.main_item.pos().y()
            self.offset_edit.setText(f"Main Image Offset: {offset_x:+.1f}, {offset_y:+.1f}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())