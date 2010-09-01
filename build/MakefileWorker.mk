#---------
#
# MakefileWorker.mk
#
# Include this helper file in your makefile
# It makes
#    A static library
#    A test executable
#
# See this example for parameter settings 
#    examples/Makefile
#
#----------
# Inputs - these variables describe what to build
#
#   INCLUDE_DIRS - Directories used to search for include files.
#                   This generates a -I for each directory
#	SRC_DIRS - Directories containing source file to built into the library
#   SRC_FILES - Specific source files to build into library. Helpful when not all code 
#				in a directory can be built for test (hopefully a temporary situation)
#	TEST_SRC_DIRS - Directories containing unit test code build into the unit test runner
#				These do not go in a library. They are explicitly included in the test runner
#	MOCKS_SRC_DIRS - Directories containing mock source files to build into the test runner
#				These do not go in a library. They are explicitly included in the test runner
#----------
# You can adjust these variables to influence how to build the test target
# and where to put and name outputs
# See below to determine defaults
#   COMPONENT_NAME - the name of the thing being built
#   CPPUTEST_HOME - where CppUTest home dir found
#   CPPUTEST_OBJS_DIR - a directory where o and d files go
#   CPPUTEST_LIB_DIR - a directory where libs go
#   CPPUTEST_ENABLE_DEBUG - build for debug
#   CPPUTEST_USE_MEM_LEAK_DETECTION - Links with overridden new and delete
#   CPPUTEST_USE_STD_CPP_LIB - Set to N to keep the standard C++ library out
#		of the test harness
#   CPPUTEST_USE_GCOV - Turn on coverage analysis
#		Clean then build with this flag set to Y, then 'make gcov'
#   CPPUTEST_MAPFILE - generate a map file
#   CPPUTEST_WARNINGFLAGS - overly picky by default
#	OTHER_MAKEFILE_TO_INCLUDE - a hook to use this makefile to make 
#		other targets. Like CSlim, which is part of fitnesse
#----------
#
#  Other flags users can initialize to sneak in their settings
#	CPPUTEST_CXXFLAGS - flags for the C++ compiler
#	CPPUTEST_CPPFLAGS - flags for the C++ AND C compiler
#	CPPUTEST_CFLAGS - C complier
#	CPPUTEST_LDFLAGS - Linker flags
#----------


ifndef COMPONENT_NAME
    COMPONENT_NAME = name_this_in_the_makefile
endif

# Debug on by default
ifndef CPPUTEST_ENABLE_DEBUG
	CPPUTEST_ENABLE_DEBUG = Y
endif

# new and delete for memory leak detection on by default
ifndef CPPUTEST_USE_MEM_LEAK_DETECTION
	CPPUTEST_USE_MEM_LEAK_DETECTION = Y
endif

# Use the standard C++ library
ifndef CPPUTEST_USE_STD_CPP_LIB
	CPPUTEST_USE_STD_CPP_LIB = Y
endif

# Use gcov, off by default
ifndef CPPUTEST_USE_GCOV
	CPPUTEST_USE_GCOV = N
endif

# Default warnings
ifndef CPPUTEST_WARNINGFLAGS
	CPPUTEST_WARNINGFLAGS = -pedantic-errors -Wall -Wextra -Werror -Wshadow -Wswitch-default -Wswitch-enum -Wconversion
endif

# Default dir for temporary files (d, o)
ifndef CPPUTEST_OBJS_DIR
    CPPUTEST_OBJS_DIR = objs
endif

# Default dir for the outout library
ifndef CPPUTEST_LIB_DIR
    CPPUTEST_LIB_DIR = lib
endif

# No map by default
ifndef CPPUTEST_MAP_FILE
	CPPUTEST_MAP_FILE = N
endif

# No extentions is default
ifndef CPPUTEST_USE_EXTENSIONS
	CPPUTEST_USE_EXTENSIONS = N
endif

# --------------------------------------
# derived flags in the following area
# --------------------------------------
ifeq ($(CPPUTEST_USE_MEM_LEAK_DETECTION), N)
	CPPUTEST_CPPFLAGS += -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED
else
    ifndef CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE
	    	CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
    endif
    ifndef CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE
	    CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE = -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
	endif	
endif

ifeq ($(CPPUTEST_ENABLE_DEBUG), Y)
	CPPUTEST_CPPFLAGS += -g
endif

ifeq ($(CPPUTEST_USE_STD_CPP_LIB), N)
	CPPUTEST_CPPFLAGS += -DCPPUTEST_STD_CPP_LIB_DISABLED
	CPPUTEST_CXXFLAGS += -nostdinc++
endif

ifeq ($(CPPUTEST_USE_GCOV), Y)
	CPPUTEST_CPPFLAGS += -fprofile-arcs -ftest-coverage
endif

CPPUTEST_CPPFLAGS += $(CPPUTEST_WARNINGFLAGS)
CPPUTEST_CXXFLAGS += $(CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE)
CPPUTEST_CFLAGS += $(CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE)

