#!/bin/bash
#
# Script for testing BugSearch app
#
#
if [ ! -r "../../build" ]; then
  echo "Creating /build folder in sibling folder of cloned repo"
  mkdir ../../build
fi

cd ../../build
echo "Generate Eclipse project file"
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../Bug_search/ 
echo "Building project"
make -f Makefile
echo "Running app."
./BugceAppLinux --bug_file ../Bug_search/test/bug.txt --landscape_file ../Bug_search/test/landscape.txt
