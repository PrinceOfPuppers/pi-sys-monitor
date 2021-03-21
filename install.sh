#!/bin/bash
PROGRAM="rpi-sys-monitor"
BIN_LOCATION="/usr/local/bin/$PROGRAM"
SERVICE_FILE="/etc/systemd/system/$PROGRAM.service"

make compile
sudo chmod +x "build/$PROGRAM"

sudo cp "build/$PROGRAM" $BIN_LOCATION

sudo touch $PROGRAM.service

echo "[Unit]"                                   | sudo tee -a $PROGRAM.service
echo "Description=rpi sense hat system monitor" | sudo tee -a $PROGRAM.service
echo ""                                         | sudo tee -a $PROGRAM.service

echo "[Service]"                                | sudo tee -a $PROGRAM.service
echo "ExecStart=$BIN_LOCATION"                  | sudo tee -a $PROGRAM.service

sudo mv $PROGRAM.service $SERVICE_FILE
sudo systemctl start $PROGRAM