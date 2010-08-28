#!/usr/bin/ruby
require File.join(File.dirname(__FILE__),  'cpp_u_test_to_unity_utils.rb')
include CppUTestToUnityUtils
  
test_lines = Array.new
expected_lines = Array.new

#---------------------------------------------------

test_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"{\n",
"}\n",
"\n"
]


group = get_test_group(test_lines)
unless /mygroup/ =~ group  
  puts "Expected mygroup, but got #{group}"
end

#---------------------------------------------------

test_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"{\n",
"    int xxxx;\n",
"    void yyyy()\n",
"    {\n",
"        int i = 0;\n",
"    }\n",
"};\n",
"\n",
"    void f()\n"
]

expected_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"\n",
"static int xxxx;\n",
"static void yyyy()\n",
"{\n",
"    int i = 0;\n",
"}\n",
"\n",
"\n",
"    void f()\n"
]

def show(lines)
  lines.each do | line |
    puts line
  end
end


demote_group(/TEST_GROUP/, test_lines)
test_lines - expected_lines
unless (test_lines - expected_lines) == []
  puts "demote group failed"
  show(test_lines)
end

#---------------------------------

test_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"{\n",
"}\n",
";\n"
]

expected_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"\n",
"\n",
";\n"
]

demote_group(/TEST_GROUP/, test_lines)
test_lines - expected_lines
unless (test_lines - expected_lines) == []
  puts "demote group failed"
  show(test_lines.inspect)
  show(expected_lines.inspect)
  puts "Difference"
  show(test_lines - expected_lines)
end


#---------------------------------

test_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"{\n",
"    int xxxx;\n",
"    void yyyy()\n",
"    {\n",
"        int i = 0;\n",
"    }\n",
"}; //End TEST_GROUP\n",
"\n"
]

expected_lines = 
["\n",
"TEST_GROUP(mygroup)\n",
"\n",
"static int xxxx;\n",
"static void yyyy()\n",
"{\n",
"    int i = 0;\n",
"}\n",
"//End TEST_GROUP\n",
"\n"
]

demote_group(/TEST_GROUP/, test_lines)
unless (test_lines - expected_lines) == []
  puts "demote group failed with comments at the TEST_GROUP closing brace"
  show(test_lines.inspect)
  show(expected_lines.inspect)
end

#---------------------------------------------------

test_lines = 
["\n",
"extern \"C\"\n",
"{\n",
"\#include \"heythere.h\"\n",
"int xxxx;\n",
"void yyyy()\n",
"{\n",
"    int i = 0;\n",
"}\n",
"}\n",
"int foo()\n",
"{\n",
"        int aaaa;\n",
"}\n"
]

expected_lines = 
["\n",
"\n",
"\n",
"\#include \"heythere.h\"\n",
"int xxxx;\n",
"void yyyy()\n",
"{\n",
"    int i = 0;\n",
"}\n",
"\n",
"int foo()\n",
"{\n",
"        int aaaa;\n",
"}\n"
]

remove_extern_c(test_lines)

unless (test_lines - expected_lines) == []
  puts "demote group extern \"C\" failed"
  show(test_lines.inspect)
  show(expected_lines.inspect)
  show(test_lines)
  show(expected_lines)
  puts "Difference"
  show(test_lines - expected_lines)
end

#---------------------------------------------------

test_lines = 
["\n",
"extern \"C\"\n",
"{\n",
"#include \"LightScheduler.h\"\n",
"#include \"FakeLightController.h\"\n",
"#include \"FakeTimeService.h\"\n",
"#include \"FakeRandomMinute.h\"\n",
"}\n",
"#include \"CppUTest/TestHarness.h\"\n"
]

expected_lines = 
["\n",
"\n",
"\n",
"#include \"LightScheduler.h\"\n",
"#include \"FakeLightController.h\"\n",
"#include \"FakeTimeService.h\"\n",
"#include \"FakeRandomMinute.h\"\n",
"\n",
"#include \"CppUTest/TestHarness.h\"\n"
]

remove_extern_c(test_lines)

