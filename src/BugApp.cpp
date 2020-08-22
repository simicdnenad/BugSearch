#include "Bug.h"

#include <algorithm>
#include <time.h>
#include <vector>

int main(int ac, char** av)
{
	unsigned int NumOfLines=0;
#ifdef CHECK_TIME
	clock_t begin, end;
	double time_spent;
	begin = clock();
#endif
#ifndef MULTI_THREAD
	CBug bubica;

	if(bubica.OnInit(ac,av))
	{
		cout << "Input files are correctly opened and loaded in memory." << '\n';
	}
	else
		cout << "Input files are not correctly opened!!!" << '\n';

	cout << "Number of lines:" << (NumOfLines = CBug::GetNumOfLines()) << '\n';
	cout << "Number of found bugs:" << (NumOfLines = bubica.NumOfBugs()) << '\n';
#else
	CBug::OnInit(ac,av);
	cout << "Number of lines:" << (NumOfLines = CBug::GetNumOfLines()) << '\n';
	vector<std::thread> vThreads;
	vector<unique_ptr<CBug>> vupBugs;
	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
		vupBugs.push_back((unique_ptr<CBug>)(new CBug()));
		vThreads.push_back(std::thread(&CBug::NumOfBugs,vupBugs.back().get(),i*LINES_PER_THREAD));				// std::unique_ptr::get -- return stored pointer
	}

	vector<std::thread>::iterator iThreads=vThreads.begin();
	vector<unique_ptr<CBug>>::iterator iupBugs=vupBugs.begin();
	while(iThreads!=vThreads.end())
	{
		iThreads->join();
		cout << "Number of Bugs (tid=" << ((*iupBugs).get())->GetThreadId() << ")=" << ((*iupBugs).get())->GetNumOfBugs() << "\n";
		iThreads++;
		iupBugs++;
	}
	vThreads.erase(vThreads.begin(), iThreads);
	vupBugs.erase(vupBugs.begin(), iupBugs);
	cout << "Total number of Bugs: " << CBug::GetTotNumOfBugs() << "\n";
#endif
#ifdef CHECK_TIME
    end = clock();
    time_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000);
    cout << "Time of program execution is:" << time_spent <<"ms\n";
#endif

    return 0;
}
