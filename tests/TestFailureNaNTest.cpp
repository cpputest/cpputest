/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
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
#include "CppUTest/TestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"

namespace
{
const int failLineNumber = 2;
const char* failFileName = "fail.cpp";
}

static const double not_a_number = -1;
static const double infinity = -2;

#define FAILURE_EQUAL(a, b) STRCMP_EQUAL_LOCATION(a, b.failure->getMessage().asCharString(), __FILE__, __LINE__)

extern "C" {
    static int IsNanStub(double d) { return (not_a_number == d); }
    static int IsInfStub(double d) { return (infinity == d); }
}

struct DoublesEqualFailureFixture
{
    UtestShell* test;
    DoublesEqualFailure* failure;

    DoublesEqualFailureFixture(double a, double b, double threshold) {
		UT_PTR_SET(PlatformSpecificIsNan, IsNanStub);
        UT_PTR_SET(PlatformSpecificIsInf, IsInfStub);
        test = new UtestShell("groupname", "testname", failFileName, failLineNumber-1);
        failure = new DoublesEqualFailure(test, failFileName, failLineNumber, a, b, threshold);
    }

    ~DoublesEqualFailureFixture() {
        delete failure;
        delete test;
    }
};

TEST_GROUP(TestFailureNaN)
{
};

TEST(TestFailureNaN, DoublesEqualExpectedIsNaN)
{
    DoublesEqualFailureFixture fixture(not_a_number, 2, 3);
    FAILURE_EQUAL("expected <Nan - Not a number>\n"
                "\tbut was  <2> threshold used was <3>\n"
                "\tCannot make comparisons with Nan", fixture);
}

TEST(TestFailureNaN, DoublesEqualActualIsNaN)
{
    DoublesEqualFailureFixture fixture(1.0, not_a_number, 3.0);
    FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <Nan - Not a number> threshold used was <3>\n"
                "\tCannot make comparisons with Nan", fixture);
}

TEST(TestFailureNaN, DoublesEqualThresholdIsNaN)
{
    DoublesEqualFailureFixture fixture(1.0, 2.0, not_a_number);
    FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <2> threshold used was <Nan - Not a number>\n"
                "\tCannot make comparisons with Nan", fixture);
}

TEST_GROUP(TestFailureInf)
{
};

TEST(TestFailureInf, DoublesEqualExpectedIsInf)
{
    DoublesEqualFailureFixture fixture(infinity, 2.0, 3.0);
    FAILURE_EQUAL("expected <Inf - Infinity>\n"
                "\tbut was  <2> threshold used was <3>", fixture);
}

TEST(TestFailureInf, DoublesEqualActualIsInf)
{
    DoublesEqualFailureFixture fixture(1.0, infinity, 3.0);
    FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <Inf - Infinity> threshold used was <3>", fixture);
}
