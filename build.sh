#!/bin/bash
if [ "$1" == "clean" ]; then rm -rf build/*;exit; fi
if [ ! -d ./build ]; then mkdir build; fi
cd build
if [ ! -f ./Makefile ]; then cmake ..; fi
cores=`cat /proc/cpuinfo | grep -c processor`
jobNum=`echo "$cores + 1" | bc`
if [ "$1" == "" ]
then
    make "-j$jobNum"
else
    make "-j$jobNum" "$*"
fi
status=$?
cd ..
exit $status

