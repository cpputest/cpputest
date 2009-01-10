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

#ifndef D_TestTestingFixture_H
#define D_TestTestingFixture_H

class TestTestingFixture
{
public:

	TestTestingFixture() {
		output = new StringBufferTestOutput();
    	result = new TestResult(*output);
  		genTest = new ExecFunctionTest();
  		registry = new TestRegistry();

  		registry->setCurrentRegistry(registry);
		registry->addTest(genTest);
	};

	virtual ~TestTestingFixture() {
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

   void setTeardown(void (*teardownFunction)()) {
      genTest->teardown_ = teardownFunction;
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

 	static void assertPrintContains(StringBufferTestOutput* output, const SimpleString& contains)
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
	ExecFunctionTest* 	genTest;
	StringBufferTestOutput* output;
	TestResult *	result;
};

#endif
