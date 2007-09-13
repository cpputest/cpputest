
all: compile test

compile:
	$(MAKE) -C ApplicationLib all
	$(MAKE) -C lib all
	$(MAKE) -C AllTests all
 
test: all
	$(MAKE) -s -C AllTests test
 
clean:
	$(MAKE) -C ApplicationLib clean
	$(MAKE) -C AllTests clean
	rm -f lib/*.a
	rm -f test/*.*

depend:
	$(MAKE) -C ApplicationLib depend
	$(MAKE) -C AllTests depend
