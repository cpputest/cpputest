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
#include "MockFailureReporterForTest.h"

TEST_GROUP(MockComparatorCopierTest)
{
  void teardown()
  {
    mock().checkExpectations();
  }
};

class MyTypeForTesting
{
public:
    MyTypeForTesting(long val)
    {
        value = new long(val);
    }
    virtual ~MyTypeForTesting()
    {
        delete value;
    }
    long *value;
};

class MyTypeForTestingComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
        const MyTypeForTesting* obj1 = (const MyTypeForTesting*) object1;
        const MyTypeForTesting* obj2 = (const MyTypeForTesting*) object2;
        return *(obj1->value) == *(obj2->value);
    }
    virtual SimpleString valueToString(const void* object)
    {
        const MyTypeForTesting* obj = (const MyTypeForTesting*) object;
        return StringFrom(*(obj->value));
    }
};

class MyTypeForTestingCopier : public MockNamedValueCopier
{
public:
    virtual void copy(void* dst_, const void* src_)
    {
        MyTypeForTesting* dst = (MyTypeForTesting*) dst_;
        const MyTypeForTesting* src = (const MyTypeForTesting*) src_;
        *(dst->value) = *(src->value);
    }
};

TEST(MockComparatorCopierTest, customObjectParameterFailsWhenNotHavingAComparisonRepository)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting object(1);
    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    MockNoWayToCompareCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopierTest, customObjectParameterFailsWhenNotHavingACopierRepository)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting object(1);
    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &object);

    MockNoWayToCopyCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopierTest, customObjectParameterSucceeds)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;
    mock().installComparator("MyTypeForTesting", comparator);

    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();

    mock().removeAllComparatorsAndCopiers();
}

static bool myTypeIsEqual(const void* object1, const void* object2)
{
    return ((MyTypeForTesting*)object1)->value == ((MyTypeForTesting*)object2)->value;
}

static SimpleString myTypeValueToString(const void* object)
{
    return StringFrom(((MyTypeForTesting*)object)->value);
}

TEST(MockComparatorCopierTest, customObjectWithFunctionComparator)
{
    MyTypeForTesting object(1);
    MockFunctionComparator comparator(myTypeIsEqual, myTypeValueToString);
    mock().installComparator("MyTypeForTesting", comparator);

    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customObjectWithFunctionComparatorThatFailsCoversValueToString)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting object(5);
    MockFunctionComparator comparator(myTypeIsEqual, myTypeValueToString);
    mock().installComparator("MyTypeForTesting", comparator);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("function")->withParameterOfType("MyTypeForTesting", "parameterName", &object);
    MockExpectedCallsDidntHappenFailure failure(UtestShell::getCurrent(), expectations);

    mock().expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE_LOCATION(failure, __FILE__, __LINE__);
}

