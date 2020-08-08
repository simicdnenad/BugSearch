#include "Bug.h"

#include <algorithm>
#include <time.h>

list<string>::iterator *CBug::s_aiBugItself;
unsigned CBug::s_uBugMaxDim, CBug::s_uBugDimNum, CBug::s_uNumOfLines;
list<string> CBug::s_lFileLand, CBug::s_lFileBug;
#ifdef MULTI_THREAD
	unsigned CBug::s_uNumOfThreads=0;
#endif

CBug::CBug()
{
#ifdef MULTI_THREAD
	m_uThreadId=s_uNumOfThreads++;
#endif
	m_uNumOfBugs = 0;
	m_aiSearchForBug = new list<string>::iterator [s_uBugDimNum];
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
			m_fWriteFound<<(*s_aiBugItself[i])<<'\n';
	}
	else
		cout << "Unable to open" <<   sPath.c_str() << "file! \n";
#endif

}

CBug::~CBug()
{
#ifdef MULTI_THREAD
	s_uNumOfThreads--;
	if(s_uNumOfThreads==0)
#endif
		delete [] s_aiBugItself;
	delete [] m_aiSearchForBug;
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
bool CBug::OnInit(int ac, char** av)
{
	ifstream infilebug(av[1]),infilelanscape(av[2]);
	string oneline;

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

	s_aiBugItself = new list<string>::iterator [s_uBugDimNum];
	list<string>::iterator i_BugItself = s_lFileBug.begin();
	for (unsigned int i = 0; i < s_uBugDimNum && i_BugItself != s_lFileBug.end(); i++)
		s_aiBugItself[i] = i_BugItself++;

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

unsigned int CBug::NumOfBugs(unsigned int start_line)
{
	/**unsigned*/ int start_from=0,found_at = 0, processed_line=start_line;
	list<string>::iterator i_SearchBug = s_lFileLand.begin();
	advance(i_SearchBug, start_line);
	for (unsigned int i = 0; i < s_uBugDimNum && i_SearchBug != s_lFileLand.end(); i++)
		m_aiSearchForBug[i] = i_SearchBug++;

	if (i_SearchBug == s_lFileLand.end())										// landscape.txt file is too small.
		return 0;

	while(m_aiSearchForBug[s_uBugDimNum-1] != s_lFileLand.end() &&  processed_line < (start_line + LINES_PER_THREAD))
	{
		while((found_at = (*m_aiSearchForBug[0]).find(*s_aiBugItself[0],start_from)) != NOT_FOUND)
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
			m_aiSearchForBug[i]++;
		start_from=0;
	}

	return m_uNumOfBugs;
}

bool CBug::SearchBugPart(/**unsigned*/ int found_at, /**unsigned*/ int &start_from, unsigned int currbugdim)
{
	unsigned int pos = 0;

	pos = (*m_aiSearchForBug[currbugdim]).find(*s_aiBugItself[currbugdim],found_at);

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

unsigned int CBug::GetNumOfLines()
{
	return s_uNumOfLines;
}

unsigned int CBug::GetNumOfBugs() const
{
	return m_uNumOfBugs;
}

unsigned CBug::MaxBugPart (unsigned dim)
{
	list<string>::iterator i_SearchBug = s_lFileBug.begin();
	unsigned max_dim = 0;
	for (unsigned i=0;i<=dim;i++)
		if((*i_SearchBug).length() > max_dim)
		{
			max_dim = (*i_SearchBug).length();
			i_SearchBug++;
		}
	return max_dim;
}
