Test Doubles
========================================================================================================================

The CppUMock framework introduces patterns that couple Component Under Test (CuT) to Dependencies of Component (DoC).
Test Doubles focuses on the testing (expectations) and decouples the DoC implementation- i.e. a single Test Double
implementation of the DoC can be used for all testing.

### Where can this go?
Since all Test Double implementations are directly understood by their interface, auto-generation of Mocks is straight
forward.

### How is this different than CppuMock?

* **Separation of interfaces for an expected call and an actual call**
    * As inclusion of MockSupport.h brings in both interfaces, developers new to CppUMock are often confused about which
        interface to use under different contexts (e.g. CuT vs DoC).
    * Under TestDoubles tests use Expect.h and test double implementations use Actual.h - and each only has access to
        the necessary interfaces (expectCall(), actualCall() respectively).
* **No support for test namespacing (i.e. _mock("namespace")_ )**
    * CppUMock supports test namespacing.  This feature is unnecessary and couples testing of the CuT directly to
        the DoC Test Double.
* **Call order is enforced only for sets of expectations requiring ordering**
    * CppUMock's _mock().strictOrder()_ forces all calls to happen in the order of expectations.
* **Testing is based on Expectations**
    * Upon a DoC call under TestDoubles, only the expected parameters are verified.  Under CppUMock, unexpected
        parameters (often unnecessary for testing the CuT behavior) results in test failures.
* **Stronger Typing**
    * TestDoubles leverages the compiler's typing, whereas CppuMock introduces its own typing which incurs implicit
        transforms based upon CppuMock choices.
* **Smaller memory usage**
    * TestDoubles avoids unnecessary heap allocations.
    * TestDoubles avoids unnecessary stack allocations.

What is a Test Double?
------------------------------------------------------------------------------------------------------------------------
In order to test the CuT, the CuT's dependencies must often have specific behavior.  Test Doubles allow the test to
inject the necessary behavior per the test's needs.

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
An Expectation Framework is a pattern for testing that focuses on expectations.  Tests created under and Expectation
Framework PASS or FAIL based upon how the CuT performs under the provided expectations.  Tests should be written to
minimize the expectations.  Not only does minimizing expectations expedite test development, but it also identifies
additional code in the implementation that is unnecessary to meet test cases.

#### What should a Test Double do?
Only enough to support your testing of the CuT.  Minimal effort should be exerted in creating a Test Double, as the
value is the ability to test the CuT.  If you find yourself investing large amounts of time in creating a Test Double,
you need to take a step back and analyze what is driving such complexity into the Test Double.

With an Expectation Framework, a single DoC TestDouble can be leveraged.
```c
    // Spy - ensure that a call is made
    test_Spy()
    {
        expectCall( "DoC" );
    }

    // Mock - validate input parameters, return output parameters and return value
    test_Mock()
    {
        const X kInput;
        const X kOutput;
        const X kReturn;
        expect().call( "DoC" )
            .with( "param0", kInput )
            .output( "param1", kOutput )
            .andReturn( kReturn );
    }

    // Mock - validate input parameters, return output parameters and return value per some modelled behavior
    test_Model()
    {
        Model model;
        expect().call( "DoC" ).use( model );
    }

    // universal test double
    bool DoC( int param0, int* pParam1 )
    {
        return actual().call( "DoC" )
            .with( "param0", param0 )
            .output( "pParam1", pParam1 )   // NOTE: see schema to provide default values
            .andReturnBool();               // NOTE: see schema to provide default values
    }
```

<a name="schema">
Schema  Legend
</a>
------------------------------------------------------------------------------------------------------------------------
```c
    {}      : optional
    <>      : specialization
```
Schema of an Expectation
------------------------------------------------------------------------------------------------------------------------
```c
expectCall( "<call>" )                  // name of method/function used by actual
    {.onObject( object ) }              // object is a reference to a harness object
    {.onObjectType( objectType ) }      // objectType is a class type
    {.times( count )}                   // number of invocations to apply expectation, default=always
    {.with( "parameter", value )}       // name of parameter and value to validate
    {.output( "parameter", value )}     // name of parameter, sets an expected value
    {.use( Model )}                     // Model of behavior
    {.returns( value )}                 // return value

<!-- TODO
Schema of a Sequential Expectation
------------------------------------------------------------------------------------------------------------------------
expectNext( "<sequence>", "<call>" )    // name of sequence and method/function used by actual,
                                            returns expectCall reference under the sequence
```
-->

```c
actualCall( "<call>" )                          // name of method/function used by actual
    {.onObject( this ) }                        // object of the DoC
    {.onObjectType( objectType ) }              // objectType of the object DoC
    {.with( "parameter", value )}               // name of parameter and value to validate
    {.output( "parameter", pointer )}           // name of parameter, sets an expected value or `true`
    {.output( "parameter", pointer, default )}  // name of parameter, sets an expected value or default
    {.return<type>()}                           // return expected value or `true`
    {.return<type>( default )}                  // return expected value or default
```