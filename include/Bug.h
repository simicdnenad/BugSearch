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

#include "Bug_impl.hpp"
