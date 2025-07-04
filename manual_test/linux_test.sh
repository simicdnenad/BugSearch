#!/bin/bash
#
# Script for Building/Testing BugSearch app
#
#

if [ -z "${BUGSEARCH_BASE}" ]
then
  BUGSEARCH_BASE=$(readlink -f "$0")
  BUGSEARCH_BASE=$(dirname "${BUGSEARCH_BASE}")
  BUGSEARCH_BASE="${BUGSEARCH_BASE}/.."
fi

SYSTEM="uname"
TARGET="${BUGSEARCH_BASE}/../build/src/BugSearchLinux"
TEST="${BUGSEARCH_BASE}/../build/tst/BugSearch_tst"

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
  if [[ "$1" == "IPC" ]]; then
    # comile project with usage of Sockets
    PREPROC_DEFINES="$1"
    cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -D PREPROC_DEFINES=${PREPROC_DEFINES} ${BUGSEARCH_BASE}
  else
    cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug  ${BUGSEARCH_BASE}
  fi
  echo "Building project"
  make -f Makefile
}

function build_gui()
{
  cd ${BUGSEARCH_BASE}/../build
  echo "Building app GUI version"
  qmake ${BUGSEARCH_BASE}/BugSearch_GUI/BugSearch_GUI/BugSearch_GUI.pro
  make -f Makefile
}

function run_GUI()
{
  if [[ "$1" == "IPC" ]]; then
    echo "Starting GUI app.\n"
    ${BUGSEARCH_BASE}/../build/Qt_GUI &
  fi
}

function run_app()
{
  echo "Running app."
  $TARGET --bug_file ${BUGSEARCH_BASE}/manual_test/bug.nfo --landscape_file ${BUGSEARCH_BASE}/manual_test/landscape.nfo
}

function run_unit_tests()
{
  echo "Running Unit Tests."
  $TEST
  echo "Creating Code Coverage Report."
  lcov -c -d ${BUGSEARCH_BASE}/../build/tst/CMakeFiles/BugSearch_tst.dir/ -o main_coverage.info
  genhtml main_coverage.info --output-directory ${BUGSEARCH_BASE}/out
}

function create_doxygen_docu()
{
  echo "Creating doxygen documentation."
  cd ${BUGSEARCH_BASE}
  doxygen
}

function main()
{
# Build everyting in case of no arguments provided
if [ "$#" -eq 0 ]  || [ "$1" == "IPC" ];
then
  printf "\n"
  create_build_dir
  printf "\n"
  clone_gtest_submodule
  printf "\n"
  build_gui
  printf "\n"
  run_GUI $1
  printf "\n"
  build_app $1
  printf "\n"
  run_app
  printf "\n"
  run_unit_tests
  printf "\n"
  create_doxygen_docu
elif [[ "$1" == "clean" ]];
then
  printf "Cleaning build dir \n"
  rm -rf ../build
elif [[ "$1" == "GUI_ONLY" ]];
then
  printf "\n"
  build_gui
else
  printf "Unknown argument!\n"
fi
}

main $*
