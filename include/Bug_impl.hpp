#pragma once
namespace landscape
{
	/**
	 * \brief Vector of pointers to loaded Bug strings.
	 *
	 * Represents the pointers to loaded rows, which are basically
	 * the parts of loaded Bug pattern (which is read from file)
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	vector<iData> CBug<Data, iData, Container>::s_viBugItself;
	/**
	 * \brief Size of Bug row which has the most characters. Used to skip the found one and proceed to next one.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::s_uBugMaxDim;
	/**
	 * \brief Number or rows which Bug pattern contains.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::s_uBugDimNum;
	/**
	 * \brief Number of rows of file through which is searched for Bug pattern.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::s_uNumOfLines;
	/**
	 * \brief Container (vector or list) of strings which basically contains rows of file through which is searched for Bug pattern.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	Container<Data> CBug<Data, iData, Container>::s_lFileLand;
	/**
	 * \brief Container (vector or list) of strings which basically contains rows of file which represents Bug pattern.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	Container<Data>  CBug<Data, iData, Container>::s_lFileBug;

#ifdef MULTI_THREAD
	/**
	 * \brief Number of threads used for searching.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::s_uNumOfThreads = 0;
	/**
	 * \brief Total number of found Bug patterns inside of Landscape file.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::s_uTotalNOB;
	/**
	 * \brief Mutex for updating global variable.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	mutex CBug<Data, iData, Container>::s_mTotalNOB;
#endif
#ifdef _DEBUG
	/**
	 * \brief Debug file handler.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	ofstream CBug<Data, iData, Container>::s_fDebugTrace;
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	string CBug<Data, iData, Container>::s_strDebugFileName("DebugTraceEmbedded.txt");
#endif
	/**
	 * \brief Hash map of file handling errors and their explanations.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	const map<typename CBug<Data, iData, Container>::EFileOpenErrors, string> CBug<Data, iData, Container>::mapFileErrors = {
		{EFileOpenErrors::ALL_SUCCESSFULL, "Every file opened successfully."},
		{EFileOpenErrors::DEBUG_FAIL, "Failed to open Debug file:"},
		{EFileOpenErrors::LANDSCAPE_FAIL, "Failed to open landscape file:"},
		{EFileOpenErrors::BUG_FAIL, "Failed to open bug file:"}
	};
	/**
	 * \brief A Constructor of an object of CBug class type.
	 *
	 * Every object should create separate thread, which should load sequential parts of landscape file,
	 * process them in parallel and update total number of found bug patterns.
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	CBug<Data, iData, Container>::CBug(string sBugName) :m_sBugName(sBugName)
	{
#ifdef MULTI_THREAD
		m_uThreadId = s_uNumOfThreads++;
#endif
		m_uNumOfBugs = 0;
#ifdef SIMPLE_LOG
#ifdef MULTI_THREAD
		m_uCurrLine = m_uThreadId * LINES_PER_THREAD + 1;
		string sPath = "WriteFound" + m_sBugName + std::to_string(m_uThreadId);
#else
		m_uCurrLine = 1;
		string sPath = "WriteFound" + m_sBugName;
#endif
		sPath += ".nfo";
#ifdef _DEBUG
		if (s_fDebugTrace.is_open())
		{
			s_fDebugTrace << "(" << __FILE__ << " : " << __LINE__ <<
				")  CBug<Data, iData, Container>::CBug(string sBugName), m_fWriteFound's sPath=" << sPath.c_str() << endl;
		}
#endif
		m_fWriteFound.open(sPath.c_str());
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

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	CBug<Data, iData, Container>::~CBug()
	{
#ifdef MULTI_THREAD
		s_uNumOfThreads--;
		if (s_uNumOfThreads == 0)
#endif
#ifdef SIMPLE_LOG
			if (m_fWriteFound.is_open())
				m_fWriteFound.close();
#endif
	}

	/**
	 * \brief Emptys global variables if search on new data should be performed
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	void CBug<Data, iData, Container>::clearLastSearch()
	{
		// Bug related data
		s_uBugDimNum = 0;
		s_uBugMaxDim = 0;
		s_lFileBug.clear();
		s_viBugItself.clear();
#ifdef MULTI_THREAD
		s_uTotalNOB = 0;
#endif
		// Landscape related data
		s_uNumOfLines = 0;
		s_lFileLand.clear();
	}

	/**
	 * @brief Initialize instance of CBug class.
	 *
	 * loads landscape and bug text files in list<string> data structures to provide searching.
	 *
	 * @param iBugFile				pointer to current bug text file name, which contains pattern to be searched for
	 * @param const std::string& strLandFile	name of file through which should be searched for
	 * @return bool status (success or false)
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	typename CBug<Data, iData, Container>::EFileOpenErrors CBug<Data, iData, Container>::OnInit(std::vector<std::string>::iterator& iBugFile, const std::string& strLandFile)
	{
		ifstream infilebug(*iBugFile), infilelanscape(strLandFile);
		Data oneline;

		if (infilebug.fail())
			return EFileOpenErrors::BUG_FAIL;
		else if (infilelanscape.fail())
			return EFileOpenErrors::LANDSCAPE_FAIL;


#ifdef _DEBUG
		if (!s_fDebugTrace.is_open())
		{
			s_fDebugTrace.open(s_strDebugFileName.c_str());
			if (s_fDebugTrace.is_open())
			{
				cout << s_strDebugFileName.c_str() << " opened successfully for writing.\n";
				s_fDebugTrace << "DebugTrace:\n" << "(" << __FILE__ << " : " << __LINE__ << ")  CBug<Data, iData, Container>::OnInit()" << endl;
			}
			else
				return EFileOpenErrors::DEBUG_FAIL;
		}

#endif

		clearLastSearch();
		//----------------------BUG------------------------------
		while (getline(infilebug, oneline))
		{
			// if(oneline.empty())
			//	continue;						Not skipping empty lines to avoid "merging" of Bug parts
			s_lFileBug.emplace_back(oneline);
			if (oneline.size() > s_uBugMaxDim)
				s_uBugMaxDim = oneline.size();
			s_uBugDimNum++;
		}
		iData i_BugItself = s_lFileBug.begin();
		for (unsigned int i = 0; i < s_uBugDimNum && i_BugItself != s_lFileBug.end(); i++)
			s_viBugItself.push_back(i_BugItself++);


		//----------------------LAND------------------------------
		while (getline(infilelanscape, oneline))		// Add also num of lines count, to determine the optimal number of created threads
		{
			// if(oneline.empty())
			//	continue;							Not skipping empty lines to avoid "merging" of Bug parts
			s_lFileLand.emplace_back(oneline);
			s_uNumOfLines++;
		}

		infilebug.close();
		infilelanscape.close();

		return EFileOpenErrors::ALL_SUCCESSFULL;
	}

	/**
	 * @brief Searches for bug pattern through loaded landscape file.
	 *
	 * Iterates through loaded landscape file and search for multi-line bug pattern. As result it updates m_uNumOfBugs counter.
	 *
	 * @param start_line starting line in loaded landscape file
	 * @return void
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	void CBug<Data, iData, Container>::NumOfBugs(unsigned int start_line)
	{
		int start_from = 0, found_at = 0, processed_line = start_line;
		for (unsigned int i = 0; i < s_uBugDimNum && i + processed_line < GetNumOfLines(); i++)
			m_viSearchForBug.push_back(Iterator(*this, start_line + i, start_line + LINES_PER_THREAD + i));
		if (processed_line + s_uBugDimNum > GetNumOfLines())
			return; 																// landscape.nfo file is too small.

		while (processed_line < (start_line + LINES_PER_THREAD - s_uBugDimNum + 1) && processed_line < (GetNumOfLines() - s_uBugDimNum + 1))
		{
			while ((found_at = (*m_viSearchForBug[0]).find(*s_viBugItself[0], start_from)) != NOT_FOUND)
			{
				unsigned int bugdim = 1;									// Compare every Bug's dimension
				bool founded = false;
				do
				{
					founded = SearchBugPart(found_at, start_from, bugdim++);
				} while (founded && bugdim < s_uBugDimNum);

				if (!founded)
					start_from = found_at + MaxBugPart(bugdim - 2);	// Bug not founded, skip its longest founded part
			}
			processed_line++;
#ifdef SIMPLE_LOG
			m_uCurrLine++;
#endif

			for (unsigned int i = 0; i < s_uBugDimNum; i++)		// shift for one line below
				m_viSearchForBug[i]++;
			start_from = 0;
		}
#ifdef MULTI_THREAD
		IncTotNumOfBugs(m_uNumOfBugs);
#endif
	}

	/**
	 * @brief Checks if rest of rows of Bug template are matching.
	 *
	 * If first row of loaded bug file is found in some row of landscape file then rest of the rows should also be compared.
	 *
	 * @param found_at column of processed row of landscape file, where bug pattern was matched
	 * @param start_from if bug pattern was found, next search should start after found bug
	 * @param currbugdim used for checking if last row of bug pattern has been matched (and therefore bug found)
	 * @return bool if checked bug pattern row is mathed or not
	 */
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	bool CBug<Data, iData, Container>::SearchBugPart(int found_at, int &start_from, unsigned int currbugdim)
	{
		unsigned int pos = 0;

		pos = (*m_viSearchForBug[currbugdim]).find(*s_viBugItself[currbugdim], found_at);

		if (pos == found_at && currbugdim == (s_uBugDimNum - 1))
		{
			m_uNumOfBugs++;
			start_from = found_at + s_uBugMaxDim;		// Skip founded Bug in next search
#ifdef SIMPLE_LOG
			if (m_fWriteFound.is_open())
				m_fWriteFound << "Bug number:" << m_uNumOfBugs << " ,found at line:" << m_uCurrLine << " ,at position:" << found_at + 1 << '\n';
#endif
			return true;
		}
		else if (pos == found_at && currbugdim < (s_uBugDimNum - 1))
			return true;
		else
			return false;
	}


	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	unsigned CBug<Data, iData, Container>::MaxBugPart(unsigned dim)
	{
		iData i_SearchBug = s_lFileBug.begin();
		unsigned max_dim = 0;
		for (unsigned i = 0; i <= dim; i++)
			if ((*i_SearchBug).length() > max_dim)
			{
				max_dim = (*i_SearchBug).length();
				i_SearchBug++;
			}
		return max_dim;
	}

