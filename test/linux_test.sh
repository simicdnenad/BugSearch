#!/bin/bash
#
# Script for Building/Testing BugSearch app
#
#

SYSTEM="uname"
TARGET="${BugSearchRepoDir}/../build/BugceAppLinux"

function create_build_dir() {
  if [ ! -r "${BugSearchRepoDir}/../build" ]; then
    echo "Creating /build folder in sibling folder of cloned repo"
    mkdir ${BugSearchRepoDir}/../build
  fi
}

function print_info() {
  echo "Running OS: " 
  eval $SYSTEM
  cat << EOF
TARGET=$TARGET

b      ... generate Makefile and Build app
r      ... run generated Binary file 
q      ... quit script

EOF
}

function build_app()
{
  cd ${BugSearchRepoDir}/../build
  echo "Generating Eclipse project file"
  cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ${BugSearchRepoDir}
  echo "Building project"
  make -f Makefile
}

function run_app()
{
  echo "Running app."
  ${BugSearchRepoDir}/../build/BugceAppLinux --bug_file ${BugSearchRepoDir}/test/bug.txt --landscape_file ${BugSearchRepoDir}/test/landscape.txt
}

function main()
{
# print_info
  printf "\n"
  create_build_dir
  printf "\n"
  build_app
  printf "\n"
  run_app  	
}

main $*


