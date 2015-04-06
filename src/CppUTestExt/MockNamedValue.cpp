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
#include "CppUTestExt/MockNamedValue.h"
#include "CppUTest/PlatformSpecificFunctions.h"


MockNamedValueComparatorRepository* MockNamedValue::defaultRepository_ = NULL;

void MockNamedValue::setDefaultComparatorRepository(MockNamedValueComparatorRepository* repository)
{
    defaultRepository_ = repository;
}

MockNamedValue::MockNamedValue(const SimpleString& name) : name_(name), type_("int"), comparator_(NULL)
{
    value_.intValue_ = 0;
}

MockNamedValue::~MockNamedValue()
{
}

void MockNamedValue::setValue(unsigned int value)
{
    type_ = "unsigned int";
    value_.unsignedIntValue_ = value;
}

void MockNamedValue::setValue(int value)
{
    type_ = "int";
    value_.intValue_ = value;
}

void MockNamedValue::setValue(long int value)
{
    type_ = "long int";
    value_.longIntValue_ = value;
}

void MockNamedValue::setValue(unsigned long int value)
{
    type_ = "unsigned long int";
    value_.unsignedLongIntValue_ = value;
}

void MockNamedValue::setValue(double value)
{
    type_ = "double";
    value_.doubleValue_ = value;
}

void MockNamedValue::setValue(void* value)
{
    type_ = "void*";
    value_.pointerValue_ = value;
}

void MockNamedValue::setValue(const void* value)
{
    type_ = "const void*";
    value_.constPointerValue_ = value;
}

void MockNamedValue::setValue(const char* value)
{
    type_ = "const char*";
    value_.stringValue_ = value;
}

void MockNamedValue::setObjectPointer(const SimpleString& type, const void* objectPtr)
{
    type_ = type;
    value_.objectPointerValue_ = objectPtr;
    if (defaultRepository_)
        comparator_ = defaultRepository_->getComparatorForType(type);
}

void MockNamedValue::setSize(size_t size)
{
    size_ = size;
}

void MockNamedValue::setName(const char* name)
{
    name_ = name;
}

SimpleString MockNamedValue::getName() const
{
    return name_;
}

SimpleString MockNamedValue::getType() const
{
    return type_;
}

unsigned int MockNamedValue::getUnsignedIntValue() const
{
    if(type_ == "int" && value_.intValue_ >= 0)
        return (unsigned int)value_.intValue_;
    else
    {
        STRCMP_EQUAL("unsigned int", type_.asCharString());
        return value_.unsignedIntValue_;
    }
}

int MockNamedValue::getIntValue() const
{
    STRCMP_EQUAL("int", type_.asCharString());
    return value_.intValue_;
}

long int MockNamedValue::getLongIntValue() const
{
    if(type_ == "int")
        return value_.intValue_;
    else if(type_ == "unsigned int")
        return (long int)value_.unsignedIntValue_;
    else
    {
        STRCMP_EQUAL("long int", type_.asCharString());
        return value_.longIntValue_;
    }
}

unsigned long int MockNamedValue::getUnsignedLongIntValue() const
{
    if(type_ == "unsigned int")
        return value_.unsignedIntValue_;
    else if(type_ == "int" && value_.intValue_ >= 0)
        return (unsigned long int)value_.intValue_;
    else if(type_ == "long int" && value_.longIntValue_ >= 0)
        return (unsigned long int)value_.longIntValue_;
    else
    {
        STRCMP_EQUAL("unsigned long int", type_.asCharString());
        return value_.unsignedLongIntValue_;
    }
}

double MockNamedValue::getDoubleValue() const
{
    STRCMP_EQUAL("double", type_.asCharString());
    return value_.doubleValue_;
}

const char* MockNamedValue::getStringValue() const
{
    STRCMP_EQUAL("const char*", type_.asCharString());
    return value_.stringValue_;
}

void* MockNamedValue::getPointerValue() const
{
    STRCMP_EQUAL("void*", type_.asCharString());
    return value_.pointerValue_;
}

const void* MockNamedValue::getConstPointerValue() const
{
    STRCMP_EQUAL("const void*", type_.asCharString());
    return value_.pointerValue_;
}

const void* MockNamedValue::getObjectPointer() const
{
    return value_.objectPointerValue_;
}

size_t MockNamedValue::getSize() const
{
    return size_;
}

MockNamedValueComparator* MockNamedValue::getComparator() const
{
    return comparator_;
}

