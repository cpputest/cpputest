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

#include "CppUTest/CppUTestConfig.h"
#include "CppUTest/TestFilter.h"

TestFilter::TestFilter() : strictMatching_(false), invertMatching_(false), next_(NULL)
{
}

TestFilter::TestFilter(const SimpleString& filter) : strictMatching_(false), invertMatching_(false), next_(NULL)
{
    filter_ = filter;
}

TestFilter::TestFilter(const char* filter) : strictMatching_(false), invertMatching_(false), next_(NULL)
{
    filter_ = filter;
}

TestFilter* TestFilter::add(TestFilter* filter)
{
    next_ = filter;
    return this;
}

TestFilter* TestFilter::getNext() const
{
    return next_;
}

void TestFilter::strictMatching()
{
    strictMatching_ = true;
}

void TestFilter::invertMatching()
{
    invertMatching_ = true;
}

bool TestFilter::match(const SimpleString& name) const
{
    bool matches = false;

    if(strictMatching_)
        matches = name == filter_;
    else
        matches = name.contains(filter_);

    return invertMatching_ ? !matches : matches;
}

bool TestFilter::operator==(const TestFilter& filter) const
{
    return (filter_ == filter.filter_ &&
            strictMatching_ == filter.strictMatching_ &&
            invertMatching_ == filter.invertMatching_);
}

bool TestFilter::operator!=(const TestFilter& filter) const
{
    return !(filter == *this);
}

SimpleString TestFilter::asString() const
{
    SimpleString textFilter =  StringFromFormat("TestFilter: \"%s\"", filter_.asCharString());
    if (strictMatching_ && invertMatching_)
        textFilter += " with strict, invert matching";
    else if (strictMatching_)
        textFilter += " with strict matching";
    else if (invertMatching_)
        textFilter += " with invert matching";

    return textFilter;
}

SimpleString StringFrom(const TestFilter& filter)
{
    return filter.asString();
}

