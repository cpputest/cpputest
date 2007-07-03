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
// TESTHARNESS_c.H
//
//
///////////////////////////////////////////////////////////////////////////////
#include "TestHarness.h"

extern "C" {
	
	#include "stdlib.h"
	
	void  CHECK_EQUAL_C_INT(int expected, int actual)
	{
			CHECK_EQUAL((long)expected, (long)actual);
	}
	
	void  CHECK_EQUAL_C_REAL(double expected, double actual)
	{
		CHECK_EQUAL(expected, actual);
	}

	void  CHECK_EQUAL_C_CHAR(char expected, char actual)
	{
		CHECK_EQUAL((long)expected, (long)actual);
	}

	void  CHECK_EQUAL_C_STRING(char* expected, char* actual)
	{
				STRCMP_EQUAL(expected, actual);
	}

	void  FAIL_TEXT_C(char* text)
	{
			FAIL(text);
	}

	void  FAIL_C ()
	{
		FAIL("");
	}
	
	char* cpputest_malloc(size_t size)
	{
		return new char[size];
	}
	
	void cpputest_free(char* buffer)
	{
		delete[] buffer;
	}
}
