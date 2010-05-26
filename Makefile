#Set this to @ to keep the makefile quiet
SILENCE = @

#Set to Y to get extensions.  This brings in string support, and stdint support
ifeq ($(CPPUTEST_ENABLE_EXTENSIONS), Y)
	ENABLE_EXTENSIONS = Y
else
	ENABLE_EXTENSIONS = N
endif

#Set to Y to disable the memory leak detection. Enabled by default
ifeq ($(CPPUTEST_DISABLE_MEMLEAKDETECTION), Y)
	ENABLE_MEMLEAKDETECTION = N
else
	ENABLE_MEMLEAKDETECTION = Y
endif

#Set to N to remove the dependency with StdC++ new header file and operator new std_badalloc throw
ifeq ($(CPPUTEST_DISABLE_STDCPP_NEW), Y)
	ENABLE_STDCPP_NEW = N
else
	ENABLE_STDCPP_NEW = Y
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

CPPFLAGS += -pedantic-errors -Wall -Wextra -Werror 
#for 64 bit linux... No, for old gcc compilers!
#CPPFLAGS += --use-cxa-atexit

ifeq ($(ENABLE_MEMLEAKDETECTION), N)
CPPFLAGS += -DUT_NEW_MACROS_DISABLED -DUT_NEW_OVERRIDES_DISABLED
endif

ifeq ($(ENABLE_DEBUG), Y)
CPPFLAGS += -g
endif

ifeq ($(ENABLE_STDCPP_NEW), N)
CPPFLAGS += -DUT_STDCPP_NEW_DISABLED
CPPFLAGS += -nostdinc++
endif

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

#Add extension directories if enabled
ifeq ($(ENABLE_EXTENSIONS), Y)
SRC_DIRS +=	src/CppUTest/Extensions
TEST_SRC_DIRS += tests/Extensions 
endif

#Flags to pass to ar, ld
LDFLAGS += 
LD_LIBRARIES += -lstdc++

include $(CPPUTEST_HOME)/build/ComponentMakefile

.PHONY: test_all
test_all:
	$(SILENCE)echo Building with the default flags.
	make
	make clean
	$(SILENCE)echo Building with the STDC++ new disabled. Extentions disabled too
	make CPPUTEST_DISABLE_STDCPP_NEW=Y
	make CPPUTEST_DISABLE_STDCPP_NEW=Y clean
	$(SILENCE)echo Building ENABLE_DEBUG disabled and ENABLE_EXTENTIONS enabled
	make ENABLE_DEBUG=N ENABLE_EXTENSIONS=Y
	make clean ENABLE_DEBUG=N ENABLE_EXTENSIONS=Y
	$(SILENCE)echo Building with Memory Leak Detection disabled
	make CPPUTEST_DISABLE_MEMLEAKDETECTION=Y
	make CPPUTEST_DISABLE_MEMLEAKDETECTION=Y clean
	$(SILENCE)echo Building examples without extentions disabled
	make examples ENABLE_EXTENSIONS=Y
	make ENABLE_EXTENSIONS=Y clean cleanExamples
	make ENABLE_DEBUG=N ENABLE_EXTENSIONS=N
	$(SILENCE)./$(TEST_TARGET) -ojunit > junit_run_output
	$(SILENCE)if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	make clean
	make ENABLE_DEBUG=Y CPPUTEST_ENABLE_EXTENSIONS=Y
	make clean CPPUTEST_ENABLE_EXTENSIONS=Y
	make ENABLE_DEBUG=Y CPPUTEST_ENABLE_EXTENSIONS=N
	make clean
	
.PHONY: examples
examples: $(TEST_TARGET) 
	make -C examples  all
	
cleanExamples: 
	make -C examples clean 
	
