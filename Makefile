export CPP_U_TEST = $(shell pwd)
export CPP_PLATFORM = Gcc
export APP_CXXFLAGS = -Wall

# Functions
get_src = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
to_obj = $(subst .c,.o, $(subst .cpp,.o,$(wildcard $1/*.cpp) $(wildcard $1/*.c)))

# Sources
CPPUTEST_SRC = $(CPP_U_TEST)/src/CppUTest
CPPUTEST_PLATFORM_SRC = $(CPP_U_TEST)/src/Platforms/$(CPP_PLATFORM)
CPPUTEST_EXAMPLE_SRC = $(CPP_U_TEST)/examples/ApplicationLib
CPPUTEST_EXAMPLE_TST = $(CPP_U_TEST)/examples/AllTests
CPPUTEST_TST = $(CPP_U_TEST)/tests

CPPUTEST_OBJS = $(call to_obj,$(CPPUTEST_SRC))
CPPUTEST_PLATFORM_OBJS = $(call to_obj,$(CPPUTEST_PLATFORM_SRC))
CPPUTEST_EXAMPLE_OBJS = $(call to_obj,$(CPPUTEST_EXAMPLE_SRC))
CPPUTEST_EXAMPLE_TST_OBJS = $(call to_obj,$(CPPUTEST_EXAMPLE_TST))
CPPUTEST_TST_OBJS = $(call to_obj,$(CPPUTEST_TST))

CPPUTEST_ALL_SRC = $(call get_src,$(CPPUTEST_SRC)) $(call get_src,$(CPPUTEST_PLATFORM_SRC))
CPPUTEST_ALL_TST = $(call get_src,$(CPPUTEST_TST))
CPPUTEST_ALL_EXAMPLE = $(call get_src,$(CPPUTEST_EXAMPLE_SRC)) $(call get_src,$(CPPUTEST_EXAMPLE_TST))

CPPUTEST_INCLUDES = -I $(CPP_U_TEST)/include  \
					-I $(CPP_U_TEST)/include/CppUTest  \
					-I $(CPP_U_TEST)/include/Platforms/$(CPP_PLATFORM)

CPPUTEST_LIB_DIR = $(CPP_U_TEST)/lib
CPPUTEST_LIB = $(CPPUTEST_LIB_DIR)/libCppUTest.a
CPPUTEST_EXAMPLE_LIB = $(CPPUTEST_LIB_DIR)/ApplicationLib.a
CPPUTEST_OUTPUT = CppUTest_tests
CPPUTEST_EXAMPLE_OUTPUT = CppUTest_example_tests

CPPFLAGS += $(CPPUTEST_INCLUDES)
LDFLAGS += -lstdc++

# Targets
.PHONY: all
all: $(CPPUTEST_OUTPUT)

$(CPPUTEST_OUTPUT): $(CPPUTEST_TST_OBJS) $(CPPUTEST_LIB) 
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	./$@

$(CPPUTEST_LIB): $(CPPUTEST_OBJS) $(CPPUTEST_PLATFORM_OBJS)
	$(AR) r $@ $^


.PHONY: example
example: $(CPPUTEST_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT)

$(CPPUTEST_EXAMPLE_OUTPUT): $(CPPUTEST_EXAMPLE_TST_OBJS) $(CPPUTEST_EXAMPLE_LIB) $(CPPUTEST_LIB)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@
	./$@

$(CPPUTEST_EXAMPLE_LIB): $(CPPUTEST_EXAMPLE_OBJS)
	$(AR) r $@ $^
 	
DEP_FILES = $(subst .c,.d,$(subst .cpp,.d,$(CPPUTEST_ALL_SRC)))
DEP_FILES += $(subst .c,.d,$(subst .cpp,.d,$(CPPUTEST_ALL_TST)))
DEP_FILES += $(subst .c,.d,$(subst .cpp,.d,$(CPPUTEST_ALL_EXAMPLE)))
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
	$(RM) $(CPPUTEST_OBJS) $(CPPUTEST_PLATFORM_OBJS) $(CPPUTEST_TST_OBJS)
	$(RM) $(CPPUTEST_LIB) 
	$(RM) $(CPPUTEST_EXAMPLE_OBJS) $(CPPUTEST_EXAMPLE_TST_OBJS)
	$(RM) $(CPPUTEST_EXAMPLE_OUTPUT) $(CPPUTEST_EXAMPLE_OUTPUT).exe $(CPPUTEST_EXAMPLE_LIB)
	$(RM) $(DEP_FILES)	
