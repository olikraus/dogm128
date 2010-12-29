#!/bin/sh

# set test configuration
dut=uno_dogxl
examples="SpaceTrash GraphicTest Walk"
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
  cp ../../libraries/dogm128.zip ./release

# unzip release
  cd release
  unzip -o dogm128.zip > /dev/null

# copy makefiles
  cp ../Makefile*${dut}* ./Dogm/examples/$e/.

# execute makefile
  cd Dogm/examples/$e/
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

