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
#include "CppUTest/TestOutput.h"
#include "CppUTestExt/MemoryReportAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

MemoryReportAllocator::MemoryReportAllocator() : result_(NULL), realAllocator_(NULL)
{

}

MemoryReportAllocator::~MemoryReportAllocator()
{
}

const char* MemoryReportAllocator::name()
{
	return realAllocator_->name();
}

const char* MemoryReportAllocator::alloc_name()
{
	return realAllocator_->alloc_name();
}

const char* MemoryReportAllocator::free_name()
{
	return realAllocator_->free_name();
}

void MemoryReportAllocator::setRealAllocator(MemoryLeakAllocator* allocator)
{
	realAllocator_ = allocator;
}

MemoryLeakAllocator* MemoryReportAllocator::getRealAllocator()
{
	return realAllocator_;
}

void MemoryReportAllocator::setTestResult(TestResult* result)
{
	result_ = result;
}

char* MemoryReportAllocator::alloc_memory(size_t size, const char* file, int line)
{
	char* memory = realAllocator_->alloc_memory(size, file, line);
	if (result_)
		report_alloc_memory(size, memory, file, line);
	return memory;
}

void MemoryReportAllocator::free_memory(char* memory, const char* file, int line)
{
	realAllocator_->free_memory(memory, file, line);
	if (result_)
		report_free_memory(memory, file, line);
}

NormalMemoryReportAllocator::NormalMemoryReportAllocator()
{
}

NormalMemoryReportAllocator::~NormalMemoryReportAllocator()
{
}

void NormalMemoryReportAllocator::report_alloc_memory(size_t size, char* memory, const char* file, int line)
{
	result_->print(StringFromFormat("Allocation using %s of size: %d pointer: %p at %s:%d\n", alloc_name(), size, memory, file, line).asCharString());
}

void NormalMemoryReportAllocator::report_free_memory(char* memory, const char* file, int line)
{
	result_->print(StringFromFormat("Deallocation using %s of pointer: %p at %s:%d\n", free_name(),  memory, file, line).asCharString());
}

#define MAX_VARIABLE_NAME_LINE_PART 10
#define MAX_VARIABLE_NAME_FILE_PART 53
#define MAX_VARIABLE_NAME_SEPERATOR_PART 1
#define MAX_VARIABLE_NAME_LENGTH MAX_VARIABLE_NAME_FILE_PART + MAX_VARIABLE_NAME_SEPERATOR_PART + MAX_VARIABLE_NAME_LINE_PART

struct CodeReportingAllocationNode
{
	char variableName_[MAX_VARIABLE_NAME_LENGTH + 1];
	void* memory_;
	CodeReportingAllocationNode* next_;
};

CodeMemoryReportAllocator::CodeMemoryReportAllocator()
	: codeReportingList_(NULL)
{
}

CodeMemoryReportAllocator::~CodeMemoryReportAllocator()
{
	while (codeReportingList_) {
		CodeReportingAllocationNode* oldNode = codeReportingList_;
		codeReportingList_ = codeReportingList_->next_;
		realAllocator_->free_memory((char*) oldNode, __FILE__, __LINE__);
	}
}


void CodeMemoryReportAllocator::addNodeToList(const char* variableName, void* memory, CodeReportingAllocationNode* next)
{
	CodeReportingAllocationNode* newNode = (CodeReportingAllocationNode*) realAllocator_->alloc_memory(sizeof(CodeReportingAllocationNode), __FILE__, __LINE__);
	newNode->memory_ = memory;
	newNode->next_ = next;
	PlatformSpecificStrNCpy(newNode->variableName_, variableName, MAX_VARIABLE_NAME_LENGTH);
	codeReportingList_ = newNode;
}

CodeReportingAllocationNode* CodeMemoryReportAllocator::findNodeAndRemoveIt(void* memory)
{
	CodeReportingAllocationNode* current = codeReportingList_;
	CodeReportingAllocationNode* previous = NULL;
	while (current->memory_ != memory) {
		previous = current;
		current = current->next_;
	}
	if (previous)
		previous->next_ = current->next_;
	else
		codeReportingList_ = current->next_;
	return current;
}

void CodeMemoryReportAllocator::report_alloc_memory(size_t size, char* memory, const char* file, int line)
{
	SimpleString variableName = StringFromFormat("%s_%d", file, line);

	if (PlatformSpecificStrCmp(realAllocator_->alloc_name(), "new") == 0)
		result_->print(StringFromFormat("\tvoid* %s = new char[%d];\n", variableName.asCharString(), size).asCharString());
	else
		result_->print(StringFromFormat("\tvoid* %s = malloc(%d);\n", variableName.asCharString(), size).asCharString());

	addNodeToList(variableName.asCharString(), memory, codeReportingList_);
}

void CodeMemoryReportAllocator::report_free_memory(char* memory, const char* file, int line)
{
	CodeReportingAllocationNode* node = findNodeAndRemoveIt(memory);

	SimpleString variableName = node->variableName_;
	result_->print(StringFromFormat("\tfree(%s) /* at: %s:%d */\n", variableName.asCharString(), file, line).asCharString());
}
