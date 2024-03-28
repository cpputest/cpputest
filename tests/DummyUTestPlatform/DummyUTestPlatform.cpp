#include <CppUTest/PlatformSpecificFunctions.h>

typedef char jmp_buf[200];

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) = NULLPTR;
int (*PlatformSpecificFork)(void) = NULLPTR;
int (*PlatformSpecificWaitPid)(int pid, int* status, int options) = NULLPTR;

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

extern "C" int setjmp(jmp_buf);
static int fakeSetJmp(void (*function)(void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}
int (*PlatformSpecificSetJmp)(void (*function)(void*), void* data) = fakeSetJmp;

extern "C" void longjmp(jmp_buf, int);
static void fakeLongJmp(void)
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}
void (*PlatformSpecificLongJmp)(void) = fakeLongJmp;

static void fakeRestoreJumpBuffer()
{
    jmp_buf_index--;
}
void (*PlatformSpecificRestoreJumpBuffer)(void) = fakeRestoreJumpBuffer;

static unsigned long fakeTimeInMillis(void)
{
    return 0;
}
unsigned long (*GetPlatformSpecificTimeInMillis)(void) = fakeTimeInMillis;

static const char* fakeTimeString(void)
{
    return "";
}
const char* (*GetPlatformSpecificTimeString)() = fakeTimeString;

extern "C" int vsnprintf(char*, size_t, const char*, va_list);
int (*PlatformSpecificVSNprintf)(char* str, size_t size, const char* format, va_list va_args_list) = vsnprintf;

extern "C" double fabs(double);
double (*PlatformSpecificFabs)(double d) = fabs;

static int fakeIsNan(double d)
{
    return d != d;
}
int (*PlatformSpecificIsNan)(double d) = fakeIsNan;

static int fakeIsInf(double d)
{
    return !fakeIsNan(d) && fakeIsNan(d - d);
}
int (*PlatformSpecificIsInf)(double d) = fakeIsInf;

extern "C" int atexit(void (*func)(void));
int (*PlatformSpecificAtExit)(void (*func)(void)) = atexit;

extern "C" void* stdout;
PlatformSpecificFile PlatformSpecificStdOut = stdout;

extern "C" void* fopen(const char*, const char*);
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = fopen;

extern "C" int fputs(const char*, void*);
static void fakeFPuts(const char* str, PlatformSpecificFile file)
{
    fputs(str, file);
}
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = fakeFPuts;

extern "C" int fclose(void* stream);
static void fakeFClose(PlatformSpecificFile file)
{
    fclose(file);
}
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = fakeFClose;

extern "C" int fflush(void* stream);
static void fakeFlush(void)
{
    fflush(stdout);
}
void (*PlatformSpecificFlush)(void) = fakeFlush;

static void fakeSrand(unsigned int){};
void (*PlatformSpecificSrand)(unsigned int) = fakeSrand;

static int fakeRand(void)
{
    return 0;
}
int (*PlatformSpecificRand)(void) = fakeRand;

extern "C" void* malloc(size_t);
void* (*PlatformSpecificMalloc)(size_t) = malloc;

extern "C" void* realloc(void* ptr, size_t new_size);
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = realloc;

extern "C" void free(void*);
void (*PlatformSpecificFree)(void* memory) = free;

extern "C" void* memcpy(void* dest, const void* src, size_t count);
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = memcpy;

extern "C" void* memset(void* dest, int ch, size_t count);
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = memset;

static PlatformSpecificMutex fakeMutexCreate(void)
{
    return 0;
}
PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = fakeMutexCreate;

static void fakeMutexFunc(PlatformSpecificMutex) {}
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex mtx) = fakeMutexFunc;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex mtx) = fakeMutexFunc;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex mtx) = fakeMutexFunc;

extern "C" void abort(void);
void (*PlatformSpecificAbort)(void) = abort;