unless (test_lines - expected_lines) == []
  puts "demote group extern \"C\" failed"
  show(test_lines.inspect)
  show(expected_lines.inspect)
  show(test_lines)
  show(expected_lines)
  puts "Difference"
  show(test_lines - expected_lines)
end

#---------------------------------------------------

test_lines = 
["\n",
"#include \"unity_fixture.h\" expected\n",
"#include \"CppUTest\/TestHarness.h\"\n",  
"\n", 
"TEST_SETUP(theGroup) expected\n",
"static void setup()\n",
"{\n",  
"    x = 1;\n",  
"}\n",  
"TEST_TEAR_DOWN(theGroup) expected\n",
"static void teardown()\n",
"{\n",  
"    y = 0;\n",  
"}\n",  
"empty line expected in place of extern\n",
"extern \"C\"\n",
"TEST(LedDriver, Create)\n",  
"{\n",  
"    FAIL(\"Start here\");\n",  
"}\n",  
"\n",  
"IGNORE_TEST(LedDriver, ignore)\n",  
"{\n",

"    TEST_ASSERT_TRUE(0 == 0); expected\n",  
"    CHECK(0 == 0);\n", 
"\n",
"    TEST_ASSERT_EQUAL(1,1); expected\n",  
"    LONGS_EQUAL(1,1);\n",  
"\n",
"    TEST_ASSERT_EQUAL_HEX8(0xab,0xab);\n",  
"    BYTES_EQUAL(0xab,0xab);\n",  
"\n", 
"    TEST_ASSERT_EQUAL(100,100); expected\n",  
"    CHECK_EQUAL(100,100);\n",  
"\n", 
"    TEST_ASSERT_TRUE(true); expected\n",  
"    CHECK_EQUAL(true,true);\n",  
"\n", 
"    TEST_ASSERT_FALSE(false); expected\n",  
"    CHECK_EQUAL(false,false);\n",  
"\n", 
"    TEST_ASSERT_EQUAL_STRING(\"THIS\", \"THIS\"); expected\n",  
"    STRCMP_EQUAL(\"THIS\", \"THIS\");\n",  
"\n", 
"    TEST_ASSERT_FLOAT_WITHIN(1.0, 1.0, .01); expected\n",  
"    DOUBLES_EQUAL(1.0, 1.0, .01);\n",  
"\n", 
"    TEST_POINTERS_EQUAL(this, this); expected\n",  
"    POINTERS_EQUAL(this, this);\n",  
"}\n"  
]

expected_lines = 
["\n",
"#include \"unity_fixture.h\" expected\n",
"#include \"unity_fixture.h\"\n",  
"\n", 
"TEST_SETUP(theGroup) expected\n",
"TEST_SETUP(theGroup)\n",
"{\n",  
"    x = 1;\n",  
"}\n",  
"TEST_TEAR_DOWN(theGroup) expected\n",
"TEST_TEAR_DOWN(theGroup)\n",
"{\n",  
"    y = 0;\n",  
"}\n",  
"empty line expected in place of extern\n",
"\n",
"TEST(LedDriver, Create)\n",  
"{\n",  
"    TEST_FAIL(\"Start here\");\n",  
"}\n",  
"\n",  
"IGNORE_TEST(LedDriver, ignore)\n",  
"{\n",  
"    TEST_ASSERT_TRUE(0 == 0); expected\n",  
"    TEST_ASSERT_TRUE(0 == 0);\n",  
"\n", 
"    TEST_ASSERT_EQUAL(1,1); expected\n",  
"    TEST_ASSERT_EQUAL(1,1);\n",  
"\n", 
"    TEST_ASSERT_EQUAL_HEX8(0xab,0xab); expected\n",  
"    TEST_ASSERT_EQUAL_HEX8(0xab,0xab);\n",  
"\n", 
"    TEST_ASSERT_EQUAL(100,100); expected\n",  
"    TEST_ASSERT_EQUAL(100,100);\n",  
"\n", 
"    TEST_ASSERT_TRUE(true); expected\n",  
"    TEST_ASSERT_TRUE(true);\n",  
"\n", 
"    TEST_ASSERT_FALSE(false); expected\n",  
"    TEST_ASSERT_FALSE(false);\n",  
"\n", 
"    TEST_ASSERT_EQUAL_STRING(\"THIS\", \"THIS\"); expected\n",  
"    TEST_ASSERT_EQUAL_STRING(\"THIS\", \"THIS\");\n",  
"\n", 
"    TEST_ASSERT_FLOAT_WITHIN(1.0, 1.0, .01); expected\n",  
"    TEST_ASSERT_FLOAT_WITHIN(1.0, 1.0, .01);\n",  
"\n", 
"    TEST_POINTERS_EQUAL(this, this); expected\n",  
"    TEST_POINTERS_EQUAL(this, this);\n",  
"}\n"  
]