TEST(MockComparatorCopierTest, customTypeOutputParameterSucceeds)
{
    MyTypeForTesting expectedObject(55);
    MyTypeForTesting actualObject(99);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject);

    mock().checkExpectations();
    CHECK_EQUAL(55, *(expectedObject.value));
    CHECK_EQUAL(55, *(actualObject.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, noActualCallForCustomTypeOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting expectedObject(1);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    MockExpectedCallsDidntHappenFailure expectedFailure(mockFailureTest(), expectations);

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, unexpectedCustomTypeOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting actualObject(8834);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo");
    MockNamedValue parameter("parameterName");
    parameter.setObjectPointer("MyTypeForTesting", &actualObject);
    MockUnexpectedOutputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo");
    mock().actualCall("foo").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject);
    mock().checkExpectations();

    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputParameterMissing)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting expectedObject(123464);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    MockExpectedParameterDidntHappenFailure expectedFailure(mockFailureTest(), "foo", expectations);

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    mock().actualCall("foo");

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputParameterOfWrongType)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting expectedObject(123464);
    MyTypeForTesting actualObject(75646);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    MockNamedValue parameter("output");
    parameter.setObjectPointer("OtherTypeForTesting", &actualObject);
    MockUnexpectedOutputParameterFailure expectedFailure(mockFailureTest(), "foo", parameter, expectations);

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    mock().actualCall("foo").withOutputParameterOfType("OtherTypeForTesting", "output", &actualObject);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, noCopierForCustomTypeOutputParameter)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting expectedObject(123464);
    MyTypeForTesting actualObject(8834);

    MockExpectedCallsListForTest expectations;
    expectations.addFunction("foo")->withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    MockNoWayToCopyCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "output", &expectedObject);
    mock().actualCall("foo").withOutputParameterOfType("MyTypeForTesting", "output", &actualObject);

    mock().checkExpectations();
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopierTest, twoCustomTypeOutputParameters)
{
    MyTypeForTesting expectedObject1(545);
    MyTypeForTesting actualObject1(979);
    MyTypeForTesting expectedObject2(123);
    MyTypeForTesting actualObject2(4567);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject1).withParameter("id", 1);
    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject2).withParameter("id", 2);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject1).withParameter("id", 1);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject2).withParameter("id", 2);

    mock().checkExpectations();
    CHECK_EQUAL(545, *(expectedObject1.value));
    CHECK_EQUAL(545, *(actualObject1.value));
    CHECK_EQUAL(123, *(expectedObject2.value));
    CHECK_EQUAL(123, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, twoInterleavedCustomTypeOutputParameters)
{
    MyTypeForTesting expectedObject1(9545);
    MyTypeForTesting actualObject1(79);
    MyTypeForTesting expectedObject2(132);
    MyTypeForTesting actualObject2(743);
    MyTypeForTestingCopier copier;

    mock().installCopier("MyTypeForTesting", copier);
    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject1).withParameter("id", 1);
    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject2).withParameter("id", 2);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject2).withParameter("id", 2);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject1).withParameter("id", 1);

    mock().checkExpectations();
    CHECK_EQUAL(9545, *(expectedObject1.value));
    CHECK_EQUAL(9545, *(actualObject1.value));
    CHECK_EQUAL(132, *(expectedObject2.value));
    CHECK_EQUAL(132, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, twoDifferentCustomTypeOutputParametersInSameFunctionCallSucceeds)
{
    MyTypeForTesting expectedObject1(11);
    MyTypeForTesting actualObject1(22);
    MyTypeForTesting expectedObject2(33);
    MyTypeForTesting actualObject2(44);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    mock().expectOneCall("foo")
        .withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &expectedObject1)
        .withOutputParameterOfTypeReturning("MyTypeForTesting", "foobar", &expectedObject2);
    mock().actualCall("foo")
        .withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject1)
        .withOutputParameterOfType("MyTypeForTesting", "foobar", &actualObject2);

    mock().checkExpectations();
    CHECK_EQUAL(11, *(expectedObject1.value));
    CHECK_EQUAL(11, *(actualObject1.value));
    CHECK_EQUAL(33, *(expectedObject2.value));
    CHECK_EQUAL(33, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputAndInputParametersOfSameNameInDifferentFunctionCallsOfSameFunctionSucceeds)
{
    MyTypeForTesting expectedObject1(911);
    MyTypeForTesting actualObject1(6576878);
    MyTypeForTesting expectedObject2(123);
    MyTypeForTesting actualObject2(123);
    MyTypeForTestingCopier copier;
    MyTypeForTestingComparator comparator;
    mock().installCopier("MyTypeForTesting", copier);
    mock().installComparator("MyTypeForTesting", comparator);

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &expectedObject1);
    mock().expectOneCall("foo").withParameterOfType("MyTypeForTesting", "bar", &expectedObject2);
    mock().actualCall("foo").withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject1);
    mock().actualCall("foo").withParameterOfType("MyTypeForTesting", "bar", &actualObject2);

    mock().checkExpectations();
    CHECK_EQUAL(911, *(expectedObject1.value));
    CHECK_EQUAL(911, *(actualObject1.value));
    CHECK_EQUAL(123, *(expectedObject2.value));
    CHECK_EQUAL(123, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, twoCustomTypeOutputParametersOfSameNameInDifferentFunctionsSucceeds)
{
    MyTypeForTesting expectedObject1(657);
    MyTypeForTesting actualObject1(984465);
    MyTypeForTesting expectedObject2(987);
    MyTypeForTesting actualObject2(987);
    MyTypeForTestingCopier copier;
    MyTypeForTestingComparator comparator;
    mock().installCopier("MyTypeForTesting", copier);
    mock().installComparator("MyTypeForTesting", comparator);

    mock().expectOneCall("foo1").withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &expectedObject1);
    mock().expectOneCall("foo2").withParameterOfType("MyTypeForTesting", "bar", &expectedObject2);
    mock().actualCall("foo1").withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject1);
    mock().actualCall("foo2").withParameterOfType("MyTypeForTesting", "bar", &actualObject2);

    mock().checkExpectations();
    CHECK_EQUAL(657, *(expectedObject1.value));
    CHECK_EQUAL(657, *(actualObject1.value));
    CHECK_EQUAL(987, *(expectedObject2.value));
    CHECK_EQUAL(987, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputAndInputParameterOfSameTypeInSameFunctionCall)
{
    MyTypeForTesting expectedObject1(45);
    MyTypeForTesting actualObject1(45);
    MyTypeForTesting expectedObject2(987765443);
    MyTypeForTesting actualObject2(0);
    MyTypeForTestingCopier copier;
    MyTypeForTestingComparator comparator;
    mock().installCopier("MyTypeForTesting", copier);
    mock().installComparator("MyTypeForTesting", comparator);

    mock().expectOneCall("foo")
          .withParameterOfType("MyTypeForTesting", "bar", &expectedObject1)
          .withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &expectedObject2);
    mock().actualCall("foo")
          .withParameterOfType("MyTypeForTesting", "bar", &actualObject1)
          .withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject2);

    mock().checkExpectations();
    CHECK_EQUAL(45, *(expectedObject1.value));
    CHECK_EQUAL(45, *(actualObject1.value));
    CHECK_EQUAL(987765443, *(expectedObject2.value));
    CHECK_EQUAL(987765443, *(actualObject2.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputParameterTraced)
{
    MyTypeForTesting actualObject(676789);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);
    mock().tracing(true);

    mock().actualCall("someFunc").withOutputParameterOfType("MyTypeForTesting", "someParameter", &actualObject);

    mock().checkExpectations();
    STRCMP_CONTAINS("Function name:someFunc MyTypeForTesting someParameter:", mock().getTraceOutput());

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, customTypeOutputParameterWithIgnoredParameters)
{
    MyTypeForTesting expectedObject(444537909);
    MyTypeForTesting actualObject(98765);
    MyTypeForTestingCopier copier;
    mock().installCopier("MyTypeForTesting", copier);

    mock().expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &expectedObject).ignoreOtherParameters();
    mock().actualCall("foo").withOutputParameterOfType("MyTypeForTesting", "bar", &actualObject).withParameter("other", 1);

    mock().checkExpectations();
    CHECK_EQUAL(444537909, *(expectedObject.value));
    CHECK_EQUAL(444537909, *(actualObject.value));

    mock().removeAllComparatorsAndCopiers();
}

