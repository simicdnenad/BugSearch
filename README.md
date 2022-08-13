# BugSearch

Small application which is searching throught "/manual_test/landscape.nfo" file for a multiline string pattern from "/manual_test/bug.nfo" file.

## Prerequisities (Tested on)
18.04.1-Ubuntu (Debian),  
cmake version 3.10.2,  
g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0,  
Eclipse 2019-06 (4.12.0);

Windows 8,  
VS 2017 Community (VC141),  
Boost 1.65.1;  

## Compilation
Clone repo with:  
1. $git clone https://github.com/simicdnenad/BugSearch.git; 
### Under Linux:
1. Define global variable "BUGSEARCH_BASE" with content of directory path of cloned repo (e.g. $export BUGSEARCH_BASE=/home/nenad/Desktop/workspace_work/BugSearchRepo/BugSearch),  
1. Run script $BUGSEARCH_BASE/manual_test/linux_test.sh;  

### Under Windows: 
1. Add Environment Variable: BOOST_ROOT (e.g. "C:\Program Files\boost\boost_1_65_1"),  
2. Generate VS2017 project file (to be able to Debug project from Visual Studio IDE): Run $cmake -G"Visual Studio 15 2017 Win64" ../Bug_search/;  

## Testing
### Under Linux:
#### Under Eclipse IDE
1. To be able to test application, under "Eclise->Project properties->Run/Debug Settings" add input arguments:  
--bug_file ../Bug_search/manual_test/bug.nfo  
--landscape_file ../Bug_search/manual_test/landscape.nfo  
(or File->Import->Run/Debug->Launch Configurations "/manual_test/BugceAppDebugConfig.launch")
#### From command line
1. Run script $Bug_search/manual_test/linux_test.sh;  

### Under Windows:
#### Under VS2017 IDE:
1. Under "Project Properties->Debugging->Command Arguments add (apsolute paths):
--bug_file "D:\\Bugce\\Bug_search\\manual_test\\bug.nfo"   --landscape_file "D:\\Bugce\\Bug_search\\manual_test\\landscape.nfo"  
2. In same panel edit "WorkingDirectory"=$(ProjectDir)\Debug;

--------------------------------------------------------------------------------------------------------------------------------------------------

## Bugfixing/Upgrading.  
TODO:  
1. Add more input Bug files (via Boost program options) and do search for all of them;  

Upgraded:  
1. Changed to creating thread body on functor objects instead on method body,  
2. Added Boost program options for handling input program arguments;  

Fixed:  
1. On Linux and Windows platforms, string::npos doesn't match (-1 under Windows and circa 10pow8 under Linux),  
2. Added finding of memory leaks on Windows with the CRT library;  

Bugs:
1. On TAB character inserted as a part of the line in "landscape.nfo" file, bug pattern is not found; (it is treated as one character)  
