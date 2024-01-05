#pragma once

#ifdef _WIN32
#define NOT_FOUND	string::npos
#define _CRTDBG_MAP_ALLOC		// for detection of memory leaks
#define DIR_SEPARATOR '\\'		// directory separator on WIN
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
#define DIR_SEPARATOR '/'		// directory separator on WIN
#define NOT_FOUND	-1
#include <stdexcept>
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

#ifdef USE_SOCKETS			// For program start over network
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#endif

namespace landscape {

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
		unsigned MaxBugPart(unsigned dim);
#ifdef SIMPLE_LOG
		unsigned m_uCurrLine;
		ofstream m_fWriteFound;
		string m_sBugName;
#endif
	public:
		CBug(string sBugName);
		virtual ~CBug();
		enum class EFileOpenErrors :uint8_t {
			ALL_SUCCESSFULL = 0x00U,
			DEBUG_FAIL = 0x01U,
			LANDSCAPE_FAIL = 0x02U,
			BUG_FAIL = 0x03U,
		};
		static const map<EFileOpenErrors, string> mapFileErrors;
		const string& GetBugName () const { return m_sBugName; }
#ifdef MULTI_THREAD
		static EFileOpenErrors OnInit(std::vector<std::string>::iterator& iBugFile, const std::string& strLandFile);
		static unsigned GetTotNumOfBugs() { return s_uTotalNOB; }
		unsigned GetThreadId() const { return m_uThreadId; }
		void IncTotNumOfBugs(unsigned int incr)
		{
			std::lock_guard<std::mutex> guard(s_mTotalNOB);
			s_uTotalNOB += incr;
		}
#else
		EFileOpenErrors OnInit(std::vector<std::string>::iterator& iBugFile, const std::string& strLandFile);
#endif
#ifdef _DEBUG
		static ofstream s_fDebugTrace;
		static string s_strDebugFileName;
#endif
		void NumOfBugs(unsigned int start_line = 0);
		static unsigned int GetNumOfLines() { return s_uNumOfLines; }
		unsigned int GetNumOfBugs() const { return m_uNumOfBugs; }
		void operator()(unsigned int start_line)			// implement as functor
		{
			NumOfBugs(start_line);
		}
		////////////////////////////////////////////////// Iterator design pattern
		class Iterator {
			CBug* p_Collection;
			iData i_CurrEl, i_LastEl;

		public:
			Iterator(CBug& rBug, unsigned start_line, unsigned last_line);
			Iterator& operator++();							// pre-increment
			Iterator operator++(int);						// post-increment
			bool operator==(const Iterator& ciBug) const;
			bool operator!=(const Iterator& ciBug) const;
			Data& operator*();
			virtual ~Iterator();
		};
#ifdef USE_SOCKETS
	    int mServerFd, NewSocket;
	    ssize_t sizeValRead;
	    struct sockaddr_in address;
	    int mOpt = 1;
	    socklen_t addrlen = sizeof(address);
	    char buffSocket[1024] = { 0 };
	    //char* strHello = "Hello from server";			// produces: warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]

#endif
	private:
		vector<Iterator> m_viSearchForBug;
		static void clearLastSearch();
	};
}
#include "Bug_impl.hpp"
