/*  ----------------------------------------------------------------------------
    Copyright(C) 2013 MindTribe Product Engineering, Inc. All Rights Reserved.

    Author:     Jerry Ryle, MindTribe Product Engineering
                <jerry@mindtribe.com>

    Target(s):  ISO/IEC 14882:2003 (C++03)

    Purpose:    Memory buffer container for testing
    ------------------------------------------------------------------------- */
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MemoryBufferContainer.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

MemoryBufferContainer::MemoryBufferContainer(void const *memoryBuffer, size_t memoryBufferLength)
    : m_memoryBuffer(NULL), m_memoryBufferLength(memoryBufferLength)
{
    if (m_memoryBufferLength > 0) {
        m_memoryBuffer = new uint8_t[m_memoryBufferLength];
        memcpy(m_memoryBuffer, memoryBuffer, m_memoryBufferLength);
    }
}

MemoryBufferContainer::MemoryBufferContainer(MemoryBufferContainer const &other)
    : m_memoryBuffer(NULL), m_memoryBufferLength(other.m_memoryBufferLength)
{
    if (m_memoryBufferLength > 0) {
        m_memoryBuffer = new uint8_t[m_memoryBufferLength];
        memcpy(m_memoryBuffer, other.m_memoryBuffer, m_memoryBufferLength);
    }
}

MemoryBufferContainer::~MemoryBufferContainer()
{
    delete[] m_memoryBuffer;
}

MemoryBufferContainer &MemoryBufferContainer::operator=(MemoryBufferContainer const &other)
{
    if (this == &other)
        return *this;

    delete[] m_memoryBuffer;
    m_memoryBuffer = NULL;

    m_memoryBufferLength = other.m_memoryBufferLength;

    if (m_memoryBufferLength > 0) {
        m_memoryBuffer = new uint8_t[m_memoryBufferLength];
        memcpy(m_memoryBuffer, other.m_memoryBuffer, m_memoryBufferLength);
    }

    return *this;
}

bool MemoryBufferContainer::operator==(const MemoryBufferContainer &other) const
{
    if (other.m_memoryBufferLength != m_memoryBufferLength) {
        return false;
    }
    if (m_memoryBufferLength > 0) {
        if (!other.m_memoryBuffer || !m_memoryBuffer) {
            return false;
        }
        return (memcmp(other.m_memoryBuffer, m_memoryBuffer, m_memoryBufferLength) == 0);
    }
    return true;
}

MemoryBufferContainer &MemoryBufferContainer::operator+=(MemoryBufferContainer const &other)
{
    uint8_t *tempBuffer = NULL;
    size_t bufferLength = m_memoryBufferLength + other.m_memoryBufferLength;

    if (bufferLength > 0) {
        tempBuffer = new uint8_t[bufferLength];
        if (m_memoryBufferLength > 0) {
            memcpy(tempBuffer, m_memoryBuffer, m_memoryBufferLength);
        }
        if (other.m_memoryBufferLength > 0) {
            memcpy(tempBuffer+m_memoryBufferLength, other.m_memoryBuffer, other.m_memoryBufferLength);
        }
    }

    delete[] m_memoryBuffer;

    m_memoryBufferLength = bufferLength;
    m_memoryBuffer = tempBuffer;
    return *this;

}

MemoryBufferContainer const MemoryBufferContainer::operator+(MemoryBufferContainer const &other) const
{
    return MemoryBufferContainer(*this) += other;
}

void const *MemoryBufferContainer::getBuffer(void) const
{
    return m_memoryBuffer;
}

size_t MemoryBufferContainer::getBufferLength(void) const
{
    return m_memoryBufferLength;
}

SimpleString StringFrom(MemoryBufferContainer const &value)
{
    return StringFrom(value.getBuffer(), value.getBufferLength());
}

SimpleString StringFrom(void const *value, size_t size)
{
    SimpleString dataString;
    size_t const maxDataBufferToPrint = 1024;
    size_t i;
    uint8_t const *dataBuffer = (uint8_t const *)value;
    size_t dataBufferToPrint = size;

    if (size > maxDataBufferToPrint) {
        dataBufferToPrint = maxDataBufferToPrint;
    }
    for (i = 0; i < size; i++) {
        dataString += StringFromFormat("%02X ",dataBuffer[i]);
    }
    if (dataBufferToPrint < size) {
        dataString += SimpleString("...");
    }

    return SimpleString("Length: ") + StringFrom((unsigned int)size) + SimpleString(", Data: ") + dataString;
}
