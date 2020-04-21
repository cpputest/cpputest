CXX := wcl
CC  := wcl
AR := wlib
LINK := wlink
LOG := ALLTESTS.LOG
NULL :=
space := $(NULL) #
comma := ,
path_separator := /
drive = $(subst $(CYGDRIVE),$(lastword $(subst /, ,$(CYGDRIVE))):/,$(1))
convert_paths = $(if $(CYGDRIVE),$(subst /,$(path_separator),$(call drive,$(1))),$(1))

-include $(CPPUTEST_HOME)/platforms/Dos/platform.mk
include $(CPPUTEST_HOME)/platforms/Dos/sources.mk

# Disable W013 unreachable code - it overreacts to CHECK_EQUAL macros
# Disable W367 conditional expression in if statement is always true - same
# Disable W368 conditional expression in if statement is always false - same
# Disable W391 assignment found in boolean expression - we don't care

CFLAGS := \
  -q -c -os -oc -d0 -we -w=3 -wcd=13 -wcd=367 -wcd=368 -wcd391 -wcd=472 -ml -zm \
  -dCPPUTEST_MEM_LEAK_DETECTION_DISABLED=1 -dCPPUTEST_STD_CPP_LIB_DISABLED=1 \
  -i$(call convert_paths,$(CPPUTEST_HOME)/include) \
  -i$(call convert_paths,$(CPPUTEST_HOME)/include/Platforms/Dos) \
  -i$(call convert_paths,$(WATCOM)/h) -i$(call convert_paths,$(WATCOM)/h/nt) \

CXXFLAGS := $(CFLAGS) -xds

.PHONY: all clean

all: CPPU.LIB CPPUX.LIB \
     CPPU1.EXE CPPU2.EXE CPPU3.EXE CPPU4.EXE CPPU5.EXE CPPU6.EXE CPPU7.EXE CPPU8.EXE \
     CPPUX1.EXE CPPUX2.EXE CPPUX3.EXE CPPUX4.EXE CPPUX5.EXE CPPUX6.EXE CPPUX7.EXE CPPUX8.EXE CPPUX9.EXE

clean:
	rm -rf ../src/CppUTest/*.o ../src/CppUTestExt/*.o \
    ../src/Platforms/dos/*.o ../tests/*.o ../tests/CppUTestExt/*.o \
    *.o *.map *.txt *.LOG *.EXE *.err *.LIB *.LST

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -fo=$(call convert_paths,$@) $(call convert_paths,$<)

%.o: %.c
	$(CC) $(CFLAGS) -fo=$(call convert_paths,$@) $(call convert_paths,$<)

.SECONDEXPANSION:

%.LIB: $$($$*_OBJECTS)
	$(AR) -q -lCPPU.LST $@ $(addprefix +,$(call convert_paths,$?))

%.EXE: $$($$*_OBJECTS) | CPPU.LIB CPPUX.LIB
	$(LINK) opt q,map,st=50k sys dos lib CPPU.LIB,CPPUX.LIB \
    file $(subst $(space),$(comma),$(call convert_paths,$?)) name $@
