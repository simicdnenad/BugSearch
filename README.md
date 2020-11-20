# Bugce

Small application which is searching throught "/test/landscape.txt" file for a char pattern written in "/test/bug.txt".

## Prerequisities
Tested on:
18.04.1-Ubuntu (Debian),
cmake version 3.10.2,
g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0,
Eclipse 2019-06 (4.12.0);

## Development
Clone repo with:
1. git clone https://github.com/simicdnenad/github-upload.git ;
### Under Linux:
Generate Eclipse project file (to be able to Debug project from Eclipse IDE):
1. Create /build folder in sibling folder of cloned repo,
2. Run $cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../github-upload/ ;

### Under Windows:
Generate VS2015 project file:
1. Run $cmake -G"Visual Studio 14 2015 Win64" ../github-upload/ ;

## Testing
### Under Linux:
### Under Eclipse IDE
To be able to test application, under "Eclise->Project properties->Run/Debug Settings" add input arguments:
../github-upload/test/bug.txt ../github-upload/test/landscape.txt
(or File->Import->Run/Debug->Launch Configurations "/test/BugceAppDebugConfig.launch")
### From command line
1. Go to /build folder,
2. $make -f Makefile,
3. $./BugceApp ../github-upload/test/bug.txt ../github-upload/test/landscape.txt;

### Under Windows:
### Under VS2015 IDE:
1. To be able to test application, under "Project Properties->Debugging->Command Arguments add:
"..\..\test\bug.txt" "..\..\test\landscape.txt"
2. In same panel edit "WorkingDirectory"=$(ProjectDir)\Debug;
--------------------------------------------------------------------------------------------------------------------------
##Bugfixing/Upgrading.
Bugs:
1.(Windows) _CrtDumpMemoryLeaks(); detect memory leaks! Same code on Linux with "valgrind" don't report any leaks;
(maybe because of statically allocated strings?!)

Upgrading Bugce:
1. When i try to replace vector<unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>> => vector<CBug<string, CONTAINER<string>::iterator, CONTAINER>>,
calling of ~CBug() after vector::push_back(CBug()) is making side effects;


Fixed:
Bug.cpp:98, On Linux string::npos (circa 10pow8) don't match std::string::find() return value if string not find (-1); On Windows (VS) this is working ok.
