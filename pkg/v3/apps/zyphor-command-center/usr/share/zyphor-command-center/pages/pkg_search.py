from core.registry import page
from ui.components.command_page import InputCommandPage


@page(id="pkg_search", title="Search Packages/Apps", section="Packages", icon="⌕", order=31)
class PkgSearchPage(InputCommandPage):
    description = "Find packages and apps that you can install."
    placeholder = "Search packages (e.g. firefox, snap, editor)"
    button_text = "Search"
    loading_text = "Searching packages…"
    empty_message = "Zyphor: ✖ Please enter a search keyword\n"
    command = "zyphor pkg search {value}"
