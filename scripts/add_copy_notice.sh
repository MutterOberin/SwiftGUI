#!/bin/bash

shopt -s globstar

text="////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of Swift3D.                                              //
//                                                                            //
// Copyright: (c) 2011-2015 Simon Schneegans & Felix Lauer                    //
//                                                                            //
// This software may be modified and distributed under the terms              //
// of the MIT license.  See the LICENSE file for details.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////"

types=(
    ".cpp"
    ".hpp"
    ".ipp"
)

folders=(
    "../include"
    "../src"
    "../examples"
)

for folder in "${folders[@]}"
do
    for type in "${types[@]}"
    do
        for file in ${folder}/**/*${type}
        do
            if [ -f $file ]
            then
                if grep -q "Swift3D" ${file}
                then
                    echo "Reformatting ${file} ..."
                    sed -e '1,7d' ${file} > /tmp/copyright_tmp && mv /tmp/copyright_tmp ${file}
                    echo "${text}" > /tmp/copyright_tmp
                    cat ${file} >> /tmp/copyright_tmp && mv /tmp/copyright_tmp ${file}
                fi
            fi
        done
    done
done



