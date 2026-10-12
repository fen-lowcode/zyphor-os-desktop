from core.registry import page
from ui.components.web_page import WebPage


@page(id="home", title="Home", section="", icon="⌂", order=0)
class HomePage(WebPage):
    url = "/usr/share/zyphor-whats-new/release-notes.html"
