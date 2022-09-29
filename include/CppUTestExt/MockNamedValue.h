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

#ifndef D_MockNamedValue_h
#define D_MockNamedValue_h

#include "CppUTest/CppUTestConfig.h"

/*
 * MockNamedValueComparator is an interface that needs to be used when creating Comparators.
 * This is needed when comparing values of non-native type.
 */

class MockNamedValueComparator
{
public:
    MockNamedValueComparator() {}
    virtual ~MockNamedValueComparator() {}

    virtual bool isEqual(const void* object1, const void* object2)=0;
    virtual SimpleString valueToString(const void* object)=0;
};

/*
 * MockNamedValueCopier is an interface that needs to be used when creating Copiers.
 * This is needed when copying values of non-native type.
 */

class MockNamedValueCopier
{
public:
    MockNamedValueCopier() {}
    virtual ~MockNamedValueCopier() {}

    virtual void copy(void* out, const void* in)=0;
};


class MockFunctionComparator : public MockNamedValueComparator
{
public:
    typedef bool (*isEqualFunction)(const void*, const void*);
    typedef SimpleString (*valueToStringFunction)(const void*);

    MockFunctionComparator(isEqualFunction equal, valueToStringFunction valToString)
        : equal_(equal), valueToString_(valToString) {}

    virtual bool isEqual(const void* object1, const void* object2) _override { return equal_(object1, object2); }
    virtual SimpleString valueToString(const void* object) _override { return valueToString_(object); }
private:
    isEqualFunction equal_;
    valueToStringFunction valueToString_;
};

class MockFunctionCopier : public MockNamedValueCopier
{
public:
    typedef void (*copyFunction)(void*, const void*);

    MockFunctionCopier(copyFunction copier) : copier_(copier) {}

    virtual void copy(void* dst, const void* src) _override { copier_(dst, src); }

private:
    copyFunction copier_;
};

/*
 * MockNamedValue is the generic value class used. It encapsulates basic types and can use them "as if one"
 * Also it enables other types by putting object pointers. They can be compared with comparators.
 *
 * Basically this class ties together a Name, a Value, a Type, and a Comparator
 */

class MockNamedValueComparatorsAndCopiersRepository;
class MockNamedValue
{
public:
    MockNamedValue(const SimpleString& name);
    DEFAULT_COPY_CONSTRUCTOR(MockNamedValue)
    virtual ~MockNamedValue();

    virtual void setValue(bool value);
    virtual void setValue(int value);
    virtual void setValue(unsigned int value);
    virtual void setValue(long int value);
    virtual void setValue(unsigned long int value);
    virtual void setValue(cpputest_longlong value);
    virtual void setValue(cpputest_ulonglong value);
    virtual void setValue(double value);
    virtual void setValue(double value, double tolerance);
    virtual void setValue(void* value);
    virtual void setValue(const void* value);
    virtual void setValue(void (*value)());
    virtual void setValue(const char* value);
    virtual void setMemoryBuffer(const unsigned char* value, size_t size);
    virtual void setConstObjectPointer(const SimpleString& type, const void* objectPtr);
    virtual void setObjectPointer(const SimpleString& type, void* objectPtr);
    virtual void setSize(size_t size);

    virtual void setName(const char* name);

    virtual bool equals(const MockNamedValue& p) const;
    virtual bool compatibleForCopying(const MockNamedValue& p) const;

    virtual SimpleString toString() const;

    virtual SimpleString getName() const;
    virtual SimpleString getType() const;

    virtual bool getBoolValue() const;
    virtual int getIntValue() const;
    virtual unsigned int getUnsignedIntValue() const;
    virtual long int getLongIntValue() const;
    virtual unsigned long int getUnsignedLongIntValue() const;
    virtual cpputest_longlong getLongLongIntValue() const;
    virtual cpputest_ulonglong getUnsignedLongLongIntValue() const;
    virtual double getDoubleValue() const;
    virtual double getDoubleTolerance() const;
    virtual const char* getStringValue() const;
    virtual void* getPointerValue() const;
    virtual const void* getConstPointerValue() const;
    virtual void (*getFunctionPointerValue() const)();
    virtual const unsigned char* getMemoryBuffer() const;
    virtual const void* getConstObjectPointer() const;
    virtual void* getObjectPointer() const;
    virtual size_t getSize() const;


    virtual MockNamedValueComparator* getComparator() const;
    virtual MockNamedValueCopier* getCopier() const;

    static void setDefaultComparatorsAndCopiersRepository(MockNamedValueComparatorsAndCopiersRepository* repository);
    static MockNamedValueComparatorsAndCopiersRepository* getDefaultComparatorsAndCopiersRepository();

    static const double defaultDoubleTolerance;
private:
    SimpleString name_;
    SimpleString type_;
    union {
        bool boolValue_;
        int intValue_;
        unsigned int unsignedIntValue_;
        long int longIntValue_;
        unsigned long int unsignedLongIntValue_;
#ifdef CPPUTEST_USE_LONG_LONG
        cpputest_longlong longLongIntValue_;
        cpputest_ulonglong unsignedLongLongIntValue_;
#else
        char longLongPlaceholder_[CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE];
#endif
        struct {
            double value;
            double tolerance;
        } doubleValue_;
        const char* stringValue_;
        void* pointerValue_;
        const void* constPointerValue_;
        void (*functionPointerValue_)();
        const unsigned char* memoryBufferValue_;
        const void* constObjectPointerValue_;
        void* objectPointerValue_;
        const void* outputPointerValue_;
    } value_;
    size_t size_;
    MockNamedValueComparator* comparator_;
    MockNamedValueCopier* copier_;
    static MockNamedValueComparatorsAndCopiersRepository* defaultRepository_;
};

class MockNamedValueListNode
{
public:
    MockNamedValueListNode(MockNamedValue* newValue);

    SimpleString getName() const;
    SimpleString getType() const;

    MockNamedValueListNode* next();
    MockNamedValue* item();

    void destroy();
    void setNext(MockNamedValueListNode* node);
private:
    MockNamedValue* data_;
    MockNamedValueListNode* next_;
};

class MockNamedValueList
{
public:
    MockNamedValueList();

    MockNamedValueListNode* begin();

    void add(MockNamedValue* newValue);
    void clear();

    MockNamedValue* getValueByName(const SimpleString& name);

private:
    MockNamedValueListNode* head_;
};

/*
 * MockParameterComparatorRepository is a class which stores comparators and copiers which can be used for comparing non-native types
 *
 */

struct MockNamedValueComparatorsAndCopiersRepositoryNode;
class MockNamedValueComparatorsAndCopiersRepository
{
    MockNamedValueComparatorsAndCopiersRepositoryNode* head_;
public:
    MockNamedValueComparatorsAndCopiersRepository();
    virtual ~MockNamedValueComparatorsAndCopiersRepository();

    virtual void installComparator(const SimpleString& name, MockNamedValueComparator& comparator);
    virtual void installCopier(const SimpleString& name, MockNamedValueCopier& copier);
    virtual void installComparatorsAndCopiers(const MockNamedValueComparatorsAndCopiersRepository& repository);
    virtual MockNamedValueComparator* getComparatorForType(const SimpleString& name);
    virtual MockNamedValueCopier* getCopierForType(const SimpleString& name);

    void clear();
};

#endif
