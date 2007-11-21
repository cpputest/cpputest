CPPUTEST_TARGET = AllTests

LOADLIBS =\
 -lCppUTest\
 -lstdc++\

LIBDIR = $(CPP_U_TEST)/lib

LIBDIRS =\
 -L$(LIBDIR) 

OBJS = \
 FailureTest.o \
 MemoryLeakWarningTest.o \
 NullTestTest.o \
 SimpleStringTest.o \
 TestInstallerTest.o \
 TestOutputTest.o \
 TestRegistryTest.o \
 UtestTest.o \
 TestHarness_cTest.o \
 CommandLineTestRunnerTest.o \
 JUnitOutputTest.o

INCLUDE =\
 -I$(CPP_U_TEST)/include/CppUTest\
 -I$(CPP_U_TEST)/include/Platforms/$(CPP_PLATFORM)\
 -I.

include $(CPP_U_TEST)/build/TestExeMakefile

$(CPPUTEST_TARGET): $(CPPUTEST_TARGET).o $(OBJS)\
		$(LIBDIR)/libCppUTest.a
	$(CXX) -o $(CPPUTEST_TARGET) $(LIBDIRS) $^ $(LOADLIBS)
