from core.registry import page
from ui.components.command_page import CommandPage


@page(id="upgrade", title="System Upgrade", section="System", icon="↑", order=10)
class UpgradePage(CommandPage):
    description = "Update all installed packages to their latest versions."
    button_text = "Run System Upgrade"
    loading_text = "Upgrading system…"
    command = "pkexec zyphor system upgrade"
