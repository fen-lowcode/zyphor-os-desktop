from core.registry import page
from ui.components.command_page import InputCommandPage


@page(id="pkg_install", title="Install Packages/Apps", section="Packages", icon="⤓", order=32)
class PkgInstallPage(InputCommandPage):
    description = "Install a new package or app."
    placeholder = "Enter app name (e.g. firefox, vscode)"
    button_text = "Install"
    loading_text = "Installing…"
    empty_message = "Zyphor: ✖ Missing app name\n"
    command = "pkexec zyphor pkg install {value}"
