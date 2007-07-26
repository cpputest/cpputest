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

#include "Platform.h"
#include "TestHarness.h"
#include "SimpleString.h"

TEST_GROUP(SimpleString)
{
};


TEST(SimpleString, Create)
{
  SimpleString s("hello");
}

TEST(SimpleString, Copy)
{
  SimpleString s1("hello");
  SimpleString s2(s1);

  CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Assignment)
{
  SimpleString s1("hello");
  SimpleString s2("goodbye");

  s2 = s1;

  CHECK_EQUAL(s1, s2);
}

TEST(SimpleString, Equality)
{
  SimpleString s1("hello");
  SimpleString s2("hello");

  CHECK(s1 == s2);
}

TEST(SimpleString, InEquality)
{
  SimpleString s1("hello");
  SimpleString s2("goodbye");

  CHECK(s1 != s2);
}

TEST(SimpleString, asCharString)
{
  SimpleString s1("hello");

  STRCMP_EQUAL("hello", s1.asCharString());
}

TEST(SimpleString, Size)
{
  SimpleString s1("hello!");

  LONGS_EQUAL(6, s1.size());
}

TEST(SimpleString, Addition)
{
  SimpleString s1("hello!");
  SimpleString s2("goodbye!");
  SimpleString s3("hello!goodbye!");
  SimpleString s4;
  s4 = s1 + s2;

  CHECK_EQUAL(s3, s4);
}

TEST(SimpleString, Concatenation)
{
  SimpleString s1("hello!");
  SimpleString s2("goodbye!");
  SimpleString s3("hello!goodbye!");
  SimpleString s4;
  s4 += s1;
  s4 += s2;

  CHECK_EQUAL(s3, s4);

  SimpleString s5("hello!goodbye!hello!goodbye!");
  s4 += s4;

  CHECK_EQUAL(s5, s4);
}


TEST(SimpleString, Contains)
{
  SimpleString s("hello!");
  SimpleString empty("");
  SimpleString beginning("hello");
  SimpleString end("lo!");
  SimpleString mid("l");
  SimpleString notPartOfString("xxxx");

  CHECK(s.contains(empty));
  CHECK(s.contains(beginning));
  CHECK(s.contains(end));
  CHECK(s.contains(mid));
  CHECK(!s.contains(notPartOfString));

  CHECK(empty.contains(empty));
  CHECK(!empty.contains(s));
}

TEST(SimpleString, ContainsNull)
{
	SimpleString s(0);
	CHECK(!s.contains("something"));
}

TEST(SimpleString, Characters)
{
  SimpleString s(StringFrom('a'));
  SimpleString s2(StringFrom('a'));
  CHECK(s == s2);
}
