#!/bin/sh

# get directory of script and cd to it
BINARY_DIR="$( cd "$( dirname "$0" )" && pwd )"
cd $BINARY_DIR

get_abs_filename() {
  echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
}

SWIFTGUI_DIR=$(get_abs_filename "../lib")
CEF_DIR=$(get_abs_filename "../../../../cef")

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SWIFTGUI_DIR:$CEF_DIR/Release
./simple --resources-dir-path="$CEF_DIR/Resources" --locales-dir-path="$CEF_DIR/Resources/locales"
