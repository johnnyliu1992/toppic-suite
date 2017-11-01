#!/bin/sh
if [ "$#" -ne 1 ];then
  echo "Usage: $0 version_number" >&2
  exit 1
fi

rm -rf toppic-linux-${1}
mkdir toppic-linux-${1}
mkdir toppic-linux-${1}/example_files
cp ../bin/toppic toppic-linux-${1} 
cp ../LICENSE toppic-linux-${1}
cp ./example_files/common_mods.txt toppic-linux-${1}/example_files
cp ./example_files/fixed_mod.txt toppic-linux-${1}/example_files
cp -r ../toppic_resources toppic-linux-${1}

mkdir toppic-linux-${1}/toppic_resources/lib

cp /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.58.0 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.58.0 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libboost_system.so.1.58.0 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.58.0 toppic-linux-${1}/toppic_resources/lib/

cp /usr/lib/x86_64-linux-gnu/libxalan-c.so.111 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libxerces-c-3.1.so toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libxalanMsg.so.111 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libicuuc.so.55 toppic-linux-${1}/toppic_resources/lib/
cp /usr/lib/x86_64-linux-gnu/libicudata.so.55 toppic-linux-${1}/toppic_resources/lib/

zip -r toppic-linux-${1}.zip toppic-linux-${1}
