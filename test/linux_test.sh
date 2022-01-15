#!/bin/bash
#
# Script for testing BugSearch app
#
#
if [ ! -r "${BugSearchRepoDir}/../build" ]; then
  echo "Creating /build folder in sibling folder of cloned repo"
  mkdir ${BugSearchRepoDir}/../build
fi

cd ${BugSearchRepoDir}/../build
echo "Generate Eclipse project file"
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ${BugSearchRepoDir}
echo "Building project"
make -f Makefile
echo "Running app."
${BugSearchRepoDir}/../build/BugceAppLinux --bug_file ${BugSearchRepoDir}/test/bug.txt --landscape_file ${BugSearchRepoDir}/test/landscape.txt
