#include "Bug.h"

#include <algorithm>
#include <time.h>
#include <vector>

int main(int ac, char** av)
{
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
#ifndef MULTI_THREAD
		CBug bubica;

		if (bubica.OnInit(ac, av))
		{
			cout << "Input files are correctly opened and loaded in memory." << '\n';
		}
		else
			cout << "Input files are not correctly opened!!!" << '\n';

		cout << "Number of lines:" << (NumOfLines = CBug::GetNumOfLines()) << '\n';
		cout << "Number of found bugs:" << (NumOfLines = bubica.NumOfBugs()) << '\n';
#else
		CBug<string, CONTAINER<string>::iterator, CONTAINER>::OnInit(ac, av);
		cout << "Number of lines:" << (NumOfLines = CBug<string, CONTAINER<string>::iterator, CONTAINER>::GetNumOfLines()) << '\n';
		vector<std::thread> vThreads;
		vector<unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>> vupBugs;
		for (unsigned int i = 0;i < NumOfLines / LINES_PER_THREAD + 1;i++)
		{
			vupBugs.push_back((unique_ptr<CBug<string, CONTAINER<string>::iterator, CONTAINER>>)(new CBug<string, CONTAINER<string>::iterator, CONTAINER>()));
			vThreads.push_back(std::thread(&CBug<string, CONTAINER<string>::iterator, CONTAINER>::NumOfBugs, vupBugs.back().get(), i*LINES_PER_THREAD));				// std::unique_ptr::get -- return stored pointer
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
}
