.SILENT:

#We don't need to clean up when we're making these targets
NODEPS  = clean

# CPU architecture. Passing to armcc.exe. For list: armcc --cpu list
ifndef CPU
	CPU = ARM7TDMI
endif

# ARM/THUMB mode. Passing to armcc.exe. May be 'thumb' or 'arm'
ifndef CPU_MODE
	CPU_MODE = thumb
endif

TARGET_PLATFORM =armcc_$(CPU)_$(CPU_MODE)

# Need to use only relative path! Path with logical letters does not working.
CPPUTEST_HOME = ../../..

COMPONENT_NAME = app

CPPUTEST_OBJS_DIR = objs

# directory for appication library
CPPUTEST_LIB_DIR = lib

CPPUTEST_USE_MEM_LEAK_DETECTION = N
CPPUTEST_USE_STD_CPP_LIB = Y
CPPUTEST_USE_VPATH = Y
CPPUTEST_USE_STD_C_LIB = Y
CPPUTEST_ENABLE_DEBUG = Y
CPPUTEST_USE_EXTENSIONS = N

SRC_DIRS = \

SRC_FILES = $(CPPUTEST_HOME)/examples/ApplicationLib/CircularBuffer.cpp \
  $(CPPUTEST_HOME)/examples/ApplicationLib/Printer.cpp

TEST_SRC_DIRS = \
	tests \

TEST_FILES = $(CPPUTEST_HOME)/examples/AllTests/CircularBufferTest.cpp


# directory with CppUTest and startup libraries
CPPUTEST_LIB_LINK_DIR = $(CPPUTEST_HOME)/lib/$(TARGET_PLATFORM)

INCLUDE_DIRS = \
  $(CPPUTEST_HOME)/include \
  $(SRC_DIRS) \
  $(CPPUTEST_HOME)/examples/ApplicationLib \

# name of extension of output file. Default Keil MDK-ARM extension is .axf which is elf format
# .axf will be .elf for eclipse debug
OUTFILE_EXT = elf

# output filename, mapfile (.map), hexfile (.mot)
OUTNAME = $(COMPONENT_NAME)
OUTFILE = $(OUTNAME).$(OUTFILE_EXT)
HEXFILE = $(OUTNAME).mot
MAPFILE = $(OUTNAME).map

# CYGWIN path
CYGWIN =C:/CYGWIN/bin
MKDIR  =$(CYGWIN)/mkdir
RM     =$(CYGWIN)/rm
TOUCH  =$(CYGWIN)/touch
ECHO   =$(CYGWIN)/echo

KEIL_DIR=D:/Keil/ARM/ARMCC
CC=$(KEIL_DIR)/bin/armcc.exe
AS=$(KEIL_DIR)/bin/armasm.exe
AR=$(KEIL_DIR)/bin/armar.exe
LD=$(KEIL_DIR)/bin/armlink.exe
FROMELF=$(KEIL_DIR)/bin/fromelf.exe
# armcc system include path
SYS_INCLUDE_DIRS =$(KEIL_DIR)/include 

JFLASH ="C:/Program Files (x86)/SEGGER/JLink_V490b/JFlash.exe"
JFLASHPRJ =AT91SAM7A3.jflash
LDSCRIPT = ROM.sct

CPUFLAGS =--cpu=$(CPU)
ifeq ($(CPU_MODE), thumb)
  CPUFLAGS +=--apcs=/interwork
endif
DEPFLAGS =-M \
 $(INCLUDES) \
 --depend_format=unix_escaped \
 --depend_single_line \
 --no_depend_system_headers

OPTFLAGS =-O3

CPPUTEST_CPPFLAGS =$(CPUFLAGS) \
 $(OPTFLAGS) \
 -c \
 -g \
 $(INCLUDES) \
 --$(CPU_MODE) \
 --exceptions \
 -D__CLK_TCK=1000 \

