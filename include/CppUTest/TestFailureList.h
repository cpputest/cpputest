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

#ifndef TESTFAILURELIST_H
#define TESTFAILURELIST_H

#include "TestFailure.h"
class TestFailureList;
class Iter;

class Node {
	friend Iter;
	friend class TestFailureList;

public:
	Node() : _next(this), _prev(this) {}
	~Node() { unlink(); }

protected:
	Node *_next, *_prev;

	void pushBack(Node* n) {
		n->_next = this;
		n->_prev = _prev;
		_prev->_next = n;
		_prev = n;
	}

	void unlink() {
		Node* next = _next;
		Node* prev = _prev;
		next->_prev = prev;
		prev->_next = next;
		_next = this;
		_prev = this;
	}
};

class Iter {
public:
	Iter& operator++() {
		_node = _node->_next;
		return *this;
	}

	bool operator==(Iter it) const {
		return _node == it._node;
	}

	bool operator!=(Iter it) const {
		return _node != it._node;
	}

protected:
	Node* _node;
	Iter(Node* node) : _node(node) {}
};

class TestFailureList {
	class Node_TestFailure : public Node {
		friend class TestFailureList;
		TestFailure _data;
		Node_TestFailure(TestFailure data) : _data(data) {}
	};

	class Iter_TestFailure : public Iter {
		friend class TestFailureList;
		Node_TestFailure* node() const {
			return static_cast<Node_TestFailure*>(_node);
		}
	public:
		TestFailure& operator*() const { return node()->_data; }
		TestFailure* operator->() const { return &node()->_data; }
		operator Iter_TestFailure() const { return _node; }
		Iter_TestFailure(Node* node) : Iter{node} {}
	};

	Node _list;

public:
	using iterator = Iter_TestFailure;

	~TestFailureList() { clear(); }

	bool empty() const { return _list._next == &_list; }

	iterator begin() const { return _list._next; }
	iterator end() const { return const_cast<Node*>(&_list); }

	void pushBack(TestFailure tf) { _list.pushBack(new Node_TestFailure(tf)); }
	void erase(iterator it) { delete it.node(); }

	void clear() {
		while (!empty()) {
			erase(begin());
		}
	}

};
#endif
