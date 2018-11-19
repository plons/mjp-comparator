#!/bin/bash

mkdir build
pushd build
cmake  -G'Eclipse CDT4 - Ninja' \
   -DCMAKE_BUILD_TYPE=Debug \
   ../project; result=$?
popd
