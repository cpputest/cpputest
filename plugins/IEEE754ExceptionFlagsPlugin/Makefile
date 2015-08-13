CPPUTEST_HOME := ../..
PLUGIN_HOME := .
AR := ar
CXX := g++
CC := gcc
CFLAGS := -g3 -Wextra -Wall -Werror \
          -I$(CPPUTEST_HOME)/include \
          -I$(PLUGIN_HOME)/include \

LPATH := -L$(CPPUTEST_HOME)/lib
CXXFLAGS := $(CFLAGS) -std=c++11
TESTS := FETests.exe
DEMO := FEDemoTests.exe
LIB := $(CPPUTEST_HOME)/lib/libIEEE754ExceptionFlagsPlugin.a
LIBS := -lCppUTest -lCppUTestExt -l$(patsubst lib%.a,%,$(notdir $(LIB)))

.PHONY: all check clean clobber demo 

all: check demo clobber

check: $(TESTS)
	./$< -v

demo: $(DEMO)
	 ! ./$< -v

lib: $(LIB)

$(LIB): src/IEEE754ExceptionFlagsPlugin.o
	$(AR) -r -s $@ $?

$(TESTS): tests/FETests.o tests/FETests_c.o $(LIB)
	$(CXX) $(LPATH) $? $(LIBS) -o $@

$(DEMO): examples/FEDemoTests.o $(LIB)
	$(CXX) $(LPATH) $? $(LIBS) -o $@

src/IEEE754ExceptionFlagsPlugin.o examples/FEDemoTests.o \
tests/FETests.o tests/FETests_c.o: Makefile

clobber:
	find . -name '*.o' -delete
	rm -f *.exe

clean: clobber
	rm -f $(LIB)
