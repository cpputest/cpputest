export CPP_U_TEST=$(shell pwd)/../CppUTest
export CPP_PLATFORM=Gcc
export APP_CXXFLAGS= -Wall

all: compile test

compile:
	$(MAKE) -C Project all
	$(MAKE) -C AllTests all
 
test: all
	$(MAKE) -s -C AllTests test
 
clean:
	$(MAKE) -C Project clean
	$(MAKE) -C AllTests clean
	rm -f lib/*.a
	rm -f test/*.*

depend:
	$(MAKE) -C Project depend
	$(MAKE) -C AllTests depend
