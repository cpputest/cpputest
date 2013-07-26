/*  ----------------------------------------------------------------------------
    Copyright(C) 2013 MindTribe Product Engineering, Inc. All Rights Reserved.

    Author:     Jerry Ryle, MindTribe Product Engineering
                <jerry@mindtribe.com>

    Target(s):  ISO/IEC 14882:2003 (C++03)

    Purpose:    Memory buffer container for testing
    ------------------------------------------------------------------------- */
#ifndef MEMORYBUFFERCONTAINER_H_
#define MEMORYBUFFERCONTAINER_H_

#include <stdint.h>

class MemoryBufferContainer
{
public:
    MemoryBufferContainer(void const *memoryBuffer, size_t memoryBufferLength);
    MemoryBufferContainer(MemoryBufferContainer const &other);
    virtual ~MemoryBufferContainer();
    MemoryBufferContainer &operator= (MemoryBufferContainer const &other);
    bool operator==(const MemoryBufferContainer &other) const;
    MemoryBufferContainer &operator+=(MemoryBufferContainer const &other);
    MemoryBufferContainer const operator+(MemoryBufferContainer const &other) const;
    void const *getBuffer(void) const;
    size_t getBufferLength(void) const;

protected:
    uint8_t *m_memoryBuffer;
    size_t m_memoryBufferLength;
};


SimpleString StringFrom(MemoryBufferContainer const &value);


#endif /* MEMORYBUFFERCONTAINER_H_ */
