#!/bin/bash
PROGRAM="rpi-sys-monitor"
BIN_LOCATION="/usr/local/bin/$PROGRAM"
SERVICE_FILE="/etc/systemd/system/$PROGRAM.service"

sudo systemctl stop $PROGRAM

sudo rm -rf $SERVICE_FILE

sudo rm -rf $BIN_LOCATION
