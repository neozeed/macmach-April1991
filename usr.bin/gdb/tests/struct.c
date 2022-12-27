/*TODO: various sizes (1,2,4,6,8,...); alignment; arrays & unions */

typedef struct {
	short s;
} small_t;

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

small_t small(v)
{
	small_t r;
	r.s = v;
	return r;
}

typedef int arr2[2];
typedef int arr3[3];

/* can't do this
	arr2 ret_arr2()
	{
		arr2 r;
		r[0] = 1;
		return r;
	}
	arr3 ret_arr3()
	{
		arr3 r;
		r[0] = 1;
		return r;
	}
*/

typedef union {
	short s;
	char c;
} u1;

u1 ret_u1()
{
	u1 ret;
	ret.s = 2;
	return ret;
}

typedef union {
	short s;
	arr3  a;
} u2;

u2 ret_u2()
{
	u2 ret;
	ret.s = 2;
	return ret;
}
