#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "IEEE754ExceptionFlagsPlugin.h"
#include <cfenv>
#include <limits>

TEST_GROUP(FE_Demo) {
};

TEST(FE_Demo, should_crash_when__feenableexcept_was_called) {
    float f = 1.0f;
    IEEE754ExceptionFlagsPlugin::enableSignal();
    CHECK(f /= 0.0f == std::numeric_limits<float>::infinity() );
}

TEST(FE_Demo, should_fail__when__FE_DIVBYZERO___is_set) {
    float f = 1.0f;
    CHECK(f /= 0.0f == std::numeric_limits<float>::infinity() );
}

TEST(FE_Demo, should_fail__when__FE_UNDERFLOW___is_set) {
    float f = 0.01f;
    while (f > 0.0f) f *= f;
    CHECK(f == 0.0f);
}

TEST(FE_Demo, should_fail__when__FE_OVERFLOW___is_set) {
    float f = 1000.0f;
    while (f < std::numeric_limits<float>::infinity()) f *= f;
    CHECK(f == std::numeric_limits<float>::infinity());
}

TEST(FE_Demo, should_fail__when__FE_INEXACT_____is_set) {
    float f = 10.0f;
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
