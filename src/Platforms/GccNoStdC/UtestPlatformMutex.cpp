#include "CppUTest/TestHarness.h"
#include "CppUTest/PlatformSpecificFunctions.h"

PlatformSpecificMutex PlatformSpecificMutexCreate(void)
{
    FAIL("PlatformSpecificMutexCreate is not implemented");
    return 0;
}

void PlatformSpecificMutexLock(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexLock is not implemented");
}

void PlatformSpecificMutexUnlock(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexUnlock is not implemented");
}

void PlatformSpecificMutexDestroy(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexDestroy is not implemented");
}

