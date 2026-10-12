import traceback

from PyQt6.QtWidgets import QHBoxLayout, QLabel, QMainWindow, QStackedWidget, QWidget

from core import registry
from ui.components.base_page import BasePage
from ui.components.console import Console
from ui.sidebar import Sidebar


class _ErrorPage(BasePage):
    """Shown instead of a page that crashed while opening, so the app keeps running."""

    show_header = False

    def __init__(self, title: str, details: str):
        super().__init__()
        heading = QLabel(f"Could not open “{title}”")
        heading.setObjectName("pageTitle")
        console = Console()
        console.write(details, "danger")
        self.body.addWidget(heading)
        self.body.addWidget(console, 1)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Zyphor Command Center")
        self.resize(1120, 700)
        self.setMinimumSize(900, 560)

        registry.discover()                 # finds every file in pages/
        self.page_infos = registry.all_pages()
        self._widgets: dict[str, QWidget] = {}

        self.sidebar = Sidebar(self.page_infos)
        self.sidebar.page_selected.connect(self.switch)

        self.stack = QStackedWidget()

        central = QWidget()
        layout = QHBoxLayout(central)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(0)
        layout.addWidget(self.sidebar)
        layout.addWidget(self.stack, 1)
        self.setCentralWidget(central)

        if self.page_infos:
            self.switch(self.page_infos[0].id)

    def switch(self, page_id: str):
        """Show a page. Pages are created the first time they are opened."""
        widget = self._widgets.get(page_id)
        if widget is None:
            info = registry.get(page_id)
            try:
                widget = info.cls()
            except Exception:
                widget = _ErrorPage(info.title, traceback.format_exc())
            self._widgets[page_id] = widget
            self.stack.addWidget(widget)

        self.stack.setCurrentWidget(widget)
        self.sidebar.select(page_id)
        widget.notify_shown()
