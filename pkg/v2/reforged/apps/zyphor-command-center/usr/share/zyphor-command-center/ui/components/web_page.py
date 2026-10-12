import html

from PyQt6.QtCore import QUrl
from PyQt6.QtGui import QColor
from PyQt6.QtWebEngineWidgets import QWebEngineView

from styles.theme import color
from ui.components.base_page import BasePage


class WebPage(BasePage):
    """
    A page that shows a web address or a local HTML file.

        url = "https://example.com"
        url = "/usr/share/something/index.html"     # local file
    """

    url = ""
    show_header = False
    margins = (0, 0, 0, 0)

    def __init__(self):
        super().__init__()
        self.view = QWebEngineView()
        self.view.page().setBackgroundColor(QColor(color("bg")))  # no white flash while loading
        self.body.addWidget(self.view)

    def before_load(self):
        """Override to prepare something first (e.g. start a local server)."""

    def on_first_show(self):
        try:
            self.before_load()
        except Exception as exc:
            bg, fg, muted = color("bg"), color("text"), color("muted")
            message = html.escape(str(exc))
            self.view.setHtml(
                f"<body style='background:{bg};color:{fg};font-family:sans-serif;padding:32px'>"
                f"<h2>Could not open this page</h2><p style='color:{muted}'>{message}</p></body>"
            )
            return

        if self.url.startswith("/"):
            self.view.load(QUrl.fromLocalFile(self.url))
        else:
            self.view.load(QUrl(self.url))
