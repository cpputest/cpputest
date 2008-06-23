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
#include "CppUTest/MockTestOutput.h"
#include "CppUTest/TestTestingFixture.h"

TEST_GROUP(Utest)
{
	TestTestingFixture* fixture;
	TEST_SETUP()
	{
		fixture = new TestTestingFixture();
	    UT_PTR_SET(PlatformSpecificExitCurrentTest, FakePlatformSpecificExitCurrentTest);
	}
	TEST_TEARDOWN()
	{
  		delete fixture;
  	}
};

static void _failMethod()
{
    FAIL("This test fails");
}

TEST(Utest, FailurePrintsSomething)
{
  fixture->setTestFunction(_failMethod);
  fixture->runAllTests(); 
  LONGS_EQUAL(1, fixture->getFailureCount());
  fixture->assertPrintContains("This test fails");
}

static void _LongsEqualFailMethod()
{
  LONGS_EQUAL(1, 0xff);
}

TEST(Utest, FailurePrintHexOutputForLongInts)
{
  fixture->setTestFunction(_LongsEqualFailMethod);
  fixture->runAllTests(); 
  fixture->assertPrintContains("expected <  1 0x01>");
  fixture->assertPrintContains("but was  <255 0xff>");
}


static void _passMethod()
{
	CHECK(true);
}

TEST(Utest, SuccessPrintsNothing)
{
  fixture->setTestFunction(_passMethod);
  fixture->runAllTests(); 
  LONGS_EQUAL(0, fixture->getFailureCount());
  fixture->assertPrintContains(".\nOK (1 tests");
}

TEST(Utest, allMacros)
{
  CHECK(0 == 0);
  LONGS_EQUAL(1,1);
  CHECK_EQUAL("THIS", "THIS");
  CHECK_EQUAL(100,100);
  STRCMP_EQUAL("THIS", "THIS");
  DOUBLES_EQUAL(1.0, 1.0, .01);
}

TEST(Utest, AssertsActLikeStatements)
{
  if (fixture != 0)
    CHECK(true)
  else
    CHECK(false)

  if (fixture != 0)
    CHECK_EQUAL(true, true)
  else
    CHECK_EQUAL(false, false)

  if (fixture != 0)
    STRCMP_EQUAL("", "")
    else
      STRCMP_EQUAL("", " ")

  if (fixture != 0)
    LONGS_EQUAL(1, 1)
    else
      LONGS_EQUAL(1, 0)

  if (fixture != 0)
    DOUBLES_EQUAL(1, 1, 0.01)
    else
      DOUBLES_EQUAL(1, 0, 0.01)

  if (false)
    FAIL("")
    else
      ;

  if (true)
    ;
  else
    FAIL("")

}

IGNORE_TEST(Utest, IgnoreTestSupportsAllMacros)
{
  CHECK(true);
  CHECK_EQUAL(true, true);
  STRCMP_EQUAL("", "");
  LONGS_EQUAL(1, 1);
  DOUBLES_EQUAL(1, 1, 0.01);
  FAIL("");
}

IGNORE_TEST(Utest, IgnoreTestAccessingFixture)
{
	CHECK(fixture != 0);
}

TEST(Utest, MacrosUsedInSetup)
{
	delete fixture->genTest;
	fixture->genTest = new ExecFunctionTest(_failMethod);
 	fixture->setTestFunction(_passMethod);
 	fixture->runAllTests(); 
	LONGS_EQUAL(1, fixture->getFailureCount());
}

TEST(Utest, MacrosUsedInTearDown)
{
	delete fixture->genTest;
	fixture->genTest = new ExecFunctionTest(0, _failMethod);
 	fixture->setTestFunction(_passMethod);
 	fixture->runAllTests(); 
	LONGS_EQUAL(1, fixture->getFailureCount());
}

TEST_BASE(MyOwnTest)
{
	MyOwnTest() : inTest(false) {}
	bool inTest;
	
	void setup()
	{
		CHECK(!inTest);
		inTest = true;
	}
	void teardown()
	{
		CHECK(inTest);
		inTest = false;
	}
};

TEST_GROUP_BASE(UtestMyOwn, MyOwnTest)
{
};

TEST(UtestMyOwn, test)
{
	CHECK(inTest);
}

class NullParameterTest : public Utest
{
};

TEST(UtestMyOwn, NullParameters)
{
	NullParameterTest nullTest; /* Bug fix tests for creating a test without a name, fix in SimpleString */
	TestRegistry* reg = TestRegistry::getCurrentRegistry();
	nullTest.shouldRun(reg->getGroupFilter(), reg->getNameFilter());
}

