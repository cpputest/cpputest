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

#ifndef D_GenericTest_H
#define D_GenericTest_H

class GenericTest : public Utest
  {
  public:
  	void (*setup_)();
  	void (*teardown_)();
  	void (*_testFunction)();
    GenericTest(void (*setup)() = 0, void (*teardown)() = 0)
        :Utest("Generic", "Generic", "Generic", 1), setup_(setup), teardown_(teardown), _testFunction(0)
    {
    }
    void testBody()
    {
    	if (_testFunction) _testFunction();
    }
    virtual void setup()
    {
    	if (setup_) setup_();
    }
    virtual void teardown()
    {
    	if (teardown_) teardown_();
    }
};

class GenericTestFixture
{
public:

	GenericTestFixture() {
		output = new MockTestOutput();
    	result = new TestResult(*output);
  		genTest = new GenericTest();
  		registry = new TestRegistry();

  		registry->setCurrentRegistry(registry);
		registry->addTest(genTest);
	};
	
	virtual ~GenericTestFixture() {
		registry->setCurrentRegistry(0);
  		delete registry;
  		delete result;
    	delete output;
  		delete genTest;
	}

	void setTestFunction(void (*testFunction)()) {
		genTest->_testFunction = testFunction;
	}

	void setSetup(void (*setupFunction)()) {
		genTest->setup_ = setupFunction;
	}
	
	void runAllTests () {
		registry->runAllTests(*result);
	}
	
	int getFailureCount () {
		return result->getFailureCount();
	}

 	void assertPrintContains(const SimpleString& contains)
 	{
 		assertPrintContains(output, contains);
 	}

 	static void assertPrintContains(MockTestOutput* output, const SimpleString& contains)
 	{
		if (output->getOutput().contains(contains))
			return;
		SimpleString message("\tActual <");
		message += output->getOutput().asCharString();
		message += ">\n";
		message += "\tdid not contain <";
		message += contains.asCharString();
		message += ">\n";
		FAIL(message.asCharString());

 	}

	TestRegistry* 	registry;
	GenericTest* 	genTest;
	MockTestOutput* output;
	TestResult *	result;
};

#endif
