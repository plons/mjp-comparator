#!/bin/bash
pushd `dirname $0` > /dev/null; SCRIPTPATH=`pwd`; popd > /dev/null;


if [ $# != 1 ]; then
   echo "Expected exactly one argument containing the name of a subdirectory (e.g. 2018). Bailing out.";
   exit 1;
fi

force_overwrite=0
sub_directory=$1

# Make sure we are working in the current directory
# NOT REQUIRED ANYMORE
#pushd $SCRIPTPATH >/dev/null
#trap 'popd >/dev/null' EXIT HUP INT QUIT PIPE TERM

################################################################################
# Check installation of required tools
################################################################################
hash xlsx2csv  2>/dev/null || { sudo aptitude install xlsx2csv;  }
hash realpath  2>/dev/null || { sudo aptitude install coreutils;  }

################################################################################
# Remove spaces from filenames!
################################################################################
find ${SCRIPTPATH} -depth -name "* *" -execdir rename 's/ /_/g' "{}" \;

################################################################################
# Remove weird symbols from created csv file
################################################################################
function clean_file() {
   if [[ $# -ne 1 ]] || [[ ! -f $1 ]]; then echo "ERROR: clean_file expects a file as argument (received $@)"; exit 1; fi
   local file=$1
   
   sed -i "s/\x80//g" $file
   sed -i "s/\xdb//g" $file

   # Fix windows cariage-return + newline
   #sed -i "s/\x0a\x0d/\x0a/g" $file
   #sed -i "s/\x0d\x0a/\x0a/g" $file
   sed -i "s/\x0d$//g" $file
}

for excel_file in $(find ${SCRIPTPATH}/${sub_directory} -type f -name "*.xlsx"); do
   dirname=$(dirname "${excel_file}")
   filename=$(basename "$excel_file" .xlsx)
   csv_file=${dirname}/${filename}.csv

   excel_file_rel=$(realpath --relative-to=${SCRIPTPATH} ${excel_file})
   csv_file_rel=$(realpath --relative-to=${SCRIPTPATH} ${csv_file})

   if [[ -f $csv_file ]] && [[ $force_overwrite != 1 ]]; then
      echo -n "$csv_file_rel already exists. Do you want to override this file? [y/N] "
      read answer
      if [[ "$answer" != "y" ]]; then
         continue
      fi
   fi
   echo "Converting '${excel_file_rel}' to '${csv_file_rel}'"
   xlsx2csv --delimiter ';' --ignoreempty $excel_file $csv_file
   clean_file $csv_file
done
