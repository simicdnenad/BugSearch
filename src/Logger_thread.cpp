/*
 * Logger_thread.cpp
 *
 *  Created on: Jul 17, 2021
 *      Author: nenad
 */
#include <cstdarg>
#include "Logger_thread.h"

namespace landscape
{

	CLoggerThread::CLoggerThread(string sBugName)
	{
		m_mapThreadsLogFiles.clear();
		m_mapThreadsBuffers.clear();
		m_sBugName.assign(sBugName);
#ifdef _DEBUG
		m_sDebugFileName.assign("DebugTraceEmbedded.txt");
#endif
	}

	CLoggerThread::~CLoggerThread(){}

	CLoggerThread::EFileOpenErrors CLoggerThread::AddBufferAndFile(unsigned uThreadId)
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

		auto it = m_mapThreadsLogFiles.find(uThreadId);
#ifdef _DEBUG
		it = m_mapThreadsLogFiles.find(69);
		if(it==m_mapThreadsLogFiles.end())
		{
			m_mapThreadsLogFiles.insert(pair<unsigned,std::tuple<string, ofstream>>(69,make_tuple("DebugTraceEmbedded.txt",ofstream())));
			it = m_mapThreadsLogFiles.find(69);
			get<1>(it->second).open(get<0>(it->second));

			if (get<1>(it->second).is_open())
			{
				cout << get<0>(it->second).c_str() << " opened successfully for writing.\n";
				get<1>(it->second) << "DebugTrace:\n" << "(" << __FILE__ << " : " << __LINE__ << ")" << endl;
			}
			else
				return EFileOpenErrors::DEBUG_FAIL;
		}
#endif
		m_mapThreadsLogFiles.insert(pair<unsigned,std::tuple<string, ofstream>>(uThreadId,make_tuple(sPath,ofstream())));
		it = m_mapThreadsLogFiles.find(uThreadId);
		get<1>(it->second).open(get<0>(it->second));

		if (get<1>(it->second).is_open())
		{
			cout << (get<0>(it->second)).c_str() << " opened successfully for writing.\n";
			get<1>(it->second) << "This is the file for logging found Bug patterns (ThreadID="
#ifdef MULTI_THREAD
				<< uThreadId
#endif
				<< ").\n";
#ifdef NOTDEF
			get<1>(it->second)<< "Bug pattern:\n";
			for (unsigned int i = 0; i < s_uBugDimNum; i++)
				get<1>(it->second) << (*s_viBugItself[i]) << '\n';
#endif
			return EFileOpenErrors::ALL_SUCCESSFULL;
		}
		else
		{
			cout << "Unable to open" << sPath.c_str() << "file! \n";
			return EFileOpenErrors::LOG_FAIL;
		}
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

	void CLoggerThread::log(const unsigned uLogId, const char *const file, int const line, const char *const fmt, ...) {
	  assert(fmt != nullptr);
	  std::array<char, 512> buffer;

	  const char *file_name = file;
	  if (file != nullptr) {
	    const char *const substr = strrchr(file, '/');
	    if (substr != nullptr) {
	      file_name = substr+1; // Point to first character after slash
	    }
	  }
	  else {
	    file_name = "<unknown>";
	  }
#ifdef NOTDEF
	  // Calculate timestamp of message
	  uint16_t year;
	  uint8_t month;
	  uint8_t day;
	  uint8_t print_h;
	  uint8_t print_m;
	  uint8_t print_s;
	  uint16_t print_ms;
	  time.getTimeParts(year, month, day, print_h, print_m, print_s, print_ms);
#endif

	  int written_size
	    = std::snprintf(buffer.data(), buffer.size(), /**[%02d:%02d:%02d.%03d]*/"[%s:%d]"/** %s: */, /**print_h, print_m, print_s, print_ms,*/ file_name, line/**, level_string*/);
	  if (0 <= written_size) {
	    if (static_cast<std::size_t>(written_size + 1) < buffer.size()) {
	      std::va_list args;
	      va_start(args, fmt);
	      const int add_written_size = std::vsnprintf(&buffer.at(written_size), buffer.size() - written_size, fmt, args);
	      va_end(args);

	      if (0 <= add_written_size) {
	        written_size += add_written_size;
	      }
	    }

	    std::size_t end_index = std::min(static_cast<std::size_t>(written_size), buffer.size() - 3);
	    buffer.at(end_index++) = '\r';
	    buffer.at(end_index++) = '\n';
	    buffer.at(end_index) = '\0';

	    auto iBuff = m_mapThreadsBuffers.find(uLogId);

	    if(iBuff!=m_mapThreadsBuffers.end())
	    {
	    	iBuff->second.push_back(buffer.data());
	    }
	    else
	    	return;

	  }
	}
}


