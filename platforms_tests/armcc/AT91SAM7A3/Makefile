#We don't need to clean up when we're making these targets
NODEPS  := clean

# CPU architecture. Passing to armcc.exe. For list: armcc --cpu list
ifndef CPU
	CPU := ARM7TDMI
endif

# ARM/THUMB mode. Passing to armcc.exe. May be 'thumb' or 'arm'
ifndef CPU_MODE
	CPU_MODE := thumb
endif

TARGET_PLATFORM :=armcc_$(CPU)_$(CPU_MODE)

CPPUTEST_HOME := ../../..

# имя компонента
COMPONENT_NAME := test

# имя библиотеки CRT
# она будет собрана в каталоге $(LIB_DIR)
CRT_NAME := crt

# промежуточный каталог, где производится компиляция crt-файлов, тестов и исходников
OBJS_DIR = objs

# каталог размещения будущих библиотек crt и приложения
# эти библиотеки собираются данными makefile из объектных файлов в $(OBJS_DIR)
LIB_DIR := lib

# расширение выходного файла. Keil выводит .axf, хотя по сути является elf форматом
# Расширение с "умолчательного" переделано на .elf для eclipse (он понимает .elf)
OUTFILE_EXT := elf

# имя выходного файла, а также mapfile (.map), hexfile (.mot)
OUTNAME := $(COMPONENT_NAME)
OUTFILE := $(OUTNAME).$(OUTFILE_EXT)
HEXFILE := $(OUTNAME).mot
MAPFILE := $(OUTNAME).map

CPPUTEST_OBJS_DIR := $(OBJS_DIR)
CPPUTEST_LIB_DIR := $(LIB_DIR)

# каталог со скомпилированными библиотеками CppUTest (и CppUTestExt, если нужна)
CPPUTEST_LIB_LINK_DIR := $(CPPUTEST_HOME)/platforms/armcc/lib/$(TARGET_PLATFORM)

# каталог с файлами приложения
# также возможно указать отдельные файлы в переменной $(SRC_FILES)
SRC_DIRS := \
  src \

# каталог с тестами
# также возможно указать отдельные файлы в переменной $(TEST_FILES)
TEST_SRC_DIRS := \
	tests \

# каталог со стартапными файлами
# также возможно указать отдельные файлы в переменной $(CRT_FILES)
CRT_DIRS := \
  crt \

# каталог с заголовочными файлами
INCLUDE_DIRS := \
  $(CPPUTEST_HOME)/include \
  $(SRC_DIRS) \

# каталог с заголовочными файлами для сборки CRT библиотеки
INCLUDE_CRT_DIRS := \
  $(CRT_DIRS) \

# CYGWIN path
CYGWIN :=C:/CYGWIN/bin
#FIND   :=@$(CYGWIN)/find
MKDIR  :=@$(CYGWIN)/mkdir
RM     :=@$(CYGWIN)/rm
TOUCH  :=@$(CYGWIN)/touch
ECHO   :=@$(CYGWIN)/echo

KEIL_DIR:=D:/Keil/ARM/ARMCC
CC:=@$(KEIL_DIR)/bin/armcc.exe
AS:=@$(KEIL_DIR)/bin/armasm.exe
AR:=@$(KEIL_DIR)/bin/armar.exe
LD:=@$(KEIL_DIR)/bin/armlink.exe
FROMELF:=@$(KEIL_DIR)/bin/fromelf.exe
# armcc system include path
SYS_INCLUDE_DIRS :=$(KEIL_DIR)/include 

# добавление системных заголовочных файлов для сборки CRT библиотеки
INCLUDE_CRT_DIRS +=$(SYS_INCLUDE_DIRS)

JFLASH :="C:/Program Files (x86)/SEGGER/JLinkARM_V478c/JFlashARM.exe"
JFLASHPRJ :=AT91SAM7A3.jflash

LDSCRIPT := ROM.sct

CPUFLAGS :=--cpu=$(CPU) --apcs=/interwork
DEPFLAGS =-M $(INCLUDES) --depend_format=unix_escaped --depend_single_line --no_depend_system_headers
OPTFLAGS :=-O3
CPPUTEST_CPPFLAGS =$(CPUFLAGS) $(OPTFLAGS) -c -g $(INCLUDES) \
  --$(CPU_MODE) \
  --bss_threshold=0 \
  --exceptions \
  -D__CLK_TCK=1000 \
  --library_type=standardlib \

CPPUTEST_LDFLAGS :=$(CPUFLAGS) --strict --entry=Reset_Handler --summary_stderr \
 --info summarysizes --map --callgraph\
 --info sizes --info totals --info veneers\
 --load_addr_map_info \
 --library_type=standardlib \

