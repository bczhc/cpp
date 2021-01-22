#!/bin/bash
if [ "$1" == "clean" ]; then rm -rf build/*;exit; fi
if [ ! -d ./build ]; then mkdir build; fi
cd build
if [ ! -f ./Makefile ]
    then cmake -G "Unix Makefiles" .. \
    -DC_COMPILER=gcc \
    -DCXX_COMPILER=g++ \
    -DSTATIC_COMPILE=0
fi
cores=`cat /proc/cpuinfo | grep -c processor`
jobNum=$(( $cores + 1 ))
if [ "$1" == "" ]
then
    make "-j$jobNum"
else
    make "-j$jobNum" "$@"
fi
status=$?
cd ..
exit $status

