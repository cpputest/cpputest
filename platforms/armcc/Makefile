# We don't need to clean up when we're making these targets
NODEPS  := clean debug

# CPU architecture. Passing to armcc.exe. For list: armcc --cpu list
ifndef CPU
	CPU = ARM7TDMI
endif

# ARM/THUMB mode. Passing to armcc.exe. May be 'thumb' or 'arm'
ifndef CPU_MODE
	CPU_MODE = thumb
endif

# Path to cygwin binary
CYGWIN_DIR :=C:/CYGWIN/bin

# Path to ARMCC of Keil MDK-ARM, with libraries, includes and executables
KEIL_DIR   :=D:/Keil/ARM/ARMCC

MKDIR      :=$(CYGWIN_DIR)/mkdir
RM         :=$(CYGWIN_DIR)/rm
CC         :=@$(KEIL_DIR)/bin/armcc.exe
AR         :=@$(KEIL_DIR)/bin/armar.exe


# source CppUTest files
CPPUTEST_HOME := ../..
COMPONENT_NAME := CppUTest

INCLUDE_DIRS :=\
  $(CPPUTEST_HOME)/include \
  $(CPPUTEST_HOME)/include/Platforms/armcc \

# armcc system include path
SYS_INCLUDE_DIRS:=$(KEIL_DIR)/include 

SRC_DIRS :=\
  $(CPPUTEST_HOME)/src/$(COMPONENT_NAME)\
  $(CPPUTEST_HOME)/src/Platforms/armcc\

TARGET_PLATFORM :=armcc_$(CPU)_$(CPU_MODE)
CPPUTEST_USE_MEM_LEAK_DETECTION := N
CPPUTEST_USE_STD_CPP_LIB := N
CPPUTEST_USE_VPATH := Y
CPPUTEST_USE_STD_C_LIB := Y
CPPUTEST_ENABLE_DEBUG := Y


# Default dir for temporary files (d, o)
# Default dir for the outout library
CPPUTEST_OBJS_DIR = objs
CPPUTEST_LIB_DIR = $(CPPUTEST_HOME)/lib/$(TARGET_PLATFORM)

TARGET_LIB = \
    $(CPPUTEST_LIB_DIR)/lib$(COMPONENT_NAME).a


#Helper Functions
get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.cc) $(wildcard $1/*.c)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
__src_to = $(subst .c,$1, $(subst .cc,$1, $(subst .cpp,$1,$(if $(CPPUTEST_USE_VPATH),$(notdir $2),$2))))
src_to = $(addprefix $(CPPUTEST_OBJS_DIR)/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1)
src_to_d = $(call src_to,.d,$1)
src_to_gcda = $(call src_to,.gcda,$1)
src_to_gcno = $(call src_to,.gcno,$1)
time = $(shell date +%s)
delta_t = $(eval minus, $1, $2)
debug_print_list = $(foreach word,$1,echo "  $(word)";) echo;

#Derived
SRC = $(call get_src_from_dir_list, $(SRC_DIRS))
OBJ = $(call src_to_o,$(SRC))

STUFF_TO_CLEAN = $(OBJ)

ALL_SRC = $(SRC)

CPUFLAGS =--cpu=$(CPU)
ifeq ($(CPU_MODE), thumb)
  CPUFLAGS +=--apcs=/interwork
endif
DEPFLAGS =$(CPUFLAGS) -M $(INCLUDES) --depend_format=unix_escaped --depend_single_line --no_depend_system_headers
OPTFLAGS =-O1
CCFLAGS =$(CPUFLAGS) $(OPTFLAGS) -c $(INCLUDES)\
  --$(CPU_MODE)\
  --exceptions\
  -D__CLK_TCK=1000\


# If we're using VPATH
ifeq ($(CPPUTEST_USE_VPATH), Y)
# gather all the source directories and add them
	VPATH += $(sort $(dir $(ALL_SRC)))
# Add the component name to the objs dir path, to differentiate between same-name objects
	CPPUTEST_OBJS_DIR := $(addsuffix /$(COMPONENT_NAME),$(CPPUTEST_OBJS_DIR))
endif

INCLUDES_DIRS_EXPANDED = $(call get_dirs_from_dirspec, $(INCLUDE_DIRS))
INCLUDES += $(foreach dir, $(INCLUDES_DIRS_EXPANDED), -I$(dir))

DEP_FILES = $(call src_to_d, $(ALL_SRC))
STUFF_TO_CLEAN += $(DEP_FILES)


# Without the C library, we'll need to disable the C++ library and ... 
ifeq ($(CPPUTEST_USE_STD_C_LIB), N)
	CPPUTEST_USE_STD_CPP_LIB = N
	CPPUTEST_USE_MEM_LEAK_DETECTION = N
	CCFLAGS += -DCPPUTEST_STD_C_LIB_DISABLED
else
	INCLUDE_DIRS +=$(SYS_INCLUDE_DIRS)
endif

ifeq ($(CPPUTEST_USE_MEM_LEAK_DETECTION), N)
	CCFLAGS += -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED
endif

ifeq ($(CPPUTEST_ENABLE_DEBUG), Y)
	CCFLAGS += -g
	ARFLAGS := --debug_symbols
endif

ifeq ($(CPPUTEST_USE_STD_CPP_LIB), N)
	CCFLAGS += -DCPPUTEST_STD_CPP_LIB_DISABLED
endif


OBJS_DIRS :=$(sort $(dir $(STUFF_TO_CLEAN)))

$(OBJS_DIRS) $(CPPUTEST_LIB_DIR):
	@echo Updating directory $@
	$(MKDIR) -p $@


#This is the rule for creating the dependency files
$(CPPUTEST_OBJS_DIR)/%.d: %.c Makefile | $(OBJS_DIRS)
	@echo Compiling C file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(CPPUTEST_OBJS_DIR)/%.d: %.cpp Makefile | $(OBJS_DIRS)
	@echo Compiling C++ file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

#This rule does the compilation C++ files
$(CPPUTEST_OBJS_DIR)/%.o: %.cpp $(CPPUTEST_OBJS_DIR)/%.d Makefile
	@echo Compiling C++ file $<. Out file $@
	$(CC) $(CCFLAGS) $< -o $@

#This rule does the compilation C files
$(CPPUTEST_OBJS_DIR)/%.o: %.c $(CPPUTEST_OBJS_DIR)/%.d Makefile
	@echo Compiling C file $<. Out file $@
	$(CC) $(CCFLAGS) $< -o $@


$(TARGET_LIB): $(OBJ) | $(CPPUTEST_LIB_DIR)
	@echo Archiving to $@
	$(AR) $(ARFLAGS) --create $@ $^


.PHONY: all

all: $(TARGET_LIB)
	@echo Done!


.PHONY: debug
debug:
	@echo
	@echo "Target Source files:"
	@$(call debug_print_list,$(SRC))
	@echo "Target Object files:"
	@$(call debug_print_list,$(OBJ))
	@echo "All Input Dependency files:"
	@$(call debug_print_list,$(DEP_FILES))
	@echo Stuff to clean:
	@$(call debug_print_list,$(STUFF_TO_CLEAN))
	@echo Includes:
	@$(call debug_print_list,$(INCLUDES))
	@echo Directories to create:
	@$(call debug_print_list,$(OBJS_DIRS))
	@echo Directories of CppUTest object files:
	@$(call debug_print_list,$(CPPUTEST_OBJS_DIR))
	@echo Flags:
	@$(call debug_print_list,$(CCFLAGS))


#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

clean:
	@$(RM) -rf $(STUFF_TO_CLEAN)
