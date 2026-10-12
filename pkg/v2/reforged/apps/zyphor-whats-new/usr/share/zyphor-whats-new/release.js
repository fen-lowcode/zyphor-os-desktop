const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u70",
    date: "Updated On: October 12, 2026 @ 02:58 PM",

    sections: [
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
            "title": "GRUB OS Probing",
            "items": [
                "Automatically enables GRUB OS probing to detect other operating systems during boot",
                "Automatically regenerates the GRUB configuration after enabling OS probing"
            ]
        }
    ]
};