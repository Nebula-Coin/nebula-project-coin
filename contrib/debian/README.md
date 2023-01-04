
Debian
====================
This directory contains files used to package nebulaprojectd/nebulaproject-qt
for Debian-based Linux systems. If you compile nebulaprojectd/nebulaproject-qt yourself, there are some useful files here.

## nebulaproject: URI support ##


nebulaproject-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install nebulaproject-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your nebulaproject-qt binary to `/usr/bin`
and the `../../share/pixmaps/nebulaproject128.png` to `/usr/share/pixmaps`

nebulaproject-qt.protocol (KDE)

