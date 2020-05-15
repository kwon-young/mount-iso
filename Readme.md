# Mount Iso Service for the Dolphin File Manager

This plugin for the kde dolphin file manager adds a contextual menu entry for .iso files to either mount or unmount the file as a loopback device.
This plugin uses udisksctl to either mount:

```
$ udisksctl loop-setup -r -f myimage.iso
```

or unmount the iso file:

```
$ udisksctl loop-delete -b /dev/loop1
```

In order to know if a specific iso file is already mounted as a loopback device, this plugin also uses the `losetup` command-line program to get the associated loopback device of an iso file:

```
$ losetup -j /home/user/Downloads/myimage.iso
/dev/loop0: []: (/home/user/Downloads/myimage.iso)
```

This plugin has the same functionality as the following .desktop file:

```mountiso.desktop
[Desktop Entry]
Type=Service
ServiceTypes=KonqPopupMenu/Plugin
MimeType=application/x-cd-image;model/x.stl-binary
Actions=mount;unmount;
X-KDE-Priority=TopLevel
X-KDE-StartupNotify=false
Icon=application-x-cd-image
X-KDE-Submenu=Mount/unmount iso image


[Desktop Action unmount]
Name=Unmount
Icon=media-eject
Exec=udisksctl loop-delete -b `losetup -j %f | cut -d ':' -f 1`

[Desktop Action mount]
Name=Mount
Icon=media-mount
Exec=udisksctl loop-setup -r -f %f
```

But instead of always having both `mount` and `unmount` option, this plugin will detect if the iso is already mounted as a loopback device and only show the appropriate menu entry.

# Credits

This plugin was made by using the plasma-vault dolphin service menu: https://github.com/KDE/plasma-vault
