# Bugce

Small application which is searching throught "/test/landscape.txt" file for a char pattern written in "/test/bug.txt".

## Getting Started

Generate Eclipse project file (to be able to Debug project from Eclipse IDE):
1. Create /build folder in sibling folder of cloned repo,
2. Run $cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../github-upload/ ;

## Usage
## Under Eclipse IDE
To be able to test application, under "Eclise->Project properties->Run/Debug Settings" add input arguments:
../github-upload/test/bug.txt ../github-upload/test/landscape.txt
(or File->Import->Run/Debug->Launch Configurations "/test/BugceAppDebugConfig.launch")

## From command line
1. Go to /build folder,
2. $make -f Makefile,
3. $./BugceApp ../github-upload/test/bug.txt ../github-upload/test/landscape.txt;
--------------------------------------------------------------------------------------------------------------------------
Bugfixing/upgrading of Bugce.

Upgrading Bugce:
1. Implement application as template. (Object should be created as (e.g.) CBug<string,array<string>::iterator,array>). 
COMPLETE FIRST THIS WAY! LATER TRY SOME SIMPLER LOGIC (CONSIDERING THAT STRING DATA TYPE IS ALWAYS USED)!!! 

2. Add std::vector for thread access (instead of use of an array of *(std::thread)).
2.1 BUG: when i do std::vector<CBug>::push_back(CBug()) comiler reports error: "use of deleted function CBug(const CBug&)"!!!


Fixed:
Bug.cpp:98, On Linux string::npos (circa 10pow8) don't match std::string::find() return value if string not find (-1); On Windows (VS) this is working ok.
