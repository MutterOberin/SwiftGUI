#!/bin/sh

# get directory of script and cd to it
BINARY_DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $BINARY_DIR

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib:.
./simple
