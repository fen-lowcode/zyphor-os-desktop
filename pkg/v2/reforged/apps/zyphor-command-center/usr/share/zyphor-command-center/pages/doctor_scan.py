from core.registry import page
from ui.components.command_page import CommandPage


@page(id="doctor_scan", title="Doctor Scan", section="Doctor", icon="◎", order=20)
class DoctorScanPage(CommandPage):
    description = "Check the health of your system and look for problems."
    button_text = "Run System Scan"
    loading_text = "Scanning system health…"
    command = "zyphor doctor scan"
