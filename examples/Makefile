export CPP_U_TEST = $(shell pwd)/..

all:
	$(MAKE) -C ApplicationLib all
	$(MAKE) -C lib all
	$(MAKE) -C AllTests all test
 
test: all
	$(MAKE) -C AllTests test
 
clean:
	$(MAKE) -C ApplicationLib clean
	$(MAKE) -C AllTests clean
	rm -f lib/*.a
	rm -f test/*.*

depend:
	$(MAKE) -C ApplicationLib depend
	$(MAKE) -C AllTests depend
