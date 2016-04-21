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
#include "CppUTestExt/MockSupport.h"
#include "CppUTest/PlatformSpecificFunctions.h"


MockNamedValueComparatorsAndCopiersRepository* MockNamedValue::defaultRepository_ = NULL;

void MockNamedValue::setDefaultComparatorsAndCopiersRepository(MockNamedValueComparatorsAndCopiersRepository* repository)
{
    defaultRepository_ = repository;
}

MockNamedValue::MockNamedValue(const SimpleString& name) : name_(name), type_("int"), size_(0), comparator_(NULL), copier_(NULL)
{
    value_.intValue_ = 0;
}

MockNamedValue::~MockNamedValue()
{
}

void MockNamedValue::setValue(bool value)
{
    type_ = "bool";
    value_.boolValue_ = value;
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

void MockNamedValue::setValue(cpputest_longlong value)
{
    type_ = "long long int";
    value_.longLongIntValue_ = value;
}

void MockNamedValue::setValue(cpputest_ulonglong value)
{
    type_ = "unsigned long long int";
    value_.unsignedLongLongIntValue_ = value;
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

void MockNamedValue::setValue(void (*value)())
{
    type_ = "void (*)()";
    value_.functionPointerValue_ = value;
}

void MockNamedValue::setValue(const char* value)
{
    type_ = "const char*";
    value_.stringValue_ = value;
}

void MockNamedValue::setMemoryBuffer(const unsigned char* value, size_t size)
{
    type_ = "const unsigned char*";
    value_.memoryBufferValue_ = value;
    size_ = size;
}

void MockNamedValue::setObjectPointer(const SimpleString& type, const void* objectPtr)
{
    type_ = type;
    value_.objectPointerValue_ = objectPtr;
    if (defaultRepository_)
    {
        comparator_ = defaultRepository_->getComparatorForType(type);
        copier_ = defaultRepository_->getCopierForType(type);
    }
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

bool MockNamedValue::getBoolValue() const
{
    STRCMP_EQUAL("bool", type_.asCharString());
    return value_.boolValue_;
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

#ifdef CPPUTEST_USE_LONG_LONG

cpputest_longlong MockNamedValue::getLongLongIntValue() const
{
    if(type_ == "int")
        return value_.intValue_;
    else if(type_ == "unsigned int")
        return (cpputest_longlong) value_.unsignedIntValue_;
    else if(type_ == "long int")
        return value_.longIntValue_;
    else if(type_ == "unsigned long int")
        return (cpputest_longlong) value_.unsignedLongIntValue_;
    else
    {
        STRCMP_EQUAL("long long int", type_.asCharString());
        return value_.longLongIntValue_;
    }
}

cpputest_ulonglong MockNamedValue::getUnsignedLongLongIntValue() const
{
    if(type_ == "unsigned int")
        return value_.unsignedIntValue_;
    else if(type_ == "unsigned long int")
        return value_.unsignedLongIntValue_;
    else if(type_ == "int" && value_.intValue_ >= 0)
        return (cpputest_ulonglong) value_.intValue_;
    else if(type_ == "long int" && value_.longIntValue_ >= 0)
        return (cpputest_ulonglong) value_.longIntValue_;
    else if(type_ == "long long int" && value_.longLongIntValue_ >= 0)
        return (cpputest_ulonglong) value_.longLongIntValue_;
    else
    {
        STRCMP_EQUAL("unsigned long long int", type_.asCharString());
        return value_.unsignedLongLongIntValue_;
    }
}

#else  /* CPPUTEST_USE_LONG_LONG */

cpputest_longlong MockNamedValue::getLongLongIntValue() const
{
    mock().getActiveReporter()->failTest(MockUnsupportedFeatureFailure(mock().getActiveReporter()->getTestToFail(), "CPPUTEST_USE_LONG_LONG"));
    return CPPUTEST_LONGLONG_DEFAULT;
}

cpputest_ulonglong MockNamedValue::getUnsignedLongLongIntValue() const
{
    mock().getActiveReporter()->failTest(MockUnsupportedFeatureFailure(mock().getActiveReporter()->getTestToFail(), "CPPUTEST_USE_LONG_LONG"));
    return CPPUTEST_ULONGLONG_DEFAULT;
}

#endif /* CPPUTEST_USE_LONG_LONG */

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

void (*MockNamedValue::getFunctionPointerValue() const)()
{
    STRCMP_EQUAL("void (*)()", type_.asCharString());
    return value_.functionPointerValue_;
}

const unsigned char* MockNamedValue::getMemoryBuffer() const
{
    STRCMP_EQUAL("const unsigned char*", type_.asCharString());
    return value_.memoryBufferValue_;
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

MockNamedValueCopier* MockNamedValue::getCopier() const
{
    return copier_;
}

MockNamedValue MockNamedValue::integerPromotion() const
{
    MockNamedValue result(*this);
    if (result.type_ == "int")
    {
        result.value_.longIntValue_ = result.value_.intValue_;
        result.type_ = "long int";
    }
    if (result.type_ == "unsigned int")
    {
        result.value_.unsignedLongIntValue_ = result.value_.unsignedIntValue_;
        result.type_ = "unsigned long int";
    }
#ifdef CPPUTEST_USE_LONG_LONG
    if (result.type_ == "long int")
    {
        result.value_.longLongIntValue_ = result.value_.longIntValue_;
        result.type_ = "long long int";
    }
    if (result.type_ == "unsigned long int")
    {
        result.value_.unsignedLongLongIntValue_ = result.value_.unsignedLongIntValue_;
        result.type_ = "unsigned long long int";
    }
#endif /* CPPUTEST_USE_LONG_LONG */
    return result;
}

bool MockNamedValue::equals(const MockNamedValue& p) const
{
    MockNamedValue promotedThis = integerPromotion();
    MockNamedValue promotedP = p.integerPromotion();

#ifdef CPPUTEST_USE_LONG_LONG
    if ((promotedThis.type_ == "long long int") && (promotedP.type_ == "long long int"))
        return (promotedThis.value_.longLongIntValue_ == promotedP.value_.longLongIntValue_);
    else if ((promotedThis.type_ == "unsigned long long int") && (promotedP.type_ == "unsigned long long int"))
        return (promotedThis.value_.unsignedLongLongIntValue_ == promotedP.value_.unsignedLongLongIntValue_);
    else if ((promotedThis.type_ == "long long int") && (promotedP.type_ == "unsigned long long int"))
        return (promotedThis.value_.longLongIntValue_ >= 0)
            && (promotedThis.value_.longLongIntValue_ == (long long)promotedP.value_.unsignedLongLongIntValue_);
    else if ((promotedThis.type_ == "unsigned long long int") && (promotedP.type_ == "long long int"))
        return (promotedP.value_.longLongIntValue_ >= 0)
            && ((long long)promotedThis.value_.unsignedLongLongIntValue_ == promotedP.value_.longLongIntValue_);
#else
    if ((promotedThis.type_ == "long int") && (promotedP.type_ == "long int"))
        return (promotedThis.value_.longIntValue_ == promotedP.value_.longIntValue_);
    else if ((promotedThis.type_ == "unsigned long int") && (promotedP.type_ == "unsigned long int"))
        return (promotedThis.value_.unsignedLongIntValue_ == promotedP.value_.unsignedLongIntValue_);
    else if ((promotedThis.type_ == "long int") && (promotedP.type_ == "unsigned long int"))
        return (promotedThis.value_.longIntValue_ >= 0)
            && (promotedThis.value_.longIntValue_ == (long)promotedP.value_.unsignedLongIntValue_);
    else if ((promotedThis.type_ == "unsigned long int") && (promotedP.type_ == "long int"))
        return (promotedP.value_.longIntValue_ >= 0)
            && ((long)promotedThis.value_.unsignedLongIntValue_ == promotedP.value_.longIntValue_);
#endif /* CPPUTEST_USE_LONG_LONG */

    if (type_ != p.type_) return false;

    if (type_ == "bool")
        return value_.boolValue_ == p.value_.boolValue_;
    else if (type_ == "const char*")
        return SimpleString(value_.stringValue_) == SimpleString(p.value_.stringValue_);
    else if (type_ == "void*")
        return value_.pointerValue_ == p.value_.pointerValue_;
    else if (type_ == "const void*")
        return value_.constPointerValue_ == p.value_.constPointerValue_;
    else if (type_ == "void (*)()")
        return value_.functionPointerValue_ == p.value_.functionPointerValue_;
    else if (type_ == "double")
        return (doubles_equal(value_.doubleValue_, p.value_.doubleValue_, 0.005));
    else if (type_ == "const unsigned char*")
    {
        if (size_ != p.size_) {
            return false;
        }
        return SimpleString::MemCmp(value_.memoryBufferValue_, p.value_.memoryBufferValue_, size_) == 0;
    }

    if (comparator_)
        return comparator_->isEqual(value_.objectPointerValue_, p.value_.objectPointerValue_);

    return false;
}

bool MockNamedValue::compatibleForCopying(const MockNamedValue& p) const
{
    if (type_ == p.type_) return true;

    if ((type_ == "const void*") && (p.type_ == "void*"))
        return true;

    return false;
}

SimpleString MockNamedValue::toString() const
{
    if (type_ == "bool")
        return StringFrom(value_.boolValue_);
    else if (type_ == "int")
        return StringFrom(value_.intValue_);
    else if (type_ == "unsigned int")
        return StringFrom(value_.unsignedIntValue_);
    else if (type_ == "long int")
        return StringFrom(value_.longIntValue_);
    else if (type_ == "unsigned long int")
        return StringFrom(value_.unsignedLongIntValue_);
    else if (type_ == "long long int")
        return StringFrom(value_.longLongIntValue_);
    else if (type_ == "unsigned long long int")
        return StringFrom(value_.unsignedLongLongIntValue_);
    else if (type_ == "const char*")
        return value_.stringValue_;
    else if (type_ == "void*")
        return StringFrom(value_.pointerValue_);
    else if (type_ == "void (*)()")
        return StringFrom(value_.functionPointerValue_);
    else if (type_ == "const void*")
        return StringFrom(value_.constPointerValue_);
    else if (type_ == "double")
        return StringFrom(value_.doubleValue_);
    else if (type_ == "const unsigned char*")
        return StringFromBinaryWithSizeOrNull(value_.memoryBufferValue_, size_);

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

struct MockNamedValueComparatorsAndCopiersRepositoryNode
{
    MockNamedValueComparatorsAndCopiersRepositoryNode(const SimpleString& name, MockNamedValueComparator* comparator, MockNamedValueComparatorsAndCopiersRepositoryNode* next)
        : name_(name), comparator_(comparator), copier_(NULL), next_(next) {}
    MockNamedValueComparatorsAndCopiersRepositoryNode(const SimpleString& name, MockNamedValueCopier* copier, MockNamedValueComparatorsAndCopiersRepositoryNode* next)
        : name_(name), comparator_(NULL), copier_(copier), next_(next) {}
    SimpleString name_;
    MockNamedValueComparator* comparator_;
    MockNamedValueCopier* copier_;
    MockNamedValueComparatorsAndCopiersRepositoryNode* next_;
};

MockNamedValueComparatorsAndCopiersRepository::MockNamedValueComparatorsAndCopiersRepository() : head_(NULL)
{

}

MockNamedValueComparatorsAndCopiersRepository::~MockNamedValueComparatorsAndCopiersRepository()
{
    clear();
}

void MockNamedValueComparatorsAndCopiersRepository::clear()
{
    while (head_) {
        MockNamedValueComparatorsAndCopiersRepositoryNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
}

void MockNamedValueComparatorsAndCopiersRepository::installComparator(const SimpleString& name, MockNamedValueComparator& comparator)
{
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(name, &comparator, head_);
}

void MockNamedValueComparatorsAndCopiersRepository::installCopier(const SimpleString& name, MockNamedValueCopier& copier)
{
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(name, &copier, head_);
}

MockNamedValueComparator* MockNamedValueComparatorsAndCopiersRepository::getComparatorForType(const SimpleString& name)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next_)
            if (p->name_ == name && p->comparator_) return p->comparator_;
    return NULL;
}

MockNamedValueCopier* MockNamedValueComparatorsAndCopiersRepository::getCopierForType(const SimpleString& name)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next_)
            if (p->name_ == name && p->copier_) return p->copier_;
    return NULL;
}

void MockNamedValueComparatorsAndCopiersRepository::installComparatorsAndCopiers(const MockNamedValueComparatorsAndCopiersRepository& repository)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_; p; p = p->next_)
      head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(p->name_, p->comparator_, head_);
}
