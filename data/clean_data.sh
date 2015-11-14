#!/bin/bash
pushd `dirname $0` > /dev/null; SCRIPTPATH=`pwd`; popd > /dev/null;

for file in $(ls $SCRIPTPATH); do
   if [ -f $file ]; then
      sed -i "s/\x80//g" $file
      sed -i "s/\xdb//g" $file
      sed -i "s/\x0a\x0d/\x0a/g" $file
      sed -i "s/\x0d\x0a/\x0a/g" $file
      sed -i "s/\x0d/\x0a/g" $file
   fi
done
