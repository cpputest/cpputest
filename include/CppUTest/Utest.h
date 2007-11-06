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

///////////////////////////////////////////////////////////////////////////////
//
// TEST.H
//
// This file contains the Test class along with the macros which make effective
// in the harness.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_UTest_h
#define D_UTest_h


#include <math.h>
#include "SimpleString.h"
#include "TestInstaller.h"

/*! \brief UTest.h
 * \ingroup someGroup
 *
 * Something about UTest.h
 *
 * \see TEST_GROUP
 * \see TEST
 */

class TestResult;

class Utest
  {
  public:

    Utest(const char* groupName,
         const char* testName,
         const char* fileName,
         int lineNumber);

    virtual ~Utest();

    virtual void testBody(){};

    virtual void run (TestResult& result);
    virtual void executePlatformSpecificTestBody();
    virtual SimpleString getFormattedName() const;

		virtual Utest* addTest(Utest* test);
    virtual Utest *getNext () const;
    virtual bool isLast () const;
    virtual int  countTests();

    bool shouldRun(const SimpleString& groupFilter, const SimpleString& nameFilter) const;
    const SimpleString getName() const;
	const SimpleString getGroup() const;
    const SimpleString getFile() const;
    int getLineNumber() const;

    virtual void setup();
    virtual void teardown();

    static TestResult* getTestResult();
    static Utest* getCurrent();

    virtual bool assertTrue(bool condition, const char* conditionString, const char* fileName, int lineNumber);
    virtual bool assertCstrEqual(const char* expected, const char* actual, const char* fileName, int lineNumber);
    virtual bool assertLongsEqual(long expected, long actual, const char* fileName, int lineNumber);
    virtual bool assertDoublesEqual(double expected, double actual, double threshold, const char* fileName, int lineNumber);
    virtual void fail(const char* text, const char* fileName, int lineNumber);
	
	void setFileName(const char* fileName);
	void setLineNumber(int lineNumber);
	void setGroupName(const char* groupName);
	void setTestName(const char* testName);
  protected:
	
	Utest();
	
    Utest(const char* groupName,
         const char* testName,
         const char* fileName,
         int lineNumber, Utest* nextTest);

    virtual SimpleString getMacroName() const
      {
        return "TEST";
      }

  private:

    const char* group_;
    const char* name_;
    const char* file_;
    int	lineNumber_;
    Utest *next_;
    static TestResult* testResult_;
    static Utest* currentTest_;
  };

  /*! \brief Define a goup of tests
   * 
   * All tests in a TEST_GROUP share the same setup
   * and teardown.  setup is run before the opening
   * curly brace of the test group and teardown is 
   * called after the closing curly brace of the test group.
   * 
   */

#define TEST_GROUP(testGroup) \
  int externTestGroup##testGroup = 0; \
  struct CppUTestGroup##testGroup : public Utest

#define TEST_GROUP_BASE(testGroup, baseclass) \
  int externTestGroup##testGroup = 0; \
  struct CppUTestGroup##testGroup : public baseclass
  

#define TEST_SETUP() \
  virtual void setup() 

#define TEST_TEARDOWN() \
  virtual void teardown() 

#define TEST(testGroup, testName) \
  class testGroup##testName##Test : public CppUTestGroup##testGroup \
{ public: testGroup##testName##Test () : CppUTestGroup##testGroup () {} \
       void testBody(); } \
    testGroup##testName##Instance; \
  TestInstaller testGroup##testName##Installer(&testGroup##testName##Instance, #testGroup, #testName, __FILE__,__LINE__); \
	void testGroup##testName##Test::testBody()

#define IGNORE_TEST(testGroup, testName)\
  class testGroup##testName##Test : public CppUTestGroup##testGroup \
{ public: testGroup##testName##Test () : CppUTestGroup##testGroup () {} \
    virtual void run (TestResult& result) { \
    	result.countIgnored(); } \
  protected:  virtual SimpleString getMacroName() const \
      { return "IGNORE_TEST"; } \
  public:          void thisNeverRuns (); } \
    testGroup##testName##Instance; \
  TestInstaller testGroup##testName##Installer(&testGroup##testName##Instance, #testGroup, #testName, __FILE__,__LINE__); \
	void testGroup##testName##Test::thisNeverRuns ()

#define IMPORT_TEST_GROUP(testGroup) \
  extern int externTestGroup##testGroup;\
  int* p##testGroup = &externTestGroup##testGroup

//Check any boolean condition

#define CHECK_LOCATION(condition, file, line)\
  {if (!Utest::getCurrent()->assertTrue(condition, #condition, file, line)) return;}

#define CHECK(condition)\
  CHECK_LOCATION(condition, __FILE__, __LINE__)



//This check needs the equality operator, and a StringFrom(YourType) function

#define CHECK_EQUAL(expected,actual)\
  CHECK_EQUAL_LOCATION(expected, actual, __FILE__, __LINE__)

#define CHECK_EQUAL_LOCATION(expected,actual, file, line)\
  if ((expected) != (actual))\
  {\
	 Utest::getTestResult()->countCheck();\
  	 EqualsFailure _f(Utest::getCurrent(), file, line, StringFrom(expected), StringFrom(actual)); \
     Utest::getTestResult()->addFailure(_f);\
     return;\
  }\
  else\
	 Utest::getTestResult()->countCheck();

//This check checks for char* string equality using strcmp.
//This makes up for the fact that CHECK_EQUAL only compares the pointers to char*'s
#define STRCMP_EQUAL(expected,actual)\
  STRCMP_EQUAL_LOCATION(expected, actual, __FILE__, __LINE__)
   
#define STRCMP_EQUAL_LOCATION(expected,actual, file, line)\
  {if (!Utest::getCurrent()->assertCstrEqual(expected, actual, file, line)) return;}

//Check two long integers for equality
#define LONGS_EQUAL(expected,actual)\
  LONGS_EQUAL_LOCATION(expected,actual,__FILE__, __LINE__)
  
#define LONGS_EQUAL_LOCATION(expected,actual,file,line)\
  { if (!Utest::getCurrent()->assertLongsEqual(expected, actual,  file, line)) return; }

//Check two doubles for equality within a tolerance threshold
#define DOUBLES_EQUAL(expected,actual,threshold)\
  DOUBLES_EQUAL_LOCATION(expected,actual,threshold,__FILE__,__LINE__)
  
#define DOUBLES_EQUAL_LOCATION(expected,actual,threshold,file,line)\
  { if (!Utest::getCurrent()->assertDoublesEqual(expected, actual, threshold,  file, line)) return; }

//Fail if you get to this macro
//The macro FAIL may already be taken, so allow FAIL_TEST too
#ifndef FAIL
#define FAIL(text)\
  FAIL_LOCATION(text, __FILE__,__LINE__)
  
#define FAIL_LOCATION(text, file, line)\
  { Utest::getCurrent()->fail(text,  file, line); return; }
#endif

#define FAIL_TEST(text, file, line)\
  FAIL_TEST_LOCATION(text, __FILE__,__LINE__)
  
#define FAIL_TEST_LOCATION(text, file,line)\
  { Utest::getCurrent()->fail(text, file, line); return; }

#endif
