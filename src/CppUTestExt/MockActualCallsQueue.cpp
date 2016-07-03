/*
 * Copyright (c) 2016, Jesus Gonzalez
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
#include "CppUTestExt/MockActualCallsQueue.h"
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTest/SimpleString.h"

MockActualCallsQueue::MockActualCallsQueue(bool owner, unsigned int maxSize)
: head_(NULL), tail_(NULL), currentSize_(0), maxSize_(maxSize), owner_(owner)
{
}

MockActualCallsQueue::~MockActualCallsQueue()
{
    clear();
}

unsigned int MockActualCallsQueue::size() const
{
    return currentSize_;
}

bool MockActualCallsQueue::isEmpty() const
{
    return currentSize_ == 0;
}

void MockActualCallsQueue::pushBack(MockCheckedActualCall* call)
{
    MockActualCallsQueueNode* newCall = new MockActualCallsQueueNode(call);

    if (currentSize_ >= maxSize_) {
        deleteFromFront();
    }

    if (tail_ == NULL) {
        head_ = newCall;
    } else {
        tail_->next_ = newCall;
    }
    tail_ = newCall;
    currentSize_++;
}

MockCheckedActualCall* MockActualCallsQueue::popFront()
{
    MockCheckedActualCall* actualCall = NULL;

    if (head_) {
        MockActualCallsQueueNode* next = head_->next_;
        actualCall = head_->call_;
        delete head_;
        currentSize_--;
        head_ = next;
        if (!head_) {
            tail_ = NULL;
        }
    }

    return actualCall;
}

void MockActualCallsQueue::clear()
{
    while (head_) {
        MockActualCallsQueueNode* next = head_->next_;
        if (owner_) {
            delete head_->call_;
        }
        delete head_;
        head_ = next;
    }
    tail_ = NULL;
    currentSize_ = 0;
}

void MockActualCallsQueue::pushBackAll(const MockActualCallsQueue& otherQueue)
{
    for (MockActualCallsQueueNode* p = otherQueue.head_; p; p = p->next_) {
        pushBack(p->call_);
    }
}

unsigned int MockActualCallsQueue::getMaxSize()
{
    return maxSize_;
}

void MockActualCallsQueue::setMaxSize(unsigned int maxSize)
{
    maxSize_ = maxSize;
    while(currentSize_ > maxSize_) {
        deleteFromFront();
    }
}

void MockActualCallsQueue::deleteFromFront()
{
    MockCheckedActualCall* frontCall = popFront();
    if (owner_) {
        delete frontCall;
    }
}
