"""Runs shell commands and streams their output back to the UI."""
import codecs
import re

from PyQt6.QtCore import QProcess, QObject, pyqtSignal

# Colour / cursor escape codes printed by CLI tools look like garbage in a text box.
_ANSI = re.compile(r"\x1b\[[0-9;?]*[ -/]*[@-~]|\x1b\][^\x07]*\x07")


def clean_output(text: str) -> str:
    text = _ANSI.sub("", text)
    return text.replace("\r\n", "\n").replace("\r", "\n")


class ProcessManager(QObject):
    started = pyqtSignal()
    finished = pyqtSignal(int)  # exit code (-1 if the command could not start)

    def __init__(self, output_callback, parent=None):
        super().__init__(parent)
        self.output_callback = output_callback

        # Incremental decoders: a multi-byte character split across two reads stays intact.
        self._out_decoder = codecs.getincrementaldecoder("utf-8")(errors="replace")
        self._err_decoder = codecs.getincrementaldecoder("utf-8")(errors="replace")

        self.process = QProcess(self)
        self.process.readyReadStandardOutput.connect(self._read_stdout)
        self.process.readyReadStandardError.connect(self._read_stderr)
        self.process.started.connect(self.started)
        self.process.finished.connect(self._on_finished)
        self.process.errorOccurred.connect(self._on_error)

    # ---- public -----------------------------------------------------
    @property
    def is_running(self) -> bool:
        return self.process.state() != QProcess.ProcessState.NotRunning

    def run(self, command: str) -> bool:
        """Run a shell command line. Returns False if something is already running."""
        if self.is_running:
            return False
        self.process.start("bash", ["-c", command])
        return True

    # ---- internals --------------------------------------------------
    def _emit(self, text: str):
        text = clean_output(text)
        if text:
            self.output_callback(text)

    def _read_stdout(self):
        self._emit(self._out_decoder.decode(bytes(self.process.readAllStandardOutput())))

    def _read_stderr(self):
        self._emit(self._err_decoder.decode(bytes(self.process.readAllStandardError())))

    def _on_finished(self, exit_code, _status):
        self._read_stdout()
        self._read_stderr()
        self.finished.emit(int(exit_code))

    def _on_error(self, error):
        # If the process can't even start, `finished` is never emitted by Qt.
        if error == QProcess.ProcessError.FailedToStart:
            self._emit("Could not start the command (is 'bash' installed?)\n")
            self.finished.emit(-1)
