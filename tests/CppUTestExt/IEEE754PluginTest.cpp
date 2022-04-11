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

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTestExt/IEEE754ExceptionsPlugin.h"

#ifdef CPPUTEST_HAVE_FENV
#if CPPUTEST_FENV_IS_WORKING_PROPERLY

extern "C"
{
    #include "IEEE754PluginTest_c.h"
}

TEST_GROUP(FE_with_Plugin)
{
    TestTestingFixture fixture;
    IEEE754ExceptionsPlugin ieee754Plugin;
    void setup(void) _override
    {
        fixture.installPlugin(&ieee754Plugin);
    }
};

TEST(FE_with_Plugin, should_fail_when_FE_DIVBYZERO_is_set)
{
    fixture.setTestFunction(set_divisionbyzero_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_DIVBYZERO) failed");
}

TEST(FE_with_Plugin, should_fail_when_FE_OVERFLOW_is_set)
{
    fixture.setTestFunction(set_overflow_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_OVERFLOW) failed");
}

TEST(FE_with_Plugin, should_fail_when_FE_UNDERFLOW_is_set)
{
    fixture.setTestFunction(set_underflow_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_UNDERFLOW) failed");
}

TEST(FE_with_Plugin, should_fail_when_FE_INEXACT_is_set_and_enabled)
{
    IEEE754ExceptionsPlugin::enableInexact();
    fixture.setTestFunction(set_inexact_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_INEXACT) failed");
}

TEST(FE_with_Plugin, should_succeed_when_FE_INEXACT_is_set_and_disabled)
{
    IEEE754ExceptionsPlugin::enableInexact();
    IEEE754ExceptionsPlugin::disableInexact();
    fixture.setTestFunction(set_inexact_c);
    fixture.runAllTests();
    fixture.assertPrintContains("OK");
}

TEST(FE_with_Plugin, should_succeed_with_5_checks_when_no_flags_are_set)
{
    IEEE754ExceptionsPlugin::enableInexact();
    fixture.setTestFunction(set_nothing_c);
    fixture.runAllTests();
    fixture.assertPrintContains("OK (1 tests, 1 ran, 5 checks, 0 ignored, 0 filtered out");
    IEEE754ExceptionsPlugin::disableInexact();
}

TEST(FE_with_Plugin, should_check_five_times_when_all_flags_are_set)
{
    fixture.setTestFunction(set_everything_c);
    fixture.runAllTests();
    LONGS_EQUAL(5, fixture.getCheckCount());
}

TEST(FE_with_Plugin, should_fail_only_once_when_all_flags_are_set)
{
    fixture.setTestFunction(set_everything_c);
    fixture.runAllTests();
    LONGS_EQUAL(1, fixture.getFailureCount());
}

static void set_everything_but_already_failed(void)
{
    set_everything_c();
    CHECK(1 == 2);
}

TEST(FE_with_Plugin, should_not_fail_again_when_test_has_already_failed)
{
    fixture.setTestFunction(set_everything_but_already_failed);
    fixture.runAllTests();
    CHECK(IEEE754ExceptionsPlugin::checkIeee754OverflowExceptionFlag());
    CHECK(IEEE754ExceptionsPlugin::checkIeee754UnderflowExceptionFlag());
    CHECK(IEEE754ExceptionsPlugin::checkIeee754InexactExceptionFlag());
    CHECK(IEEE754ExceptionsPlugin::checkIeee754DivByZeroExceptionFlag());
    LONGS_EQUAL(1, fixture.getCheckCount());
    LONGS_EQUAL(1, fixture.getFailureCount());
}

static IEEE754ExceptionsPlugin ip;

TEST_GROUP(IEEE754ExceptionsPlugin2)
{
    void setup(void) _override
    {
        TestRegistry::getCurrentRegistry()->installPlugin(&ip);
    }
};

IGNORE_TEST(IEEE754ExceptionsPlugin2, should_not_fail_in_ignored_test)
{
    set_everything_c();
}

#endif
#endif
