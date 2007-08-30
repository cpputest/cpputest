#include "MemoryLeakWarning.h"

#include <e32base.h>

MemoryLeakWarning* MemoryLeakWarning::_latest = NULL;

// naming convention due to CppUTest generic class name
class MemoryLeakWarningData : public CBase {
public:
	TInt iInitialAllocCells;
	TInt iExpectedLeaks;
};

MemoryLeakWarning::MemoryLeakWarning()
{
	_latest = this; 
	CreateData();	
}

MemoryLeakWarning::~MemoryLeakWarning()
{
	DestroyData();
}

void MemoryLeakWarning::Enable()
{
}

const char* MemoryLeakWarning::FinalReport(int toBeDeletedLeaks)
{
	if(_impl->iInitialAllocCells != User::CountAllocCells()) {
		return "Heap imbalance after test\n";
	}
	return "";
}

void MemoryLeakWarning::CheckPointUsage()
{
	_impl->iInitialAllocCells = User::CountAllocCells();
}

bool MemoryLeakWarning::UsageIsNotBalanced()
{
	TInt allocatedCells(User::CountAllocCells());
	if(_impl->iExpectedLeaks != 0) {
		TInt difference(Abs(_impl->iInitialAllocCells - allocatedCells));
		return difference != _impl->iExpectedLeaks;
	}
	return allocatedCells != _impl->iInitialAllocCells;
}

const char* MemoryLeakWarning::Message()
{
	return "";
}

void MemoryLeakWarning::ExpectLeaks(int n)
{
	_impl->iExpectedLeaks = n;
}

// this method leaves (no naming convention followed due to CppUTest framework
void MemoryLeakWarning::CreateData()
{
	_impl = new(ELeave) MemoryLeakWarningData();
}

void MemoryLeakWarning::DestroyData()
{
	delete _impl;
	_impl = NULL;
}

MemoryLeakWarning* MemoryLeakWarning::GetLatest()
{
	return _latest;
}

void MemoryLeakWarning::SetLatest(MemoryLeakWarning* latest)
{
	_latest = latest;
}
