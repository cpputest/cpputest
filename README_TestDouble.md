**Test Double** (An alternative to CppuMock)
========================================================================================================================

The CppUMock framework introduces patterns that unnecessarily couple the Component Under Test (CuT) to
Dependencies of Component (DoC).  The **Test Double** focuses on the testing (expectations) and decouples the DoC
implementation- i.e. a single **Test Double** implementation of a DoC can be used for all testing.  With **Test Double**
you can perform the exact same testing as under CppuMock more easily.


### Where can this go?

Since **Test Double** DoC implementations directly follow their header interface, auto-generation of DoC implementations
is straight-forward.  In this way, CppuTest can provide a much closer parity to GoogleTest for auto-generation of
test doubles.


### How is this different than CppuMock?

<!-- TODO
* **Separate interfaces for expectations <CppuTestExt/TestDouble.h> and a Test Double <CppuTestExt/ActualCall.h>**
    * MockSupport.h brings in both interfaces, developers new to CppUMock are often confused about which interfaces to
        use under different contexts (e.g. testing vs DoC).
    * With **Test Double**, attempts to use expectation interfaces in a DoC result in compile time errors.
-->

* **No support for test namespacing (i.e. _mock("namespace")_ )**
    * CppUMock supports test namespacing.  This feature is unnecessary as it couples testing directly to the DoC test
        double.

* **Testing is based on Expectations**
    * Upon a DoC call under **Test Double**, only the expected parameters are verified.  Under CppUMock, unexpected
        parameters (often unnecessary for testing the CuT behavior) results in test failures.

* **Stronger Typing**
    * **Test Double** leverages the compiler's typing, whereas CppuMock introduces its own typing which incurs implicit
        transforms based upon the CppuMock implementation.

* **Smaller memory usage**
    * **Test Double** uses less heap and stack memory than CppUMock, while providing the same functionality.

* **Smaller (simpler) Implementation**
    * `cloc` for **Test Double**
            <!--
        include/CppUTestExt/ActualCall.h
        include/CppUTestExt/ExpectCall.h
        include/CppUTestExt/TestDouble.h
        include/CppUTestExt/TestDoubleParameter.h
        src/CppUTestExt/ActualCall.cpp
        src/CppUTestExt/CMakeLists.txt
        src/CppUTestExt/TestDouble.cpp
        src/CppUTestExt/TestDoubleParameter.cpp
        -->
        ~~~
        github.com/AlDanial/cloc v 1.74  T=0.01 s (534.3 files/s, 74734.9 lines/s)
        -------------------------------------------------------------------------------
        Language                     files          blank        comment           code
        -------------------------------------------------------------------------------
        C++                              3             82             89            410
        C/C++ Header                     4             76            130            278
        CMake                            1              2              0             52
        -------------------------------------------------------------------------------
        SUM:                             8            160            219            740
        -------------------------------------------------------------------------------
        ~~~
    * `cloc` for CppUMock
        <!--
        ./include/CppUTestExt/MockActualCall.h
        ./include/CppUTestExt/MockCheckedActualCall.h
        ./include/CppUTestExt/MockExpectedCallsList.h
        ./include/CppUTestExt/MockFailure.h
        ./include/CppUTestExt/MockSupport.h
        ./include/CppUTestExt/MockSupportPlugin.h
        ./include/CppUTestExt/MockCheckedExpectedCall.h
        ./include/CppUTestExt/MockExpectedCall.h
        ./include/CppUTestExt/MockNamedValue.h
        ./scripts/CppUnitTemplates/MockClassName.h
        ./scripts/CppUnitTemplates/MockClassNameC.c
        ./scripts/CppUnitTemplates/MockClassNameC.h
        ./scripts/templates/MockClassName.h
        ./scripts/templates/MockClassNameC.c
        ./scripts/templates/MockClassNameC.h
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockSupportPlugin.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockExpectedCallsList.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockFailure.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockSupport.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockExpectedCall.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockActualCall.cpp.o
        ./src/CppUTestExt/CMakeFiles/CppUTestExt.dir/MockNamedValue.cpp.o
        ./src/CppUTestExt/MockActualCall.cpp
        ./src/CppUTestExt/MockExpectedCallsList.cpp
        ./src/CppUTestExt/MockFailure.cpp
        ./src/CppUTestExt/MockSupportPlugin.cpp
        ./src/CppUTestExt/MockExpectedCall.cpp
        ./src/CppUTestExt/MockNamedValue.cpp
        ./src/CppUTestExt/MockSupport.cpp
        -->
        ~~~
            github.com/AlDanial/cloc v 1.74  T=0.04 s (549.3 files/s, 133612.5 lines/s)
        -------------------------------------------------------------------------------
        Language                     files          blank        comment           code
        -------------------------------------------------------------------------------
        C++                              7            580            185           2870
        C/C++ Header                    12            244            280            922
        C                                2             10              1             16
        -------------------------------------------------------------------------------
        SUM:                            21            834            466           3808
        -------------------------------------------------------------------------------
        ~~~



