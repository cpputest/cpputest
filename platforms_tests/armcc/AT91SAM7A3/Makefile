NODEPS := clean

# CPU architecture. Passing to armcc.exe. For list: armcc --cpu list
ifndef CPU
	CPU = ARM7TDMI
endif

# ARM/THUMB mode. Passing to armcc.exe. May be 'thumb' or 'arm'
ifndef CPU_MODE
	CPU_MODE = thumb
endif

TARGET_PLATFORM :=armcc_$(CPU)_$(CPU_MODE)

CPPUTEST_HOME := ../../..

CPPUTEST_LIB := $(CPPUTEST_HOME)/platforms/armcc/lib/$(TARGET_PLATFORM)/libCppUTest.a

COMPONENT_NAME := SimpleStringTest

# CYGWIN path
CYGWIN :=C:/CYGWIN64/bin
FIND   :=$(CYGWIN)/find
MKDIR  :=$(CYGWIN)/mkdir
RM     :=$(CYGWIN)/rm

#We don't need to clean up when we're making these targets
NODEPS  := clean

SRC_DIR := src
OBJ_DIR := obj
OUTFILE := $(COMPONENT_NAME).axf
LDSCRIPT := ROM.sct

#Find all the C/C++ files in the project directory
C_SOURCES := $(shell $(FIND) $(SRC_DIR) -name '*.c')
CPP_SOURCES += $(shell $(FIND) $(SRC_DIR) -name '*.cpp')

DIR_SOURCES := $(shell $(FIND) $(SRC_DIR) -type d)
DIR_OBJS:=$(subst $(SRC_DIR),$(OBJ_DIR),$(DIR_SOURCES))

#Find all the asm files in the project directory
ASM_SOURCES += $(shell $(FIND) $(SRC_DIR) -name '*.asm')

#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(C_SOURCES:.c=.d)
DEPFILES+=$(CPP_SOURCES:.cpp=.d)
DEPFILES:=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(DEPFILES))

OBJECTS:=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(ASM_SOURCES))
OBJECTS+=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(CPP_SOURCES))
OBJECTS+=$(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(C_SOURCES))
OBJECTS:=$(OBJECTS:.asm=.o)
OBJECTS:=$(OBJECTS:.cpp=.o)
OBJECTS:=$(OBJECTS:.c=.o)

.PHONY: all

all: $(OBJ_DIR)/$(OUTFILE)
	@echo Done!


KEIL_DIR:=D:/Keil/ARM/ARMCC
CC:=@$(KEIL_DIR)/bin/armcc.exe
AS:=@$(KEIL_DIR)/bin/armasm.exe
LD:=@$(KEIL_DIR)/bin/armlink.exe
FROMELF:=@$(KEIL_DIR)/bin/fromelf.exe

JFLASH:="C:/Program Files (x86)/SEGGER/JLinkARM_V478c/JFlashARM.exe"
JFLASHPRJ:=AT91SAM7A3.jflash

OPTFLAGS:=-O3
CPUFLAGS:=--cpu=$(CPU) --apcs=interwork


# find .h files and add them to include path
H_SOURCES := $(dir $(shell $(FIND) -name '*.h'))
INCLUDE_DIRS:=$(sort $(H_SOURCES))
SYS_INCLUDE_DIRS:=$(KEIL_DIR)\include # system headers
INCLUDE :=$(addprefix -I,$(INCLUDE_DIRS)) $(addprefix -I,$(SYS_INCLUDE_DIRS))
INCLUDE += -I$(CPPUTEST_HOME)/include

CCFLAGS:=$(CPUFLAGS) $(OPTFLAGS) -c -g $(INCLUDE) --$(CPU_MODE)\
  --bss_threshold=0 \
  --exceptions \
  -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED \
  -DCPPUTEST_USE_STD_CPP_LIB \
  -DCPPUTEST_USE_STD_C_LIB \
  -D__CLK_TCK=1000 \
  --library_type=standardlib\

DEPFLAGS:=-M $(INCLUDE) --depend_format=unix_escaped --depend_single_line --no_depend_system_headers

LDFLAGS:=$(CPUFLAGS) --strict --entry=Reset_Handler --summary_stderr \
 --info summarysizes --map --callgraph\
 --info sizes --info totals --info veneers\
 --load_addr_map_info \

$(DIR_OBJS):
	@echo Updating directory $@
	$(MKDIR) -p $@


#This is the rule for creating the dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c Makefile | $(DIR_OBJS)
	@echo Compiling C file $< for dependency. Out file $@
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$<)'

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp Makefile | $(DIR_OBJS)
	@echo Compiling C++ file $< for dependency. Out file $@
	$(CC) $(DEPFLAGS) $< --depend=$@ --depend_target='$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)'



#This rule does the compilation ASM
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm Makefile | $(DIR_OBJS)
	@echo Assembling file $<. Out file $@
	$(AS) $(CPUFLAGS) --diag-suppress 1786 $< -o $@

#This rule does the compilation C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)/%.d Makefile
	@echo Compiling C++ file $<. Out file $@
	$(CC) $(CCFLAGS) $< -o $@

#This rule does the compilation C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR)/%.d Makefile
	@echo Compiling C file $<. Out file $@
	$(CC) $(CCFLAGS) $< -o $@


$(OBJ_DIR)/$(OUTFILE): $(DEPFILES) $(OBJECTS) $(CPPUTEST_LIB) ROM.sct Makefile
	@echo Linking!
	$(LD) $(LDFLAGS) $(OBJECTS) $(CPPUTEST_LIB) --scatter $(LDSCRIPT) --list ./$(OUTFILE:.axf=.map) -o $@

.PHONY: prog
prog:	$(OBJ_DIR)/$(OUTFILE:.axf=.mot)
	$(JFLASH) -openprj$(JFLASHPRJ) -open$(OBJ_DIR)/$(OUTFILE:.axf=.mot) -auto -exit 

$(OBJ_DIR)/$(OUTFILE:.axf=.mot): $(OBJ_DIR)/$(OUTFILE)
	@echo Converting to Motorola S19
	$(FROMELF) --m32 --output=$@ $<


.PHONY: clean
clean:
	$(RM) -rf $(OBJ_DIR)

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    #Chances are, these files don't exist.  GMake will create them and
    #clean up automatically afterwards
    -include $(DEPFILES)
endif
