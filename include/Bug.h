#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>				// added for smart pointers usage
////////////////////////////
#include <algorithm>
#include <time.h>
////////////////////////////
using namespace std;

#define CONTAINER vector

#define LINES_PER_THREAD		50000
#ifdef _WIN32
#define NOT_FOUND	string::npos
#elif linux
#define NOT_FOUND	-1
#endif

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
class CBug
{
	unsigned m_uNumOfBugs;
	static unsigned s_uBugMaxDim, s_uBugDimNum, s_uNumOfLines;
#ifdef MULTI_THREAD
	static unsigned s_uNumOfThreads, s_uTotalNOB;
	unsigned m_uThreadId;
	static mutex s_mTotalNOB;
#endif
	static Container<Data> s_lFileLand, s_lFileBug;
	vector<iData> m_viSearchForBug;
	static vector<iData> s_viBugItself;
	bool SearchBugPart(/**unsigned*/ int found_at, /**unsigned*/ int &start_from, unsigned int currbugdim);
	unsigned MaxBugPart (unsigned dim);
#ifdef SIMPLE_LOG
	unsigned m_uCurrLine;
	ofstream m_fWriteFound;
#endif
public:
	CBug();
	virtual ~CBug();
#ifdef MULTI_THREAD
	static bool OnInit(int ac, char** av);
	static unsigned GetTotNumOfBugs(){ return s_uTotalNOB;}
	unsigned GetThreadId() const { return m_uThreadId;}
	void IncTotNumOfBugs(unsigned int incr)
	{
		std::lock_guard<std::mutex> guard(s_mTotalNOB);
		s_uTotalNOB+=incr;
	}
#else
	bool OnInit(int ac, char** av);
#endif
	void NumOfBugs(unsigned int start_line=0);
	static unsigned int GetNumOfLines(){return s_uNumOfLines;}
	unsigned int GetNumOfBugs() const {  return m_uNumOfBugs;}
#ifdef NOTDEF 					// not working!!!
	void operator()()			// should be thread's function
	{
		NumOfBugs(0);
	}
#endif
};
////////////////////// static fields
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
vector<iData> CBug<Data,iData,Container>::s_viBugItself;

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
unsigned CBug<Data,iData,Container>::s_uBugMaxDim;
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
unsigned CBug<Data,iData,Container>::s_uBugDimNum;
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
unsigned CBug<Data,iData,Container>::s_uNumOfLines;

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
Container<Data> CBug<Data,iData,Container>::s_lFileLand;
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
Container<Data>  CBug<Data,iData,Container>::s_lFileBug;

#ifdef MULTI_THREAD
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data,iData,Container>::s_uNumOfThreads=0;
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data,iData,Container>::s_uTotalNOB;
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	mutex CBug<Data,iData,Container>::s_mTotalNOB;
#endif
////////////////////////// methods
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
CBug<Data,iData,Container>::CBug()
{
#ifdef MULTI_THREAD
	m_uThreadId=s_uNumOfThreads++;
#endif
	m_uNumOfBugs = 0;
#ifdef SIMPLE_LOG
#ifdef MULTI_THREAD
	m_uCurrLine = m_uThreadId*LINES_PER_THREAD+1;
	string sPath = "WriteFoundBugs" + std::to_string(m_uThreadId);
#else
	m_uCurrLine = 1;
	string sPath = "WriteFoundBugs";
#endif
#ifdef linux
	sPath += ".nfo";
#elif _WIN32
	sPath += ".txt";
#endif
	m_fWriteFound.open(sPath.c_str());
	if (m_fWriteFound.is_open())
	{
		cout << sPath.c_str() << " opened successfully for writing.\n";
		m_fWriteFound << "This is the file for logging founded Bug patterns ("
#ifdef MULTI_THREAD
				<< m_uThreadId
#endif
				<< ").\n";

		m_fWriteFound<<"Bug pattern:\n";
		for (unsigned int i = 0; i < s_uBugDimNum; i++)
			m_fWriteFound<<(*s_viBugItself[i])<<'\n';
	}
	else
		cout << "Unable to open" <<   sPath.c_str() << "file! \n";
#endif

}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
CBug<Data,iData,Container>::~CBug()
{
#ifdef MULTI_THREAD
	s_uNumOfThreads--;
	if(s_uNumOfThreads==0)
#endif
#ifdef SIMPLE_LOG
	if (m_fWriteFound.is_open())
		m_fWriteFound.close();
#endif
}

