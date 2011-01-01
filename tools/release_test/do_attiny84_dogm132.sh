#!/bin/sh

# set test configuration
dut=attiny84_dogm132
examples="spacetrash graphics_test"
currdir=`pwd`

# create mount point
#mkdir release

# note: ensure, that mount can be called via sudo (see visudo)
#sudo mount -t tmpfs -o size=10M,nr_inodes=5k,mode=0777 tmpfs ./release

for e in  $examples
do

  cd  $currdir

  rm -rf ./release/*	

# copy release
#  cp ../../libraries/dogm128.zip ./release
  cp ../../atmel_avr/release/dogm128_avr.zip ./release/.


# unzip release
  cd release
  unzip -o dogm128_avr.zip > /dev/null

# execute makefile
  cd attiny84/$e/
  for m in Makefile.*
  do
    echo START: $e $m
    pwd
    make -f $m clean
    make -f $m upload
    make -f $m clean
    echo DONE: $e $m
    sleep 2
  done

done

cd  $currdir

#sudo umount ./release

