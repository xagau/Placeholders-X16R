
Debian
====================
This directory contains files used to package placehd/placeh-qt
for Debian-based Linux systems. If you compile placehd/placeh-qt yourself, there are some useful files here.

## placeh: URI support ##


placeh-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install placeh-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your placeh-qt binary to `/usr/bin`
and the `../../share/pixmaps/placeh128.png` to `/usr/share/pixmaps`

placeh-qt.protocol (KDE)

