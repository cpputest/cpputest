#include "CppUTest/TestHarness.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestPlugin.h"

static void orig_func1()
{
}

static void stub_func1()
{
}

static void orig_func2()
{
}

static void stub_func2()
{
}

static void (*fp1)();
static void (*fp2)();

TEST_GROUP(SetPointerPluginTest)
{
    SetPointerPlugin* plugin_;
    TestRegistry* myRegistry_;
    StringBufferTestOutput* output_;
    TestResult* result_;

    void setup()
    {
        myRegistry_ = new TestRegistry();
        plugin_ = new SetPointerPlugin("TestSetPlugin");
        myRegistry_->setCurrentRegistry(myRegistry_);
        myRegistry_->installPlugin(plugin_);
        output_ = new StringBufferTestOutput();
        result_ = new TestResult(*output_);
    }

    void teardown()
    {
        myRegistry_->setCurrentRegistry(0);
        delete myRegistry_;
        delete plugin_;
        delete output_;
        delete result_;
    }
};

class FunctionPointerUtest : public Utest
{
public:
   void setup() _override
   {
      UT_PTR_SET(fp1, stub_func1);
      UT_PTR_SET(fp2, stub_func2);
      UT_PTR_SET(fp2, stub_func2);
   }
   void testBody() _override
   {
      CHECK(fp1 == stub_func1);
      CHECK(fp2 == stub_func2);
   }
};

class FunctionPointerUtestShell: public UtestShell
{
public:
   virtual Utest* createTest()
   {
      return new FunctionPointerUtest();
   }
};

TEST(SetPointerPluginTest, installTwoFunctionPointer)
{
    FunctionPointerUtestShell *tst = new FunctionPointerUtestShell();
    ;

    fp1 = orig_func1;
    fp2 = orig_func2;
    myRegistry_->addTest(tst);
    myRegistry_->runAllTests(*result_);
    CHECK(fp1 == orig_func1);
    CHECK(fp2 == orig_func2);
    LONGS_EQUAL(0, result_->getFailureCount());
    LONGS_EQUAL(2, result_->getCheckCount());
    delete tst;
}

class MaxFunctionPointerUtest : public Utest
{
public:
   int numOfFpSets;
   MaxFunctionPointerUtest(int num) :
      numOfFpSets(num)
   {
   }

   void setup()
   {
      for (int i = 0; i < numOfFpSets; ++i)
      {
         UT_PTR_SET(fp1, stub_func1);
      }
   }
};

class MaxFunctionPointerUtestShell: public UtestShell
{
public:
   int numOfFpSets;
    MaxFunctionPointerUtestShell(int num) :
       numOfFpSets(num)
    {
    }

    virtual Utest* createTest()
    {
       return new MaxFunctionPointerUtest(numOfFpSets);
    }
};

TEST(SetPointerPluginTest, installTooMuchFunctionPointer)
{
    MaxFunctionPointerUtestShell *tst = new MaxFunctionPointerUtestShell(SetPointerPlugin::MAX_SET + 1);
    myRegistry_->addTest(tst);

    myRegistry_->runAllTests(*result_);

    LONGS_EQUAL(1, result_->getFailureCount());
    delete tst;
}

static double orig_double = 3.0;
static double* orig_double_ptr = &orig_double;
static double stub_double = 4.0;

class SetDoublePointerUtest : public Utest
{
public:
   void setup()
   {
      UT_PTR_SET(orig_double_ptr, &stub_double);
   }
   void testBody()
   {
      CHECK(orig_double_ptr == &stub_double);
   }
};

class SetDoublePointerUtestShell: public UtestShell
{
public:
   Utest * createTest()
   {
      return new SetDoublePointerUtest();
   }
};

TEST(SetPointerPluginTest, doublePointer)
{
    SetDoublePointerUtestShell *doubletst = new SetDoublePointerUtestShell();
    myRegistry_->addTest(doubletst);
    myRegistry_->runAllTests(*result_);

    CHECK(orig_double_ptr == &orig_double);
    LONGS_EQUAL(1, result_->getCheckCount());
    delete doubletst;
}
