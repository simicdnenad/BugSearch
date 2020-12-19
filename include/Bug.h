#pragma once

#ifdef _WIN32
#define NOT_FOUND	string::npos
#define _CRTDBG_MAP_ALLOC		// for detection of memory leaks
#include <stdlib.h>
#include <crtdbg.h>				//for malloc and free
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
#elif linux
#define NOT_FOUND	-1
#endif

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>								// added for smart pointers usage
////////////////////////////
#include <algorithm>
#include <time.h>
#include <boost/program_options.hpp>			// Boost Program Options
//////////////////////////////
using namespace std;
namespace po = boost::program_options;
#define CONTAINER vector

#ifdef MULTI_THREAD
#define LINES_PER_THREAD		50000
#else
#define LINES_PER_THREAD		1000000			// MAX_NUMOF_LINES
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
	static bool OnInit(po::variables_map& mapInputArgs);
	static unsigned GetTotNumOfBugs(){ return s_uTotalNOB;}
	unsigned GetThreadId() const { return m_uThreadId;}
	void IncTotNumOfBugs(unsigned int incr)
	{
		std::lock_guard<std::mutex> guard(s_mTotalNOB);
		s_uTotalNOB+=incr;
	}
#else
	bool OnInit(po::variables_map& mapInputArgs);
#endif
	void NumOfBugs(unsigned int start_line=0);
	static unsigned int GetNumOfLines(){return s_uNumOfLines;}
	unsigned int GetNumOfBugs() const {  return m_uNumOfBugs;}
	void operator()(unsigned int start_line)			// implement as functor
	{
		NumOfBugs(start_line);
	}
////////////////////////////////////////////////// Iterator design pattern
	class Iterator{
		CBug* p_Collection;
		iData i_CurrEl, i_LastEl;

	public:
		Iterator(CBug& rBug,unsigned start_line, unsigned last_line);
	    Iterator& operator++();							// pre-increment
	    Iterator operator++(int);						// post-increment
	    bool operator==(const Iterator& ciBug) const;
	    bool operator!=(const Iterator& ciBug) const;
	    Data& operator*();
		virtual ~Iterator();
	};
private:
	vector<Iterator> m_viSearchForBug;
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
bool CBug<Data,iData,Container>::OnInit(po::variables_map& mapInputArgs)
{
	ifstream infilebug(mapInputArgs["bug_file"].as<std::string>()),infilelanscape(mapInputArgs["landscape_file"].as<std::string>());
	Data oneline;

	if (infilebug.fail() || infilelanscape.fail())
		return false;

	while (getline(infilebug,oneline))
	{
		/**if(oneline.empty())
			continue;						Not skipping empty lines to avoid "merging" of Bug parts */
		s_lFileBug.emplace_back(oneline);
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
		s_lFileLand.emplace_back(oneline);
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
	for (unsigned int i = 0; i < s_uBugDimNum && i+processed_line < GetNumOfLines(); i++)
		m_viSearchForBug.push_back(Iterator(*this,start_line+i,start_line + LINES_PER_THREAD+i));
	if(processed_line + s_uBugDimNum > GetNumOfLines())
		return; 																// landscape.txt file is too small.

	while(processed_line < (start_line + LINES_PER_THREAD-s_uBugDimNum+1) && processed_line < (GetNumOfLines()-s_uBugDimNum+1))
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
#ifdef MULTI_THREAD
	IncTotNumOfBugs(m_uNumOfBugs);
#endif
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

////////////////////////////////////////////////// Iterator design pattern
template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
CBug<Data,iData,Container>::Iterator::Iterator(CBug& rBug,unsigned start_line, unsigned last_line)
{
	p_Collection=&rBug;
	i_CurrEl=i_LastEl=p_Collection->s_lFileLand.begin();
	advance(i_CurrEl, start_line);

	if(s_lFileLand.end()-i_CurrEl > LINES_PER_THREAD)
		advance(i_LastEl, start_line + LINES_PER_THREAD);
	else
		i_LastEl= s_lFileLand.end();
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
typename CBug<Data,iData,Container>::Iterator&  CBug<Data,iData,Container>::Iterator::operator++()
{
	i_CurrEl++;

	return *this;
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
typename CBug<Data,iData,Container>::Iterator  CBug<Data,iData,Container>::Iterator::operator++(int)
{
	Iterator bugTemp=*this;

	i_CurrEl++;
	return bugTemp;
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
CBug<Data,iData,Container>::Iterator::~Iterator()
{
	p_Collection=NULL;
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
bool CBug<Data,iData,Container>::Iterator::operator==(const Iterator& ciBug) const
{
	return i_CurrEl==ciBug.i_CurrEl;
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
bool CBug<Data,iData,Container>::Iterator::operator!=(const Iterator& ciBug) const
{
	return !(i_CurrEl==ciBug.i_CurrEl);
}

template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
Data&  CBug<Data,iData,Container>::Iterator::operator*()
{
	return *i_CurrEl;
}
