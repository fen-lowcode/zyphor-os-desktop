const releaseNotes = {
    distro: "Zyphor OS 3 \"Bethany\" LTS",
    version: "3.2.0-bethany-lts-u4",
    date: "Updated On: October 20, 2026 @ 09:52 AM",

    sections: [
        {
            "title": "Dev Environment Version Management and Undo",
            "items": [
                "Added the new 'zyphor setup dev version' command, which lists the PHP and Node.js versions available on your system and shows which one is currently installed or active",
                "Use 'zyphor setup dev version php <X.Y>' to install a specific PHP version (for example 8.3) together with the common extensions and switch the php command to it, with an optional prompt to add the Sury PHP repository when the version is not available in your current sources",
                "Use 'zyphor setup dev version node <version>' to install and switch to a specific Node.js version (for example 22), and a minor version such as 22.5 is also accepted and pinned when it is still available",
                "Added the new 'zyphor setup dev undo <target>' command, which reverses a development setup for PHP, Node.js, Composer, Git, Android Studio with Flutter, and OS development",
                "Every undo asks for confirmation first, shows exactly what will be removed before anything changes, and keeps packages the base system depends on so the machine is never left in a broken state",
                "Your personal data is protected: SSH keys and the Android SDK are only deleted when you confirm them separately, and your shell configuration is backed up before the PATH entries added by the setup are removed"
            ]
        },
        {
            "title": "New Zyphor CLI Features",
            "items": [
                "Added the new 'zyphor setup dev <target>' command, which sets up a complete development environment in one step with targets for PHP, Git, Node.js, Composer, Android Studio with Flutter, and OS development",
                "The PHP target now installs PHP together with the GD, cURL, mbstring, XML, SQLite, Zip, Intl, BCMath and MySQL extensions, then installs Composer and Node.js 20 and verifies that each extension loaded correctly",
                "The Git target configures your name and email, generates an SSH key in your own home directory (even when run with sudo), and tests the connection to GitHub",
                "The Android Studio target installs Android Studio and Flutter from the archives you download, detects them automatically in your Downloads folder, supports optional SHA-256 checksum verification, and sets up your PATH for you",
                "The OS target installs everything needed to build kernels, initramfs images, bootable ISOs, and live-build projects, including QEMU and OVMF for testing in a virtual machine",
                "Improved security of the setup tools: the Composer installer is verified against its official signature, and the NodeSource script is downloaded to a file before it runs instead of being piped directly into the shell",
                "Added helpful messages and clear error handling across all setup targets, with automatic detection of your real user account so files are never written to the root account by mistake"
            ]
        },
        {
            "title": "Windows/Super Key Opens Whisker Menu",
            "items": [
                "Configured the Windows/Super key to trigger the XFCE Whisker Menu, providing a familiar Start Menu experience similar to Windows",
                "Improved keyboard navigation by allowing users to access the application menu directly using the Windows/Super key"
            ]
        },
        {
            "title": "Redesigned Zyphor Command Center",
            "items": [
                "Introduced a completely redesigned Zyphor Command Center with a modern dark look",
                "Added a new sidebar with the Zyphor logo, grouped sections (Account, System, Doctor, Packages), and a highlighted current page",
                "Every page now has a clear title and a short description of what it does",
                "Added a terminal-style output panel with easier-to-read text and a Clear button",
                "Added a progress animation while System Upgrade, System Clean, Doctor, and Package tasks are running",
                "Added clear ✔ Finished and ✖ Failed messages at the end of every task, so you always know how it went",
                "Task buttons are now disabled while a task is running to prevent starting the same task twice",
                "Pressing Enter in the Search, Install, and Uninstall boxes now starts the action",
                "Colored terminal codes are no longer shown as unreadable symbols in the output panel",
                "Command Center now opens faster, and each page loads only when you open it",
                "The Packages/Apps list now loads automatically the first time you open it",
                "User Management now starts its local service only when you open it",
                "If a page fails to open, an error message is shown instead of closing the whole app"
            ]
        },
        {
            "title": "Security & Reliability",
            "items": [
                "Improved the safety of the Search, Install, and Uninstall package boxes so typed text can no longer be run as system commands",
                "Package names that begin with a dash are now rejected as invalid input",
                "Improved how long task output is displayed, so lines no longer break in the wrong places"
            ]
        },
        {
            "title": "New Desktop Environment",
            "items": [
                "Introduced a refreshed Zyphor OS desktop environment with an updated visual experience",
                "Added a new default desktop layout with updated panel launchers and desktop shortcuts",
                "Added new default wallpapers and refreshed desktop artwork",
                "Updated the default application launcher for a cleaner and more accessible desktop experience",
                "Improved the default XFCE desktop configuration for new installations and newly created users",
                "Desktop environment updates can be applied through sudo zy system upgrade",
                "New desktop configurations are applied to newly created users through /etc/skel",
                "Existing users can keep their current desktop configuration until they deliberately apply the new desktop environment updates",
                "Desktop configuration updates are being tested in a virtual machine to help prevent existing user configurations from being overwritten or damaged"
            ]
        }
    ]
};