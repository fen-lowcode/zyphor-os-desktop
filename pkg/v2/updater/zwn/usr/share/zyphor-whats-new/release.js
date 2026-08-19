const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u38",
    date: "Updated On: August 19, 2026 @ 01:40 PM",

    sections: [
        { 
            "title": "Zyphor Repository Build",
            "items": [
                "Added LTS codename input to the zyphor build repo command for generating package repository metadata under the selected LTS codename"
            ]
        },
        {
            "title": "ZyWin-UI Package Security",
            "items": [
                "Added post-installation file ownership configuration to ensure /usr/bin/zywin-ui is owned by root"
            ]
        },
        {
            "title": "ZyWin-UI Package",
            "items": [
                "Added a graphical user interface for installing Windows applications on Zyphor OS",
                "Added file browser support for selecting EXE, MSI, and ISO application installers",
                "Added installation status and progress feedback while ZyWin is running",
                "Added automatic desktop launcher creation for ZyWin-UI",
                "Added ZyWin-UI application icon and desktop menu integration",
                "Added support for launching ZyWin directly from the graphical installer"
            ]
        },
        {
            "title": "SMB Module — Samba Sharing",
            "items": [
                "Added zyphor smb share-add to publish a local folder as a Samba share, with folder/user validation and automatic smbd restart",
                "Added zyphor smb share-list to display all configured shares from smb.conf",
                "Added zyphor smb share-remove to delete an existing share and restart smbd",
                "Extended zyphor_smb_ensure to check for and install the samba package alongside smbclient, cifs-utils, and ntfs-3g",
                "share-add now skips re-prompting for a Samba password if the user already has one set via pdbedit",
                "share-add prints the ready-to-use Windows UNC path after a successful share creation"
            ]
        },
    ]
};