from core.registry import page
from ui.components.command_page import CommandPage


@page(id="clean", title="System Clean", section="System", icon="✂", order=11)
class CleanPage(CommandPage):
    description = "Remove unused packages and clear caches to free up disk space."
    button_text = "Run System Clean"
    loading_text = "Cleaning system…"
    command = "pkexec zyphor system clean"
