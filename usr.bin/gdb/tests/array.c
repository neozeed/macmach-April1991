struct foo {
	int a;
	int b;
} some_array[3], 
  init_array[] = { {1,2}, {3,4} };
main()
{
	static struct foo main_local[10];
	foo(init_array[1]);
}

foo(s)
struct foo s;
{
	printf("%x %x\n", s.a, s.b);
}
