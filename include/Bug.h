#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
using namespace std;

class CBug
{
	unsigned m_uNumOfBugs, m_uNumOfLines, m_uBugMaxDim, m_uBugDimNum;
	list<string> m_lFileLand, m_lFileBug;
	list<string>::iterator *m_aiSearchForBug, *m_aiBugItself;
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
};
