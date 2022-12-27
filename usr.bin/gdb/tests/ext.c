main(argc,argv)
	char **argv;
{
	int n = 12;
	if (argc > 1)
		n = atoi(argv[1]);

	printf("Lev1\n");
	lev(n,2);
}

lev(max,n)
{
	printf("Lev%d\n", n);
	if (n == max)
		return;
	lev(max,n+1);
}
