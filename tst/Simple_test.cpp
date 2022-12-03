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

TEST_F( CBugTest, TestNumOfBugs )
{
	vector<string> vBugTestFilesPaths;
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/spiderman_2.nfo" );
	vBugTestFilesPaths.push_back( "../BugSearch/manual_test/spiderman.nfo" );
	auto iBugTestFilesPaths = vBugTestFilesPaths.begin();

	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::ALL_SUCCESSFULL );
	up_BugTest->NumOfBugs( 0 );
	EXPECT_EQ( up_BugTest->GetNumOfBugs(), 0 );

	iBugTestFilesPaths++;
	EXPECT_EQ( up_BugTest->OnInit(iBugTestFilesPaths,"../BugSearch/manual_test/landscape.nfo") , CBugT::EFileOpenErrors::ALL_SUCCESSFULL );
	up_BugTest->NumOfBugs( 0 );
	EXPECT_EQ( up_BugTest->GetNumOfBugs(), 3 );
}
