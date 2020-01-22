#! /bin/bash
currentDir=$PWD
rm -rf build
mkdir build
cd build
# Add your install path
cmake .. -DCMAKE_INSTALL_PREFIX=../../../npm/phd/g4npm
make -j
make install
cd $currentDir