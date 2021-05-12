**Test Double** (An alternative to CppuMock)
========================================================================================================================

With **Test Double** you can perform the exact same testing as under CppuMock more easily.

The CppUMock framework introduces patterns that unnecessarily couple the Component Under Test (CuT) to
Dependencies of Component (DoC).  **Test Double** focuses on the testing (expectations) and decouples the DoC
implementation- i.e. a single **Test Double** implementation of a DoC can be used for all testing.

### Why should I trust **Test Double**?
* The original prototype of **Test Double** used the CppUMock API to demonstrate correctness.
* In the development of **Test Double** the CppUMock usage was maintained until its opinions conflicted with the
    design objectives of **Test Double**
* The behavior of **Test Double** is tested via 
```
 tests/CppUTestExt/TestDoubleReturnsTest.cpp
 tests/CppUTestExt/TestDoubleParametersTest.cpp
 tests/CppUTestExt/TestDoubleOutputParametersTest.cpp
 tests/CppUTestExt/TestDoubleTest.cpp
```


### Where can this go?

Since **Test Double** DoC implementations directly follow their header interface, auto-generation of DoC implementations
is straight-forward.  In this way, CppuTest can provide a much closer parity to GoogleTest for auto-generation of
universal test doubles implementations.


### How is this different than CppuMock?

* **Separate interfaces for test expectations <CppuTestExt/TestDouble.h> and a Test Double <CppuTestExt/ActualCall.h>**
    * MockSupport.h brings in all interfaces.  Developers new to CppUMock are often confused about which interfaces to
        use under different contexts (e.g. testing vs DoC).
    * With **Test Double**, attempts to use expectation interfaces in a Test Double result in compile time errors.

* **No support for test namespacing (i.e. _mock("namespace")_ )**
    * CppUMock supports test namespacing.  This feature is unnecessary as it couples testing directly to the DoC test
        double.

* **Testing is based on Expectations**
    * **Test Double** DoCs only test the expected parameters.  Under CppUMock, unexpected parameters
        (often unnecessary for testing the CuT behavior) results in test failures.

* **Stronger Typing**
    * **Test Double** leverages the compiler's typing, whereas CppuMock introduces its own typing which incurs implicit
        transforms based upon the CppuMock implementation.
    * **Test Double** expected return values are abstract as the compiler makes the decision to implicitly convert
        return values.  As implicit conversion is a choice of the compiler, **Test Double** does not opine as to the
        type of an expected return value, as compiler settings and interface design (allowing implicit conversion) are
        a responsibility of the design and not a quality that should be tested at run-time- (i.e. incorrect usage
        should result in compilation failure).

* **Smaller memory usage**
    * **Test Double** uses less heap and stack memory than CppUMock, while providing the same functionality.

* **Smaller (simpler) Source Code**
    * `cloc` for **Test Double**
        <!--
            include/CppUTestExt/ActualCall.h
            include/CppUTestExt/ExpectCall.h
            include/CppUTestExt/TestDouble.h
            include/CppUTestExt/TestDoubleParameter.h
            src/CppUTestExt/ActualCall.cpp
            src/CppUTestExt/TestDouble.cpp
            src/CppUTestExt/TestDoubleParameter.cpp
        -->
        ~~~
        github.com/AlDanial/cloc v 1.74  T=0.02 s (391.0 files/s, 67530.2 lines/s)
        -------------------------------------------------------------------------------
        Language                     files          blank        comment           code
        -------------------------------------------------------------------------------
        C++                              3             92             96            464
        C/C++ Header                     4             90            139            328
        -------------------------------------------------------------------------------
        SUM:                             7            182            235            792
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
            ./src/CppUTestExt/MockActualCall.cpp
            ./src/CppUTestExt/MockExpectedCallsList.cpp
            ./src/CppUTestExt/MockFailure.cpp
            ./src/CppUTestExt/MockSupportPlugin.cpp
            ./src/CppUTestExt/MockExpectedCall.cpp
            ./src/CppUTestExt/MockNamedValue.cpp
            ./src/CppUTestExt/MockSupport.cpp
        -->
        ~~~
        github.com/AlDanial/cloc v 1.74  T=0.06 s (285.7 files/s, 89753.1 lines/s)
        -------------------------------------------------------------------------------
        Language                     files          blank        comment           code
        -------------------------------------------------------------------------------
        C++                              7            580            185           2870
        C/C++ Header                     9            232            260            899
        -------------------------------------------------------------------------------
        SUM:                            16            812            445           3769
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
Framework PASS or FAIL based upon how the CuT performs under the provided DoC.  Tests should be written to minimize
DoC usage.  Not only does minimizing DoC usage expedite test development, but it also identifies unnecessary interfaces
and code of the actual dependency.

