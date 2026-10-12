"""
HOW TO ADD A NEW PAGE
=====================
1. Copy this file inside the `pages/` folder and give it any name
   that does NOT start with "_"   (e.g. `network_status.py`).
2. Change the @page(...) line and the class below.
3. Restart the app. Your page appears in the sidebar. Done!

Pick ONE of the examples below and delete the others.
(More details: /usr/share/doc/zyphor-command-center/ADDING_A_PAGE.md)
"""
from core.registry import page
from ui.components.command_page import CommandPage, InputCommandPage
from PyQt6.QtWidgets import QLabel
from ui.components.base_page import BasePage
from ui.components.web_page import WebPage


# ---------------------------------------------------------------
# A) A button that runs a command and shows its output
# ---------------------------------------------------------------
@page(id="my_command", title="My Command", section="Extras", icon="★", order=100)
class MyCommandPage(CommandPage):
    description = "What this page does, in one sentence."
    button_text = "Run it"
    loading_text = "Working…"
    command = "echo 'Hello from my new page'"
    # auto_run = True        # uncomment to run as soon as the page opens


# ---------------------------------------------------------------
# B) A text box + button; {value} is replaced by what the user types
# ---------------------------------------------------------------
@page(id="my_input", title="My Input Page", section="Extras", icon="✎", order=101)
class MyInputPage(InputCommandPage):
    description = "Type something and run a command with it."
    placeholder = "Type a name…"
    button_text = "Go"
    command = "echo 'You typed:' {value}"
    # confirm = "Really run it for '{value}'?"    # uncomment to ask yes/no first


# ---------------------------------------------------------------
# C) A fully custom page: add any Qt widgets to self.body
# ---------------------------------------------------------------
@page(id="my_custom", title="My Custom Page", section="Extras", icon="◆", order=102)
class MyCustomPage(BasePage):
    description = "A page where you build the layout yourself."

    def __init__(self):
        super().__init__()
        self.body.addWidget(QLabel("Put any widgets here."))
        self.body.addStretch()

    def on_show(self):
        pass   # runs every time the page is opened (refresh your data here)


# ---------------------------------------------------------------
# D) A web page / local HTML file
# ---------------------------------------------------------------
@page(id="my_web", title="My Web Page", section="Extras", icon="◍", order=103)
class MyWebPage(WebPage):
    url = "https://example.com"
