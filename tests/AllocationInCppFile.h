#ifndef ALLOCATIONINCPPFILE_H
#define ALLOCATIONINCPPFILE_H

char* newAllocation();
char* newArrayAllocation();

char* newAllocationWithoutMacro();
char* newArrayAllocationWithoutMacro();

class ClassThatThrowsAnExceptionInTheConstructor
{
public:
  ClassThatThrowsAnExceptionInTheConstructor() __no_return__;
};

#endif
