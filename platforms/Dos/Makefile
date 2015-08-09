CXX := wcl
CC  := wcl
AR := wlib
LINK := wlink
LOG := ALLTESTS.LOG
NULL :=
S := $(NULL) #
C := ,
V := /

-include ../platforms/Dos/platform.mk

CFLAGS := \
  -q -c -os -oc -d0 -w=3 -ml \
  -dCPPUTEST_MEM_LEAK_DETECTION_DISABLED=1 \
  -dCPPUTEST_STD_CPP_LIB_DISABLED=1 \
  -i$(CPPUTEST_HOME)/include \
  -i$(CPPUTEST_HOME)/include/Platforms/dos \
  -i$(WATCOM)/h \
  -i$(WATCOM)/h/nt \

CXXFLAGS := $(CFLAGS) -xds

OBJECTS := \
  $(CPPUTEST_HOME)/src/CppUTest/CommandLineArguments.obj \
  $(CPPUTEST_HOME)/src/CppUTest/CommandLineTestRunner.obj \
  $(CPPUTEST_HOME)/src/CppUTest/JUnitTestOutput.obj \
  $(CPPUTEST_HOME)/src/CppUTest/MemoryLeakDetector.obj \
  $(CPPUTEST_HOME)/src/CppUTest/MemoryLeakWarningPlugin.obj \
  $(CPPUTEST_HOME)/src/CppUTest/SimpleMutex.obj \
  $(CPPUTEST_HOME)/src/CppUTest/SimpleString.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestFailure.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestFilter.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestHarness_c.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestMemoryAllocator.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestOutput.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestPlugin.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestRegistry.obj \
  $(CPPUTEST_HOME)/src/CppUTest/TestResult.obj \
  $(CPPUTEST_HOME)/src/CppUTest/Utest.obj \
  $(CPPUTEST_HOME)/src/Platforms/Dos/UtestPlatform.obj \
  $(CPPUTEST_HOME)/tests/AllTests.obj \

.PHONY: all clean

all: CPPU1$(EXE)

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -fo=$(notdir $@) $(subst /,$V, $<)

%.obj: %.c
	$(CC) $(subst /,$V, $(CFLAGS)) -fo=$(notdir $@) $(subst /,$V, $<)

CPPU1$(EXE): \
    $(OBJECTS)
	$(LINK) opt map, stack=50k sys dos file $(subst $(S),$(C),$(notdir $?)) name $@

clean:
	rm -rf exit *.obj *.map *.txt *.LOG *.EXE *.err
