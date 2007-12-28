
SILENCE = @

CPPUTEST_HOME = $(shell pwd)
CPPUTEST_PLATFORM = Gcc
CPPUTEST_APPFLAGS = -Wall

#To enable the CPPUTEST_EXTENSIONS
#	set this variable to any non-null string
#	un comment-out the include in AllTests.cpp
#CPPUTEST_EXTENSIONS = y

# Functions
get_src  = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
to_obj   = $(subst .c,.o, $(subst .cpp,.o,$(wildcard $1/*.cpp) $(wildcard $1/*.c)))
src_to_o = $(subst .c,.o, $(subst .cpp,.o,$1))
src_to_d = $(subst .c,.d, $(subst .cpp,.d,$1))

# Sources
CPPUTEST_SRC_DIR = $(CPPUTEST_HOME)/src/CppUTest
PLATFORM_SRC_DIR = $(CPPUTEST_HOME)/src/Platforms/$(CPPUTEST_PLATFORM)
CPPUTEST_TST_DIR = $(CPPUTEST_HOME)/tests
CPPUTEST_EXTENSIONS_SRC_DIR = $(if $(CPPUTEST_EXTENSIONS), $(CPPUTEST_HOME)/src/CppUTest/Extensions)
CPPUTEST_EXTENSIONS_TST_DIR = $(if $(CPPUTEST_EXTENSIONS), $(CPPUTEST_HOME)/tests/Extensions)
EXAMPLE_SRC_DIR = $(CPPUTEST_HOME)/examples/ApplicationLib
EXAMPLE_TST_DIR = $(CPPUTEST_HOME)/examples/AllTests

CPPUTEST_SRC  = $(call get_src,$(CPPUTEST_SRC_DIR)) \
				        $(call get_src,$(CPPUTEST_EXTENSIONS_SRC_DIR)) \
				        $(call get_src,$(PLATFORM_SRC_DIR))
CPPUTEST_OBJS = $(call src_to_o,$(CPPUTEST_SRC)) \

CPPUTEST_TST = $(call get_src,$(CPPUTEST_TST_DIR)) \
				       $(call get_src,$(CPPUTEST_EXTENSIONS_TST_DIR))
				       
CPPUTEST_TST_OBJS = $(call src_to_o,$(CPPUTEST_TST))

EXAMPLE_SRC  = $(call get_src,$(EXAMPLE_SRC_DIR))
EXAMPLE_OBJS = $(call src_to_o,$(EXAMPLE_SRC))

EXAMPLE_TST      = $(call get_src,$(EXAMPLE_TST_DIR))
EXAMPLE_TST_OBJS = $(call src_to_o,$(EXAMPLE_TST))

ALL_SRC   = $(CPPUTEST_SRC) $(CPPUTEST_TST) $(EXAMPLE_SRC) $(EXAMPLE_TST)
DEP_FILES = $(call src_to_d, $(ALL_SRC)) 

CPPUTEST_INCLUDES       = -I $(CPPUTEST_HOME)/include  -I $(CPPUTEST_HOME)/include/Platforms/$(CPPUTEST_PLATFORM)
CPPUTEST_LIB_DIR        = $(CPPUTEST_HOME)/lib
CPPUTEST_LIB            = $(CPPUTEST_LIB_DIR)/libCppUTest.a
CPPUTEST_EXAMPLE_LIB    = $(CPPUTEST_LIB_DIR)/ApplicationLib.a
CPPUTEST_OUTPUT         = CppUTest_tests
CPPUTEST_EXAMPLE_OUTPUT = CppUTest_example_tests

CPPFLAGS += -Wall $(CPPUTEST_INCLUDES) $(CPPUTEST_APPFLAGS)
LDFLAGS  += -lstdc++

# Targets
.PHONY: all
all: $(CPPUTEST_OUTPUT)

$(CPPUTEST_OUTPUT): $(CPPUTEST_TST_OBJS) $(CPPUTEST_LIB) 
	$(SILENCE)echo Linking $@
	$(SILENCE)$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	$(SILENCE)./$@

$(CPPUTEST_LIB): $(CPPUTEST_OBJS)
	$(SILENCE)echo Building archive $@
	$(SILENCE)$(AR) r $@ $^

.PHONY: example
example: $(CPPUTEST_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT)

$(CPPUTEST_EXAMPLE_OUTPUT): $(EXAMPLE_TST_OBJS) $(CPPUTEST_EXAMPLE_LIB) $(CPPUTEST_LIB)
	$(SILENCE)echo Linking $@
	$(SILENCE)$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	$(SILENCE)./$@

$(CPPUTEST_EXAMPLE_LIB): $(EXAMPLE_OBJS)
	$(SILENCE)echo Building archive $@
	$(AR) r $@ $^

%.o: %.cpp
	@echo compiling $(CXXFLAGS) $(CPPFLAGS) $(notdir $<)
	$(SILENCE)$(CC) $(CPPFLAGS) -M -MF $(subst .o,.d,$@) -MT "$@ $(subst .o,.d,$@)" $<
	$(SILENCE)$(COMPILE.cpp) $(OUTPUT_OPTION) $<

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILES)
endif

.PHONY: clean
clean:
	$(SILENCE)echo Making clean
	$(SILENCE)$(RM) $(CPPUTEST_OUTPUT) $(CPPUTEST_OUTPUT).exe
	$(SILENCE)$(RM) $(CPPUTEST_OBJS) $(CPPUTEST_TST_OBJS)
	$(SILENCE)$(RM) $(CPPUTEST_LIB) 
	$(SILENCE)$(RM) $(EXAMPLE_OBJS) $(EXAMPLE_TST_OBJS)
	$(SILENCE)$(RM) $(CPPUTEST_EXAMPLE_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT).exe $(CPPUTEST_EXAMPLE_LIB)
	$(SILENCE)$(RM) $(DEP_FILES)	
