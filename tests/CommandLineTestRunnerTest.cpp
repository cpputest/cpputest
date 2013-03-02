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
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/TestPlugin.h"

class DummyPluginWhichCountsThePlugins : public TestPlugin
{
public:

	bool returnValue;
	int amountOfPlugins;

	DummyPluginWhichCountsThePlugins(const SimpleString& name, TestRegistry* registry) :
		TestPlugin(name), returnValue(true), amountOfPlugins(0), registry_(registry)
	{
	}

	virtual bool parseArguments(int, const char**, int)
	{
		/* Remove ourselves from the count */
		amountOfPlugins = registry_->countPlugins() - 1;
		return returnValue;
	}
private:
	TestRegistry* registry_;

};


TEST_GROUP(CommandLineTestRunner)
{
	TestRegistry registry;
	StringBufferTestOutput output;
	DummyPluginWhichCountsThePlugins* pluginCountingPlugin;

	void setup()
	{
		pluginCountingPlugin = new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
	}
	void teardown()
	{
		delete pluginCountingPlugin;
	}
};

TEST(CommandLineTestRunner, OnePluginGetsInstalledDuringTheRunningTheTests)
{
	const char* argv[] = { "tests.exe", "-psomething"};

	registry.installPlugin(pluginCountingPlugin);

	CommandLineTestRunner commandLineTestRunner(2, argv, &output, &registry);
	commandLineTestRunner.runAllTestsMain();
	registry.removePluginByName("PluginCountingPlugin");

	LONGS_EQUAL(0, registry.countPlugins());
	LONGS_EQUAL(1, pluginCountingPlugin->amountOfPlugins);
}

TEST(CommandLineTestRunner, NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
	const char* argv[] = { "tests.exe", "-fdskjnfkds"};

	CommandLineTestRunner commandLineTestRunner(2, argv, &output, &registry);
	commandLineTestRunner.runAllTestsMain();

	LONGS_EQUAL(0, registry.countPlugins());
}
