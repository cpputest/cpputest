#include "CppUTest/TestHarness.h"
#include "CppUTest/SimpleStringExtensions.h"
#include "ProjectBuildTime.h"
#include <string.h>

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

TEST(ProjectBuildTime, CrudeBuildTimeTest)
{
  int expectedLength = strlen(__DATE__ " " __TIME__);
  int actualLength = strlen(projectBuildTime->GetDateTime());

  if (actualLength != expectedLength)
  {
      FAIL(projectBuildTime->GetDateTime());
  }
}