ARFLAGS := --debug_symbols



CPPUTEST_USE_MEM_LEAK_DETECTION := N
CPPUTEST_USE_STD_CPP_LIB := Y
CPPUTEST_USE_VPATH := N
CPPUTEST_USE_STD_C_LIB := Y
CPPUTEST_ENABLE_DEBUG := Y

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
else
  INCLUDE_DIRS +=$(SYS_INCLUDE_DIRS)
endif

# Link with CppUTest lib
CPPUTEST_LIB := $(CPPUTEST_LIB_LINK_DIR)/libCppUTest.a

# Link with CppUTestExt lib
ifeq ($(CPPUTEST_USE_EXTENSIONS), Y)
CPPUTEST_LIB += $(CPPUTEST_LIB_LINK_DIR)/libCppUTestExt.a
endif

TARGET_LIB := \
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
__src_to = $(subst .asm,$1, $(subst .c,$1, $(subst .cc,$1, $(subst .cpp,$1, $(if $(CPPUTEST_USE_VPATH),$(notdir $2),$2)))))
#__src_to = $(subst .asm,$1, $(subst .c,$1, $(subst .cc,$1, $(subst .cpp,$1, $2))))
src_to = $(addprefix $3/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1,$2)
src_to_d = $(call src_to,.d,$1,$2)
time = $(shell date +%s)
delta_t = $(eval minus, $1, $2)
debug_print_list = $(foreach word,$1,$(ECHO) "  $(word)";) $(ECHO)

# for building CRT library
CRT_LIB := \
    $(CPPUTEST_LIB_DIR)/lib$(CRT_NAME).a
SRC_CRT := $(call get_src_from_dir_list, $(CRT_DIRS) $(CRT_FILES))
OBJS_CRT_DIR := $(addsuffix /$(CRT_NAME),$(OBJS_DIR))
OBJ_CRT := $(call src_to_o,$(SRC_CRT),$(OBJS_CRT_DIR))
STUFF_TO_CLEAN_CRT := $(OBJ_CRT)
INCLUDES_CRT += $(foreach dir, $(INCLUDE_CRT_DIRS), -I$(dir))
DEP_FILES_CRT := $(call src_to_d, $(SRC_CRT), $(OBJS_CRT_DIR))
STUFF_TO_CLEAN_CRT += $(DEP_FILES_CRT) $(CRT_LIB)
#Don't create CRT dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES_CRT)
endif
$(CRT_LIB): $(OBJ_CRT) | $(LIB_DIR)
	$(AR) $(ARFLAGS) --create $@ $^
	$(ECHO) Build CRT library done!

.PHONY: crt
crt:
	$(ECHO)
	$(ECHO) CRT_LIB =$(CRT_LIB)
	$(ECHO) SRC_CRT =$(SRC_CRT)
	$(ECHO) OBJ_CRT =$(OBJ_CRT)
	$(ECHO) INCLUDES_CRT =$(INCLUDES_CRT)
	$(ECHO) DEP_FILES_CRT =$(DEP_FILES_CRT)
	$(ECHO) STUFF_TO_CLEAN_CRT =$(STUFF_TO_CLEAN_CRT)
	$(ECHO) OBJS_CRT_DIR =$(OBJS_CRT_DIR)


# исходники программных модулей
SRC = $(call get_src_from_dir_list, $(SRC_DIRS) $(SRC_FILES))
ALL_SRC = $(SRC) $(SRC_CRT)
# If we're using VPATH
ifeq ($(CPPUTEST_USE_VPATH), Y)
# gather all the source directories and add them
	VPATH += $(sort $(dir $(ALL_SRC)))
# Add the component name to the objs dir path, to differentiate between same-name objects
	CPPUTEST_OBJS_DIR := $(addsuffix /$(COMPONENT_NAME),$(CPPUTEST_OBJS_DIR))
endif
OBJ := $(call src_to_o,$(SRC),$(CPPUTEST_OBJS_DIR))
INCLUDES += $(foreach dir, $(INCLUDE_DIRS), -I$(dir))
DEP_FILES := $(call src_to_d, $(SRC), $(CPPUTEST_OBJS_DIR))
STUFF_TO_CLEAN := $(OBJ) $(DEP_FILES)

