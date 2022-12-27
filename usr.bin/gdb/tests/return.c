main()
{
	int r;
	r = foo(1);
	printf("foo(1) returns %d\n", r);
	printf("foo(-1) returns %d\n", foo(-1));
}
foo(n)
{
	return -n;
}
