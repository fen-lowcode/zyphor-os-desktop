import shlex

from PyQt6.QtWidgets import QHBoxLayout, QLineEdit, QMessageBox, QPushButton

from core.process import ProcessManager
from ui.components.base_page import BasePage
from ui.components.console import Console
from ui.components.loading import LoadingIndicator


def _button(text: str, variant: str | None = None) -> QPushButton:
    btn = QPushButton(text)
    if variant:
        btn.setProperty("variant", variant)
    return btn


class CommandPage(BasePage):
    """
    A ready-made page: [Run button] + live console output.

    Most pages only need to set a few class attributes:

        command      shell command to run
        button_text  label of the run button
        loading_text text shown while the command runs
        auto_run     True = run as soon as the page is first opened
    """

    command = ""
    button_text = "Run"
    loading_text = "Running..."
    auto_run = False

    def __init__(self):
        super().__init__()

        self.console = Console()
        self.loading = LoadingIndicator(self.loading_text)
        self.process = ProcessManager(self.console.write, self)
        self.process.started.connect(lambda: self.loading.start(self.loading_text))
        self.process.finished.connect(self._handle_finished)

        # Toolbar row: [extra widgets...] [Run] ........ [Clear]
        self.toolbar = QHBoxLayout()
        self.toolbar.setSpacing(10)
        self.run_button = _button(self.button_text, "primary")
        self.run_button.clicked.connect(self.run)
        clear_button = _button("Clear", "ghost")
        clear_button.clicked.connect(self.console.clear)
        self.toolbar.addWidget(self.run_button)
        self.toolbar.addStretch()
        self.toolbar.addWidget(clear_button)

        self.body.addLayout(self.toolbar)
        self.body.addWidget(self.loading)
        self.body.addWidget(self.console, 1)

    # ---- override these for custom behaviour -----------------------
    def build_command(self) -> str | None:
        """Return the command to run, or None to cancel (e.g. invalid input)."""
        return self.command

    def on_finished(self, exit_code: int):
        """Called after the command ends. Override to post-process results."""

    # ---- internals ---------------------------------------------------
    def on_first_show(self):
        if self.auto_run:
            self.run()

    def run(self):
        if self.process.is_running:
            return
        command = self.build_command()
        if not command:
            return
        self.console.clear()
        self.run_button.setEnabled(False)
        self.process.run(command)

    def _handle_finished(self, exit_code: int):
        self.loading.stop()
        self.run_button.setEnabled(True)
        if exit_code == 0:
            self.console.write_line("\n✔ Finished", "success")
        else:
            self.console.write_line(f"\n✖ Failed (exit code {exit_code})", "danger")
        self.on_finished(exit_code)


class InputCommandPage(CommandPage):
    """
    Like CommandPage, but with a text box. Put {value} in `command`;
    it is replaced by what the user typed (safely quoted).

        command = "pkexec zyphor pkg install {value}"
        confirm = "Install '{value}'?"      # optional yes/no popup
    """

    placeholder = "Type here…"
    empty_message = "Zyphor: ✖ Please enter a value\n"
    confirm: str | None = None

    def __init__(self):
        super().__init__()
        self.input = QLineEdit()
        self.input.setPlaceholderText(self.placeholder)
        self.input.setClearButtonEnabled(True)
        self.input.returnPressed.connect(self.run)
        self.toolbar.insertWidget(0, self.input, 1)

    def build_command(self) -> str | None:
        tokens = self.input.text().split()

        if not tokens:
            self.console.write(self.empty_message, "danger")
            return None
        # A leading "-" would be read as an option by the CLI, not as a name.
        if any(t.startswith("-") for t in tokens):
            self.console.write("Zyphor: ✖ Invalid input\n", "danger")
            return None

        # Quote every word so the text can never be run as shell code (pkexec = root!).
        value = " ".join(shlex.quote(t) for t in tokens)

        if self.confirm:
            answer = QMessageBox.question(
                self,
                "Please confirm",
                self.confirm.replace("{value}", " ".join(tokens)),
                QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
            )
            if answer != QMessageBox.StandardButton.Yes:
                return None

        return self.command.replace("{value}", value)
