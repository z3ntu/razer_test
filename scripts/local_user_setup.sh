#!/bin/sh

set -ex

# Permissions to hidraw files
sudo chmod g+rw /dev/hidraw*
sudo chgrp $(id -g) /dev/hidraw*

user=$(id -nu)
# Permission to dbus
sudo sed -i 's|user="root"|user="'$user'"|' /usr/share/dbus-1/system.d/io.github.openrazer1.conf

