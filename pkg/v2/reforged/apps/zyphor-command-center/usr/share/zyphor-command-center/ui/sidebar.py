from PyQt6.QtCore import Qt, pyqtSignal
from PyQt6.QtGui import QPixmap
from PyQt6.QtWidgets import (
    QButtonGroup, QFrame, QHBoxLayout, QLabel, QPushButton, QScrollArea, QVBoxLayout, QWidget,
)

from core.paths import ICON_PATH


class Sidebar(QFrame):
    """Navigation menu. It is built automatically from the registered pages."""

    page_selected = pyqtSignal(str)   # emits the page id

    def __init__(self, pages):
        super().__init__()
        self.setObjectName("sidebar")
        self.setFixedWidth(250)
        self._buttons: dict[str, QPushButton] = {}

        outer = QVBoxLayout(self)
        outer.setContentsMargins(14, 18, 14, 14)
        outer.setSpacing(10)
        outer.addWidget(self._build_brand())

        # Scrollable list, so the menu keeps working however many pages you add.
        host = QWidget()
        host.setObjectName("navHost")
        nav = QVBoxLayout(host)
        nav.setContentsMargins(0, 0, 0, 0)
        nav.setSpacing(2)

        group = QButtonGroup(self)
        group.setExclusive(True)

        current_section = None
        for info in pages:
            if info.section != current_section:
                current_section = info.section
                if current_section:
                    nav.addWidget(QLabel(current_section.upper(), objectName="navSection"))

            btn = QPushButton(f"{info.icon}   {info.title}")
            btn.setObjectName("navButton")
            btn.setCheckable(True)
            btn.setCursor(Qt.CursorShape.PointingHandCursor)
            btn.clicked.connect(lambda _checked=False, pid=info.id: self.page_selected.emit(pid))
            group.addButton(btn)
            nav.addWidget(btn)
            self._buttons[info.id] = btn

        nav.addStretch()

        scroll = QScrollArea()
        scroll.setObjectName("navScroll")
        scroll.setWidgetResizable(True)
        scroll.setFrameShape(QFrame.Shape.NoFrame)
        scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
        scroll.setWidget(host)
        outer.addWidget(scroll, 1)

    def _build_brand(self) -> QWidget:
        brand = QWidget()
        row = QHBoxLayout(brand)
        row.setContentsMargins(6, 0, 6, 8)
        row.setSpacing(10)

        logo = QLabel()
        pixmap = QPixmap(str(ICON_PATH))
        if not pixmap.isNull():
            logo.setPixmap(
                pixmap.scaled(34, 34, Qt.AspectRatioMode.KeepAspectRatio,
                              Qt.TransformationMode.SmoothTransformation)
            )
        row.addWidget(logo)

        text = QVBoxLayout()
        text.setSpacing(0)
        text.addWidget(QLabel("Zyphor", objectName="brandTitle"))
        text.addWidget(QLabel("Command Center", objectName="brandSub"))
        row.addLayout(text)
        row.addStretch()
        return brand

    def select(self, page_id: str):
        """Highlight a page in the menu (does not emit page_selected)."""
        if page_id in self._buttons:
            self._buttons[page_id].setChecked(True)
