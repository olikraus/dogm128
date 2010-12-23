#!/bin/sh

# set test configuration
set dut=uno_dogxl

# create mount point
mkdir release

# note: ensure, that mount can be called via sudo (see visudo)
#sudo mount -t tmpfs -o size=10M,nr_inodes=5k,mode=0777 tmpfs ./release

# copy release
cp ../../libraries/dogm128.zip ./release

# unzip release
cd release
unzip -o dogm128.zip

# copy makefiles
cp ../Makefile*${dut}* ./Dogm/examples/SpaceTrash/.

# execute makefile
cd Dogm/examples/SpaceTrash/
for m in Makefile.*
do
  make -f $m clean
  make -f $m upload
done

cd ../../../..




#sudo umount ./release

