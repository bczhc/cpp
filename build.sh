#!/bin/bash
if [ "$1" == "clean" ]; then rm -rf build/*;exit; fi
if [ ! -d ./build ]; then mkdir build; fi
if [ ! -f ./build/Makefile ]; then cmake -S . -B ./build; fi
cores=`cat /proc/cpuinfo | grep -c processor`
jobNum=`echo "$cores + 1" | bc`
cd build
if [ "$1" == "" ]
then
    make "-j$jobNum"
else
    make "-j$jobNum" "$*"
fi
cd ..

