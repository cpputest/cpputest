
#include "CppUTest/StandardCLibrary.h"

extern "C"
{
#include "AllocLetTestFree.h"
}

#include "CppUTest/TestHarness.h"

#if CPPUTEST_USE_STD_C_LIB
/*
 * This test makes sure that memory leak malloc macros are forced into .cpp and .c files
 */
TEST_GROUP(AllocLetTestFree)
{
    AllocLetTestFree allocLetTestFree;

    void setup()
    {
        allocLetTestFree = AllocLetTestFree_Create();
    }

    void teardown()
    {
        AllocLetTestFree_Destroy(allocLetTestFree);
    }
};

TEST(AllocLetTestFree, Create)
{
    free(allocLetTestFree);
}

#endif
