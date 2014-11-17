#!/bin/bash

mkdir build
pushd build
cmake  "-GEclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=true ../project; result=$?
popd