What is a Test Double?
------------------------------------------------------------------------------------------------------------------------
In order to test the CuT, the CuT's dependencies must often have specific behavior.  **Test Double** allows the test to
inject the necessary behavior per the test's needs (and no more).

For instance
```c++
long Cut() { return DoC(); }

TEST( TestGroup, CuT_returns_1 )
{
    expectCall( "DoC" ).returns( 1 );   // set the return value of DoC()
    LONGS_EQUAL( 1, CuT() );            // CuT now returns the expected value
}
```

### Expectation Framework
An Expectation Framework is a pattern for testing that focuses on expectations.  Tests created under an Expectation
Framework PASS or FAIL based upon how the CuT performs under the provided expectations.  Tests should be written to
minimize the expectations.  Not only does minimizing expectations expedite test development, but it also identifies
additional code in the implementation that is unnecessary to meet test cases.

#### What should a test double do?
Only enough to support your testing of the CuT.  Minimal effort should be exerted in creating a test double, as the
value is the ability to test the CuT.  If you find yourself investing large amounts of time in creating a Test Double,
you need to take a step back and analyze what is driving such complexity into the Test Double.

With an Expectation Framework, a single DoC TestDouble can be leveraged.
```c
    // universal test double
    bool DoC( int param0, int* pParam1 )
    {
        return actual().call( "DoC" )
            .with( "param0", param0 )
            .output( "pParam1", pParam1 )   // NOTE: see schema to provide default values
            .andReturnBool();               // NOTE: see schema to provide default values
    }

    // Spy - ensure that a call is made
    test_Spy()
    {
        expectCall( "DoC" );
    }

    // Mock - validate input parameters, return output parameters and return value
    test_Mock()
    {
        int output;
        expect().call( "DoC" )
            .with( "param0", 1 )
            .output( "param1", &output )
            .andReturn( true );
        CHECK( true == CuT() );
    }

    // Model - validate input parameters, manage output parameters and return value per some modelled behavior
    test_Model()
    {
        IModel model;
        expect().call( "DoC" ).use( model );
    }
```

<a name="schema">
Schema  Legend
</a>
------------------------------------------------------------------------------------------------------------------------
```c
    []      : optional
    {}      : specialization (i.e. developer must provide the rest of the syntax)
    <>      : literal (i.e. developer must provide the literal)
```

Schema of an Test(Expectation) Context
------------------------------------------------------------------------------------------------------------------------
```c
[failUnexpected()]                      // fail the test if an actual call was not expected
[strictOrder()]                         // fail the test if an actual call did not follow the expected order
[checkExpectation()]                    // fail the test if unmet expectations

expectCall( "<call>" )                                  // expect a call with the DoC name matching <call>
    [.times( count )]                                   // necessary invocations for the CuT test (default=infinity)
    [.with( "<parameter>", value )]                     // expected input parameter for CuT test
    [.withBuffer( "<parameter>", buffer, size )]        // expected input buffer data for CuT test
    [.output( "<parameter>", value )]                   // necessary value to set for the CuT test
    [.outputBuffer( "<parameter>", buffer, size )]      // necessary buffer value for the CuT test
    [.use( <Model> )]                                   // necessary Model for the CuT (i.e. override the DoC defaults)
    [.returns( value )]                                 // necessary return value for the CuT test
```

Schema of a DoC Context
------------------------------------------------------------------------------------------------------------------------
```c
actualCall( "<call>" )                                  // name of method/function used by actual
    [.with( "<parameter>", value )]                     // actual input parameter
    [.withBuffer( "<parameter>", buffer, size )]        // actual input buffer data
    [.output( "<parameter>", [defaultValue] )]          // necessary output value for general testing
    [.outputBuffer( "<parameter>", [buffer, size] )]    // necessary output buffer value for general testing
    [.return<type>( value )]                            // necessary return value for general testing
```
