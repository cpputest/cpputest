#Set this to @ to keep the makefile quiet
SILENCE = @

#Set to Y to get extensions.  This brings in string support, and stdint support
ENABLE_EXTENSIONS = N

# Set to Y for enabling debug
ENABLE_DEBUG = N

#---- Outputs ----#
COMPONENT_NAME = CppUTest
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
CPPUTEST_HOME = .
CPP_PLATFORM = Gcc

CPPFLAGS += -Wall -Werror

ifeq ($(ENABLE_DEBUG), Y)
CPPFLAGS += -g
endif

#CPPFLAGS += -DUT_NEW_OVERRIDES_DISABLED -DUT_NEW_MACROS_DISABLED
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
	
include $(CPPUTEST_HOME)/build/ComponentMakefile

.PHONY: test_all
test_all:
	make ENABLE_DEBUG=N ENABLE_EXTENSIONS=Y
	make examples ENABLE_EXTENSIONS=N
	make clean
	make ENABLE_DEBUG=N ENABLE_EXTENSIONS=N
	./$(TEST_TARGET) -ojunit > junit_run_output
#	if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	make clean
	make ENABLE_DEBUG=Y ENABLE_EXTENSIONS=Y
	make clean
	make ENABLE_DEBUG=Y ENABLE_EXTENSIONS=N
	make clean
	
.PHONY: examples
examples: 
	make -C examples  all
	
cleanExamples: 
	make -C examples clean 
	
