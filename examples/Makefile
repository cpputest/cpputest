#---------
#
# CppUTest Examples Makefile
#
#----------

#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Inputs ----#
COMPONENT_NAME = CppUTestExamples
ifndef CPPUTEST_HOME
	CPPUTEST_HOME = ..
endif

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc

SRC_DIRS = \
	ApplicationLib

TEST_SRC_DIRS = \
	AllTests

INCLUDE_DIRS =\
  .\
  ApplicationLib\
  $(CPPUTEST_HOME)/include\

include $(CPPUTEST_HOME)/build/MakefileWorker.mk


