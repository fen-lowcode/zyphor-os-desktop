from core.registry import page
from ui.components.command_page import CommandPage


@page(id="doctor_fix", title="Doctor Fix", section="Doctor", icon="✚", order=21)
class DoctorFixPage(CommandPage):
    description = "Automatically repair problems found on your system."
    button_text = "Run Doctor Fix"
    loading_text = "Repairing system…"
    command = "pkexec zyphor doctor fix"