t1:
	$(ECHO) CPPUTEST_USE_VPATH =$(CPPUTEST_USE_VPATH)
	$(ECHO) VPATH =$(VPATH)
	$(ECHO) CPPUTEST_OBJS_DIR =$(CPPUTEST_OBJS_DIR)
	$(ECHO) SRC =$(SRC)
	$(ECHO) OBJ =$(OBJ)
	$(ECHO) OBJS_DIR =$(OBJS_DIR)
	$(ECHO) INCLUDES =$(INCLUDES)
	$(ECHO) DEP_FILES =$(DEP_FILES)

tl: $(TARGET_LIB)

$(TARGET_LIB): $(OBJ) | $(LIB_DIR)
	$(ECHO) Building application library $@ from $^
	$(AR) $(ARFLAGS) --create $@ $^


STUFF_TO_CLEAN += $(OUTFILE) $(MAPFILE)



$(OUTFILE): $(OBJ) $(CPPUTEST_LIB) ROM.sct Makefile
	$(ECHO) Linking!
	$(LD) $(LDFLAGS) $(OBJ) $(CPPUTEST_LIB) --scatter $(LDSCRIPT) --list $(MAPFILE) -o $@

.PHONY: all

all: $(OUTFILE)
	$(ECHO) Done!

.PHONY: prog
prog:	$(HEXFILE)
	$(JFLASH) -openprj$(JFLASHPRJ) -open$< -auto -exit 

$(HEXFILE): $(OUTFILE)
	$(ECHO) Converting to Motorola S19
	$(FROMELF) --m32 --output=$@ $<

.PHONY: debug
debug:
	$(ECHO)
	$(ECHO) "Target Source files:"
	@$(call debug_print_list,$(SRC))
	$(ECHO) "Target Object files:"
	@$(call debug_print_list,$(OBJ))
	$(ECHO) "All Input Dependency files:"
	@$(call debug_print_list,$(DEP_FILES))
	$(ECHO) Stuff to clean:
	@$(call debug_print_list,$(STUFF_TO_CLEAN))
	$(ECHO) Includes:
	@$(call debug_print_list,$(INCLUDES))
	$(ECHO) Directories to create:
	@$(call debug_print_list,$(OBJS_DIRS))
	$(ECHO) Directories of CppUTest object files:
	@$(call debug_print_list,$(OBJS_DIR))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEP_FILES)
endif

$(LIB_DIR) $(OBJS_CRT_DIR) $(CPPUTEST_OBJS_DIR):
	$(ECHO) Updating directory $@
	$(MKDIR) -p $@

#This is the rule for creating the dependency files
$(OBJS_DIR)/%.d: %.c Makefile | $(CPPUTEST_OBJS_DIR) $(OBJS_CRT_DIR)
	$(ECHO) Compiling C file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(OBJS_DIR)/%.d: %.cpp Makefile | $(CPPUTEST_OBJS_DIR) $(OBJS_CRT_DIR)
	$(ECHO) Compiling C++ file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(OBJS_DIR)/%.d: %.cc Makefile | $(CPPUTEST_OBJS_DIR) $(OBJS_CRT_DIR)
	$(ECHO) Compiling CC++ file $< for dependency. Out file $@.
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst %.d,%.o,$@)'

$(OBJS_DIR)/%.d: %.asm | $(CPPUTEST_OBJS_DIR) $(OBJS_CRT_DIR)
	$(ECHO) Compiling ASM file $< for dependency. Out file $@.
	$(TOUCH) $@

#This rule does the compilation C++ files
$(OBJS_DIR)/%.o: %.cpp $(OBJS_DIR)/%.d
	$(ECHO) Compiling C++ file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

#This rule does the compilation CC++ files
$(OBJS_DIR)/%.o: %.cc $(OBJS_DIR)/%.d
	$(ECHO) Compiling CC++ file $<. Out file $@
	$(CC) $(CPPUTEST_CPPFLAGS) $< -o $@

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


t2:
	$(ECHO) SRC_DIRS:
	@$(call debug_print_list,$(SRC_DIRS))
	$(ECHO) SRC:
	@$(call debug_print_list,$(SRC))
	$(ECHO) OBJ:
	@$(call debug_print_list,$(OBJ))
	$(ECHO) OBJS_DIR:
	@$(call debug_print_list,$(OBJS_DIR))

t3:
	$(ECHO) INCLUDE_DIRS:
	@$(call debug_print_list,$(INCLUDE_DIRS))
	$(ECHO) INCLUDES:
	@$(call debug_print_list,$(INCLUDES))
	$(ECHO) DEP_FILES:
	@$(call debug_print_list,$(DEP_FILES))
	$(ECHO) STUFF_TO_CLEAN:
	@$(call debug_print_list,$(STUFF_TO_CLEAN))
