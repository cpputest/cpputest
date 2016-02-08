#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/IEEE754ExceptionsPlugin.h"

#ifdef COMPILER_FULLY_SUPPORTS_CXX11

#include <cfenv>
#include <limits>

static volatile float f;

TEST_GROUP(FE_Demo) {
};

TEST(FE_Demo, should_fail_when__FE_DIVBYZERO__is_set) {
    f = 1.0f;
    CHECK(f /= 0.0f == std::numeric_limits<float>::infinity() );
}

TEST(FE_Demo, should_fail_when__FE_UNDERFLOW__is_set) {
    f = 0.01f;
    while (f > 0.0f) f *= f;
    CHECK(f == 0.0f);
}

TEST(FE_Demo, should_fail_when__FE_OVERFLOW__is_set) {
    f = 1000.0f;
    while (f < std::numeric_limits<float>::infinity()) f *= f;
    CHECK(f == std::numeric_limits<float>::infinity());
}

TEST(FE_Demo, should_fail_when__FE_INEXACT____is_set) {
    f = 10.0f;
    DOUBLES_EQUAL(f / 3.0f, 3.333f, 0.001f);
}

TEST(FE_Demo, should_succeed_when_no_flags_are_set) {
    CHECK(5.0f == 15.0f / 3.0f);
}

static IEEE754ExceptionFlagsPlugin ieee754Plugin{"IEEE754"};

int main(int ac, char** av) {
    TestRegistry::getCurrentRegistry()->installPlugin(&ieee754Plugin);
    ieee754Plugin.enableInexact();
    return RUN_ALL_TESTS(ac, av);
}

#endif
