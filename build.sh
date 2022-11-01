#!/bin/bash
if [ "$1" == "clean" ]; then rm -rf build/*;exit; fi
if [ ! -d ./build ]; then mkdir build; fi
cd build
if [ ! -f ./Makefile ]
    then cmake -G Ninja .. \
    -DC_COMPILER=gcc \
    -DCXX_COMPILER=g++ \
    -DSTATIC_COMPILE=0
fi

ninja
