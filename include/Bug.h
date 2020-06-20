#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <thread>
using namespace std;

#define LINES_PER_THREAD		50000

class CBug
{
	unsigned m_uNumOfBugs, m_uNumOfLines;
	static unsigned s_uBugMaxDim, s_uBugDimNum;
	static list<string> s_lFileLand, s_lFileBug;
	list<string>::iterator *m_aiSearchForBug;
	static list<string>::iterator *s_aiBugItself;
	bool SearchBugPart(/**unsigned*/ int found_at, /**unsigned*/ int &start_from, unsigned int currbugdim);
	unsigned MaxBugPart (unsigned dim);
#ifdef SIMPLE_LOG
	unsigned m_uCurrLine;
	ofstream m_fWriteFound;
#endif
public:
	CBug();
	virtual ~CBug();
	bool OnInit(int ac, char** av);
	unsigned int NumOfBugs(unsigned int start_line=0);
	unsigned int GetNumOfLines() const;
	unsigned int GetNumOfBugs() const;
	void operator ()();								// should be thread's function
};
