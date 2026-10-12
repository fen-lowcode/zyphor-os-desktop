from core import webserver
from core.registry import page
from ui.components.web_page import WebPage


@page(id="profile", title="User Management", section="Account", icon="☺", order=5)
class ProfilePage(WebPage):
    url = "http://127.0.0.1:2437/profile.php"

    def before_load(self):
        # The local PHP server only starts when this page is opened for the first time.
        webserver.start()
