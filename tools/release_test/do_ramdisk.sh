#!/bin/sh
mkdir release
sudo mount -t tmpfs -o size=10M,nr_inodes=5k,mode=0777 tmpfs ./release
df -k ./release