#### What should a test double do?
Only enough to support your testing of the CuT.  Minimal effort should be exerted in creating a test double, as the
only value is the ability to test the CuT.  If you find yourself investing large amounts of time in creating a test
double you need to take a step back and analyze what is driving such complexity.

With an Expectation Framework, a single DoC TestDouble can be leveraged.
```c
    // universal test double
    bool DoC( int param0, int &param1 )
    {
        return actual().call( "DoC" )
            .with( "param0", param0 )
            .output( "param1", param1 )     // NOTE: see schema to provide default values or model behavior
            .andReturnBool();               // NOTE: see schema to provide default values or model behavior
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
            .andReturnBool( true );
        CHECK( true == CuT() );
    }

    // Model - validate input parameters, manage output parameters and return value per some modelled behavior
    test_Model()
    {
        expect().call( "DoC" ).use( model );        // NOTE: see IModel interface for how to set output and return values
        CHECK( true == CuT() );
    }
```

<a name="schema">
Schema  Legend
</a>
------------------------------------------------------------------------------------------------------------------------
```c
    []      : optional
    {}      : specialization (i.e. developer must provide the rest of the syntax)
    <>      : syntax literal provided by the test developer (e.g. a string value "<call>" could be "foo"
                or a method specialization `.return<type>()` could be `.returnBool()` )
```

Schema of an Test(Expectation) Context
------------------------------------------------------------------------------------------------------------------------
```c
[failUnexpected()]                      // fail the test if an actual call was not expected
[strictOrder()]                         // fail the test if an actual call did not follow the expected order
[checkExpectation()]                    // called after CuT to fail the test if there are unmet expectations

expectCall( "<call>" )                                  // expect a call with the DoC name matching <call>
    [.times( count )]                                   // necessary invocations for the CuT test (default=infinity)
    [.with( "<parameter>", value )]                     // expected input parameter for CuT test
    [.withBuffer( "<parameter>", buffer, size )]        // expected input buffer data for CuT test
                                                        //      (NOTE: buffer must be static)
    [.output( "<parameter>", value )]                   // necessary output value for the CuT test
    [.outputBuffer( "<parameter>", buffer, size )]      // necessary output buffer for the CuT test
                                                        //      (NOTE: buffer must be static)
    [.use( <Model> )]                                   // necessary Model for the CuT (i.e. override the DoC defaults)
    [.returns( value )]                                 // necessary return value for the CuT test
```

Schema of a DoC Context
------------------------------------------------------------------------------------------------------------------------
```c
actualCall( "<call>" )                                          // name of method/function used by actual
    [.with( "<parameter>", value )]                             // actual input parameter
    [.withBuffer( "<parameter>", buffer, size )]                // actual input buffer data
                                                                //      (NOTE: buffer must be static)
    [.output( "<parameter>", [default] )]                       // default output value for general testing
    [.outputBuffer( "<parameter>", buffer, size, default )]     // default buffer value for general testing
                                                                //      (NOTE: buffer must be static)
    [.return<type>( [default] )]                                // default return value for general testing
```
