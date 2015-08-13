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
#include "IEEE754ExceptionFlagsPlugin.h"
#include <cfenv>

extern "C" { 
    #include "FETests_c.h"
}

TEST_GROUP(FE__with_Plugin) {
    TestTestingFixture fixture;
    IEEE754ExceptionFlagsPlugin ieee754Plugin{"IEEE754"};
    void setup(void) override {
        fixture.registry_->installPlugin(&ieee754Plugin);
    }
};

TEST(FE__with_Plugin, should_fail_when__FE_DIVBYZERO__is_set) {
    fixture.setTestFunction(set_divisionbyzero_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(std::fetestexcept(FE_DIVBYZERO)) failed");
}

TEST(FE__with_Plugin, should_fail_when__FE_OVERFLOW___is_set) {
    fixture.setTestFunction(set_overflow_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(std::fetestexcept(FE_OVERFLOW)) failed");
}

TEST(FE__with_Plugin, should_fail_when__FE_UNDERFLOW__is_set) {
    fixture.setTestFunction(set_underflow_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(std::fetestexcept(FE_UNDERFLOW)) failed");
}

TEST(FE__with_Plugin, should_fail_when__FE_INVALID____is_set) {
    fixture.setTestFunction(set_invalid_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(std::fetestexcept(FE_INVALID)) failed");
}

TEST(FE__with_Plugin, should_fail_when__FE_INEXACT____is_set) {
    fixture.setTestFunction(set_inexact_c);
    fixture.runAllTests();
    fixture.assertPrintContains("IEEE754_CHECK_CLEAR(std::fetestexcept(FE_INEXACT)) failed");
}

TEST(FE__with_Plugin, should_succeed_with_5_checks_when_no_flags_are_set) {
    fixture.setTestFunction(set_nothing_c);
    fixture.runAllTests();
    fixture.assertPrintContains("OK (1 tests, 1 ran, 5 checks, 0 ignored, 0 filtered out, 0 ms)");
}

TEST(FE__with_Plugin, should_check_only_once_when_all_flags_are_set) {
    fixture.setTestFunction(set_everything_c);
    fixture.runAllTests();
    LONGS_EQUAL(1, fixture.getCheckCount());
}

TEST(FE__with_Plugin, should_fail_only_once_when_all_flags_are_set) {
    fixture.setTestFunction(set_everything_c);
    fixture.runAllTests();
    LONGS_EQUAL(1, fixture.getFailureCount());
}

static void set_everything_but_already_failed(void) {
    set_everything_c();
    CHECK(1 == 2);
}

TEST(FE__with_Plugin, should_not_fail_again_when_test_has_already_failed) {
    fixture.setTestFunction(set_everything_but_already_failed);
    fixture.runAllTests();
    LONGS_EQUAL(1, fixture.getCheckCount());
    LONGS_EQUAL(1, fixture.getFailureCount());
}

int main(int ac, char** av) {
	return RUN_ALL_TESTS(ac, av);
}
