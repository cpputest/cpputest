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

#ifndef D_OrderedTest_h
#define D_OrderedTest_h

class OrderedTest : public Utest
{
public:
   OrderedTest();
   virtual ~OrderedTest();

   virtual OrderedTest* addOrderedTest(OrderedTest* test);
   virtual OrderedTest* getNextOrderedTest();

   int getLevel();
   void setLevel(int level);

   static void addOrderedTestToHead(OrderedTest* test);
   static OrderedTest* getOrderedTestHead();
   static bool firstOrderedTest();

   static void setOrderedTestHead(OrderedTest* test);
private:
   static OrderedTest* _orderedTestsHead;
   OrderedTest* _nextOrderedTest;

   int _level;

};

class OrderedTestInstaller
{
  public:
    explicit OrderedTestInstaller(OrderedTest* test, const char* groupName, const char* testName, const char* fileName, int lineNumber, int level);
    virtual ~OrderedTestInstaller();

  private:
     void addOrderedTestInOrder(OrderedTest* test);
	void addOrderedTestInOrderNotAtHeadPosition(OrderedTest* test);

};

#define TEST_ORDERED(testGroup, testName, testLevel) \
  class TEST_##testGroup##_##testName##_Test : public TEST_GROUP_##CppUTestGroup##testGroup \
{ public: TEST_##testGroup##_##testName##_Test () : TEST_GROUP_##CppUTestGroup##testGroup () {} \
       void testBody(); } \
    TEST_##testGroup##_##testName##_Instance; \
  OrderedTestInstaller TEST_##testGroup##_##testName##_Installer(&TEST_##testGroup##_##testName##_Instance, #testGroup, #testName, __FILE__,__LINE__, testLevel); \
   void TEST_##testGroup##_##testName##_Test::testBody()

#endif

