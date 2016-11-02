/*
 * Copyright (c) 2015, Stephan Veigl
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


// system under test interface
class SUT
{
public:
	const char* name() { return "SystemUnderTest"; }
};

MIXIN_PARAMS(DemoMixInGroup)
{
	SUT* obj;
	char const* expectedName;
	bool initializedByMixInSetup;
};

MIXIN_GROUP(DemoMixInGroup) {
	void setup()
	{
		params.initializedByMixInSetup = true;
		init = true;
	}

	bool init;
};
TEST_GROUP(MixInTestGroup) {};

MIXIN_APPLY(MixInTestGroup, DemoMixInGroup, ImplA_test)
{
	SUT sut;
	params.obj = &sut;
	params.expectedName = "SystemUnderTest";
	params.initializedByMixInSetup = false;
}

MIXIN_TEST(DemoMixInGroup, paramsSet)
{
	CHECK(params.obj);
	CHECK(params.expectedName);
}

MIXIN_TEST(DemoMixInGroup, paramsObjectSubFunction)
{
	STRCMP_EQUAL( params.expectedName, params.obj->name() );
}

MIXIN_TEST(DemoMixInGroup, mixInSetupWasCalled)
{
	CHECK(init);
	CHECK(params.initializedByMixInSetup);
}