convert_macros(test_lines, "theGroup")
test_lines - expected_lines
unless (test_lines - expected_lines) == []
  puts "convert_macros failed"
  show(test_lines)
  puts "unmatched lines"
  show(test_lines - expected_lines)
end

#---------------------------------------------------

test_lines = 
[
  "    aaaa expected\n",
  "\taaa\n",
  "                bbbb expected\n",
  "\t\t\t\tbbbb\n",
 "    int a3; expected\n",
 "  int a3;\n",
   "    int a4; expected\n",
  "   int a4;\n"
]

expected_lines = 
[
  "    aaaa expected\n",
  "    aaa\n",
  "                bbbb expected\n",
  "                bbbb\n",
  "    int a3; expected\n",
  "    int a3;\n",
  "    int a4; expected\n",
  "    int a4;\n"
]

adjust_tabs(test_lines)

unless (test_lines - expected_lines) == []
  puts "adjust_tabs failed"
  show(test_lines)
  puts "unmatched lines"
  show(test_lines - expected_lines)
end


#---------------------------------------------------

test_lines = 
[
  "TEST(LedDriver, Create)\n",  
  "{\n",  
  "    FAIL(\"Start here\");\n",  
  "}\n",  
  "\n",  
  "TEST(LedDriver, XXXXX)\n",  
  "{\n",  
  "    FAIL(\"Start here\");\n",  
  "}\n",  
  "\n",  
  "IGNORE_TEST(LedDriver, ignore)\n",  
  "{\n",

  "    TEST_ASSERT_TRUE(0 == 0); expected\n",  
  "}\n"  
]

expected_group_runner = 
[
  "\n",
  "TEST_GROUP_RUNNER(LedDriver)\n",
  "{\n",
  "    RUN_TEST_CASE(LedDriver, Create);\n",
  "    RUN_TEST_CASE(LedDriver, XXXXX);\n",
  "    RUN_TEST_CASE(LedDriver, ignore);\n",
  "}\n",
  "\n"
]

group_runner = generate_group_runner("LedDriver", test_lines)
unless (expected_group_runner - group_runner) == []
  puts "generate_group_runner failed"
  show(group_runner)
  puts "unmatched lines"
  show(group_runner - expected_group_runner)
end


#---------------------------------------------------

test_filename = "tests/pooltable/EightballTest.cpp"
expected_unity_filename = "unity/pooltable/EightballTest.c"

unity_filename = convert_test_filename_to_unity_filename(test_filename)

if expected_unity_filename != unity_filename
  puts("Failure in: convert_test_path_to_unity_path")
  puts("Expected: " + expected_unity_filename.inspect)
  puts("  Actual: " + unity_filename.inspect)
end

#---------------------------------------------------

test_filename = "tests/pooltable/EightballTest.cpp"
expected_unity_runner_filename = "unity/pooltable/EightballTest_runner.c"

unity_runner_filename = convert_test_filename_to_unity_testrunner_filename(test_filename)

if expected_unity_runner_filename != unity_runner_filename
  puts("Failure in: convert_test_path_to_unity_path")
  puts("Expected: " + expected_unity_runner_filename.inspect)
  puts("  Actual: " + unity_runner_filename.inspect)
end




