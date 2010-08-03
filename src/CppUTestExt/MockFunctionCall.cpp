
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
#include "CppUTestExt/MockFunctionCall.h"

struct MockParameterComparatorRepositoryNode
{
	MockParameterComparatorRepositoryNode(const SimpleString& name, MockParameterComparator& comparator, MockParameterComparatorRepositoryNode* next)
		: name_(name), comparator_(comparator), next_(next) {};
	SimpleString name_;
	MockParameterComparator& comparator_;
	MockParameterComparatorRepositoryNode* next_;
};

MockParameterComparatorRepository::MockParameterComparatorRepository() : head_(NULL)
{

}

MockParameterComparatorRepository::~MockParameterComparatorRepository()
{
	clear();
}

void MockParameterComparatorRepository::clear()
{
	while (head_) {
		MockParameterComparatorRepositoryNode* next = head_->next_;
		delete head_;
		head_ = next;
	}
}

void MockParameterComparatorRepository::installComparator(const SimpleString& name, MockParameterComparator& comparator)
{
	head_ = new MockParameterComparatorRepositoryNode(name, comparator, head_);
}

MockParameterComparator* MockParameterComparatorRepository::getComparatorForType(const SimpleString& name)
{
	for (MockParameterComparatorRepositoryNode* p = head_; p; p = p->next_)
			if (p->name_ == name) return &p->comparator_;
	return NULL;;
}

MockFunctionCall::MockFunctionCall() : comparatorRepository_(NULL)
{
}

MockFunctionCall::~MockFunctionCall()
{
}

void MockFunctionCall::setComparatorRepository(MockParameterComparatorRepository* repository)
{
	comparatorRepository_ = repository;
}

void MockFunctionCall::setName(const SimpleString& name)
{
	functionName_ = name;
}

SimpleString MockFunctionCall::getName() const
{
	return functionName_;
}

MockParameterComparator* MockFunctionCall::getComparatorForType(const SimpleString& type) const
{
	if (comparatorRepository_)
		return comparatorRepository_->getComparatorForType(type);
	return NULL;
}

