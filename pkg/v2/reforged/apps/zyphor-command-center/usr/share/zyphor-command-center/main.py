import sys

# Must be imported before QApplication is created (a Qt WebEngine requirement).
from PyQt6.QtWebEngineWidgets import QWebEngineView  # noqa: F401
from PyQt6.QtGui import QIcon
from PyQt6.QtWidgets import QApplication

from app import MainWindow
from core import webserver
from core.paths import ICON_PATH
from styles.theme import build_stylesheet

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setApplicationName("Zyphor Command Center")
    app.setWindowIcon(QIcon(str(ICON_PATH)))
    app.setStyleSheet(build_stylesheet())

    window = MainWindow()
    window.show()

    exit_code = app.exec()

    webserver.stop()
    sys.exit(exit_code)
