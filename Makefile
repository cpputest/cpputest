#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Inputs ----#
COMPONENT_NAME = CppUTest
CPP_PLATFORM = Gcc
CPPUTEST_HOME = .
OLD_MAKE = oldmake

SRC_DIRS = \
	src/CppUTest \
	src/Platforms/$(CPP_PLATFORM)
	
TEST_SRC_DIRS = \
	tests

INCLUDE_DIRS =\
  include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

#these are a sample of the other alternative flag settings
.PHONY: test_all
test_all: start test_old_make
	$(SILENCE)echo Building with the default flags.
	make clean
	$(TIME) make 
	./$(TEST_TARGET) -r
	make clean
	$(SILENCE)echo Building with the STDC++ new disabled. 
	$(TIME) make CPPUTEST_USE_STD_CPP_LIB=Y extensions 
	make CPPUTEST_USE_STD_CPP_LIB=Y cleanExtensions
	$(SILENCE)echo Building with Memory Leak Detection disabled
	$(TIME) make CPPUTEST_USE_MEM_LEAK_DETECTION=N extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N cleanExtensions
	$(SILENCE)echo Building with Memory Leak Detection disabled and STD C++ disabled
	$(TIME) make CPPUTEST_USE_MEM_LEAK_DETECTION=N CPPUTEST_USE_STD_CPP_LIB=Y extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N CPPUTEST_USE_STD_CPP_LIB=Y cleanExtensions
	$(SILENCE)echo Building with debug disabled
	$(TIME) make CPPUTEST_ENABLE_DEBUG=N extensions
	make CPPUTEST_ENABLE_DEBUG=N cleanExtensions
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and CPPFLAGS
	$(TIME) make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude"
	make CFLAGS="" CXXFLAGS="" clean
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and memory leak and STDC++ disabled
	$(TIME) make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED"
	make CFLAGS="" CXXFLAGS="" CPPFLAGS="-DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED" clean
	$(SILENCE)echo Building examples 
	make cleanExamples
	$(TIME) make examples
	make cleanExamples
	$(SILENCE)echo Testing JUnit output
	$(TIME) make
	$(SILENCE)./$(TEST_TARGET) -ojunit > junit_run_output
	$(SILENCE)if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	make clean
	make CPPUTEST_MAP_FILE=map.txt
	make clean
	$(TIME) make CPPUTEST_USE_GCOV=Y
	make gcov
	make clean
	$(SILENCE)echo Testing VPATH usage
	$(TIME) make CPPUTEST_USE_VPATH=Y everythingInstall
	make CPPUTEST_USE_VPATH=Y cleanEverythingInstall
	
HAS_OLD_MAKE = $(shell $(OLD_MAKE) -v 2>/dev/null)

test_old_make:
	$(SILENCE)if [ "$(HAS_OLD_MAKE)" = "" ]; then \
		echo "Old make with the name $(OLD_MAKE) not found. Skipping testing with old make version"; \
	else \
		$(OLD_MAKE) -f Makefile_for_old_make clean && \
		$(OLD_MAKE) -f Makefile_for_old_make && \
		$(OLD_MAKE) -f Makefile_for_old_make extensions && \
		$(OLD_MAKE) -f Makefile_for_old_make clean; \
	fi

.PHONY: examples
examples: $(TEST_TARGET) extensions
	$(TIME) make -C examples  all

extensions: $(TEST_TARGET)
	$(TIME) make -f Makefile_CppUTestExt all CPPUTEST_USE_STD_CPP_LIB=$(CPPUTEST_USE_STD_CPP_LIB) CPPUTEST_USE_MEM_LEAK_DETECTION=$(CPPUTEST_USE_MEM_LEAK_DETECTION)

cleanExtensions: clean
	$(TIME) make -f Makefile_CppUTestExt clean CPPUTEST_USE_STD_CPP_LIB=$(CPPUTEST_USE_STD_CPP_LIB) CPPUTEST_USE_MEM_LEAK_DETECTION=$(CPPUTEST_USE_MEM_LEAK_DETECTION)
	
cleanExamples: clean cleanExtensions
	$(TIME) make -C examples clean 

.PHONY: everythingInstall
everythingInstall: all extensions examples

.PHONY: cleanEverythingInstall
cleanEverythingInstall: clean cleanExtensions cleanExamples

