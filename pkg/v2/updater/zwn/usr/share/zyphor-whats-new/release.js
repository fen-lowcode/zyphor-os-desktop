const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u42",
    date: "Updated On: August 24, 2026 @ 10:54 AM",

    sections: [
        {
            "title": "Zyphor Defaults Reset Command",
            "items": [
                "Added 'zyphor defaults' command to reset the system to first-boot state",
                "Automatically detects the current logged-in user without requiring sudo",
                "Removes the '.zyphor_firstboot_done' marker file so the setup wizard runs again on next boot",
                "Reboots the system automatically after the reset"
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
        },
        { 
            "title": "Zyphor Repository Build & Configuration",
            "items": [
                "Added LTS codename input to the zyphor build repo command for generating package repository metadata under the selected LTS codename",
                "Updated the App Registry JSON repository URL from https://zyphor-os.github.io/v2-ada-lovelace/registry/registry.json to https://zyphor-os.github.io/ada-lovelace-lts/registry/registry.json",
                "Updated the Zyphor APT repository from https://zyphor-os.github.io/v2-ada-lovelace stable main to https://zyphor-os.github.io/ada-lovelace-lts ada-lovelace main"
            ]
        },
        {
            "title": "ZyWin-UI Package Security",
            "items": [
                "Added post-installation file ownership configuration to ensure /usr/bin/zywin-ui is owned by root"
            ]
        },
    ]
};