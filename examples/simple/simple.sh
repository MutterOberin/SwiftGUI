#!/bin/sh

# get directory of script and cd to it
DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $DIR

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DIR ./simple