TARGET_MAP = $(COMPONENT_NAME).map.txt
ifeq ($(CPPUTEST_MAP_FILE), Y)
	CPPUTEST_LDFLAGS += -Wl,-map,$(TARGET_MAP)
endif

# Link with CppUTest lib
CPPUTEST_LIB = $(CPPUTEST_HOME)/lib/libCppUTest.a 

ifeq ($(CPPUTEST_USE_EXTENSIONS), Y)
CPPUTEST_LIB += $(CPPUTEST_HOME)/lib/libCppUTestExt.a
endif

LD_LIBRARIES += -lstdc++ -lgcov

TARGET_LIB = \
    $(CPPUTEST_LIB_DIR)/lib$(COMPONENT_NAME).a
    
TEST_TARGET = \
    $(COMPONENT_NAME)_tests

#Helper Functions
get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))				        
src_to_o = $(subst .c,.o, $(subst .cpp,.o,$1))
src_to_d = $(subst .c,.d, $(subst .cpp,.d,$1))
change_o_file_location = $(patsubst %.o,$(CPPUTEST_OBJS_DIR)/%.o, $1)
change_d_file_location = $(patsubst %.d,$(CPPUTEST_OBJS_DIR)/%.d, $1)
src_to = $(subst .c,$1, $(subst .cpp,$1,$2))
time = $(shell date +%s)
delta_t = $(eval minus, $1, $2)

#Derived
STUFF_TO_CLEAN += $(TEST_TARGET) $(TEST_TARGET).exe $(TARGET_LIB) $(TARGET_MAP)

SRC += $(call get_src_from_dir_list, $(SRC_DIRS)) $(SRC_FILES)			        
OBJ = $(call change_o_file_location, $(call src_to_o,$(SRC)))

STUFF_TO_CLEAN += $(OBJ)

TEST_SRC = $(call get_src_from_dir_list, $(TEST_SRC_DIRS))
TEST_OBJS = $(call change_o_file_location, $(call src_to_o,$(TEST_SRC)))
STUFF_TO_CLEAN += $(TEST_OBJS)


MOCKS_SRC = $(call get_src_from_dir_list, $(MOCKS_SRC_DIRS))
MOCKS_OBJS = $(call change_o_file_location, $(call src_to_o,$(MOCKS_SRC)))
STUFF_TO_CLEAN += $(MOCKS_OBJS)

ALL_SRC = $(SRC) $(TEST_SRC) $(MOCKS_SRC)

#Test coverage with gcov
GCOV_OUTPUT = gcov_output.txt
GCOV_REPORT = gcov_report.txt
GCOV_ERROR = gcov_error.txt
GCOV_GCDA_FILES = $(call src_to,.gcda, $(ALL_SRC))
GCOV_GCNO_FILES = $(call src_to,.gcno, $(ALL_SRC))
TEST_OUTPUT = $(TEST_TARGET).txt
STUFF_TO_CLEAN += \
	$(GCOV_OUTPUT)\
	$(GCOV_REPORT)\
	$(GCOV_REPORT).html\
	$(GCOV_ERROR)\
	$(GCOV_GCDA_FILES)\
	$(GCOV_GCNO_FILES)\
	$(TEST_OUTPUT)


CPPUTEST_CPPFLAGS +=  $(INCLUDES) 

#The gcda files for gcov need to be deleted before each run
#To avoid annoying messages.
GCOV_CLEAN = $(SILENCE)rm -f $(GCOV_GCDA_FILES) $(GCOV_OUTPUT) $(GCOV_REPORT) $(GCOV_ERROR)
RUN_TEST_TARGET = $(SILENCE)  $(GCOV_CLEAN) ; echo "Running $(TEST_TARGET)"; ./$(TEST_TARGET) $(CPPUTEST_EXE_FLAGS)

INCLUDES_DIRS_EXPANDED = $(call get_dirs_from_dirspec, $(INCLUDE_DIRS))
INCLUDES += $(foreach dir, $(INCLUDES_DIRS_EXPANDED), -I$(dir))
MOCK_DIRS_EXPANDED = $(call get_dirs_from_dirspec, $(MOCKS_SRC_DIRS))
INCLUDES += $(foreach dir, $(MOCK_DIRS_EXPANDED), -I$(dir))


DEP_FILES = $(call src_to_d, $(ALL_SRC))
STUFF_TO_CLEAN += $(DEP_FILES) + $(PRODUCTION_CODE_START) + $(PRODUCTION_CODE_END)
STUFF_TO_CLEAN += $(STDLIB_CODE_START) + $(MAP_FILE) + cpputest_*.xml junit_run_output

# We'll use the CPPUTEST_CFLAGS etc so that you can override AND add to the CppUTest flags
CFLAGS = $(CPPUTEST_CFLAGS) $(CPPUTEST_ADDITIONAL_CFLAGS)
CPPFLAGS = $(CPPUTEST_CPPFLAGS) $(CPPUTEST_ADDITIONAL_CPPFLAGS)
CXXFLAGS = $(CPPUTEST_CXXFLAGS) $(CPPUTEST_ADDITIONAL_CXXFLAGS)
LDFLAGS = $(CPPUTEST_LDFLAGS) $(CPPUTEST_ADDITIONAL_LDFLAGS)

