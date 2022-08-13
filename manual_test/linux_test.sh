#!/bin/bash
#
# Script for Building/Testing BugSearch app
#
#

SYSTEM="uname"
TARGET="${BUGSEARCH_BASE}/../build/src/BugSearchLinux"

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

function clone_gtest_submodule()
{
  echo "Cloning gtest submodule."
  cd ${BUGSEARCH_BASE}/lib
  git submodule update --init --recursive
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
  $TARGET --bug_file ${BUGSEARCH_BASE}/manual_test/bug.nfo --landscape_file ${BUGSEARCH_BASE}/manual_test/landscape.nfo
}

function main()
{
# print_info
  printf "\n"
  create_build_dir
  printf "\n"
  clone_gtest_submodule
  printf "\n"
  build_app
  printf "\n"
  run_app  	
}

main $*


