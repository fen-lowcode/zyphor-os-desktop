const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u32",
    date: "Updated On: August 04, 2026 @ 10:14 AM",

    sections: [
        {
            "title": "🛡️ Pentest Toolkit Setup - Latest",
            "items": [
                "Added a new `zyphor setup pentest` command for installing security testing tools by category.",
                "Introduced targets: recon, web, wireless, passwords, sniffing, forensics, exploitation, and full.",
                "Recon target installs nmap, masscan, amass, gobuster, theharvester, and related tools.",
                "Web target installs sqlmap, nikto, burpsuite, zaproxy, wpscan, and ffuf.",
                "Wireless target installs aircrack-ng, kismet, wifite, and reaver.",
                "Passwords target installs john, hashcat, hydra, medusa, and crunch.",
                "Sniffing target installs wireshark, tcpdump, and bettercap, and adds the user to the wireshark group for non-root capture.",
                "Forensics target installs autopsy, sleuthkit, binwalk, and volatility3.",
                "Exploitation target installs the Metasploit Framework, exploitdb, and set.",
                "Setup commands now skip packages that are already installed instead of reinstalling them on every run.",
                "Wireless and exploitation targets display an authorized-use reminder after installation."
            ]
        },
        {
            "title": "📸 Screenshot Tool Update",
            "items": [
                "Removed Flameshot from the default Zyphor OS software stack.",
                "Replaced Flameshot with Ksnip as the default screenshot and screen capture application.",
                "Added Ksnip to the default Zyphor OS package dependencies.",
                "Improved screenshot capture and annotation capabilities with Ksnip.",
                "Updated the Zyphor OS package configuration to prevent conflicts between Ksnip and Flameshot.",
                "Users upgrading from previous Zyphor OS releases will transition from Flameshot to Ksnip."
            ]
        },
        {
            "title": "✨ Profile Management Improvements",
            "items": [
                "Introduced a redesigned Profile page for easier account management.",
                "Users can now upload and update their profile picture directly from the Command Center.",
                "Added automatic square cropping for uploaded profile images.",
                "Improved image validation to support PNG and JPG formats.",
                "Enhanced profile data loading and saving reliability.",
                "Profile images are now displayed in the desktop panel Start Menu for a more personalized experience.",
                "Integrated user profile pictures into the LightDM login screen.",
                "Optimized profile picture processing for better quality and consistency.",
                "Improved the overall profile management user experience."
            ]
        },
        {
            "title": "Change Log",
            "items": [
                "Relocated the release variable from release.js to release-notes.html for easier maintenance."
            ]
        },
        {
            "title": "Repository Configuration",
            "items": [
                "Updated the default Zyphor OS repository configuration to use the 'arch=amd64' filter.",
                "APT will now only fetch Zyphor packages for the AMD64 architecture, reducing unnecessary package index downloads.",
                "This change improves repository consistency and prepares the package manager for future multi-architecture support."
            ]
        },
        {
            "title": "Package Manager",
            "items": [
                "Added Flatpak support to 'zyphor pkg install', 'zyphor pkg list', and 'zyphor pkg remove', including automatic Flathub remote setup and retry-on-failure handling.",
                "Installing a Flatpak app now creates a CLI shim (e.g. 'wechat') so it can be launched by name instead of requiring 'flatpak run <app-id>'.",
                "Removing a Flatpak app now also cleans up its associated CLI shim."
            ]
        }
    ]
};