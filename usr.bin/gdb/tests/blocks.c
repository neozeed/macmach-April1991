main(a,b)
{
	int a = 2;
	{
		int a = 3;
		printf("a=%d", a);
	}
	{
		int a = 4;
		printf("a=%d", a);
		{
			int a = 5;
			printf("a=%d", a);
		}
	}
	return 33;
}
