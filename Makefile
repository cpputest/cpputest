CPP_U_TEST = $(shell pwd)
CPP_PLATFORM = Gcc
APP_CPPFLAGS = -Wall
#To enable the extensions
#	set this variable to any non-null string
#	un comment-out the include in AllTests.cpp
EXTENSIONS = Y

# Functions
get_src = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
to_obj = $(subst .c,.o, $(subst .cpp,.o,$(wildcard $1/*.cpp) $(wildcard $1/*.c)))
src_to_o = $(subst .c,.o, $(subst .cpp,.o,$1))
src_to_d = $(subst .c,.d, $(subst .cpp,.d,$1))

# Sources
CPPUTEST_SRC_DIR = $(CPP_U_TEST)/src/CppUTest
PLATFORM_SRC_DIR = $(CPP_U_TEST)/src/Platforms/$(CPP_PLATFORM)
CPPUTEST_TST_DIR = $(CPP_U_TEST)/tests
EXTENSIONS_SRC_DIR = $(if $(EXTENSIONS), $(CPP_U_TEST)/src/CppUTest/Extensions)
EXTENSIONS_TST_DIR = $(if $(EXTENSIONS), $(CPP_U_TEST)/tests/Extensions)
EXAMPLE_SRC_DIR = $(CPP_U_TEST)/examples/ApplicationLib
EXAMPLE_TST_DIR = $(CPP_U_TEST)/examples/AllTests

CPPUTEST_SRC = $(call get_src,$(CPPUTEST_SRC_DIR)) \
				$(call get_src,$(EXTENSIONS_SRC_DIR)) \
				$(call get_src,$(PLATFORM_SRC_DIR))
CPPUTEST_OBJS = $(call src_to_o,$(CPPUTEST_SRC)) \

CPPUTEST_TST = $(call get_src,$(CPPUTEST_TST_DIR)) \
				$(call get_src,$(EXTENSIONS_TST_DIR))
CPPUTEST_TST_OBJS = $(call src_to_o,$(CPPUTEST_TST))

EXAMPLE_SRC = $(call get_src,$(EXAMPLE_SRC_DIR))
EXAMPLE_OBJS = $(call src_to_o,$(EXAMPLE_SRC))

EXAMPLE_TST = $(call get_src,$(EXAMPLE_TST_DIR))
EXAMPLE_TST_OBJS = $(call src_to_o,$(EXAMPLE_TST))

ALL_SRC = $(CPPUTEST_SRC) $(CPPUTEST_TST) $(EXAMPLE_SRC) $(EXAMPLE_TST)

CPPUTEST_INCLUDES = -I $(CPP_U_TEST)/include  \
					-I $(CPP_U_TEST)/include/CppUTest  \
					-I $(CPP_U_TEST)/include/Platforms/$(CPP_PLATFORM)

CPPUTEST_LIB_DIR = $(CPP_U_TEST)/lib
CPPUTEST_LIB = $(CPPUTEST_LIB_DIR)/libCppUTest.a
CPPUTEST_EXAMPLE_LIB = $(CPPUTEST_LIB_DIR)/ApplicationLib.a
CPPUTEST_OUTPUT = CppUTest_tests
CPPUTEST_EXAMPLE_OUTPUT = CppUTest_example_tests

CPPFLAGS += $(CPPUTEST_INCLUDES) $(APP_CPPFLAGS)
LDFLAGS += -lstdc++

# Targets
.PHONY: all
all: $(CPPUTEST_OUTPUT)

$(CPPUTEST_OUTPUT): $(CPPUTEST_TST_OBJS) $(CPPUTEST_LIB) 
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	./$@

$(CPPUTEST_LIB): $(CPPUTEST_OBJS)
	$(AR) r $@ $^


.PHONY: example
example: $(CPPUTEST_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT)

$(CPPUTEST_EXAMPLE_OUTPUT): $(EXAMPLE_TST_OBJS) $(CPPUTEST_EXAMPLE_LIB) $(CPPUTEST_LIB)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	./$@

$(CPPUTEST_EXAMPLE_LIB): $(EXAMPLE_OBJS)
	$(AR) r $@ $^
 	
DEP_FILES = $(call src_to_d, $(ALL_SRC)) 
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILES)
endif

%.d: %.cpp
	@$(CXX) $(CFLAGS) $(CPPFLAGS) -M -MF $@ -MT "$(subst .cpp,.o,$< $(subst .cpp,.d,$<))" $<
	@echo Generating dependency information for $(notdir $<)

%.d: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -M -MF $@ -MT "$(subst .c,.o,$<$(subst .c,.d,$<))" $<
	@echo Generating dependency information for $(notdir $<)

.PHONY: clean
clean:
	$(RM) $(CPPUTEST_OUTPUT) $(CPPUTEST_OUTPUT).exe
	$(RM) $(CPPUTEST_OBJS) $(CPPUTEST_TST_OBJS)
	$(RM) $(CPPUTEST_LIB) 
	$(RM) $(EXAMPLE_OBJS) $(EXAMPLE_TST_OBJS)
	$(RM) $(CPPUTEST_EXAMPLE_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT).exe $(CPPUTEST_EXAMPLE_LIB)
	$(RM) $(DEP_FILES)	
