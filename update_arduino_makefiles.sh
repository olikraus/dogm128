#!/bin/sh

# in all makefiles, replace the version number of the Arduino IDE
find . -name "Makefile*" -exec sed -i -e "s/\(duino.\)0021/\10022/g" {} \;
