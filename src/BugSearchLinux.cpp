#include "Bug.h"

#include <algorithm>
#include <time.h>
#include <vector>
using namespace landscape;
typedef CBug<string, CONTAINER<string>::iterator, CONTAINER> CBugT;






#ifdef IPC
#include "SocketClass.h"
#endif

/**
 * @brief loads from files Bug (pattern to be searched for) and Landscape (bigger file through which is searched) inside of program structures
 *
 * loads landscape and bug text files in container<string> data structures to provide searching. In case when multithreading preprocessor const is enabled
 * data is processed between multiple threads in parallel, and number of found Bugs is updated.
 *
 * @param vm			program arguments stored in variables_map structure. Represent names of files which are storing data to be processed.
 * @param vBugFilesPaths	vector of strings. Those strings are names of files which store Bug patterns to be serched for. 
 * @return int (-1 is false, >=0 is true)
 */
int processData(po::variables_map& vm, vector<string>& vBugFilesPaths) {
	unsigned int NumOfLines = 0;
	vector<string>::iterator iBugFilesPaths = vBugFilesPaths.begin();
#ifndef MULTI_THREAD
	while (iBugFilesPaths != vBugFilesPaths.end()) {
		string sFileName = (*iBugFilesPaths).substr((*iBugFilesPaths).rfind(DIR_SEPARATOR) + 1,
			(*iBugFilesPaths).rfind('.') - (*iBugFilesPaths).rfind(DIR_SEPARATOR) - 1);
		CBug<string, CONTAINER<string>::iterator, CONTAINER> bubica(sFileName);

		if (bubica.OnInit(iBugFilesPaths, vm["landscape_file"].as<std::string>()) == CBug<string, CONTAINER<string>::iterator, CONTAINER>::EFileOpenErrors::ALL_SUCCESSFULL) {
			cout << "Input files are correctly opened and loaded in memory." << '\n';
		}
		else
			cout << "Input files are not correctly opened!!!" << '\n';

		cout << "Number of lines:" << (NumOfLines = CBug<string, CONTAINER<string>::iterator, CONTAINER>::GetNumOfLines()) << '\n';
		bubica.NumOfBugs();
		cout << "Number of found bugs:" << (NumOfLines = bubica.GetNumOfBugs()) << '\n';
		iBugFilesPaths++;
	}
#else
	while (iBugFilesPaths != vBugFilesPaths.end()) {
		CBugT::EFileOpenErrors eFileOpen;
		if ((eFileOpen = CBugT::OnInit(iBugFilesPaths, vm["landscape_file"].as<std::string>())) != CBugT::EFileOpenErrors::ALL_SUCCESSFULL) {
			cout << CBugT::mapFileErrors.at(eFileOpen);
			if (eFileOpen == CBugT::EFileOpenErrors::LANDSCAPE_FAIL) {
				cout << vm["landscape_file"].as<std::string>() << endl;
				return -1;
			}
			else if (eFileOpen == CBugT::EFileOpenErrors::BUG_FAIL)
				cout << *iBugFilesPaths << endl;
			else
				cout << endl;
			iBugFilesPaths++;
			continue;
		}

		cout << "Number of lines:" << (NumOfLines = CBugT::GetNumOfLines()) << '\n';

		string sFileName = (*iBugFilesPaths).substr((*iBugFilesPaths).rfind(DIR_SEPARATOR)+1,
			(*iBugFilesPaths).rfind('.')-(*iBugFilesPaths).rfind(DIR_SEPARATOR)-1);
		vector<std::thread> vThreads;
		vector<unique_ptr<CBugT>> vupBugs;
		for (unsigned int i = 0; i < NumOfLines / LINES_PER_THREAD + 1; i++) {
			vupBugs.push_back((unique_ptr<CBugT>)(new CBugT(sFileName)));
			vThreads.push_back(std::thread(std::ref(*(vupBugs.back().get())), i*LINES_PER_THREAD));				// must use std::ref() to avoid object copying to created thread
		}

		auto iupBugs = vupBugs.begin();
		for (std::thread& rThread : vThreads) {
			if (rThread.joinable())
				rThread.join();
			cout << "Number of Bugs (tid=" << ((*iupBugs).get())->GetThreadId() << ")=" << ((*iupBugs).get())->GetNumOfBugs() << "\n";
			iupBugs++;
		}
		vThreads.erase(vThreads.begin(), vThreads.end());
		vupBugs.erase(vupBugs.begin(), iupBugs);
		cout << "Total number of Bugs: " << CBugT::GetTotNumOfBugs() << "\n";

		iBugFilesPaths++;
	}
#endif
	return 0;
}

