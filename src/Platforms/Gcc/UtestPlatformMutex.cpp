#include "CppUTest/TestHarness.h"
#include "CppUTest/PlatformSpecificFunctions.h"

PlatformSpecificMutex PlatformSpecificMutexCreate(void)
{
    pthread_mutex_t *mutex = new pthread_mutex_t;
    
    pthread_mutex_init(mutex, NULL);
    
    return (PlatformSpecificMutex)mutex;
}

void PlatformSpecificMutexLock(PlatformSpecificMutex mtx)
{
    pthread_mutex_lock((pthread_mutex_t *)mtx);
}

void PlatformSpecificMutexUnlock(PlatformSpecificMutex mtx)
{
    pthread_mutex_unlock((pthread_mutex_t *)mtx);
}

void PlatformSpecificMutexDestroy(PlatformSpecificMutex mtx)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)mtx;
    pthread_mutex_destroy(mutex);
    delete mutex;
}
