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
// TESTPlugin.H
//
// This file contains the ability to plugin general checks to all tests.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_TestPlugin_h
#define D_TestPlugin_h

class Utest;
class TestResult;

class TestPlugin
  {
  public:

    TestPlugin();
    virtual ~TestPlugin();

		virtual void preTestAction(Utest&, TestResult&) {};
		virtual void postTestAction(Utest&, TestResult&) {};

		virtual void runAllPreTestAction(Utest&, TestResult&);
		virtual void runAllPostTestAction(Utest&, TestResult&);
		
		virtual TestPlugin* addPlugin(TestPlugin*);
        
	protected:
        TestPlugin(TestPlugin* next);

	private:
		TestPlugin* next_;
};

///////////////////////////////////////////////////////////////////////////////
//
// FunctionPointerPlugin
//
// This is a very small plugin that resets function pointers to their original value.
//
///////////////////////////////////////////////////////////////////////////////

extern void CppUTestStoreFp(void (**location)(), void(*value)());

class FunctionPointerPlugin : public TestPlugin
{
public:
	FunctionPointerPlugin();
	virtual ~FunctionPointerPlugin();
	virtual void postTestAction(Utest&, TestResult&);

	enum {MAX_FPS = 1024};
};

#define UT_FPSET(a, b) { CppUTestStoreFp( (void(**)()) &a, (void (*)()) a); a = b; }

///////////// Null Plugin

class NullTestPlugin : public TestPlugin
  {
  public:

    NullTestPlugin();
    virtual ~NullTestPlugin() {};

		virtual void runAllPreTestAction(Utest& test, TestResult& result);
		virtual void runAllPostTestAction(Utest& test, TestResult& result);
    
    static NullTestPlugin* instance();
};

#endif
