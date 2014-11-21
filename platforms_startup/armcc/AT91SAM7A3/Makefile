# Makefile to create startup library for AT91SAM7A3 chip

.SILENT:

CPU = ARM7TDMI
CPU_MODE = thumb
TARGET_PLATFORM =armcc_$(CPU)_$(CPU_MODE)

CPPUTEST_HOME = ../../..

# component name
COMPONENT_NAME = Startup_AT91SAM7A3

# directory to place artefacts
OBJS_DIR = objs

# directory to place startup library
LIB_DIR = $(CPPUTEST_HOME)/lib/$(TARGET_PLATFORM)

# directory with source files
SRC_DIRS = .

# directory with header files
INCLUDE_DIRS = .

# CYGWIN path
CYGWIN =C:/CYGWIN/bin
MKDIR  =$(CYGWIN)/mkdir
RM     =$(CYGWIN)/rm
TOUCH  =$(CYGWIN)/touch

KEIL_DIR=D:/Keil/ARM/ARMCC
CC=$(KEIL_DIR)/bin/armcc.exe
AS=$(KEIL_DIR)/bin/armasm.exe
AR=$(KEIL_DIR)/bin/armar.exe
# armcc system include path
SYS_INCLUDE_DIRS =$(KEIL_DIR)/include 

# adding Keil MDK-ARM system headers to build startup library
INCLUDE_DIRS +=$(SYS_INCLUDE_DIRS)

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
 -D__CLK_TCK=1000 \

ARFLAGS = --debug_symbols


TARGET_LIB = \
    $(LIB_DIR)/lib$(COMPONENT_NAME).a

#Helper Functions
get_src_from_dir  = $(wildcard $1/*.c) $(wildcard $1/*.asm)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
__src_to = $(subst .asm,$1, $(subst .c,$1,$2))
src_to = $(addprefix $3/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1,$2)
src_to_d = $(call src_to,.d,$1,$2)
time = $(shell date +%s)
delta_t = $(eval minus, $1, $2)
debug_print_list = $(foreach word,$1,echo "  $(word)";) echo

# for building CRT library
SRC = $(call get_src_from_dir_list, $(SRC_DIRS))
OBJ = $(call src_to_o,$(SRC),$(OBJS_DIR))
INCLUDES += $(foreach dir, $(INCLUDE_DIRS), -I$(dir))
DEP_FILES = $(call src_to_d, $(SRC), $(OBJS_DIR))
STUFF_TO_CLEAN = $(OBJ) $(DEP_FILES)

#Don't create CRT dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

all: $(TARGET_LIB)
	echo Done!

$(TARGET_LIB): $(OBJ) | $(LIB_DIR)
	$(AR) $(ARFLAGS) --create $@ $^
	echo Build CRT library done!

$(LIB_DIR) $(OBJS_DIR):
	echo Updating directory $@
	$(MKDIR) -p $@

#This is the rule for creating the dependency files
$(OBJS_DIR)/%.d: %.c Makefile | $(OBJS_DIR)
	echo Compiling C file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(OBJS_DIR)/%.d: %.asm | $(OBJS_DIR)
	echo Compiling ASM file $< for dependency. Out file $@.
	$(TOUCH) $@

#This rule does the compilation C files
$(OBJS_DIR)/%.o: %.c $(OBJS_DIR)/%.d
	echo Compiling C file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

#This rule does the compilation ASM
$(OBJS_DIR)/%.o: %.asm $(OBJS_DIR)/%.d
	echo Assembling file $<. Out file $@
	$(AS) $(CPUFLAGS) --diag-suppress 1786 $< -o $@


clean:
	@$(RM) -f $(STUFF_TO_CLEAN)

