
#include "Utest.h"
#include "TestResult.h"
#include <sys/time.h>

void Utest::executePlatformSpecificTestBody()
{
	testBody();
}

long TestResult::GetPlatformSpecificTimeInMillis()
{
	struct timeval tv;
	struct timezone tz;
	::gettimeofday(&tv, &tz);
	return (tv.tv_sec * 10) + (tv.tv_usec * 0.001);	
}
