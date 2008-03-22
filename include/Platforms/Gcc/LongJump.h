#ifndef D_LongJump_H
#define D_LongJump_H

///////////////////////////////////////////////////////////////////////////////
//
//  LongJump is a wrapper module for OS depdendent setjump and longjump
//  implementations
//
///////////////////////////////////////////////////////////////////////////////
#include <setjmp.h>

extern jmp_buf platform_jmp_buf;

#define PLATFORM_SETJMP     setjmp(platform_jmp_buf)
#define PLATFORM_LONGJMP  longjmp(platform_jmp_buf, 1)

#endif  // D_LongJump_H
