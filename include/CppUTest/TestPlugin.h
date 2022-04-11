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

#ifndef D_TestPlugin_h
#define D_TestPlugin_h

class UtestShell;
class TestResult;

class TestPlugin
{
public:

    TestPlugin(const SimpleString& name);
    virtual ~TestPlugin();

    virtual void preTestAction(UtestShell&, TestResult&)
    {
    }

    virtual void postTestAction(UtestShell&, TestResult&)
    {
    }

    virtual bool parseArguments(int /* ac */, const char *const * /* av */, int /* index */ )
    {
        return false;
    }

    virtual void runAllPreTestAction(UtestShell&, TestResult&);
    virtual void runAllPostTestAction(UtestShell&, TestResult&);
    virtual bool parseAllArguments(int ac, const char *const *av, int index);
    virtual bool parseAllArguments(int ac, char** av, int index);

    virtual TestPlugin* addPlugin(TestPlugin*);
    virtual TestPlugin* removePluginByName(const SimpleString& name);
    virtual TestPlugin* getNext();

    virtual void disable();
    virtual void enable();
    virtual bool isEnabled();

    const SimpleString& getName();
    TestPlugin* getPluginByName(const SimpleString& name);

protected:
    TestPlugin(TestPlugin* next_);

private:
    TestPlugin* next_;
    SimpleString name_;
    bool enabled_;
};

///////////////////////////////////////////////////////////////////////////////
//
// SetPointerPlugin
//
// This is a very small plugin_ that resets pointers to their original value.
//
///////////////////////////////////////////////////////////////////////////////

extern void CppUTestStore(void **location);

class SetPointerPlugin: public TestPlugin
{
public:
    SetPointerPlugin(const SimpleString& name);
    virtual void postTestAction(UtestShell&, TestResult&) _override;

    enum
    {
        MAX_SET = 32
    };
};

#define UT_PTR_SET(a, b) do { CppUTestStore( (void**)&a ); a = b; } while(0)

///////////// Null Plugin

class NullTestPlugin: public TestPlugin
{
public:

    NullTestPlugin();

    virtual void runAllPreTestAction(UtestShell& test, TestResult& result) _override;
    virtual void runAllPostTestAction(UtestShell& test, TestResult& result) _override;

    static NullTestPlugin* instance();
};

#endif
