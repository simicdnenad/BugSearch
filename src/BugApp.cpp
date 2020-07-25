#include "Bug.h"

#include <algorithm>
#include <time.h>

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
	CBug** apBubice=NULL;
	thread** apThreads=NULL;
	apThreads= new thread* [NumOfLines/LINES_PER_THREAD+1];
	apBubice= new CBug* [NumOfLines/LINES_PER_THREAD+1];
	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
		apBubice[i]=new CBug();
#ifdef NOTDEF
		apThreads[i]=new thread(&CBug::NumOfBugs,apBubice[i],i*LINES_PER_THREAD);
#endif
	}

	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
#ifdef NOTDEF
		apThreads[i]->join();								// how to get calculate value from thread (bubica.NumOfBugs()?!)
		//cout << "Number of Bugs:" << bubica.NumOfBugs() << '\n';
		delete apThreads[i];
#endif
		delete apBubice[i];
	}
	delete[] apThreads;
	delete[] apBubice;
#endif
#ifdef CHECK_TIME
    end = clock();
    time_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000);
    cout << "Time of program execution is:" << time_spent <<"ms\n";
#endif

    return 0;
}
