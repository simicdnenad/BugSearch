#include "Bug.h"

#include <algorithm>
#include <time.h>
#include <vector>

int main(int ac, char** av)
{
	po::variables_map vm;
	po::options_description desc("Allowed Options");

	// declare arguments (boost)
	desc.add_options()
			("bug_file", po::value<std::string>()->required(), "Provide relative path to \"bug.txt\" file")
			("landscape_file", po::value<std::string>()->required(), "Provide relative path to \"landscape.txt\" file");

	 try {
#ifdef WIN32
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld);				//take a snapshot at app start
#endif
	{
		unsigned int NumOfLines = 0;
#ifdef CHECK_TIME
		clock_t begin, end;
		double time_spent;
		begin = clock();
#endif
		po::store(po::parse_command_line(ac, av, desc), vm);
		std::cout << "Inputed arguments are: bug_file:" << vm["bug_file"].as<std::string>()
				<< "  landscape_file:" << vm["landscape_file"].as<std::string>() << std::endl;

		po::notify(vm);						// for reporting exception

#ifndef MULTI_THREAD
		CBug<string, CONTAINER<string>::iterator, CONTAINER> bubica;

		if (bubica.OnInit(vm))
		{
			cout << "Input files are correctly opened and loaded in memory." << '\n';
		}
		else
			cout << "Input files are not correctly opened!!!" << '\n';

		cout << "Number of lines:" << (NumOfLines = CBug<string, CONTAINER<string>::iterator, CONTAINER>::GetNumOfLines()) << '\n';
		bubica.NumOfBugs();
		cout << "Number of found bugs:" << (NumOfLines = bubica.GetNumOfBugs()) << '\n';
#else
		CBug<string, CONTAINER<string>::iterator, CONTAINER>::OnInit(vm);
		cout << "Number of lines:" << (NumOfLines = CBug<string, CONTAINER<string>::iterator, CONTAINER>::GetNumOfLines()) << '\n';
		vector<std::thread> vThreads;
		vector<unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>> vupBugs;
		for (unsigned int i = 0;i < NumOfLines / LINES_PER_THREAD + 1;i++)
		{
			vupBugs.push_back((unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>)(new CBug<string, CONTAINER<string>::iterator, CONTAINER>()));
			vThreads.push_back(std::thread(std::ref(*(vupBugs.back().get())),i*LINES_PER_THREAD));				// must use std::ref() to avoid object copying to created thread
		}

		auto iupBugs = vupBugs.begin();
		for(std::thread& rThread :vThreads)
		{
			if (rThread.joinable())
				rThread.join();
			cout << "Number of Bugs (tid=" << ((*iupBugs).get())->GetThreadId() << ")=" << ((*iupBugs).get())->GetNumOfBugs() << "\n";
			iupBugs++;
		}
		vThreads.erase(vThreads.begin(), vThreads.end());
		vupBugs.erase(vupBugs.begin(), iupBugs);
		cout << "Total number of Bugs: " << CBug<string, CONTAINER<string>::iterator, CONTAINER>::GetTotNumOfBugs() << "\n";
#endif
#ifdef CHECK_TIME
		end = clock();
		time_spent = (double)(end - begin) / (CLOCKS_PER_SEC / 1000);
		cout << "Time of program execution is:" << time_spent << "ms\n";
#endif
	}
#ifdef WIN32
	_CrtMemCheckpoint(&sNew);				//take a snapshot at app end
	if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
	{
		//OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		//_CrtMemDumpStatistics(&sDiff);
		//OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		//CrtMemDumpAllObjectsSince(&sOld);
		//OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		_CrtDumpMemoryLeaks();
	}
#endif
	return 0;

	} catch (std::exception& e) {
	  std::cout << "Error: " << e.what() << std::endl;
	  std::cout << desc << std::endl;
	  return 1;
	}
}
