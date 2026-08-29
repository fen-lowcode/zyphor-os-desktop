const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u51",
    date: "Updated On: August 29, 2026 @ 09:00 AM",

    sections: [
        {
            "title": "Package Updates",
            "items": [
                "Upgraded 10 packages, including zyphor-cli, zyphor-os-release, zyphor-repo-config, zyphor-whats-new, zysh, zywin, exfatprogs, ksnip, nano, and libmsgraph-1-1",
                "Installed 3 new dependencies: libkcolorpicker-qt6-0, libkimageannotator-qt6-0, and zyphor-archive-keyring",
                "Added zyphor-archive-keyring to provide the official Zyphor OS repository signing key for package authenticity verification",
                "Removed 6 obsolete packages, including older Qt5 dependencies and linux-base-7.0.12+kali-amd64 and linux-binary-7.0.12+kali-amd64",
                "Removed dependencies associated with older Linux kernel versions 6.19.14 and 7.0.12",
                "Cleaned up obsolete kernel dependencies and packages that are no longer required by the current Zyphor OS installation"
            ]
        },
        {
            "title": "ZyWin UI Installer and Zywin Configuration",
            "items": [
                "Added a Wine configuration check on launch that detects whether Wine is set up and prompts the user to run \"sudo zyphor setup wine\" if it isn't",
                "Application installs are handed off to the zywin CLI, with live status updates and error reporting surfaced in the UI",
                "Added a dark, Zyphor-branded interface consistent with the rest of the desktop",
                "zywin CLI now ensures the Wine prefix directory (~/.local/zywin/prefixes/) exists before use, creating it automatically if missing"
            ]
        },
        {
            "title": "Zyphor Package Repository Signing",
            "items": [
                "Added cryptographic GPG signing for the official Zyphor OS APT repository",
                "Implemented signed InRelease and Release.gpg repository metadata",
                "Added the official Zyphor OS archive keyring for APT repository verification",
                "APT now verifies repository authenticity using the Zyphor archive signing key instead of relying on trusted=yes",
                "Added automated repository signature generation and verification to the Zyphor build process"
            ]
        },
        {
            "title": "Zyphor System Upgrade — Linux Kernel 7.1.5",
            "items": [
                "Updated 8 packages, including hyperv-daemons, Linux kernel components, zyphor-os-release, zyphor-whats-new, and zysh",
                "Installed 7 new kernel-related dependencies required for Linux kernel 7.1.5+kali-amd64",
                "Updated Linux kernel image, modules, headers, and build components",
                "Completed the full upgrade with 0 packages removed and 0 packages left unupgraded"
            ]
        },
        {
            "title": "Zyphor Release Banner — Build Identification",
            "items": [
                "Updated the system banner to dynamically display the OS release name from '/etc/os-release'",
                "Added a 'Latest Build' indicator below the release banner",
                "Displays the current build identifier (2026.08.22-r41) for easier release and troubleshooting reference",
                "Improved banner readability by separating the OS release information from the build number"
            ]
        }
    ]
};