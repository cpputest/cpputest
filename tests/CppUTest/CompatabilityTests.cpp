
#include "CppUTest/TestHarness.h"

#if CPPUTEST_USE_STD_CPP_LIB

#include <memory>

TEST_GROUP(StandardCppLibrary)
{
};

#if defined(__cplusplus) && __cplusplus >= 201402L

TEST(StandardCppLibrary, UniquePtrConversationToBool)
{
    auto const aNull = std::unique_ptr<int>(nullptr);
    CHECK_FALSE(aNull);

    auto const notNull = std::make_unique<int>(1);
    CHECK_TRUE(notNull);
}

#endif

#endif

