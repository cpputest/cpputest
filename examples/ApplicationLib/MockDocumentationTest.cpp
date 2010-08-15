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
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(MockDocumentation)
{
};

void productionCode()
{
	mock().actualCall("productionCode");
}

TEST(MockDocumentation, SimpleScenario)
{
	mock().expectOneCall("productionCode");
	productionCode();
	mock().checkExpectations();
}

class ClassFromProductionCode
{
public:
	virtual void importantFunction(){}
};

class ClassFromProductionCodeMock : public ClassFromProductionCode
{
public:
	virtual void importantFunction()
	{
		mock().actualCall("importantFunction").onObject(this);
	}
};

TEST(MockDocumentation, SimpleScenarioObject)
{
	ClassFromProductionCode* object = new ClassFromProductionCodeMock; /* create mock instead of real thing */

	mock().expectOneCall("importantFunction").onObject(object);
	object->importantFunction();
	mock().checkExpectations();

	delete object;
}

void parameters_function(int p1, const char* p2)
{
	void* object = (void*) 1;
	mock().actualCall("function").onObject(object).withParameter("p1", p1).withParameter("p2", p2);
}

TEST(MockDocumentation, parameters)
{
	void* object = (void*) 1;
	mock().expectOneCall("function").onObject(object).withParameter("p1", 2).withParameter("p2", "hah");
	parameters_function(2, "hah");
}

TEST(MockDocumentation, returnValue)
{
	mock().expectOneCall("function").andReturnValue(10);
	int value = mock().actualCall("function").returnValue().getIntValue();
	value = mock().returnValue().getIntValue();
}

TEST(MockDocumentation, setData)
{
	ClassFromProductionCode object;
	mock().setData("importantValue", 10);
	mock().setDataObject("importantObject", "ClassFromProductionCode", &object);

	ClassFromProductionCode * pobject;
	int value = mock().getData("importantValue").getIntValue();
	pobject = (ClassFromProductionCode*) mock().getData("importantObject").getObjectPointer();

	LONGS_EQUAL(10, value);
	POINTERS_EQUAL(pobject, &object);
}