CPPUTEST_LDFLAGS =$(CPUFLAGS) \
 --strict \
 --entry=Reset_Handler \
 --summary_stderr \
 --map \
 --callgraph \
 --info=summarysizes \
 --info=sizes \
 --info=totals \
 --info=veneers\
 --load_addr_map_info \
 --library_type=standardlib \
 --remove \
 --debug \

ARFLAGS = --debug_symbols


# new and delete for memory leak detection on by default
ifndef CPPUTEST_USE_MEM_LEAK_DETECTION
	CPPUTEST_USE_MEM_LEAK_DETECTION = Y
endif

# Use the standard C library
ifndef CPPUTEST_USE_STD_C_LIB
	CPPUTEST_USE_STD_C_LIB = Y
endif

# Use the standard C++ library
ifndef CPPUTEST_USE_STD_CPP_LIB
	CPPUTEST_USE_STD_CPP_LIB = Y
endif

# No extentions is default
ifndef CPPUTEST_USE_EXTENSIONS
	CPPUTEST_USE_EXTENSIONS = N
endif

# No VPATH is default
ifndef CPPUTEST_USE_VPATH
	CPPUTEST_USE_VPATH = N
endif
# Make empty, instead of 'N', for usage in $(if ) conditionals
ifneq ($(CPPUTEST_USE_VPATH), Y)
	CPPUTEST_USE_VPATH =
endif

# Without the C library, we'll need to disable the C++ library and ... 
ifeq ($(CPPUTEST_USE_STD_C_LIB), N)
	CPPUTEST_USE_STD_CPP_LIB = N
	CPPUTEST_USE_MEM_LEAK_DETECTION = N
	CPPUTEST_CPPFLAGS += -DCPPUTEST_STD_C_LIB_DISABLED
	CPPUTEST_LDFLAGS +=--no_scanlib
else
	INCLUDE_DIRS +=$(SYS_INCLUDE_DIRS)
endif


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


ifeq ($(CPPUTEST_USE_STD_CPP_LIB), N)
	CPPUTEST_CPPFLAGS += -DCPPUTEST_STD_CPP_LIB_DISABLED
endif

# Link with CppUTest lib
CPPUTEST_LIB = $(CPPUTEST_LIB_LINK_DIR)/libCppUTest.a

# Link with startup lib
CPPUTEST_LIB += $(CPPUTEST_LIB_LINK_DIR)/libStartup_AT91SAM7A3.a

# Link with CppUTestExt lib
ifeq ($(CPPUTEST_USE_EXTENSIONS), Y)
CPPUTEST_LIB += $(CPPUTEST_LIB_LINK_DIR)/libCppUTestExt.a
endif

TARGET_LIB = \
    $(CPPUTEST_LIB_DIR)/lib$(COMPONENT_NAME).a

ifndef TEST_TARGET
	ifndef TARGET_PLATFORM
		TEST_TARGET = $(COMPONENT_NAME)_tests
	else
		TEST_TARGET = $(COMPONENT_NAME)_$(TARGET_PLATFORM)_tests
	endif
endif