bool MockNamedValue::equals(const MockNamedValue& p) const
{
    if((type_ == "long int") && (p.type_ == "int"))
        return value_.longIntValue_ == p.value_.intValue_;
    else if((type_ == "int") && (p.type_ == "long int"))
        return value_.intValue_ == p.value_.longIntValue_;
    else if((type_ == "unsigned int") && (p.type_ == "int"))
        return (long)value_.unsignedIntValue_ == (long)p.value_.intValue_;
    else if((type_ == "int") && (p.type_ == "unsigned int"))
        return (long)value_.intValue_ == (long)p.value_.unsignedIntValue_;
    else if((type_ == "unsigned long int") && (p.type_ == "int"))
        return value_.unsignedLongIntValue_ == (unsigned long)p.value_.intValue_;
    else if((type_ == "int") && (p.type_ == "unsigned long int"))
        return (unsigned long)value_.intValue_ == p.value_.unsignedLongIntValue_;
    else if((type_ == "unsigned int") && (p.type_ == "long int"))
        return (long int)value_.unsignedIntValue_ == p.value_.longIntValue_;
    else if((type_ == "long int") && (p.type_ == "unsigned int"))
        return value_.longIntValue_ == (long int)p.value_.unsignedIntValue_;
    else if((type_ == "unsigned int") && (p.type_ == "unsigned long int"))
        return value_.unsignedIntValue_ == p.value_.unsignedLongIntValue_;
    else if((type_ == "unsigned long int") && (p.type_ == "unsigned int"))
        return value_.unsignedLongIntValue_ == p.value_.unsignedIntValue_;
    else if((type_ == "long int") && (p.type_ == "unsigned long int"))
        return (value_.longIntValue_ >= 0) && (value_.longIntValue_ == (long) p.value_.unsignedLongIntValue_);
    else if((type_ == "unsigned long int") && (p.type_ == "long int"))
        return (p.value_.longIntValue_ >= 0) && ((long)value_.unsignedLongIntValue_ == p.value_.longIntValue_);

    if (type_ != p.type_) return false;

    if (type_ == "int")
        return value_.intValue_ == p.value_.intValue_;
    else if (type_ == "unsigned int")
        return value_.unsignedIntValue_ == p.value_.unsignedIntValue_;
    else if (type_ == "long int")
        return value_.longIntValue_ == p.value_.longIntValue_;
    else if (type_ == "unsigned long int")
        return value_.unsignedLongIntValue_ == p.value_.unsignedLongIntValue_;
    else if (type_ == "const char*")
        return SimpleString(value_.stringValue_) == SimpleString(p.value_.stringValue_);
    else if (type_ == "void*")
        return value_.pointerValue_ == p.value_.pointerValue_;
    else if (type_ == "const void*")
        return value_.constPointerValue_ == p.value_.constPointerValue_;
    else if (type_ == "double")
        return (doubles_equal(value_.doubleValue_, p.value_.doubleValue_, 0.005));

    if (comparator_)
        return comparator_->isEqual(value_.objectPointerValue_, p.value_.objectPointerValue_);

    return false;
}

SimpleString MockNamedValue::toString() const
{
    if (type_ == "int")
        return StringFrom(value_.intValue_);
    else if (type_ == "unsigned int")
        return StringFrom(value_.unsignedIntValue_);
    else if (type_ == "long int")
        return StringFrom(value_.longIntValue_);
    else if (type_ == "unsigned long int")
        return StringFrom(value_.unsignedLongIntValue_);
    else if (type_ == "const char*")
        return value_.stringValue_;
    else if (type_ == "void*")
        return StringFrom(value_.pointerValue_);
    else if (type_ == "const void*")
        return StringFrom(value_.constPointerValue_);
    else if (type_ == "double")
        return StringFrom(value_.doubleValue_);

    if (comparator_)
        return comparator_->valueToString(value_.objectPointerValue_);

    return StringFromFormat("No comparator found for type: \"%s\"", type_.asCharString());

}

void MockNamedValueListNode::setNext(MockNamedValueListNode* node)
{
    next_ = node;
}

MockNamedValueListNode* MockNamedValueListNode::next()
{
    return next_;
}

MockNamedValue* MockNamedValueListNode::item()
{
    return data_;
}

void MockNamedValueListNode::destroy()
{
    delete data_;
}

MockNamedValueListNode::MockNamedValueListNode(MockNamedValue* newValue)
    : data_(newValue), next_(NULL)
{
}

SimpleString MockNamedValueListNode::getName() const
{
    return data_->getName();
}

SimpleString MockNamedValueListNode::getType() const
{
    return data_->getType();
}

MockNamedValueList::MockNamedValueList() : head_(NULL)
{
}

void MockNamedValueList::clear()
{
    while (head_) {
        MockNamedValueListNode* n = head_->next();
        head_->destroy();
        delete head_;
        head_ = n;
    }
}

void MockNamedValueList::add(MockNamedValue* newValue)
{
    MockNamedValueListNode* newNode = new MockNamedValueListNode(newValue);
    if (head_ == NULL)
        head_ = newNode;
    else {
        MockNamedValueListNode* lastNode = head_;
        while (lastNode->next()) lastNode = lastNode->next();
        lastNode->setNext(newNode);
    }
}

MockNamedValue* MockNamedValueList::getValueByName(const SimpleString& name)
{
    for (MockNamedValueListNode * p = head_; p; p = p->next())
        if (p->getName() == name)
            return p->item();
    return NULL;
}

MockNamedValueListNode* MockNamedValueList::begin()
{
    return head_;
}

struct MockNamedValueComparatorRepositoryNode
{
    MockNamedValueComparatorRepositoryNode(const SimpleString& name, MockNamedValueComparator& comparator, MockNamedValueComparatorRepositoryNode* next)
        : name_(name), comparator_(comparator), next_(next) {}
    SimpleString name_;
    MockNamedValueComparator& comparator_;
    MockNamedValueComparatorRepositoryNode* next_;
};

MockNamedValueComparatorRepository::MockNamedValueComparatorRepository() : head_(NULL)
{

}

MockNamedValueComparatorRepository::~MockNamedValueComparatorRepository()
{
    clear();
}

void MockNamedValueComparatorRepository::clear()
{
    while (head_) {
        MockNamedValueComparatorRepositoryNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
}

void MockNamedValueComparatorRepository::installComparator(const SimpleString& name, MockNamedValueComparator& comparator)
{
    head_ = new MockNamedValueComparatorRepositoryNode(name, comparator, head_);
}

MockNamedValueComparator* MockNamedValueComparatorRepository::getComparatorForType(const SimpleString& name)
{
    for (MockNamedValueComparatorRepositoryNode* p = head_; p; p = p->next_)
            if (p->name_ == name) return &p->comparator_;
    return NULL;
}

void MockNamedValueComparatorRepository::installComparators(const MockNamedValueComparatorRepository& repository)
{
    for (MockNamedValueComparatorRepositoryNode* p = repository.head_; p; p = p->next_)
            installComparator(p->name_, p->comparator_);
}

