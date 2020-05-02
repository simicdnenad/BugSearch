# Bugce

Small application which is searching throught "/test/landscape.txt" file for a char pattern written in "/test/bug.txt".

## Getting Started

Generate Eclipse project file (to be able to Debug project from Eclipse IDE):
1. Create /build folder,
2. Run $cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug .. ;

## Usage

To be able to test application, under "Eclise->Project properties->Run/Debug Settings" add input arguments:
../test/bug.txt ../test/landscape.txt

--------------------------------------------------------------------------------------------------------------------------
Bugfixing/upgrading of Bugce.

Upgrading Bugce:
1. Add auto f1 = std::async(&Test::calculate, this, 11, 20);
       auto res1 = f1.get();
   for fetching results from thread. Try (only for training) std::future and std::promise things...
2. Add static fields for loading bug and landscape one time instead of multiple times...

Fixed:
Bug.cpp:98, On Linux string::npos (circa 10pow8) don't match std::string::find() return value if string not find (-1); On Windows (VS) this is working ok.
