/*
 * Copyright (c) 2016, Michael Feathers, James Grenning, Bas Vodde
 * and Arnd Strube. All rights reserved.
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

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"

#ifdef CPPUTEST_HAVE_FENV
#include "CppUTestExt/IEEE754ExceptionsPlugin.h"

/*
 * To see a demonstration of tests failing as a result of IEEE754ExceptionsPlugin
 * picking up floating point errors, run the test executable with the -ri option.
 *
 */

extern "C" {
    #include <fenv.h>
}

#include <limits>

static volatile float f;

TEST_GROUP(FE_Demo)
{
    void setup()
    {
        IEEE754ExceptionsPlugin::disableInexact();
    }
};

IGNORE_TEST(FE_Demo, should_fail_when__FE_DIVBYZERO__is_set)
{
    f = 1.0f;
    CHECK((f /= 0.0f) >= std::numeric_limits<float>::infinity());
}

IGNORE_TEST(FE_Demo, should_fail_when__FE_UNDERFLOW__is_set)
{
    f = 0.01f;
    while (f > 0.0f) f *= f;
    CHECK(f == 0.0f);
}

IGNORE_TEST(FE_Demo, should_fail_when__FE_OVERFLOW__is_set)
{
    f = 1000.0f;
    while (f < std::numeric_limits<float>::infinity()) f *= f;
    CHECK(f >= std::numeric_limits<float>::infinity());
}

IGNORE_TEST(FE_Demo, should_fail_when__FE_INEXACT____is_set)
{
    IEEE754ExceptionsPlugin::enableInexact();
    f = 10.0f;
    DOUBLES_EQUAL((double) (f / 3.0f), (double) 3.333f, (double) 0.001f);
}

TEST(FE_Demo, should_succeed_when_no_flags_are_set)
{
    CHECK(5.0f == 15.0f / 3.0f);
}

#endif
