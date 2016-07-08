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

#ifndef D_MockActualCallsQueue_h
#define D_MockActualCallsQueue_h

class MockCheckedActualCall;
class SimpleString;

/**
 * This class implements a FIFO queue of MockCheckedActualCall calls.
 *
 * @note The queue can be created as owner of its stored calls or not. If the queue is the owner, then the ownership of the calls added
 * to the queue is transferred to the queue, and therefore it's responsible for managing their life cycle (i.e. deleting them).
 *
 * @note The queue has a size limit. When the queue reaches its maximum size, adding new calls on the back will remove calls from the front.
 */
class MockActualCallsQueue
{

public:
    /**
     * Constructor.
     *
     * Creates an empty queue.
     *
     * @param owner [in] Indicates if the ownership of the calls added to the queue shall be transferred to the queue
     * @param maxSize [in] Maximum size for the queue
     */
    MockActualCallsQueue(bool owner, unsigned int maxSize = (unsigned int) -1);

    /**
     * Destructor.
     *
     * Discards all items, and if the queue is the owner of the calls, then deletes their associated calls.
     */
    virtual ~MockActualCallsQueue();

    /**
     * Returns the number of calls stored in the queue.
     *
     * @return Size of the queue
     */
    virtual unsigned int size() const;

    /**
     * Checks if the queue is empty.
     *
     * @return @c true if the queue is empty, @c false otherwise
     */
    virtual bool isEmpty() const;

    /**
     * Clears the queue, discarding all items, and if the queue is the owner of its calls, then also deleting their associated calls.
     */
    virtual void clear();

    /**
     * Appends the call to the back of the queue.
     *
     * @note If the queue is the owner of its calls, then it becomes the owner of the passed object.
     *
     * @note If the queue is at its size limit, then the first item is discarded, and if the queue is the owner of its calls, then the
     * associated call is also deleted.
     *
     * @param call [in] Pointer to the call to be appended
     */
    virtual void pushBack(MockCheckedActualCall* call);

    /**
     * Removes a call from the front of the queue, and returns it.
     *
     * @note If the queue is the owner of its calls, then the caller becomes the owner of the returned object.
     *
     * @return Pointer to the call that was in front of the queue, or NULL if the queue was empty
     */
    virtual MockCheckedActualCall* popFront();

    /**
     * Appends all the calls from @c otherQueue to the back of this queue.
     *
     * @note If this queue is the owner of calls, then it becomes the owner all the calls from @c otherQueue. This means that either
     * this queue shall not be owner of calls, or that it's owner of calls and the calls from @c otherQueue do not have an owner
     * yet (i.e. @c otherQueue is not owner of calls).
     *
     * @param otherQueue [in] Another queue
     */
    virtual void pushBackAll(const MockActualCallsQueue& otherQueue);

    /**
     * Gets the maximum size for the queue.
     * @return Maximum size
     */
    virtual unsigned int getMaxSize();

    /**
     * Sets the maximum size for the queue.
     *
     * @note If the current size is greater than @c maxSize, then calls are removed in order from the front of the queue until the
     * current size is equal to the maximum size.
     *
     * @param maxSize [in] Maximum size
     */
    virtual void setMaxSize(unsigned int maxSize);

    /**
     * Returns a string representation of the queue as a list with the description of all its stored calls, each on a new line.
     *
     * @param linePrefix String to be added before each individual call description
     * @return String representation of the queue
     */
    virtual SimpleString toString(const SimpleString& linePrefix) const;

    /**
     * Returns a string representation of the queue as a list with the description of its stored calls for function @c functionName,
     * each on a new line.
     *
     * @param functionName Name of the function to be filtered through
     * @param linePrefix String to be added before each individual call description
     * @return String representation of the queue
     */
    virtual SimpleString toStringFilterByFunction(const SimpleString& functionName, const SimpleString& linePrefix) const;

protected:
    class MockActualCallsQueueNode
    {
    public:
        MockCheckedActualCall* call_;

        MockActualCallsQueueNode* next_;
        MockActualCallsQueueNode(MockCheckedActualCall* call) : call_(call), next_(NULL) {}
    };

private:
    MockActualCallsQueueNode* head_;
    MockActualCallsQueueNode* tail_;

    unsigned int currentSize_;
    unsigned int maxSize_;

    bool owner_;

    virtual void deleteFromFront();
};

#endif
