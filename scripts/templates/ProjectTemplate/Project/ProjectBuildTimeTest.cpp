#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ProjectBuildTime.h"

TEST_GROUP(ProjectBuildTime)
{
  ProjectBuildTime* projectBuildTime;

  void setup()
  {
    projectBuildTime = new ProjectBuildTime();
  }
  void teardown()
  {
    delete projectBuildTime;
  }
};

TEST(ProjectBuildTime, Create)
{
  STRCMP_EQUAL(__DATE__ " " __TIME__, projectBuildTime->GetDateTime());
}

