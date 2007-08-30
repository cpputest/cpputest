
#include "Utest.h"

#include <e32def.h>
#include <e32std.h>

void Utest::executePlatformSpecificTestBody()
{
		TInt err(KErrNone); 
		TRAP(err, testBody()); 
		if(err != KErrNone) {
			Utest::getCurrent()->fail("Leave in test method", "", 0);
		}
}