#Helper Functions
get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.cc) $(wildcard $1/*.c) $(wildcard $1/*.asm)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
__src_to = $(subst .c,$1, $(subst .cc,$1, $(subst .cpp,$1, $(if $(CPPUTEST_USE_VPATH),$(notdir $2),$2))))
src_to = $(addprefix $3/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1,$2)
src_to_d = $(call src_to,.d,$1,$2)
time = $(shell date +%s)
delta_t = $(eval minus, $1, $2)
debug_print_list = $(foreach word,$1,echo "  $(word)";) echo

# исходники программных модулей
SRC = $(call get_src_from_dir_list, $(SRC_DIRS)) $(SRC_FILES)
OBJ = $(call src_to_o,$(SRC),$(CPPUTEST_OBJS_DIR))
# исходники тестов
TEST_SRC = $(call get_src_from_dir_list, $(TEST_SRC_DIRS)) $(TEST_FILES)
TEST_OBJ = $(call src_to_o,$(TEST_SRC),$(CPPUTEST_OBJS_DIR))

# If we're using VPATH
ALL_SRC = $(SRC) $(TEST_SRC)
ifeq ($(CPPUTEST_USE_VPATH), Y)
# gather all the source directories and add them
	VPATH += $(sort $(dir $(ALL_SRC)))
# Add the component name to the objs dir path, to differentiate between same-name objects
	CPPUTEST_OBJS_DIR := $(addsuffix /$(COMPONENT_NAME),$(CPPUTEST_OBJS_DIR))
endif

# for building application library
INCLUDES += $(foreach dir, $(INCLUDE_DIRS), -I$(dir))
DEP_FILES = $(call src_to_d, $(SRC), $(CPPUTEST_OBJS_DIR))
DEP_TEST_FILES = $(call src_to_d, $(TEST_SRC), $(CPPUTEST_OBJS_DIR))
STUFF_TO_CLEAN = $(OBJ) $(TEST_OBJ) $(DEP_FILES) $(DEP_TEST_FILES) $(TARGET_LIB)

#Don't create CRT dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

STUFF_TO_CLEAN += $(OUTFILE) $(MAPFILE)


all: $(CPPUTEST_OBJS_DIR)/$(OUTFILE)
	echo Done!

$(TARGET_LIB): $(OBJ) | $(CPPUTEST_LIB_DIR)
	echo Building application library $@
	$(AR) $(ARFLAGS) --create $@ $^

$(CPPUTEST_OBJS_DIR)/$(OUTFILE): $(TEST_OBJ) $(CPPUTEST_LIB) $(TARGET_LIB) | ROM.sct Makefile
	echo Linking!
	$(LD) $(CPPUTEST_LDFLAGS) $^ --scatter $(LDSCRIPT) --list $(MAPFILE) -o $@


.PHONY: prog
prog:	$(CPPUTEST_OBJS_DIR)/$(HEXFILE)
	$(JFLASH) -openprj$(JFLASHPRJ) -open$< -auto -exit 

$(CPPUTEST_OBJS_DIR)/$(HEXFILE): $(CPPUTEST_OBJS_DIR)/$(OUTFILE)
	echo Converting to Motorola S19
	$(FROMELF) --m32 --output=$@ $<

debug:
	echo
	echo "Target Source files:"
	$(call debug_print_list,$(SRC))
	echo "Target Object files:"
	$(call debug_print_list,$(OBJ))
	echo "All Input Dependency files:"
	$(call debug_print_list,$(DEP_FILES))
	echo Stuff to clean:
	$(call debug_print_list,$(STUFF_TO_CLEAN))
	echo Includes:
	$(call debug_print_list,$(INCLUDES))
	echo Directories to create:
	$(call debug_print_list,$(OBJS_DIRS))
	echo Directories of CppUTest object files:
	$(call debug_print_list,$(OBJS_DIR))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

OBJS_DIRS =$(sort $(dir $(STUFF_TO_CLEAN)))

$(LIB_DIR) $(OBJS_DIRS):
	echo Updating directory $@
	$(MKDIR) -p $@

#This is the rule for creating the dependency files
$(CPPUTEST_OBJS_DIR)/%.d: %.c Makefile | $(OBJS_DIRS)
	echo Compiling C file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(CPPUTEST_OBJS_DIR)/%.d: %.cpp Makefile | $(OBJS_DIRS)
	echo Compiling C++ file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(CPPUTEST_OBJS_DIR)/%.d: %.cc Makefile | $(OBJS_DIRS)
	echo Compiling CC++ file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

#This rule does the compilation C++ files
$(CPPUTEST_OBJS_DIR)/%.o: %.cpp $(CPPUTEST_OBJS_DIR)/%.d
	echo Compiling C++ file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

#This rule does the compilation CC++ files
$(CPPUTEST_OBJS_DIR)/%.o: %.cc $(CPPUTEST_OBJS_DIR)/%.d
	echo Compiling CC++ file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

#This rule does the compilation C files
$(CPPUTEST_OBJS_DIR)/%.o: %.c $(CPPUTEST_OBJS_DIR)/%.d
	echo Compiling C file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

clean:
	$(RM) -f $(STUFF_TO_CLEAN)