/**
 * Initialize instance of CBug class.
 *
 * loads landscape and bug text files in list<string> data structures
 *
 * @param int ac		number of arguments in application call
 * @param char** av		passed arguments
 * @return bool status (success or false)
 */
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
bool CBug<Data,iData,Container>::OnInit(int ac, char** av)
{
	ifstream infilebug(av[1]),infilelanscape(av[2]);
	Data oneline;

	if (infilebug.fail() || infilelanscape.fail())
		return false;

	while (getline(infilebug,oneline))
	{
		/**if(oneline.empty())
			continue;						Not skipping empty lines to avoid "merging" of Bug parts */
		s_lFileBug.push_back(oneline);
		if (oneline.size() > s_uBugMaxDim)
			s_uBugMaxDim = oneline.size();
		s_uBugDimNum++;
	}
	iData i_BugItself = s_lFileBug.begin();
	for (unsigned int i = 0; i < s_uBugDimNum && i_BugItself != s_lFileBug.end(); i++)
		s_viBugItself.push_back(i_BugItself++);


//----------------------LAND------------------------------
	while (getline(infilelanscape,oneline))		// Add also num of lines count, to determine the optimal number of created threads
	{
		/**if(oneline.empty())
			continue;							Not skipping empty lines to avoid "merging" of Bug parts */
		s_lFileLand.push_back(oneline);
		s_uNumOfLines++;
	}

	infilebug.close();
	infilelanscape.close();

	return true;
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
void CBug<Data,iData,Container>::NumOfBugs(unsigned int start_line)
{
	/**unsigned*/ int start_from=0,found_at = 0, processed_line=start_line;
	iData i_SearchBug = s_lFileLand.begin();
	advance(i_SearchBug, start_line);
	for (unsigned int i = 0; i < s_uBugDimNum && i_SearchBug != s_lFileLand.end(); i++)
		m_viSearchForBug.push_back(i_SearchBug++);

	if (i_SearchBug == s_lFileLand.end())										// landscape.txt file is too small.
		return;

	while(m_viSearchForBug[s_uBugDimNum-1] != s_lFileLand.end() &&  processed_line < (start_line + LINES_PER_THREAD))
	{
		while((found_at = (*m_viSearchForBug[0]).find(*s_viBugItself[0],start_from)) != NOT_FOUND)
		{
			unsigned int bugdim = 1;									// Compare every Bug's dimension
			bool founded = false;
			do
			{
				founded = SearchBugPart(found_at, start_from, bugdim++);
			}while(founded && bugdim < s_uBugDimNum);

			if(!founded)
				start_from = found_at + MaxBugPart(bugdim-2);	// Bug not founded, skip its longest founded part
		}
		processed_line++;
#ifdef SIMPLE_LOG
		m_uCurrLine++;
#endif

		for (unsigned int i = 0; i < s_uBugDimNum ; i++)		// shift for one line below
			m_viSearchForBug[i]++;
		start_from=0;
	}

	IncTotNumOfBugs(m_uNumOfBugs);
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
bool CBug<Data,iData,Container>::SearchBugPart(/**unsigned*/ int found_at, /**unsigned*/ int &start_from, unsigned int currbugdim)
{
	unsigned int pos = 0;

	pos = (*m_viSearchForBug[currbugdim]).find(*s_viBugItself[currbugdim],found_at);

	if (pos == found_at && currbugdim == (s_uBugDimNum-1))
	{
		m_uNumOfBugs++;
		start_from = found_at + s_uBugMaxDim;		// Skip founded Bug in next search
#ifdef SIMPLE_LOG
		if (m_fWriteFound.is_open())
			m_fWriteFound<<"Bug number:" << m_uNumOfBugs << " ,found at line:"<< m_uCurrLine << " ,at position:"<<  found_at+1<< '\n';
#endif
		return true;
	}
	else if(pos == found_at && currbugdim < (s_uBugDimNum-1))
		return true;
	else
		return false;
}


template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
unsigned CBug<Data,iData,Container>::MaxBugPart (unsigned dim)
{
	iData i_SearchBug = s_lFileBug.begin();
	unsigned max_dim = 0;
	for (unsigned i=0;i<=dim;i++)
		if((*i_SearchBug).length() > max_dim)
		{
			max_dim = (*i_SearchBug).length();
			i_SearchBug++;
		}
	return max_dim;
}