static void myTypeCopy(void* dst_, const void* src_)
{
    MyTypeForTesting* dst = (MyTypeForTesting*) dst_;
    const MyTypeForTesting* src = (const MyTypeForTesting*) src_;
    *(dst->value) = *(src->value);
}

TEST(MockComparatorCopierTest, customObjectWithFunctionCopier)
{
    MyTypeForTesting expectedObject(9874452);
    MyTypeForTesting actualObject(2034);
    MockFunctionCopier copier(myTypeCopy);
    mock().installCopier("MyTypeForTesting", copier);

    mock().expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &expectedObject);
    mock().actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &actualObject);

    mock().checkExpectations();
    CHECK_EQUAL(9874452, *(expectedObject.value));
    CHECK_EQUAL(9874452, *(actualObject.value));

    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, removingComparatorsWorksHierachically)
{
    MockFailureReporterInstaller failureReporterInstaller;

    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;

    mock("scope").installComparator("MyTypeForTesting", comparator);
    mock().removeAllComparatorsAndCopiers();
    mock("scope").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("scope").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    MockNoWayToCompareCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopierTest, removingCopiersWorksHierachically)
{
    MockFailureReporterInstaller failureReporterInstaller;
    MyTypeForTesting object(1);

    MyTypeForTestingCopier copier;

    mock("scope").installCopier("MyTypeForTesting", copier);
    mock().removeAllComparatorsAndCopiers();
    mock("scope").expectOneCall("foo").withOutputParameterOfTypeReturning("MyTypeForTesting", "bar", &object);
    mock("scope").actualCall("foo").withOutputParameterOfType("MyTypeForTesting", "bar", &object);

    MockNoWayToCopyCustomTypeFailure expectedFailure(mockFailureTest(), "MyTypeForTesting");
    CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockComparatorCopierTest, installComparatorWorksHierarchicalOnBothExistingAndDynamicallyCreatedMockSupports)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;

    mock("existing");
    mock().installComparator("MyTypeForTesting", comparator);
    mock("existing").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("existing").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("dynamic").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("dynamic").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();
    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, installComparatorsWorksHierarchical)
{
    MyTypeForTesting object(1);
    MyTypeForTestingComparator comparator;
    MockNamedValueComparatorsAndCopiersRepository repos;
    repos.installComparator("MyTypeForTesting", comparator);

    mock("existing");
    mock().installComparatorsAndCopiers(repos);
    mock("existing").expectOneCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);
    mock("existing").actualCall("function").withParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();
    mock().removeAllComparatorsAndCopiers();
}

TEST(MockComparatorCopierTest, installCopiersWorksHierarchically)
{
    MyTypeForTesting object(1);
    MyTypeForTestingCopier copier;

    mock("existing");
    mock().installCopier("MyTypeForTesting", copier);
    mock("existing").expectOneCall("function").withOutputParameterOfTypeReturning("MyTypeForTesting", "parameterName", &object);
    mock("existing").actualCall("function").withOutputParameterOfType("MyTypeForTesting", "parameterName", &object);

    mock().checkExpectations();
    mock().removeAllComparatorsAndCopiers();
}

class StubComparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void*, const void*)
    {
        return true;
    }
    virtual SimpleString valueToString(const void*)
    {
        return "";
    }
};

struct SomeClass
{
    int someDummy_;
};

static void functionWithConstParam(const SomeClass param)
{
    mock().actualCall("functionWithConstParam").withParameterOfType("SomeClass", "param", &param);
}

TEST(MockComparatorCopierTest, shouldSupportConstParameters)
{
    StubComparator comparator;
    mock().installComparator("SomeClass", comparator);

    SomeClass param;
    mock().expectOneCall("functionWithConstParam").withParameterOfType("SomeClass", "param", &param);
    functionWithConstParam(param);

    mock().checkExpectations();
}

