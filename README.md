# Bugce

Small application which is searching throught "/test/landscape.txt" file for a char pattern written in "/test/bug.txt".

## Prerequisities
Tested on:  
18.04.1-Ubuntu (Debian),  
cmake version 3.10.2,  
g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0,  
Eclipse 2019-06 (4.12.0);

Windows 8,
VS 2017 Community (VC141),
Boost 1.65.1,

## Development
Clone repo with:  
1. git clone https://github.com/simicdnenad/github-upload.git,  
2. Create /build folder in sibling folder of cloned repo;  
### Under Linux:
Generate Eclipse project file (to be able to Debug project from Eclipse IDE):  
1. Run $cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../github-upload/ ;  

### Under Windows: 
1. Add Environment Variable: BOOST_ROOT (e.g. "C:\Program Files\boost\boost_1_65_1"),  
2. Generate VS2017 project file: Run $cmake -G"Visual Studio 15 2017 Win64" ../github-upload/;  

## Testing
### Under Linux:
### Under Eclipse IDE
To be able to test application, under "Eclise->Project properties->Run/Debug Settings" add input arguments:  
--bug_file ../github-upload/test/bug.txt  
--landscape_file ../github-upload/test/landscape.txt  
(or File->Import->Run/Debug->Launch Configurations "/test/BugceAppDebugConfig.launch")
### From command line
1. Go to /build folder,  
2. $make -f Makefile,  
3. $./BugceAppLinux --bug_file ../github-upload/test/bug.txt --landscape_file ../github-upload/test/landscape.txt;  

### Under Windows:
### Under VS2017 IDE:
1. To be able to test application, under "Project Properties->Debugging->Command Arguments add (apsolute paths):
--bug_file "D:\\Bugce\\github-upload\\test\\bug.txt"   --landscape_file "D:\\Bugce\\github-upload\\test\\landscape.txt"  
2. In same panel edit "WorkingDirectory"=$(ProjectDir)\Debug;

--------------------------------------------------------------------------------------------------------------------------------------------------

## Bugfixing/Upgrading.
Upgrading Bugce:
1. Changed to creating thread body on funciton objects instead on method body,  
2. Added Boost program options for handling input program arguments,  
3. When i try to replace vector<unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>> => vector<CBug<string, CONTAINER<string>::iterator, CONTAINER>>,
calling of ~CBug() after vector::push_back(CBug()) is making side effects;


Fixed:
1. On Linux platform, string::npos (circa 10pow8) doesn't match return value of std::string::find() in "string not find (-1)" case (under Windows),
2. Added finding memory leaks on Windows with the CRT library;
