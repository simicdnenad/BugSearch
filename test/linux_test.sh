#!/bin/bash
#
# Script for Building/Testing BugSearch app
#
#

SYSTEM="uname"
TARGET="${BUGSEARCH_BASE}/../build/BugceAppLinux"

function create_build_dir() {
  if [ ! -r "${BUGSEARCH_BASE}/../build" ]; then
    echo "Creating /build folder in sibling folder of cloned repo"
    mkdir ${BUGSEARCH_BASE}/../build
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
  cd ${BUGSEARCH_BASE}/../build
  echo "Generating Eclipse project file"
  cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ${BUGSEARCH_BASE}
  echo "Building project"
  make -f Makefile
}

function run_app()
{
  echo "Running app."
  ${BUGSEARCH_BASE}/../build/BugceAppLinux --bug_file ${BUGSEARCH_BASE}/test/bug.nfo --landscape_file ${BUGSEARCH_BASE}/test/landscape.nfo
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


