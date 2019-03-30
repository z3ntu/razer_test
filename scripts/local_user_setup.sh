#!/bin/bash

set -ex

# Permissions to hidraw files
sudo chmod g+rw /dev/hidraw*
sudo chgrp $(id -g) /dev/hidraw*

# Permission to dbus
dbusconf=/usr/share/dbus-1/system.d/io.github.openrazer1.conf
user=$(id -nu)
# Check if the current username is already in the file
if [ -z "$(grep $user $dbusconf)" ]; then
    # Note: This could be made easier, but I wanted the formatting of the resulting file to be perfect ;)
    # extract policy fragment from file
    policy=$(sed -n '/<policy user/,/<\/policy>/p' $dbusconf)
    # replace newlines with \n (thanks to https://stackoverflow.com/a/33419167/3527128)
    policy=$(echo "$policy" | awk -vORS="\\\n" '1')
    # replace the user with the current one
    policy=$(echo "$policy" | sed 's|user="root"|user="'$user'"|')
    # remove trailing newline
    policy=$(echo "$policy" | head -c -3)
    # append the updated fragment to the file
    sudo sed -i "/<busconfig>/a \\\n$policy" $dbusconf
fi
