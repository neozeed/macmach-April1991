main()
{ 
	foo ();
	baz();
	foo_incl(1,2,3);
}

#include "foo_incl.c"

baz()
{
}

#include "foo_incl1.c"
