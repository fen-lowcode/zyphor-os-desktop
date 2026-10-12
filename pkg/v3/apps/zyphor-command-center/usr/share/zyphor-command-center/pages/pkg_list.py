from core.registry import page
from ui.components.command_page import CommandPage


@page(id="pkg_list", title="Packages/Apps", section="Packages", icon="▤", order=30)
class PkgListPage(CommandPage):
    description = "Everything installed on this system."
    button_text = "Refresh Package List"
    loading_text = "Loading packages…"
    command = "zyphor pkg list"
    auto_run = True   # runs by itself the first time you open the page
