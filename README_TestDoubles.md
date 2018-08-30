What is a Test Double?
------------------------------------------------------------------------------------------------------------------------
Test Doubles are created to focus on the Code Under Test (CuT) based on how the CuT behaves in regards to a Dependency
of Component (DoC).

By using Test Doubles, testing efforts can focus on the behavior of the CuT.  Once the developer is confident in the CuT
implementation, Test Doubles can be replaced with the actual DoC implementation creating a new CuT-
the integration of the previous CuT and DoC(s).  In this way, confidence in the overall behavior can be incrementally
increased based on the simularity of the CuT to the final system.


How do I make my CuT use a Test Double?
------------------------------------------------------------------------------------------------------------------------
For the CuT to use a Test Double, some form of dependency injection must be introduced.

Patterns for Dependency Injection
* Link time substitution (use another implementation with the same symbol name)
* Interface abstraction (architect the CuT to interact with DoC via an interface)
* and many others...


What should a Test Double do?
------------------------------------------------------------------------------------------------------------------------
Only enough to support your testing of the CuT.  Minimal effort should be exerted in creating a Test Double, as the
value is the ability to test the CuT.  If you find yourself investing large amounts of time in creating a Test Double,
you need to take a step back and analyze what is driving such complexity into the Test Double.

### Common Test Double Patterns
As there are common Test Double patterns, frameworks may provide more expedient ways to implement Test Doubles.

Test Double Patterns:
* Fake - to make the CuT testable, the DoC only needs to return valid return code
```C
    bool Foo( ... ) { return true; }
```
* Spy - to test CuT, one wants to observe the invocation of the DoC
```C
    static void spyFoo( ... ) { /* record/publish invocation */ }
    bool Foo( ... ) { spyFoo(...); return true; }
```
* Mock - to test CuT, one wants to validate expectations of the invocation
```C
    static void mockFoo( ... ) { /* validate expected invocation */ }
    bool Foo( ... ) { return mockFoo(...) }
```
* Model - to test CuT, one wants to perform some behavior
```C
    void behave( ... ) { /* behave per invocation */ }
    bool Foo( ... ) { return behave(...); }
```

### Expectation Framework
The Test Double patterns above all build upon each other.  The underlying theme defines an Expectation Framework- the
tester needs the Test Double to minimally satisfy the CuT.  An Expectation Framework expedites the ability to create
Test Doubles in a generic fashion and then modify expectations as necessary.

With an Expectation Framework, the intent of an expectation can be expressive and extendable as necessary to meet new
test case needs.
```C
    // DoC interface
    bool DoC( int param0, int* OUTparam1 );

    // Fake
    test_Fake()
    {
        expect().call( "DoC" ).andReturn( true );
    }

    // Spy
    test_Spy()
    {
        expect().call( "DoC" ).andReturn( true );
    }

    // Mock
    test_Mock()
    {
        const X kInput;
        const X kOutput;
        const X kReturn;
        expect().call( "DoC" )
            .with( "param0", kInput )
            .output( "OUTparam1", kOutput )
            .andReturn( kReturn );
    }

    // Model
    test_Model()
    {
        Model model;
        expect().call( "DoC" ).use( model );
    }

    // expectation based DoC Test Double
    bool DoC( int param0, int param1 )
    {
        return actual().call( "DoC" )
            .with( "param0", param0 )
            .with( "OUTparam1", OUTparam1 )
            .andReturnBool();
    }
```


What is an Expectation?
------------------------------------------------------------------------------------------------------------------------
The DoC behaves based upon inputs and state.  Therefore expectations of the Test Double replacing the DoC only need to
be concerned about inputs and state.  By providing a universal way to express the actual invocation, the tester can
choose the concerns of the DoC per the test case of the CuT.  In this way, the CuT can continue working per the concerns
under test.

As demonstrated above, Fakes, Spys, Mocks, and Models can all be implemented with a single Test Double.  In this way,
the Test Double implementation can be written once and the expectations per test can be specialized by expressing
the concerns under test.

### How is this different than CppUMock?
CppUMock is strongly opinionated as to how a DoC interface is invoked- producing FAIL cases when expectations
don't match actual calls.  Under an Expectation Framework the unexpected calls are only failures when the tester
explicitly requires it.  In this way, the tester can concentrate on the CuT behavior and not be overly concerned about
how the DoC is invoked.

#### Differences from CppUMock
Expectation Frameworks are expressive rather than constrained.  If constraints are needed, they can be expressed, rather
than forcing unexpected constraints upon all use cases.

* Parameters are only necessary to be matched if part of the expectation
```C
    /* Mock Foo */
    void Bar( int param0, char param1 )
    {
        mock().actualCall( "Bar" )
            .withParameter( "param0", param0 );
            .withParameter( "param1", param1 );
    }

    /* CppUMock implementation */
    TEST( Foo, FakeBar )
    {
        mock().expectOneCall( "Bar" );  // expects a call
        Foo();
    }
```
Under CppUMock the above code will FAIL because the test didn't specify .ignoreOtherParameters().  If the test had no
need of the other parameters to test the CuT, there should be no need to opine upon the invocation.
* Strong typing can be turned on/off - use the compile time implicit conversion by default

    Under CppUMock the above code will FAIL because the test didn't explicitly match the parameter types.  If the test
    has no concern about the compiler's implicit type conversion, there should be no need to opine upon the values.

* Aspect Oriented Programming (AOP) - the use of a DoC interface only needs to fulfill the needs of the test for the
    CuT.    
    When testing, a Model is only needed when the DoC performs some global behavior.  Under CppUMock, the Model behavior
    would have to be implemented as part of the Test Double. Under an Expectation Framework, the Model behavior can be
    imposed as needed.

* Unexpected DoC invocations **only** FAIL when expected.
    The CppUMock framework forces all DoC invocations to be explicitly identified.  Instead, an Expectation Framework
    focuses on the expected invocations.  The Expectation Framework can be configured to FAIL upon unexpected invocation
    of the DoC, but doesn't opine about the tester's understanding of the CuT's usage of the DoC.


Schema  Legend
------------------------------------------------------------------------------------------------------------------------
```C
    {}      : optional
    <>      : specialization
```
Schema of an Expectation
------------------------------------------------------------------------------------------------------------------------
```C
expect( {<context>} )                   // expectation context, default is global
    .call( <DoC> )
    {.times( count )}                   // number of invocations to apply expectation, default=always
    {.next( sequence )}                 // assert that previous sequence expectations have occurred
    {.with( <parameter>, value )}       // parameter is either a uuid (i.e. string) or Matcher, validates parameter
    {.output( <parameter>, value )}     // parameter is either a uuid (i.e. string) or Matcher, sets an expected value
    {.use( Model )}                     // Model of behavior
    {.returns<type>()}                  // return value
```

Schema of an Actual
------------------------------------------------------------------------------------------------------------------------
```C
actual( {<context>} )                   // expectation context, default is global
    .call( <DoC> )
    {.with( String, value )}            // parameter name and value upon invocation
    {.output( <parameter>, value )}     // sets parameter to expected value
    {.returns<type>()}                  // return value
```




