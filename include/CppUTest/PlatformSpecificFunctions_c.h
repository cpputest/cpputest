/*
 * PlatformSpecificFunctions_c.h
 *
 *  Created on: Nov 29, 2011
 *      Author: james
 */

#ifndef PLATFORMSPECIFICFUNCTIONS_C_H_
#define PLATFORMSPECIFICFUNCTIONS_C_H_

/* Time operations */
long GetPlatformSpecificTimeInMillis();
void SetPlatformSpecificTimeInMillisMethod(long(*platformSpecific)());

const char* GetPlatformSpecificTimeString();
void SetPlatformSpecificTimeStringMethod(const char* (*platformMethod)());

/* String operations */
int PlatformSpecificAtoI(const char*str);
size_t PlatformSpecificStrLen(const char* str);
char* PlatformSpecificStrCat(char* s1, const char* s2);
char* PlatformSpecificStrCpy(char* s1, const char* s2);
char* PlatformSpecificStrNCpy(char* s1, const char* s2, size_t size);
int PlatformSpecificStrCmp(const char* s1, const char* s2);
int PlatformSpecificStrNCmp(const char* s1, const char* s2, size_t size);
char* PlatformSpecificStrStr(const char* s1, const char* s2);

int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format,
		va_list va_args_list);

char PlatformSpecificToLower(char c);

/* Misc */
double PlatformSpecificFabs(double d);
int PlatformSpecificIsNan(double d);
int PlatformSpecificAtExit(void(*func)());

/* IO operations */
typedef void* PlatformSpecificFile;

PlatformSpecificFile PlatformSpecificFOpen(const char* filename,
		const char* flag);
void PlatformSpecificFPuts(const char* str, PlatformSpecificFile file);
void PlatformSpecificFClose(PlatformSpecificFile file);

int PlatformSpecificPutchar(int c);
void PlatformSpecificFlush();

/* Dynamic Memory operations */
void* PlatformSpecificMalloc(size_t size);
void* PlatformSpecificRealloc(void* memory, size_t size);
void PlatformSpecificFree(void* memory);
void* PlatformSpecificMemCpy(void* s1, const void* s2, size_t size);
void* PlatformSpecificMemset(void* mem, int c, size_t size);

#endif /* PLATFORMSPECIFICFUNCTIONS_C_H_ */
