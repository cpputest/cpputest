#ifndef ALLOCATIONINCPPFILE_H
#define ALLOCATIONINCPPFILE_H

char* newAllocation();
char* newArrayAllocation();

char* newAllocationWithoutMacro();
char* newArrayAllocationWithoutMacro();

#if CPPUTEST_USE_STD_CPP_LIB

class ClassThatThrowsAnExceptionInTheConstructor
{
public:
    _no_return_ ClassThatThrowsAnExceptionInTheConstructor();
};

#endif

#endif
