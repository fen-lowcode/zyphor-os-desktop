const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u45",
    date: "Updated On: August 25, 2026 @ 10:54 AM",

    sections: [
        {
            "title": "Zyphor Release Banner — Build Identification",
            "items": [
                "Updated the system banner to dynamically display the OS release name from '/etc/os-release'",
                "Added a 'Latest Build' indicator below the release banner",
                "Displays the current build identifier (2026.08.22-r41) for easier release and troubleshooting reference",
                "Improved banner readability by separating the OS release information from the build number"
            ]
        },
        {
            "title": "Zyphor System Upgrade — Progress Visibility",
            "items": [
                "Fixed 'zyphor system upgrade' appearing to hang during step [1/4]",
                "Removed the '-qq' quiet flag from 'apt update' so real package-list output streams to the user",
                "'apt full-upgrade -y' output is no longer suppressed, showing live download/install progress",
                "Prevents users from mistaking a slow-but-active update for a frozen process",
                "Fallback: quiet steps can use a Braille-spinner helper ('zyphor_spinner') for visual feedback when output must stay suppressed"
            ]
        },
        {
            "title": "Zyphor Refresh Command",
            "items": [
                "Added 'zyphor refresh' command to reset the system to first-boot state",
                "Automatically detects the current logged-in user without requiring sudo",
                "Removes the '.zyphor_firstboot_done' marker file so the setup wizard runs again on next boot",
                "Reboots the system automatically after the reset",
                "Correction: The command name is zyphor refresh, not zyphor defaults."
            ]
        },
        {
            "title": "Zyphor CLI Command Shortcut Update",
            "items": [
                "Added the 'zy' command as a shortcut alias for 'zyphor'",
                "You can now use 'zy' instead of 'zyphor' for faster command execution",
                "All existing Zyphor CLI commands can be used with the 'zy' shortcut"
            ]
        },
        {
            "title": "Zyphor CLI Setup Dev Environment Update",
            "items": [
                "Added android-studio target to zyphor setup dev for provisioning Android Studio + Flutter",
                "Installs base dependencies (git, curl, unzip, zip, default-jdk, adb, fastboot) via apt",
                "Locates downloaded Android Studio and Flutter archives (checking ~/Downloads first) and extracts them to /opt",
                "Appends Flutter/Android SDK PATH exports to the real user's shell rc file (.zshrc or .bashrc), guarded against duplicate entries on re-run"
            ]
        }
    ]
};