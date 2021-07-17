/*
 * Logger_thread.cpp
 *
 *  Created on: Jul 17, 2021
 *      Author: nenad
 */

#include "Logger_thread.h"

namespace landscape
{

	CLoggerThread::CLoggerThread(string sBugName)
	{
		m_mapThreadsLogFiles.clear();
		m_mapThreadsBuffers.clear();
		m_sBugName.assign(sBugName);
	}

	CLoggerThread::~CLoggerThread(){}

	bool CLoggerThread::AddBufferFile(unsigned uThreadId)
	{
#ifdef MULTI_THREAD
		string sPath = "WriteFound" + m_sBugName + std::to_string(uThreadId);
#else
		string sPath = "WriteFound" + m_sBugName;
#endif
#ifdef linux
		sPath += ".nfo";
#elif _WIN32
		sPath += ".txt";
#endif

		m_mapThreadsLogFiles.insert(pair<unsigned,std::tuple<string, ofstream>>(uThreadId,make_tuple(sPath,ofstream())));
#ifdef NOTDEF
		#ifdef _DEBUG
				if (s_fDebugTrace.is_open())
				{
					s_fDebugTrace << "(" << __FILE__ << " : " << __LINE__ <<
						")  CBug<Data, iData, Container>::CBug(string sBugName), m_fWriteFound's sPath=" << sPath.c_str() << endl;
				}
		#endif
#endif
		auto it = m_mapThreadsLogFiles.find(uThreadId);
		get<1>(it->second).open(get<0>(it->second));
		if (get<1>(it->second).is_open())
		{
			cout << (get<0>(it->second)).c_str() << " opened successfully for writing.\n";
			get<1>(it->second) << "This is the file for logging found Bug patterns (ThreadID="
#ifdef MULTI_THREAD
				<< uThreadId
#endif
				<< ").\n";
			return true;
		}
		else
			return false;
#ifdef NOTDEF
			get<1>(it->second)<< "Bug pattern:\n";
			for (unsigned int i = 0; i < s_uBugDimNum; i++)
				get<1>(it->second) << (*s_viBugItself[i]) << '\n';
		}
		else
			cout << "Unable to open" << sPath.c_str() << "file! \n";
#endif

	}


	void CLoggerThread::init()
	{
#ifdef NOTDEF
		m_fWriteFound.open(m_sPath);
		if (m_fWriteFound.is_open())
		{
			cout << sPath.c_str() << " opened successfully for writing.\n";
			m_fWriteFound << "This is the file for logging found Bug patterns (ThreadID="
#ifdef MULTI_THREAD
				<< m_uThreadId
#endif
				<< ").\n";

			m_fWriteFound << "Bug pattern:\n";
			for (unsigned int i = 0; i < s_uBugDimNum; i++)
				m_fWriteFound << (*s_viBugItself[i]) << '\n';
		}
		else
			cout << "Unable to open" << sPath.c_str() << "file! \n";
#endif
	}
}


