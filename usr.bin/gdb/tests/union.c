#include "union.h"

foo_t ret_foo()
{
	union foo r;
	return r;
}

main()
{
	foo_t v = ret_foo();
	return v.b;
}

