#!/bin/bash

# Matthew Diamond 2016
# This script will make a build directory, enter it
# and then use cmake to generate a makefile, followed
# by make to build the program.

if [ ! -d bin ]; then
    mkdir bin
fi
cd bin
cmake -G Ninja ..
ninja

