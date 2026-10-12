"""
Zyphor theme
============
All colours live in PALETTES below. Change a value here and the whole app
follows. Switch between palettes with the ZYPHOR_THEME environment variable
(``dark`` or ``light``) or by changing DEFAULT_THEME.
"""
import os
from string import Template

DEFAULT_THEME = "dark"

PALETTES = {
    "dark": {
        "bg": "#0e1016",            # window background
        "surface": "#151922",       # sidebar, inputs
        "surface2": "#1c212d",      # buttons, hover
        "hover": "#252b3a",
        "border": "#262c3b",
        "border_strong": "#363e52",
        "text": "#e7eaf3",
        "muted": "#8d96ab",
        "accent": "#7c8cff",        # main brand colour
        "accent_hover": "#95a2ff",
        "accent_text": "#0b0d14",   # text drawn on top of the accent colour
        "accent_soft": "#222a4f",   # selected sidebar item background
        "success": "#3ecf8e",
        "danger": "#ff6b7a",
        "console_bg": "#0a0c11",
        "console_text": "#cfd6e6",
    },
    "light": {
        "bg": "#f4f6fb",
        "surface": "#ffffff",
        "surface2": "#eef1f8",
        "hover": "#e4e9f4",
        "border": "#dfe4ef",
        "border_strong": "#c9d0e0",
        "text": "#1a2033",
        "muted": "#667089",
        "accent": "#4f5fe8",
        "accent_hover": "#3f4ed1",
        "accent_text": "#ffffff",
        "accent_soft": "#e3e7ff",
        "success": "#12915a",
        "danger": "#d8384a",
        "console_bg": "#ffffff",
        "console_text": "#27304a",
    },
}

SANS = '"Inter", "Noto Sans", "Ubuntu", "Cantarell", "DejaVu Sans", sans-serif'
MONO = '"JetBrains Mono", "Fira Code", "DejaVu Sans Mono", "Monospace", monospace'


def _name() -> str:
    name = os.environ.get("ZYPHOR_THEME", DEFAULT_THEME).lower()
    return name if name in PALETTES else DEFAULT_THEME


def palette() -> dict:
    return PALETTES[_name()]


def color(key: str) -> str:
    """Look up a palette colour, e.g. color("accent")."""
    return palette()[key]


_QSS = Template("""
QWidget {
    color: $text;
    font-family: $sans;
    font-size: 13px;
}
QMainWindow { background: $bg; }
QLabel { background: transparent; }
QToolTip {
    background: $surface2; color: $text;
    border: 1px solid $border_strong; padding: 5px 8px;
}

/* ---------- Sidebar ---------- */
QFrame#sidebar {
    background: $surface;
    border: none;
    border-right: 1px solid $border;
}
QLabel#brandTitle { font-size: 15px; font-weight: 700; }
QLabel#brandSub  { color: $muted; font-size: 11px; }
QLabel#navSection {
    color: $muted; font-size: 10px; font-weight: 700;
    padding: 14px 12px 4px 12px;
}
QScrollArea#navScroll, QWidget#navHost { background: transparent; border: none; }

QPushButton#navButton {
    text-align: left;
    background: transparent;
    border: none;
    border-radius: 8px;
    padding: 9px 12px;
    color: $muted;
    font-weight: 500;
}
QPushButton#navButton:hover   { background: $surface2; color: $text; }
QPushButton#navButton:checked { background: $accent_soft; color: $accent; font-weight: 600; }

/* ---------- Page header ---------- */
QLabel#pageTitle    { font-size: 24px; font-weight: 700; }
QLabel#pageSubtitle { color: $muted; font-size: 13px; }

/* ---------- Buttons ---------- */
QPushButton {
    background: $surface2;
    border: 1px solid $border;
    border-radius: 8px;
    padding: 9px 18px;
    font-weight: 600;
}
QPushButton:hover    { background: $hover; border-color: $border_strong; }
QPushButton:disabled { color: $muted; background: $surface; border-color: $border; }

QPushButton[variant="primary"] {
    background: $accent; border-color: $accent; color: $accent_text;
}
QPushButton[variant="primary"]:hover    { background: $accent_hover; border-color: $accent_hover; }
QPushButton[variant="primary"]:disabled { background: $border_strong; border-color: $border_strong; color: $muted; }

QPushButton[variant="danger"] {
    background: $danger; border-color: $danger; color: $accent_text;
}
QPushButton[variant="danger"]:hover { background: $danger; border-color: $text; }

QPushButton[variant="ghost"] {
    background: transparent; border-color: transparent; color: $muted;
}
QPushButton[variant="ghost"]:hover { background: $surface2; color: $text; }

/* ---------- Inputs ---------- */
QLineEdit {
    background: $surface;
    border: 1px solid $border;
    border-radius: 8px;
    padding: 9px 12px;
    selection-background-color: $accent;
    selection-color: $accent_text;
}
QLineEdit:focus { border-color: $accent; }

/* ---------- Console ---------- */
QPlainTextEdit#console {
    background: $console_bg;
    color: $console_text;
    border: 1px solid $border;
    border-radius: 10px;
    padding: 12px;
    font-family: $mono;
    font-size: 12px;
    selection-background-color: $accent;
    selection-color: $accent_text;
}

/* ---------- Scrollbars ---------- */
QScrollBar:vertical   { background: transparent; width: 10px; margin: 2px; }
QScrollBar:horizontal { background: transparent; height: 10px; margin: 2px; }
QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
    background: $border_strong; border-radius: 4px; min-height: 28px; min-width: 28px;
}
QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover { background: $muted; }
QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; }
QScrollBar::add-page, QScrollBar::sub-page { background: transparent; }

/* ---------- Dialogs ---------- */
QMessageBox, QDialog { background: $surface; }
""")


def build_stylesheet() -> str:
    return _QSS.substitute(sans=SANS, mono=MONO, **palette())
