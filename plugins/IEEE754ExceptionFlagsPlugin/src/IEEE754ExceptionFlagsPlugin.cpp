/*
 * Copyright (c) 2015, Michael Feathers, James Grenning, Bas Vodde
 * and Arnd R. Strube. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "IEEE754ExceptionFlagsPlugin.h"

#include <cfenv>

/* This code is taken from:
 
 Title:  Floating-point exception handling example
 Author:  David N. Williams
 File:  fe-handlng-example.c
 License:  Public Domain
 Version:  0.5.0
 Started:  21-Sep-09
 Revised:  22-Sep-09
 Revised:  30-Sep-09 (comment typo)

*/

extern "C" {

#if 0
static int zXfegetexcept (void)
{
    static fenv_t fenv;

    return fegetenv (&fenv) ? -1 : (fenv.__control & FE_ALL_EXCEPT);
}
#endif

static int feenableexcept (unsigned int excepts)
{
    static fenv_t fenv;
    unsigned int new_excepts = excepts & FE_ALL_EXCEPT,
    old_excepts;  // previous masks

    if ( fegetenv (&fenv) ) return -1;
    old_excepts = fenv.__control & FE_ALL_EXCEPT;

    // unmask
    fenv.__control &= ~new_excepts;
    fenv.__mxcsr   &= ~(new_excepts << 7);

    return ( fesetenv (&fenv) ? -1 : old_excepts );
}

static int fedisableexcept (unsigned int excepts)
{
    static fenv_t fenv;
    unsigned int new_excepts = excepts & FE_ALL_EXCEPT,
    old_excepts;  // all previous masks

    if ( fegetenv (&fenv) ) return -1;
    old_excepts = fenv.__control & FE_ALL_EXCEPT;

    // mask
    fenv.__control |= new_excepts;
    fenv.__mxcsr   |= new_excepts << 7;

    return ( fesetenv (&fenv) ? -1 : old_excepts );
}

}

/****** End of Public Domain code ******/

#define IEEE754_CHECK_CLEAR(flag) { \
    if(!hasFailed_) { \
        result_->countCheck(); \
        if(flag) { \
            CheckFailure failure(test_, __FILE__, __LINE__, "IEEE754_CHECK_CLEAR", #flag); \
            result_->addFailure(failure); \
            hasFailed_ = true; \
        } \
    } \
}

void IEEE754ExceptionFlagsPlugin::preTestAction(UtestShell&, TestResult&)
{
    std::feclearexcept(FE_ALL_EXCEPT);
}

void IEEE754ExceptionFlagsPlugin::postTestAction(UtestShell& test, TestResult& result)
{
    hasFailed_ = test.hasFailed();
    test_ = &test;
    result_ = &result;
    IEEE754_CHECK_CLEAR(std::fetestexcept(FE_DIVBYZERO));
    IEEE754_CHECK_CLEAR(std::fetestexcept(FE_OVERFLOW));
    IEEE754_CHECK_CLEAR(std::fetestexcept(FE_UNDERFLOW));
    IEEE754_CHECK_CLEAR(std::fetestexcept(FE_INVALID));
    if (inexactEnabled_) {
        IEEE754_CHECK_CLEAR(std::fetestexcept(FE_INEXACT));
    }
}

void IEEE754ExceptionFlagsPlugin::disableInexact()
{
    inexactEnabled_ = false;
}

void IEEE754ExceptionFlagsPlugin::enableInexact()
{
    inexactEnabled_ = true;
}

void IEEE754ExceptionFlagsPlugin::disableSignal()
{
    fedisableexcept(FE_ALL_EXCEPT);
}

void IEEE754ExceptionFlagsPlugin::enableSignal()
{
    feenableexcept(FE_ALL_EXCEPT);
}
