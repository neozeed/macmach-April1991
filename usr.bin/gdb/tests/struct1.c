typedef struct {
	int a, b;
	short c,d;
	char e,f,g,h;
	int extra1, extra2;
} foo_t;

typedef struct {
	int a,b;
} baz_t;

foo_t foo(f)
	foo_t f;
{
	f.a++;
	return f;
}

baz_t baz(a,b)
{
	baz_t bb;
	bb.a = a;
	bb.b = b;
	return bb;
}

main()
{
	foo_t f;
	baz_t b;

	f = foo(f);
	b = baz(33,44);
}

