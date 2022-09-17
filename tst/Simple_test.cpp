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
		p_BugTest = new CBugT ("TestBug1");
	}

	void TearDown() override { delete p_BugTest;}
	CBugT*	p_BugTest;
};

TEST_F(CBugTest, TestBasic)
{
	EXPECT_TRUE(!(p_BugTest->GetBugName().compare("TestBug1")));
}
