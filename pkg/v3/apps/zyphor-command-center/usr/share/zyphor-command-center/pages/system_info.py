from core.registry import page
from ui.components.command_page import CommandPage


@page(id="sysinfo", title="System Info", section="System", icon="ⓘ", order=12)
class SysInfoPage(CommandPage):
    description = "Show details about this computer and your Zyphor OS installation."
    button_text = "Run System Info"
    loading_text = "Collecting information…"
    command = "zyphor system info"
