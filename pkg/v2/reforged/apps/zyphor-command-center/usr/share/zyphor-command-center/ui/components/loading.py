from PyQt6.QtCore import Qt, QVariantAnimation, QRectF
from PyQt6.QtGui import QColor, QPainter
from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel

from styles.theme import color


class _BusyBar(QWidget):
    """Thin animated bar (a segment sliding left to right)."""

    def __init__(self):
        super().__init__()
        self.setFixedHeight(4)
        self._pos = 0.0
        self._anim = QVariantAnimation(self)
        self._anim.setStartValue(0.0)
        self._anim.setEndValue(1.0)
        self._anim.setDuration(1100)
        self._anim.setLoopCount(-1)
        self._anim.valueChanged.connect(self._tick)

    def _tick(self, value):
        self._pos = float(value)
        self.update()

    def run(self, on: bool):
        if on:
            self._anim.start()
        else:
            self._anim.stop()

    def paintEvent(self, _event):
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing)
        w, h = self.width(), self.height()
        p.setPen(Qt.PenStyle.NoPen)
        p.setBrush(QColor(color("border")))
        p.drawRoundedRect(QRectF(0, 0, w, h), h / 2, h / 2)
        seg = w * 0.3
        x = -seg + (w + seg) * self._pos
        p.setBrush(QColor(color("accent")))
        p.setClipRect(QRectF(0, 0, w, h))
        p.drawRoundedRect(QRectF(x, 0, seg, h), h / 2, h / 2)


class LoadingIndicator(QWidget):
    """Progress text + animated bar. Hidden until start() is called."""

    def __init__(self, text="Running..."):
        super().__init__()
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(6)

        self.label = QLabel(text)
        self.label.setObjectName("pageSubtitle")
        self.bar = _BusyBar()
        layout.addWidget(self.label)
        layout.addWidget(self.bar)

        # Keep the layout from jumping when the indicator appears / disappears.
        policy = self.sizePolicy()
        policy.setRetainSizeWhenHidden(True)
        self.setSizePolicy(policy)
        self.setVisible(False)

    def start(self, text="Running..."):
        self.label.setText(text)
        self.bar.run(True)
        self.setVisible(True)

    def stop(self):
        self.bar.run(False)
        self.setVisible(False)
