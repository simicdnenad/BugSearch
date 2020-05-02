#include "Bug.h"

#include <algorithm>
#include <time.h>



CBug::CBug()
{
	m_uNumOfBugs = 0;
	m_uNumOfLines = 0;
	m_uBugMaxDim = 0;
	m_uBugDimNum = 0;
	m_aiBugItself = NULL;
#ifdef SIMPLE_LOG
	m_uCurrLine = 1;
	m_fWriteFound.open("WriteFoundBugs.txt");
	if (m_fWriteFound.is_open())
	{
		cout << "WriteFoundBugs.txt opened successfully for writing.\n";
		m_fWriteFound << "This is the file for logging founded Bug patterns.\n";
	}
	else
		cout << "Unable to open WriteFoundBugs.txt file! \n";
#endif

}

CBug::~CBug()
{
	delete [] m_aiBugItself;
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
		if(oneline.empty())
			continue;
		m_lFileBug.push_back(oneline);
		if (oneline.size() > m_uBugMaxDim)
			m_uBugMaxDim = oneline.size();
		m_uBugDimNum++;
	}

	m_aiBugItself = new list<string>::iterator [m_uBugDimNum];
	list<string>::iterator i_BugItself = m_lFileBug.begin();
	for (unsigned int i = 0; i < m_uBugDimNum && i_BugItself != m_lFileBug.end(); i++)
		m_aiBugItself[i] = i_BugItself++;

#ifdef SIMPLE_LOG
	if (m_fWriteFound.is_open())
	{
		m_fWriteFound<<"Bug pattern:\n";
		for (unsigned int i = 0; i < m_uBugDimNum; i++)
			m_fWriteFound<<(*m_aiBugItself[i])<<'\n';
	}
	else
		cout << "Output file for writing found bugs open ERROR!!!" << '\n';
#endif
//----------------------LAND------------------------------
	while (getline(infilelanscape,oneline))	// Add also num of lines count, to determine the optimal number of created threads
	{
		/**if(oneline.empty())
			continue;							Not skipping empty lines to avoid "merging" of Bug parts */
		m_lFileLand.push_back(oneline);
		m_uNumOfLines++;
	}
	m_aiSearchForBug = new list<string>::iterator [m_uBugDimNum];

	infilebug.close();
	infilelanscape.close();

	return true;
}

unsigned int CBug::NumOfBugs(unsigned int start_line)
{
	/**unsigned*/ int start_from=0,found_at = 0;
	list<string>::iterator i_SearchBug = m_lFileLand.begin();
	for (unsigned int i = 0; i < m_uBugDimNum && i_SearchBug != m_lFileLand.end(); i++)
		m_aiSearchForBug[i] = i_SearchBug++;

	if (i_SearchBug == m_lFileLand.end())										// landscape.txt file is too small.
		return 0;

	while(m_aiSearchForBug[m_uBugDimNum-1] != m_lFileLand.end())
	{
		while((found_at = (*m_aiSearchForBug[0]).find(*m_aiBugItself[0],start_from)) != -1 /**string::npos*/)
		{
			unsigned int bugdim = 1;									// Compare every Bug's dimension
			bool founded = false;
			do
			{
				founded = SearchBugPart(found_at, start_from, bugdim++);
			}while(founded && bugdim < m_uBugDimNum);

			if(!founded)
				start_from = found_at + MaxBugPart(bugdim-2);	// Bug not founded, skip its longest founded part
		}
#ifdef SIMPLE_LOG
		m_uCurrLine++;
#endif

		for (unsigned int i = 0; i < m_uBugDimNum ; i++)
			m_aiSearchForBug[i]++;
		start_from=0;
	}

	return m_uNumOfBugs;
}

bool CBug::SearchBugPart(/**unsigned*/ int found_at, /**unsigned*/ int &start_from, unsigned int currbugdim)
{
	unsigned int pos = 0;

	pos = (*m_aiSearchForBug[currbugdim]).find(*m_aiBugItself[currbugdim],found_at);

	if (pos == found_at && currbugdim == (m_uBugDimNum-1))
	{
		m_uNumOfBugs++;
		start_from = found_at + m_uBugMaxDim;		// Skip founded Bug in next search
#ifdef SIMPLE_LOG
		if (m_fWriteFound.is_open())
			m_fWriteFound<<"Bug number:" << m_uNumOfBugs << " ,found at line:"<< m_uCurrLine << " ,at position:"<<  found_at+1<< '\n';
#endif
		return true;
	}
	else if(pos == found_at && currbugdim < (m_uBugDimNum-1))
		return true;
	else
		return false;
}

unsigned int CBug::GetNumOfLines() const
{
	return m_uNumOfLines;
}

unsigned int CBug::GetNumOfBugs() const
{
	return m_uNumOfBugs;
}

unsigned CBug::MaxBugPart (unsigned dim)
{
	list<string>::iterator i_SearchBug = m_lFileBug.begin();
	unsigned max_dim = 0;
	for (unsigned i=0;i<=dim;i++)
		if((*i_SearchBug).length() > max_dim)
		{
			max_dim = (*i_SearchBug).length();
			i_SearchBug++;
		}
	return max_dim;
}
