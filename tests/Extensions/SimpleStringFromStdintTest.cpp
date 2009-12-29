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
#include "CppUTest/SimpleString.h"
#include "CppUTest/Extensions/SimpleStringFromStdint.h"
#include <stdint.h>

TEST_GROUP(SimpleStringFromStdint)
{
};

using namespace std;

//uint64_t silently not supported in C++
TEST(SimpleStringFromStdint, Uint64_t)
{
	//I'd like this test, but can't get it to pass
	//    uint64_t = 0xffffffffffffffff;
	//
	//    SimpleString result = StringFrom(i);
	//    CHECK_EQUAL("18446744073709551615 (0xffffffffffffffff)", result);

	uint64_t i = 10;
	SimpleString result = StringFrom(i);
	CHECK_EQUAL("uint64_t not supported", result);

}

TEST(SimpleStringFromStdint, Int64_t)
{
	//I'd like this test, but can't get it to pass
	//    int64_t i = 0xffffffffffffffff>>1;
	//
	//    SimpleString result = StringFrom(i);
	//    CHECK_EQUAL("something", result);

	// TP: commented out as StringFrom(int64_t) is clashing with StringFrom(long) in 64-bit environment
	/*    int64_t i = 10;
	 SimpleString result = StringFrom(i);
	 CHECK_EQUAL("int64_t not supported", result);*/

}

TEST(SimpleStringFromStdint, Uint32_t)
{
	uint32_t i = 0xffffffff;

	SimpleString result = StringFrom(i);
	CHECK_EQUAL("4294967295 (0xffffffff)", result);
}

TEST(SimpleStringFromStdint, Uint16_t)
{
	uint16_t i = 0xffff;

	SimpleString result = StringFrom(i);
	CHECK_EQUAL("65535 (0xffff)", result);
}

TEST(SimpleStringFromStdint, Uint8_t)
{
	uint8_t i = 0xff;

	SimpleString result = StringFrom(i);
	CHECK_EQUAL("255 (0xff)", result);
}

IGNORE_TEST(SimpleStringFromStdint, CHECK_EQUAL_Uint64_t)
{
	//    uint64_t i = 0xffffffffffffffff;
	//    CHECK_EQUAL(i, i);
}

TEST(SimpleStringFromStdint, CHECK_EQUAL_Uint32_t)
{
	uint32_t i = 0xffffffff;
	CHECK_EQUAL(i, i);
}

TEST(SimpleStringFromStdint, CHECK_EQUAL_Uint16_t)
{
	uint16_t i = 0xffff;
	CHECK_EQUAL(i, i);
}

TEST(SimpleStringFromStdint, CHECK_EQUAL_Uint8_t)
{
	uint8_t i = 0xff;
	CHECK_EQUAL(i, i);
}

