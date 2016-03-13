#!/bin/sh

# get directory of script and cd to it
BINARY_DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $BINARY_DIR

LIB_DIR=$BINARY_DIR/../lib

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIB_DIR:$BINARY_DIR ./simple
