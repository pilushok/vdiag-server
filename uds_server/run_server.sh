#!/bin/sh

sudo modprobe vcan
sudo modprobe can-isotp
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
../bin/uds_server/uds_server $1 $2 $3
