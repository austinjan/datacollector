#!/bin/sh

path = /home/dest
update_firmware = firmware_update

# update firmware
if [ -f "$update_firmware" ]
then
    cp ${path}/update/${update_firmware} ${path}
    chmod +x ${path}/${update_firmware}
fi