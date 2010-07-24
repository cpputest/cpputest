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
#include "CppUTestExt/MemoryReporterPlugin.h"

MemoryReporterPlugin::MemoryReporterPlugin()
	: TestPlugin("MemoryReporterPlugin"), reportType_(MEMORY_REPORT_TYPE_OFF),
	  mallocAllocator(NULL), newAllocator(NULL), newArrayAllocator(NULL)
{
}

MemoryReporterPlugin::~MemoryReporterPlugin()
{
	removeGlobalMemoryReportAllocators();
}

void MemoryReporterPlugin::setReporting(MemoryReportType reportType)
{
	reportType_ = reportType;
}

MemoryReportType MemoryReporterPlugin::getReporting()
{
	return reportType_;
}

bool MemoryReporterPlugin::parseArguments(int /* ac */, const char** av, int index)
{
	SimpleString argument (av[index]);
	if (argument.contains("-pmemoryreport=")) {

		if (argument == ("-pmemoryreport=normal"))
			reportType_ = MEMORY_REPORT_TYPE_NORMAL;
		else if (argument == ("-pmemoryreport=code"))
			reportType_ = MEMORY_REPORT_TYPE_CODE;
		return true;
	}
	return false;
}

void MemoryReporterPlugin::setGlobalMemoryReportAllocators()
{
    mallocAllocator->setRealAllocator(MemoryLeakAllocator::getCurrentMallocAllocator());
	MemoryLeakAllocator::setCurrentMallocAllocator(mallocAllocator);

	newAllocator->setRealAllocator(MemoryLeakAllocator::getCurrentNewAllocator());
	MemoryLeakAllocator::setCurrentNewAllocator(newAllocator);

	newArrayAllocator->setRealAllocator(MemoryLeakAllocator::getCurrentNewArrayAllocator());
	MemoryLeakAllocator::setCurrentNewArrayAllocator(newArrayAllocator);
}

#include <stdio.h>

void MemoryReporterPlugin::removeGlobalMemoryReportAllocators()
{
	if (MemoryLeakAllocator::getCurrentNewAllocator() == newAllocator)
		MemoryLeakAllocator::setCurrentNewAllocator(newAllocator->getRealAllocator());

	if (MemoryLeakAllocator::getCurrentNewArrayAllocator() == newArrayAllocator)
		MemoryLeakAllocator::setCurrentNewArrayAllocator(newArrayAllocator->getRealAllocator());

	if (MemoryLeakAllocator::getCurrentMallocAllocator() == mallocAllocator)
		MemoryLeakAllocator::setCurrentMallocAllocator(mallocAllocator->getRealAllocator());

    delete mallocAllocator; mallocAllocator = NULL;
    delete newAllocator; newAllocator = NULL;
    delete newArrayAllocator; newArrayAllocator = NULL;
}


void MemoryReporterPlugin::createAllocator(MemoryReportAllocator** allocator, TestResult & result)
{
	if (reportType_ == MEMORY_REPORT_TYPE_CODE)
		*allocator = new CodeMemoryReportAllocator();
	else
		*allocator = new NormalMemoryReportAllocator();
    (*allocator)->setTestResult((&result));
}

void MemoryReporterPlugin::preTestAction(Utest& test, TestResult& result)
{
	if (reportType_ == MEMORY_REPORT_TYPE_OFF) return;

	createAllocator(&mallocAllocator, result);
	createAllocator(&newAllocator, result);
	createAllocator(&newArrayAllocator, result);

	setGlobalMemoryReportAllocators();
	result.print(StringFromFormat("TEST(%s, %s)\n", test.getGroup().asCharString(), test.getName().asCharString()).asCharString());
}

void MemoryReporterPlugin::postTestAction(Utest& test, TestResult& result)
{
	if (reportType_ == MEMORY_REPORT_TYPE_OFF) return;

	removeGlobalMemoryReportAllocators();
	result.print(StringFromFormat("ENDTEST(%s, %s)\n", test.getGroup().asCharString(), test.getName().asCharString()).asCharString());

}
