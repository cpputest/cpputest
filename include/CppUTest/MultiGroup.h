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

#ifndef D_MultiGroup_h
#define D_MultiGroup_h

/*! \brief Define multi group for tests.
 *
 *--------------------------------- Purpose -------------------------------------
 *
 * Many times we have multiple initial conditions, but our test case remains the
 * same. Without built-in mulitgroups support, we have to define many test groups,
 * then rewrite test code for each test group. So, if we have 4 different initial
 * conditions, then 4 test groups, which means 4 identical test cases.
 * This makes it hard to debug and maintain these test cases.
 * This file extends CppUTest to include support of testing on multiple groups.
 * It defines macros which enables us to write test one time and have it run on
 * all test groups instead of repeating test code for all test groups. It makes it
 * easier to write and maintain these tests.
 *
 *--------------------------- DEPENDENCY COMMENTS -------------------------------
 *
 * This code only supports maximum of 10 test groups.
 *
 *-------------------------- PROJECT SPECIFIC DATA ------------------------------
 *
 * 1) First, we have to define TOTAL_TEST_GROUPS to indicate how many groups to create.
 *
 * 2) We have to declare test groups with a unique name using DEFINE_MULTI_GROUP
 *     macro, e-g to define 4 test groups, do this,
 *        #define TOTAL_TEST_GROUPS   4
 *        DEFINE_MULTI_GROUP(MyMultiTestGroupName)
 *     MyMultiTestGroupName can be anything which we want to use as multigroup label.
 *
 * 3) Then, we can use,
 *      TEST_GROUP_SETUP(MyMultiTestGroupName, GroupNumber)
 *      {
 *          test group specific setup code here
 *      }
 *    to define setup for each test group, where GroupNumber can vary from 1 to
 *    max test group number.
 *    e-g If we defined total of 4 test groups, GroupNumber can be 1 or 2 or 3 or 4
 *
 * 4) We can use,
 *      TEST_GROUP_TEARDOWN(MyMultiTestGroupName, GroupNumber)
 *      {
 *          test group specific teardown code here
 *      }
 *    to define teardown for each test group, where GroupNumber can vary from 1 to
 *    max test group number.
 *    e-g If we defined total of 4 test groups, GroupNumber can be 1 or 2 or 3 or 4
 *
 * 5) If all test groups have a common teardown, then it can be defined like this,
 *      TEST_GROUPS_TEARDOWN(MyMultiTestGroupName)
 *      {
 *          teardown code
 *      }
 *
 * 6) We can use,
 *      TEST_ON_ALL_GROUPS(MyMultiTestGroupName)
 *      {
 *          test code
 *      }
 *    to define a test case. This test will run on all groups.
 *
 * 7) Inside our test case, if we have something we want to do differently for
 *    a single group only, then we can check the value of TEST_GROUP_NUM variable,
 *    which will always have the value of current group on which test is running.
 *    e-g if we want to do something unique during our test for Group 1, then
 *      TEST_ON_ALL_GROUPS(MyMultiTestGroupName)
 *      {
 *          if( TEST_GROUP_NUM == 1 )
 *          {
 *              do something here which will only be executed for test group 1
 *          }
 *
 *          test code
 *      }
 *    TEST_GROUP_NUM value will vary from 1, 2, ... upto TOTAL_TEST_GROUPS value.
 *
 */

/* Recursion Macro */
#define PRIMITIVE_CAT(a, ...) a##__VA_ARGS__
#define CONCAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)

#define INC_9 9
#define INC_8 9
#define INC_7 8
#define INC_6 7
#define INC_5 6
#define INC_4 5
#define INC_3 4
#define INC_2 3
#define INC_1 2
#define INC_0 1
#define INC(x) PRIMITIVE_CAT(INC_, x)

#define DEC_9 8
#define DEC_8 7
#define DEC_7 6
#define DEC_6 5
#define DEC_5 4
#define DEC_4 3
#define DEC_3 2
#define DEC_2 1
#define DEC_1 0
#define DEC_0 0
#define DEC(x) PRIMITIVE_CAT(DEC_, x)

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define BITAND(x) PRIMITIVE_CAT(BITAND_, x)
#define BITAND_0(y) 0
#define BITAND_1(y) y

#define CHECK_MACRO_N(x, n, ...) n
#define CHECK_MACRO(...) CHECK_MACRO_N(__VA_ARGS__, 0, )
#define PROBE(x) x, 1,

#define NOT(x) CHECK_MACRO(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

#define BOOL(x) COMPL(NOT(x))
#define IF(c) IIF(BOOL(c))

#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()

#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) \
  IF(c)         \
  (EXPAND, EAT)

#define EVAL(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__

#define REPEAT(count, macro, ...)         \
  WHEN(count)                             \
  (                                       \
      OBSTRUCT(REPEAT_INDIRECT)()(        \
          DEC(count), macro, __VA_ARGS__) \
          OBSTRUCT(macro)(                \
              DEC(count), __VA_ARGS__))
#define REPEAT_INDIRECT() REPEAT

/* CppUTest MultiGroup Extension */
#define _TEST(a, b) TEST(a, b)

#define _TEST_ON_GROUP(gNum, gName, tName, _)   \
  _TEST(CONCAT(gName##GROUP, INC(gNum)), tName) \
  {                                             \
    TEST_GROUP_NUM = INC(gNum);                 \
    gName##tName();                             \
  }

#define TEST_ON_ALL_GROUPS(gName, tName)                           \
  void gName##tName();                                             \
  EVAL(REPEAT(TOTAL_TEST_GROUPS, _TEST_ON_GROUP, gName, tName, ~)) \
  void gName##tName()

#define _TEST_GROUP_TEARDOWN(gNum, gName, _)                                         \
  void CONCAT(TEST_GROUP_CppUTestGroup, CONCAT(gName##GROUP, INC(gNum)))::teardown() \
  {                                                                                  \
    gName##_teardown();                                                              \
  }

#define TEST_GROUPS_TEARDOWN(gName)                               \
  void gName##_teardown();                                        \
  EVAL(REPEAT(TOTAL_TEST_GROUPS, _TEST_GROUP_TEARDOWN, gName, ~)) \
  void gName##_teardown()

#define _TEST_GROUP_SETUP(gNum, gName, _)                                         \
  void CONCAT(TEST_GROUP_CppUTestGroup, CONCAT(gName##GROUP, INC(gNum)))::setup() \
  {                                                                               \
    gName##_setup();                                                              \
  }

#define TEST_GROUPS_SETUP(gName)                               \
  void gName##_setup();                                        \
  EVAL(REPEAT(TOTAL_TEST_GROUPS, _TEST_GROUP_SETUP, gName, ~)) \
  void gName##_setup()

#define TEST_GROUP_SETUP(gName, gNum) \
  void TEST_GROUP_CppUTestGroup##gName##GROUP##gNum::setup()

#define TEST_GROUP_TEARDOWN(gName, gNum) \
  void TEST_GROUP_CppUTestGroup##gName##GROUP##gNum::teardown()

#define _DECLARE_TEST_GROUP(gNum, gName, _)   \
  TEST_GROUP(CONCAT(gName##GROUP, INC(gNum))) \
  {                                           \
    void setup();                             \
    void teardown();                          \
  };

#define DEFINE_MULTI_GROUP(gName)  \
  unsigned int TEST_GROUP_NUM = 0; \
  EVAL(REPEAT(TOTAL_TEST_GROUPS, _DECLARE_TEST_GROUP, gName, ~))

#endif /*D_MultiGroup_h*/
