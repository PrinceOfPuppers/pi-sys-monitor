#!/bin/bash
PROGRAM="rpi-sys-monitor"
BIN_LOCATION="/usr/local/bin/$PROGRAM"
SERVICE_FILE="/etc/systemd/system/$PROGRAM.service"

make compile
sudo chmod +x "build/$PROGRAM"

sudo cp "build/$PROGRAM" $BIN_LOCATION

sudo cp $PROGRAM.service $SERVICE_FILE
sudo chmod 640 $SERVICE_FILE

sudo systemctl daemon-reload
sudo systemctl enable $PROGRAM
sudo systemctl start $PROGRAM