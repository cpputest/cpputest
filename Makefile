#Set this to @ to keep the makefile quiet
SILENCE = @

#Set to Y to get extensions.  This brings in string support, and stdint support
ENABLE_EXTENSIONS = Y

#---- Outputs ----#
COMPONENT_NAME = CppUTest
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a
	
TEST_TARGET = \
	$(COMPONENT_NAME)_tests

#--- Inputs ----#
CPPUTEST_HOME = .
CPP_PLATFORM = Gcc

CPPFLAGS += -Wall
CPPFLAGS += -Werror
CPPFLAGS += -g
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
	tests\
	tests/Extensions

#includes for all compiles	
INCLUDES =\
  -I.\
  -I$(CPPUTEST_HOME)/include

#Add extension directories if enabled
ifeq ($(ENABLE_EXTENSIONS), Y)
SRC_DIRS +=	src/CppUTest/Extensions
TEST_SRC_DIRS += tests/Extensions 
endif

#Flags to pass to ar, ld
LDFLAGS +=
	
include $(CPPUTEST_HOME)/build/ComponentMakefile

.PHONY: examples
examples: 
	make -C examples  all
	
cleanExamples: 
	make -C examples clean 
	