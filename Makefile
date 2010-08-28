#Set this to @ to keep the makefile quiet
SILENCE = @

#--- Inputs ----#
COMPONENT_NAME = CppUTest
CPP_PLATFORM = Gcc
ifndef CPPUTEST_HOME
	CPPUTEST_HOME = .
endif

SRC_DIRS = \
	src/CppUTest \
	src/Platforms/$(CPP_PLATFORM)
	
TEST_SRC_DIRS = \
	tests

INCLUDE_DIRS =\
  $(CPPUTEST_HOME)/include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

#these are a sample of the other alternative flag settings
.PHONY: test_all
test_all:
	$(SILENCE)echo Building with the default flags.
	make
	./$(TEST_TARGET) -r
	make clean
	$(SILENCE)echo Building with the STDC++ new disabled. 
	make CPPUTEST_USE_STD_CPP_LIB=Y extensions 
	make CPPUTEST_USE_STD_CPP_LIB=Y cleanExtensions
	$(SILENCE)echo Building with Memory Leak Detection disabled
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N cleanExtensions
	$(SILENCE)echo Building with Memory Leak Detection disabled and STD C++ disabled
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N CPPUTEST_USE_STD_CPP_LIB=Y extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION=N CPPUTEST_USE_STD_CPP_LIB=Y cleanExtensions
	$(SILENCE)echo Building with malloc Memory Leak Detection disabled
	make CPPUTEST_USE_MEM_LEAK_DETECTION_FOR_MALLOC=N extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION_FOR_MALLOC=N cleanExtensions
	$(SILENCE)echo Building with malloc Memory Leak Detection disabled and STD C++ disabled
	make CPPUTEST_USE_MEM_LEAK_DETECTION_FOR_MALLOC=N CPPUTEST_USE_STD_CPP_LIB=Y extensions
	make CPPUTEST_USE_MEM_LEAK_DETECTION_FOR_MALLOC=N CPPUTEST_USE_STD_CPP_LIB=Y cleanExtensions
	$(SILENCE)echo Building with debug disabled
	make CPPUTEST_ENABLE_DEBUG=N extensions
	make CPPUTEST_ENABLE_DEBUG=N cleanExtensions
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and CPPFLAGS
	make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude"
	make CFLAGS="" CXXFLAGS="" clean
	$(SILENCE)echo Building with overridden CXXFLAGS and CFLAGS and memory leak and STDC++ disabled
	make CLFAGS="" CXXFLAGS="" CPPFLAGS="-Iinclude -DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED"
	make CFLAGS="" CXXFLAGS="" CPPFLAGS="-DCPPUTEST_STD_CPP_LIB_DISABLED -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED" clean
	$(SILENCE)echo Building examples 
	make cleanExamples
	make examples
	make cleanExamples
	$(SILENCE)echo Testing JUnit output
	make
	$(SILENCE)./$(TEST_TARGET) -ojunit > junit_run_output
	$(SILENCE)if [ -s junit_run_output ]; then echo "JUnit run has output. Build failed!"; exit 1; fi
	make clean
	make CPPUTEST_MAP_FILE=map.txt
	make clean
	make CPPUTEST_USE_GCOV=Y
	make gcov
	make clean
	
.PHONY: examples
examples: $(TEST_TARGET) extensions
	make -C examples  all

extensions: $(TEST_TARGET)
	make -f Makefile_CppUTestExt all CPPUTEST_USE_STD_CPP_LIB=$(CPPUTEST_USE_STD_CPP_LIB) CPPUTEST_USE_MEM_LEAK_DETECTION=$(CPPUTEST_USE_MEM_LEAK_DETECTION)

cleanExtensions: clean
	make -f Makefile_CppUTestExt clean CPPUTEST_USE_STD_CPP_LIB=$(CPPUTEST_USE_STD_CPP_LIB) CPPUTEST_USE_MEM_LEAK_DETECTION=$(CPPUTEST_USE_MEM_LEAK_DETECTION)
	
cleanExamples: clean cleanExtensions
	make -C examples clean 
	
