CXX := wcl
CC  := wcl
AR := wlib
LINK := wlink
LOG := ALLTESTS.LOG
NULL :=
S := $(NULL) #
C := ,
V := /

-include ../platforms/Dos/platform.mk
include ../platforms/Dos/sources.mk

CFLAGS := \
  -q -c -os -oc -d0 -w=3 -ml \
  -dCPPUTEST_MEM_LEAK_DETECTION_DISABLED=1 \
  -dCPPUTEST_STD_CPP_LIB_DISABLED=1 \
  -i$(CPPUTEST_HOME)/include \
  -i$(CPPUTEST_HOME)/include/Platforms/dos \
  -i$(WATCOM)/h \
  -i$(WATCOM)/h/nt \

CXXFLAGS := $(CFLAGS) -xds

.PHONY: all clean

all: CPPU1$(EXE)

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -fo=$(notdir $@) $(subst /,$V, $<)

%.obj: %.c
	$(CC) $(subst /,$V, $(CFLAGS)) -fo=$(notdir $@) $(subst /,$V, $<)

CPPU1$(EXE): $(CPPU_OBJECTS) $(CPPU1_OBJECTS)
	$(LINK) opt map, stack=50k sys dos file $(subst $S,$C,$(notdir $?)) name $@

clean:
	rm -rf exit *.obj *.map *.txt *.LOG *.EXE *.err
