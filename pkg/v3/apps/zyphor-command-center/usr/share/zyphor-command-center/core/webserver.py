"""Starts / stops the local PHP server used by the Profile (User Management) page."""
import os
import socket
import subprocess
import time

HOST = "127.0.0.1"
PORT = 2437
# Production path. For development: export ZYPHOR_WEB_DIR=/path/to/zcc-web/usr/share/zyphor-command-center-web
WEB_DIR = os.environ.get("ZYPHOR_WEB_DIR", "/usr/share/zyphor-command-center-web")

_process = None


def is_running() -> bool:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.settimeout(0.2)
        return s.connect_ex((HOST, PORT)) == 0


def start() -> None:
    global _process

    if is_running():
        return

    _process = subprocess.Popen(
        ["php", "-S", f"{HOST}:{PORT}"],
        cwd=WEB_DIR,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )

    timeout = 10.0
    while timeout > 0:
        if is_running():
            return
        time.sleep(0.2)
        timeout -= 0.2

    raise RuntimeError("The local web server failed to start.")


def stop() -> None:
    global _process

    if _process:
        _process.terminate()
        _process.wait()
        _process = None
