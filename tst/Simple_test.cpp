#define SIMPLE_LOG
#include "gtest/gtest.h"
#include "../include/Bug.h"
using namespace landscape;
typedef CBug<string, CONTAINER<string>::iterator, CONTAINER> CBugT;

// Should make test class
class CBugTest : public ::testing::Test {
 protected:
	void SetUp() override
	{
		up_BugTest = std::unique_ptr<CBugT>(new CBugT ("TestBug1"));
	}

	void TearDown() override {}
	unique_ptr<CBugT> up_BugTest;
};

TEST_F( CBugTest, TestFileInput )
{
	EXPECT_TRUE( !(up_BugTest->GetBugName().compare("TestBug1")) );

	vector<string> vBugTestFilesPaths;
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/bug.nfo" );
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/bug1.nfo" );
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/bug2.nfo" );
	auto iBugTestFilesPaths = vBugTestFilesPaths.begin();

	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::ALL_SUCCESSFULL );
	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape1.nfo") , CBugT::EFileOpenErrors::LANDSCAPE_FAIL );
	iBugTestFilesPaths++;
	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::BUG_FAIL );
	iBugTestFilesPaths++;
	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::BUG_FAIL );

}

TEST( BugTest, TestNumOfBugs )
{
	vector<string> vBugTestFilesPaths;
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/spiderman_2.nfo" );
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/spiderman.nfo" );
	auto iBugTestFilesPaths = vBugTestFilesPaths.begin();

	while ( iBugTestFilesPaths != vBugTestFilesPaths.end() )
	{
		string sFileName = (*iBugTestFilesPaths).substr((*iBugTestFilesPaths).rfind(DIR_SEPARATOR) + 1,
						   (*iBugTestFilesPaths).rfind('.') - (*iBugTestFilesPaths).rfind(DIR_SEPARATOR) - 1);
		CBugT Bug(sFileName);
		ASSERT_EQ( Bug.OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::ALL_SUCCESSFULL );
		Bug.NumOfBugs( 0 );

		if ( sFileName.compare("spiderman_2") == 0  )
			EXPECT_EQ( Bug.GetNumOfBugs(), 0 );
		else if ( sFileName.compare("spiderman") == 0 )
			EXPECT_EQ( Bug.GetNumOfBugs(), 4 );

		iBugTestFilesPaths++;
	}
}
