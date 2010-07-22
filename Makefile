#Set this to @ to keep the makefile quiet
SILENCE = @

#Set to Y to disable the memory leak detection. Enabled by default
ifeq ($(CPPUTEST_DISABLE_MEM_LEAK_DETECTION), Y)
	CPPUTEST_USE_MEM_LEAK_DETECTION = N
else
	CPPUTEST_USE_MEM_LEAK_DETECTION = Y
endif

#Set to N to remove the dependency with StdC++ new header file and operator new std_badalloc throw
ifeq ($(CPPUTEST_DISABLE_STD_CPP_LIB), Y)
	CPPUTEST_USE_STD_CPP_LIB = N
else
	CPPUTEST_USE_STD_CPP_LIB = Y
endif

# Set to Y for enabling debug
ENABLE_DEBUG = Y

#---- Outputs ----#
COMPONENT_NAME = CppUTest
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
CPPUTEST_HOME = .
CPP_PLATFORM = Gcc

WARNINGFLAGS = -pedantic-errors -Wall -Wextra -Werror -Wshadow -Wswitch-default -Wswitch-enum -Wconversion
MEMLEAK_DETECTOR_NEW_MACROS += -include include/CppUTest/MemoryLeakDetectorNewMacros.h
MEMLEAK_DETECTOR_MALLOC_MACROS += -include include/CppUTest/MemoryLeakDetectorMallocMacros.h

ifeq ($(CPPUTEST_USE_MEM_LEAK_DETECTION), N)
CPPUTEST_CPPFLAGS += -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED
endif

ifeq ($(ENABLE_DEBUG), Y)
CPPUTEST_CPPFLAGS += -g
endif

ifeq ($(CPPUTEST_USE_STD_CPP_LIB), N)
CPPUTEST_CPPFLAGS += -DCPPUTEST_STD_CPP_LIB_DISABLED
CPPUTEST_CXXFLAGS += -nostdinc++
endif

CPPUTEST_CPPFLAGS += $(WARNINGFLAGS)
CPPUTEST_CXXFLAGS += $(MEMLEAK_DETECTOR_NEW_MACROS)
CPPUTEST_CFLAGS += $(MEMLEAK_DETECTOR_MALLOC_MACROS)

#GCOVFLAGS = -fprofile-arcs -ftest-coverage

#SRC_DIRS is a list of source directories that make up the target library
#If test files are in these directories, their IMPORT_TEST_GROUPs need
#to be included in main to force them to be linked in.  By convention
#put them into an AllTests.h file in each directory
SRC_DIRS = \
	src/CppUTest \
	src/Platforms/$(CPP_PLATFORM)
	
#TEST_SRC_DIRS is a list of directories including 
# - A test main (AllTests.cpp by convention)
# - OBJ files in these directories are included in the TEST_TARGET
# - Consequently - AllTests.h containing the IMPORT_TEST_GROUPS is not needed
# - 
TEST_SRC_DIRS = \
	tests

#includes for all compiles	
INCLUDES =\
  -I$(CPPUTEST_HOME)/include

#Flags to pass to ar, ld
LD_LIBRARIES += -lstdc++

include $(CPPUTEST_HOME)/build/ComponentMakefile

.PHONY: test_all
test_all:
	$(SILENCE)echo Building with the default flags.
	make
	make clean
	$(SILENCE)echo Building with the STDC++ new disabled. 
	make CPPUTEST_DISABLE_STD_CPP_LIB=Y
	make CPPUTEST_DISABLE_STD_CPP_LIB=Y clean
	$(SILENCE)echo Building with Memory Leak Detection disabled
	make CPPUTEST_DISABLE_MEM_LEAK_DETECTION=Y
	make CPPUTEST_DISABLE_MEM_LEAK_DETECTION=Y clean
	$(SILENCE)echo Building with Memory Leak Detection disabled and STD C++ disabled
	make CPPUTEST_DISABLE_MEM_LEAK_DETECTION=Y CPPUTEST_DISABLE_STD_CPP_LIB=Y
	make CPPUTEST_DISABLE_MEM_LEAK_DETECTION=Y CPPUTEST_DISABLE_STD_CPP_LIB=Y clean
	$(SILENCE)echo Building with debug disabled
	make ENABLE_DEBUG=N 
	make ENABLE_DEBUG=N clean
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and CPPFLAGS
	make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude"
	make CFLAGS="" CXXFLAGS="" clean
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and memory leak and STDC++ disabled
	make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED"
	make CFLAGS="" CXXFLAGS="" CPPFLAGS="-DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED" clean
	$(SILENCE)echo Building extensions
	make
	make extensions
	make clean
	make cleanExtensions
	$(SILENCE)echo Building examples 
	make examples
	make cleanExamples
	$(SILENCE)echo Testing JUnit output
	make
	$(SILENCE)./$(TEST_TARGET) -ojunit > junit_run_output
	$(SILENCE)if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	make clean
	
.PHONY: examples
examples: $(TEST_TARGET) 
	make -C examples  all

extensions:
	make -f Makefile_CppUTestExt all

cleanExtensions:
	make -f Makefile_CppUTestExt clean
	
cleanExamples: 
	make -C examples clean 
	
