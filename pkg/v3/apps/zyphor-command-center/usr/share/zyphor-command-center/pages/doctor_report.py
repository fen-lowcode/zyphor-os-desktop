from core.registry import page
from ui.components.command_page import CommandPage

REPORT_FILE = "/tmp/zyphor_doctor_report.txt"


@page(id="doctor_report", title="Doctor Report", section="Doctor", icon="☰", order=22)
class DoctorReportPage(CommandPage):
    description = "Generate a full system report you can read or share."
    button_text = "Generate System Report"
    loading_text = "Generating system report…"
    command = "pkexec zyphor doctor report"

    # Example of post-processing: once the command ends, show the saved report file.
    def on_finished(self, exit_code):
        try:
            with open(REPORT_FILE, "r") as f:
                content = f.read()
            self.console.write("\n\n--- SYSTEM REPORT ---\n\n")
            self.console.write(content)
        except Exception as exc:
            self.console.write(f"\nFailed to read report file: {exc}\n", "danger")
