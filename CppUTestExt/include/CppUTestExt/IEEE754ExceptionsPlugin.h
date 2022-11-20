/*
 * Copyright (c) 2015, Michael Feathers, James Grenning, Bas Vodde
 * and Arnd Strube. All rights reserved.
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

#ifndef D_IEEE754ExceptionsPlugin_h
#define D_IEEE754ExceptionsPlugin_h

#include "CppUTest/TestPlugin.h"

class IEEE754ExceptionsPlugin: public TestPlugin
{
public:
    IEEE754ExceptionsPlugin(const SimpleString& name = "IEEE754ExceptionsPlugin");

    virtual void preTestAction(UtestShell& test, TestResult& result) _override;
    virtual void postTestAction(UtestShell& test, TestResult& result) _override;

    static void disableInexact(void);
    static void enableInexact(void);
    static bool checkIeee754OverflowExceptionFlag();
    static bool checkIeee754UnderflowExceptionFlag();
    static bool checkIeee754InexactExceptionFlag();
    static bool checkIeee754DivByZeroExceptionFlag();

private:
    void ieee754Check(UtestShell& test, TestResult& result, int flag, const char* text);
    static bool inexactDisabled_;
};

#endif
