Parametrized tests are implemented as mix-ins (just a couple of tests you can insert, mix-in, to other test groups). Mix-ins are organized just like normal tests: there is a MIXIN_GROUP and a couple of MIXIN_TESTs within this group.

## MIXIN_PARAMS
The new extension is the MIXIN_PARAMS macro, which is used to define the parameter for your parametrized tests (MIXIN_TESTs). 
Suppose you have a generic interface for your system under test. This interface can be used as parameter for the MIXIN_TESTs
```C++
class SUT	
{
public:
	virtual const char* className() = 0;
};

MIXIN_PARAMS(DemoMixInGroup) // MIXIN_GROUP name
{
	SUT* obj;
	char* expectedName;
};
```
The SUT and MIXIN_PARAMS declaration have to be accessible from both, your MIXIN_TEST implementation and the TESTs where you apply the MIXIN_TESTs. So this declaration will typically be in a header file.

## MIXIN_TEST, MIXIN_GROUP
Now you can write your actual MIXIN_TESTs.
```C++
MIXIN_GROUP(DemoMixInGroup) {};

MIXIN_TEST(DemoMixInGroup, className)
{
	const char* className = params.obj->className();
	STRCMP_EQUAL( params.expectedName, className );
}
```
Use the params struct in your test scope to access your MIXIN_PARAMS. The params struct is also available in the setup and teardown functions of the MIXIN_GROUP definition.

Let’s suppose you have different implementations of your system under test interface: `ImplA`, `ImplB`
```C++
TEST_GROUP(ImplATestGroup)
{
	ImplA objA;
};
```
## MIXIN_APPLY
To use a mix-in group within your normal test group, you have to specify the parameters for the mix-in tests.
```C++
MIXIN_APPLY(ImplATestGroup, DemoMixInGroup, ImplA_test)
{
	params->obj = &objA;
	params->expectedName = "ImplA";
}
```
Now all your tests from the _DemoMixInGroup_ will be executed in the _ImplATestGroup_ with a test name of _ImplA_test_.