# Targets

.PHONY: all
all: start $(TEST_TARGET)  
	$(RUN_TEST_TARGET)	

.PHONY: start
start: $(TEST_TARGET) 
	$(SILENCE)START_TIME=$(call time)

.PHONY: all_no_tests
all_no_tests: $(TEST_TARGET)

.PHONY: flags
flags: 
	$(SILENCE)echo Compile with these flags:
	$(SILENCE)for f in $(CXXFLAGS) ; do \
		echo "    Cxx $$f" ; \
	done
	$(SILENCE)for f in $(CPPFLAGS) ; do \
		echo "    C++ $$f" ; \
	done
	$(SILENCE)for f in $(CFLAGS) ; do \
		echo "    C   $$f" ; \
	done
	$(SILENCE)for f in $(LDFLAGS) ; do \
		echo "    LD   $$f" ; \
	done
	$(SILENCE)for f in $(ARFLAGS) ; do \
		echo "    AR   $$f" ; \
	done
	
	
$(TEST_TARGET): $(TEST_OBJS) $(MOCKS_OBJS)  $(PRODUCTION_CODE_START) $(TARGET_LIB) $(USER_LIBS) $(PRODUCTION_CODE_END) $(CPPUTEST_LIB) $(STDLIB_CODE_START) 
	$(SILENCE)echo Linking $@
	$(SILENCE)$(LINK.o) -o $@ $^ $(LD_LIBRARIES)

$(TARGET_LIB): $(OBJ)
	$(SILENCE)echo Building archive $@
	$(SILENCE)mkdir -p lib
	$(SILENCE)$(AR) $(ARFLAGS) $@ $^
	$(SILENCE)ranlib $@

test: $(TEST_TARGET)
	$(RUN_TEST_TARGET) | tee $(TEST_OUTPUT)
	
vtest: $(TEST_TARGET)
	$(RUN_TEST_TARGET) -v  | tee $(TEST_OUTPUT)

$(CPPUTEST_OBJS_DIR)/%.o: %.cpp
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(COMPILE.cpp) -M -MF $(subst .o,.d,$@) -MT "$@ $(subst .o,.d,$@)" $<
	$(SILENCE)$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(CPPUTEST_OBJS_DIR)/%.o: %.c
	@echo compiling $(notdir $<)
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(COMPILE.c) -M -MF $(subst .o,.d,$@) -MT "$@ $(subst .o,.d,$@)" $<
	$(SILENCE)$(COMPILE.c) $(OUTPUT_OPTION) $<

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILES)
endif

.PHONY: clean
clean:
	$(SILENCE)echo Making clean
	$(SILENCE)$(RM) $(STUFF_TO_CLEAN)
	$(SILENCE)rm -rf gcov
	$(SILENCE)find . -name "*.gcno" | xargs rm -f
	$(SILENCE)find . -name "*.gcda" | xargs rm -f
	
#realclean gets rid of all gcov, o and d files in the directory tree
#not just the ones made by this makefile
.PHONY: realclean
realclean: clean
	$(SILENCE)rm -rf gcov
	$(SILENCE)find . -name "*.gdcno" | xargs rm -f
	$(SILENCE)find . -name "*.[do]" | xargs rm -f	

gcov: test
	$(SILENCE)for d in $(SRC_DIRS) ; do \
		gcov --object-directory $(CPPUTEST_OBJS_DIR)/$$d $$d/*.c $$d/*.cpp >> $(GCOV_OUTPUT) 2>>$(GCOV_ERROR) ; \
	done
	$(SILENCE)for f in $(SRC_FILES) ; do \
		gcov --object-directory $(CPPUTEST_OBJS_DIR)/$$f $$f >> $(GCOV_OUTPUT) 2>>$(GCOV_ERROR) ; \
	done
	$(CPPUTEST_HOME)/scripts/filterGcov.sh $(GCOV_OUTPUT) $(GCOV_ERROR) $(GCOV_REPORT) $(TEST_OUTPUT)
	$(SILENCE)cat $(GCOV_REPORT)
	$(SILENCE)mkdir -p gcov
	$(SILENCE)mv *.gcov gcov
	$(SILENCE)mv gcov_* gcov
	$(SILENCE)echo "See gcov directory for details"
 
.PHONEY: format
format: 
	$(CPPUTEST_HOME)/scripts/reformat.sh $(PROJECT_HOME_DIR)
	
debug:
	echo Stuff to clean
	$(SILENCE)for f in $(STUFF_TO_CLEAN) ; do \
		echo "$$f" ; \
	done
	echo Includes
	$(SILENCE)for i in $(INCLUDES) ; do \
		echo "$$i" ; \
	done

ifneq "$(OTHER_MAKEFILE_TO_INCLUDE)" ""
-include $(OTHER_MAKEFILE_TO_INCLUDE)
endif

	
	