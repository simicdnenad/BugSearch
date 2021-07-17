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

		CLoggerThread(string sBugName);
		void init();
		bool AddBufferFile(unsigned uThreadId=0);
	private:
		map<unsigned,std::tuple<string, ofstream>> m_mapThreadsLogFiles;
		map<unsigned,CONTAINER<string>> m_mapThreadsBuffers;
		string m_sBugName;
//		ofstream m_fWriteFound;

		virtual ~CLoggerThread();

	};
}



#endif /* LOGGER_THREAD_H_ */
