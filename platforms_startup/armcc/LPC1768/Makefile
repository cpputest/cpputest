CPU := Cortex-M3
CPU_MODE := thumb
TARGET_PLATFORM :=armcc_$(CPU)_$(CPU_MODE)

CPPUTEST_HOME := ../../..

# имя компонента
COMPONENT_NAME := Startup_LPC1768

# промежуточный каталог, где производится компиляция startup-файлов
OBJS_DIR = objs

# каталог с библиотекой`
LIB_DIR := $(CPPUTEST_HOME)/lib/$(TARGET_PLATFORM)

# каталог с файлами приложения
SRC_DIRS := .

# каталог с заголовочными файлами
INCLUDE_DIRS := .

# CYGWIN path
CYGWIN :=C:/CYGWIN/bin
MKDIR  :=@$(CYGWIN)/mkdir
RM     :=@$(CYGWIN)/rm
TOUCH  :=@$(CYGWIN)/touch
ECHO   :=@$(CYGWIN)/echo

KEIL_DIR:=D:/Keil/ARM/ARMCC
CC:=@$(KEIL_DIR)/bin/armcc.exe
AS:=@$(KEIL_DIR)/bin/armasm.exe
AR:=@$(KEIL_DIR)/bin/armar.exe
# armcc system include path
SYS_INCLUDE_DIRS :=$(KEIL_DIR)/include 

# добавление системных заголовочных файлов для сборки CRT библиотеки
INCLUDE_DIRS +=$(SYS_INCLUDE_DIRS)

CPUFLAGS :=--cpu=$(CPU)
ifeq ($(CPU_MODE), thumb)
  CPUFLAGS +=--apcs=/interwork
endif
DEPFLAGS =-M $(INCLUDES) --depend_format=unix_escaped --depend_single_line --no_depend_system_headers
OPTFLAGS :=-O3
CPPUTEST_CPPFLAGS =$(CPUFLAGS) $(OPTFLAGS) -c -g $(INCLUDES) \
  --$(CPU_MODE) \
  -D__CLK_TCK=1000 \
  -D__UART0 \

ARFLAGS := --debug_symbols


TARGET_LIB := \
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
debug_print_list = $(foreach word,$1,$(ECHO) "  $(word)";) $(ECHO)

# for building CRT library
SRC := $(call get_src_from_dir_list, $(SRC_DIRS))
OBJ := $(call src_to_o,$(SRC),$(OBJS_DIR))
INCLUDES += $(foreach dir, $(INCLUDE_DIRS), -I$(dir))
DEP_FILES := $(call src_to_d, $(SRC), $(OBJS_DIR))
STUFF_TO_CLEAN := $(OBJ) $(DEP_FILES)

#Don't create CRT dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

$(TARGET_LIB): $(OBJ) | $(LIB_DIR)
	$(AR) $(ARFLAGS) --create $@ $^
	$(ECHO) Build CRT library done!

.PHONY: crt
crt:
	$(ECHO)
	$(ECHO) CRT =$(CRT)
	$(ECHO) SRC =$(SRC)
	$(ECHO) OBJ =$(OBJ)
	$(ECHO) INCLUDES =$(INCLUDES)
	$(ECHO) DEP_FILES =$(DEP_FILES)
	$(ECHO) STUFF_TO_CLEAN =$(STUFF_TO_CLEAN)
	$(ECHO) OBJS_DIR =$(OBJS_DIR)

$(LIB_DIR) $(OBJS_DIR):
	$(ECHO) Updating directory $@
	$(MKDIR) -p $@

#This is the rule for creating the dependency files
$(OBJS_DIR)/%.d: %.c Makefile | $(OBJS_DIR)
	$(ECHO) Compiling C file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(OBJS_DIR)/%.d: %.asm | $(OBJS_DIR)
	$(ECHO) Compiling ASM file $< for dependency. Out file $@.
	$(TOUCH) $@

#This rule does the compilation C files
$(OBJS_DIR)/%.o: %.c $(OBJS_DIR)/%.d
	$(ECHO) Compiling C file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

#This rule does the compilation ASM
$(OBJS_DIR)/%.o: %.asm $(OBJS_DIR)/%.d
	$(ECHO) Assembling file $<. Out file $@
	$(AS) $(CPUFLAGS) --diag-suppress 1786 $< -o $@


clean:
	@$(RM) -f $(STUFF_TO_CLEAN)


all: $(TARGET_LIB)
	$(ECHO) Done!