	////////////////////////////////////////////////// Iterator design pattern
	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	CBug<Data, iData, Container>::Iterator::Iterator(CBug& rBug, unsigned start_line, unsigned last_line)
	{
		p_Collection = &rBug;
		i_CurrEl = i_LastEl = p_Collection->s_lFileLand.begin();
		advance(i_CurrEl, start_line);

		if (s_lFileLand.end() - i_CurrEl > LINES_PER_THREAD)
			advance(i_LastEl, start_line + LINES_PER_THREAD);
		else
			i_LastEl = s_lFileLand.end();
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	typename CBug<Data, iData, Container>::Iterator&  CBug<Data, iData, Container>::Iterator::operator++()
	{
		i_CurrEl++;

		return *this;
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	typename CBug<Data, iData, Container>::Iterator  CBug<Data, iData, Container>::Iterator::operator++(int)
	{
		Iterator bugTemp = *this;

		i_CurrEl++;
		return bugTemp;
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	CBug<Data, iData, Container>::Iterator::~Iterator()
	{
		p_Collection = NULL;
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	bool CBug<Data, iData, Container>::Iterator::operator==(const Iterator& ciBug) const
	{
		return i_CurrEl == ciBug.i_CurrEl;
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	bool CBug<Data, iData, Container>::Iterator::operator!=(const Iterator& ciBug) const
	{
		return !(i_CurrEl == ciBug.i_CurrEl);
	}

	template <class Data, class iData, template<typename _Tp, typename _Alloc = std::allocator<_Tp> > class Container >
	Data&  CBug<Data, iData, Container>::Iterator::operator*()
	{
		return *i_CurrEl;
	}
}
