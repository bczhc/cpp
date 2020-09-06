#!/bin/bash
if [ "$1" == "" ]; then exit; fi
./build.sh "$1"
./build/$*
