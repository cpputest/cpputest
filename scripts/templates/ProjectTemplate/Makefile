export CPP_U_TEST=CppUTestEnvironmentVariable
export CPP_PLATFORM=Gcc
export APP_CXXFLAGS= -Wall

DIRS = Project AllTests

all: compile

compile:
	for d in $(DIRS) ; do \
		$(MAKE) -C $$d all ; \
	done
	 
test: all
	$(MAKE) -s -C AllTests test
 
clean:
	for d in $(DIRS) ; do \
		$(MAKE) -C $$d clean ; \
	done
	rm -f lib/*.a
	rm -f test/*.*

depend:
	for d in $(DIRS) ; do \
		$(MAKE) -C $$d depend ; \
	done
