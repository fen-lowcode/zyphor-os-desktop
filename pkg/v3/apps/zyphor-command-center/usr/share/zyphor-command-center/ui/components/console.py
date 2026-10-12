from PyQt6.QtGui import QTextCharFormat, QColor, QTextCursor
from PyQt6.QtWidgets import QPlainTextEdit

from styles.theme import color


class Console(QPlainTextEdit):
    """Read-only, terminal-looking text box."""

    def __init__(self):
        super().__init__()
        self.setObjectName("console")
        self.setReadOnly(True)
        self.setPlaceholderText("Output will appear here…")

    def write(self, text: str, color_key: str | None = None):
        """Append text exactly as given (no extra blank lines). Optionally coloured."""
        cursor = self.textCursor()
        cursor.movePosition(QTextCursor.MoveOperation.End)
        fmt = QTextCharFormat()
        if color_key:
            fmt.setForeground(QColor(color(color_key)))
        cursor.insertText(text, fmt)
        self.setTextCursor(cursor)
        bar = self.verticalScrollBar()
        bar.setValue(bar.maximum())

    def write_line(self, text: str, color_key: str | None = None):
        self.write(text + "\n", color_key)
