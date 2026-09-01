const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u53",
    date: "Updated On: September 1, 2026 @ 07:59 PM",

    sections: [
        {
            "title": "System Information Banner",
            "items": [
                "Introduced a new professional Zyphor OS terminal banner",
                "Added the current Zyphor OS build version to the system banner",
                "Added a clear upgrade message with the recommended Zyphor system upgrade command",
                "Improved terminal startup presentation with cleaner formatting and visual hierarchy"
            ]
        },
        {
            "title": "GRUB Bootloader Theme",
            "items": [
                "Updated the GRUB bootloader theme with the new Zyphor OS visual design",
                "Replaced the default Kali GRUB background with the Zyphor OS splash screen",
                "Added the Zyphor OS splash image as the GRUB desktop background",
                "Improved the OS selection menu layout with centered positioning and cleaner spacing",
                "Updated boot menu colors and selection styling for better readability",
                "Added a boot countdown message showing the remaining seconds before automatic boot"
            ]
        },
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
        }
    ]
};