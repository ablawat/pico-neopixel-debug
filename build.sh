#!/bin/sh

# set output directory name
BUILD_DIR=build

# create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

export PICO_SDK_PATH=../../pico-sdk

# generate makefiles
cmake ..

if [ $? -eq 0 ]
then
    # build sources
    make
fi
