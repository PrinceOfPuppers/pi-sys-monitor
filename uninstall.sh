#!/bin/bash
PROGRAM="rpi-sys-monitor"
BIN_LOCATION="/usr/local/bin/$PROGRAM"
SERVICE_FILE="/etc/systemd/system/$PROGRAM.service"

sudo systemctl stop $PROGRAM
sudo systemctl disable $PROGRAM

sudo rm $SERVICE_FILE

sudo rm $BIN_LOCATION

#rm symlink
sudo rm /etc/systemd/system/multi-user.target.wants/raspberrypi-net-mods.service

systemctl daemon-reload