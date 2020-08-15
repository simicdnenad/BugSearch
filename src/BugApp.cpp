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
	vector<CBug*> vpBugs;
	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
		vpBugs.push_back(new CBug());					// TODO: reporting error when i want to forward CBug() as a argument!!!
		vThreads.push_back(std::thread(&CBug::NumOfBugs,vpBugs.back(),i*LINES_PER_THREAD));
	}

	std::vector<std::thread>::iterator iThreads=vThreads.begin();
	std::vector<CBug*>::iterator ipBugs=vpBugs.begin();
	while(iThreads!=vThreads.end())
	{
		iThreads->join();
		cout << "Number of Bugs (tid=" << (*ipBugs)->GetThreadId() << ")=" << (*ipBugs)->GetNumOfBugs() << "\n";
		iThreads++;
		delete (*ipBugs);
		ipBugs++;
	}
	vThreads.erase(vThreads.begin(), iThreads);
	vpBugs.erase(vpBugs.begin(), ipBugs);
	cout << "Total number of Bugs: " << CBug::GetTotNumOfBugs() << "\n";
#endif
#ifdef CHECK_TIME
    end = clock();
    time_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000);
    cout << "Time of program execution is:" << time_spent <<"ms\n";
#endif

    return 0;
}
