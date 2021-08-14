/*
 * Logger_thread.h
 *
 *  Created on: Jul 17, 2021
 *      Author: nenad
 */

#ifndef LOGGER_THREAD_H_
#define LOGGER_THREAD_H_

#include <map>

#include "Bug.h"

using namespace std;

namespace landscape{

	class CLoggerThread {
		struct CLoggerData{
			CLoggerData():m_bShouldFlash(false){}
			CLoggerData(const CLoggerData& LoggerData){}							// implemented CopyConstructor due to error when inserting class object into a STL map
			CONTAINER<string> m_LogBuffer;
			mutex m_mutexBuffer;
			bool m_bShouldFlash;
		};

		map<unsigned,std::tuple<string, ofstream>> m_mapThreadsLogFiles;
		map<unsigned,CLoggerData> m_mapThreadsBuffers;
		string m_sBugName;
	public:
		using EFileOpenErrors = landscape::CBugT::EFileOpenErrors;

		CLoggerThread(string sBugName="");
		virtual ~CLoggerThread();
		void init();
		EFileOpenErrors  AddBufferAndFile(unsigned uThreadId=0);
		void init(string sBugName,unsigned uNumOfThreads);
		void log(const unsigned uLogId, const char *const file, int const line, const char *const fmt, ...);
		void assignName(const string& bugName){m_sBugName.assign(bugName);}
		const string& getBugName()const{ return m_sBugName;}
	};

#define LOG(ThreadID, ...) CLoggerThread::log(ThreadID, __FILE__, __LINE__, __VA_ARGS__)

}



#endif /* LOGGER_THREAD_H_ */
