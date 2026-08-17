const releaseNotes = {
    distro: "Zyphor OS Ada Lovelace LTS",
    version: "2.2.1-ada-lovelace-lts-u35",
    date: "Updated On: August 17, 2026 @ 03:21 PM",

    sections: [
        {
            "title": "SMB Module — Samba Sharing - Latest",
            "items": [
                "Added zyphor smb share-add to publish a local folder as a Samba share, with folder/user validation and automatic smbd restart",
                "Added zyphor smb share-list to display all configured shares from smb.conf",
                "Added zyphor smb share-remove to delete an existing share and restart smbd",
                "Extended zyphor_smb_ensure to check for and install the samba package alongside smbclient, cifs-utils, and ntfs-3g",
                "share-add now skips re-prompting for a Samba password if the user already has one set via pdbedit",
                "share-add prints the ready-to-use Windows UNC path after a successful share creation"
            ]
        },
        {
            "title": "ZyWin Package Update",
            "items": [
                "Adjusted how the return value of std::vector::at() is handled inside execIso to remove compiler warnings",
                "Changed sanitizeFileName to a static class member",
                "Changed getWinePrefix to a static class member",
                "Changed ensureWinePrefix to a static class member",
                "Changed findExecutables to a static class member"
            ]
        },
    ]
};