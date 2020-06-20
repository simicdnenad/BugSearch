#include "Bug.h"

#include <algorithm>
#include <time.h>

int main(int ac, char** av)
{
#ifdef CHECK_TIME
	clock_t begin, end;
	double time_spent;
	begin = clock();
#endif
	CBug bubica;
	unsigned int NumOfLines=0;
	thread** apThreads=NULL;
	if(bubica.OnInit(ac,av))
	{
		cout << "Input files are correctly opened and loaded in memory." << '\n';
	}
	else
		cout << "Input files are not correctly opened!!!" << '\n';

	cout << "Number of lines:" << (NumOfLines = bubica.GetNumOfLines()) << '\n';
	apThreads= new thread* [NumOfLines/LINES_PER_THREAD+1];
	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
		apThreads[i]=new thread((CBug()));
	}

	for (unsigned int i=0;i<NumOfLines/LINES_PER_THREAD+1;i++)
	{
		apThreads[i]->join();								// how to get calculate value from thread (bubica.NumOfBugs()?!)
		//cout << "Number of Bugs:" << bubica.NumOfBugs() << '\n';
	}

#ifdef CHECK_TIME
    end = clock();
    time_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000);
    cout << "Time of program execution is:" << time_spent <<"ms\n";
#endif

    return 0;
}