#ifdef IPC
/**
 * @brief loads Bug and Landscape files paths
 *
 * loads landscape and bug text files paths from GUI application via IPC.
 * Number of bug patterns found in landscape file is calculated.
 *
 * @return int (!=0 is false, ==0 is true)
 */
int main()
{
	int retVal = 0;

	CSocket socketWaitClient;
	socketWaitClient.initSocket();
	std::cout << "Waiting IPC command for program start!" << endl;
	if (socketWaitClient.listenSocket() == false) {
		retVal = 1;
		return retVal;
	}
	socketWaitClient.ReadMsg();
	uint8_t* pPaths = 0;
	uint8_t uSize = socketWaitClient.GetBuff(pPaths);

	std::string sPaths((char*)pPaths, uSize);
	cout << "Received file paths: " << sPaths << endl;

	string strPath;
	unsigned found_at = 0, start_from = 0;
	while ( (found_at = sPaths.find(".", start_from )) != NOT_FOUND && start_from <= uSize ){
		strPath = sPaths.substr( start_from, found_at - start_from + 1 + FILE_EXT_SIZE );
		start_from = found_at + 1 + FILE_EXT_SIZE;
		cout << "found_at =" << found_at << endl;
		cout << strPath << endl;
	}

	// TODO: forward paths to data processing methods.

	return retVal;
}

#else
/**
 * @brief loads Bug and Landscape files paths
 *
 * loads landscape and bug text files paths via program arguments. Then they are mapped using po::variables_map for easier handling.
 * Number of bug patterns found in landscape file is calculated.
 *
 * @param ac			number of program arguments (including program name)
 * @param av			paths to the files following "--bug_file(s) arg --landscape_file arg" format
 * @return int (!=0 is false, ==0 is true)
 */
int main(int ac, char** av)
{
	int retVal = 0;
	po::variables_map vm;
	po::options_description desc("Allowed Options");

	// declare arguments (boost)
	desc.add_options()
		("bug_file(s)", po::value<vector<string>>()->multitoken(), "Provide relative path to \"bug.nfo\" file(s)")
		("landscape_file", po::value<std::string>()->required(), "Provide relative path to \"landscape.nfo\" file");
	try {
#ifdef CHECK_TIME
		clock_t begin, end;
		double time_spent;
		begin = clock();
#endif
		po::store(po::parse_command_line(ac, av, desc), vm);
		vector<string> vBugFilesPaths = vm["bug_file(s)"].as<vector<string> >();

		std::cout << "Inputed arguments are:" << endl;
		std::cout << "bug_file(s):" << endl;
		for (auto elBugFilesPath : vBugFilesPaths) {
			std::cout << elBugFilesPath << endl;
		}
		std::cout << " landscape_file:" << endl << vm["landscape_file"].as<std::string>() << std::endl;

		po::notify(vm);							// for reporting exception
		retVal = processData(vm, vBugFilesPaths);			// loads (and process) data inside of program structures. 
		if (-1 == retVal) {
			return retVal;
		}
#ifdef CHECK_TIME
		end = clock();
		time_spent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);
		cout << "Time of program execution is:" << time_spent << "ms\n";
#endif

		retVal = 0;
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		std::cout << desc << std::endl;
		retVal = 1;
	}
	return retVal;
}
#endif
