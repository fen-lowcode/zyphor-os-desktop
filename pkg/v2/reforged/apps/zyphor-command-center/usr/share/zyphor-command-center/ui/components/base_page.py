from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel


class BasePage(QWidget):
    """
    Parent of every page. Gives you:
      * a heading (from the title given in @page) and an optional description
      * `self.body`  - a layout where you add your own widgets
      * on_first_show() / on_show() hooks that run when the page is opened
    """

    description = ""        # grey text under the heading
    show_header = True      # set False for pages that fill the whole area (web pages)
    margins = (32, 28, 32, 28)

    def __init__(self):
        super().__init__()
        self._shown_once = False

        self.root = QVBoxLayout(self)
        self.root.setContentsMargins(*self.margins)
        self.root.setSpacing(16)

        if self.show_header:
            title = QLabel(getattr(self, "page_title", ""))
            title.setObjectName("pageTitle")
            self.root.addWidget(title)
            if self.description:
                sub = QLabel(self.description)
                sub.setObjectName("pageSubtitle")
                sub.setWordWrap(True)
                self.root.addWidget(sub)

        self.body = QVBoxLayout()
        self.body.setSpacing(14)
        self.root.addLayout(self.body, 1)

    # ---- hooks you can override ----------------------------------
    def on_first_show(self):
        """Runs once, the first time the page is opened."""

    def on_show(self):
        """Runs every time the page is opened."""

    # ---- called by the main window --------------------------------
    def notify_shown(self):
        if not self._shown_once:
            self._shown_once = True
            self.on_first_show()
        self.on_show()
