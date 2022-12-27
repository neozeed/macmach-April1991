typedef enum  {red, blue, green} color;

color baz(c)
	color c;
{
	if (c == green)
		return red;
	else
		return (color)((int)c + 1);
}

main(argc,argv)
	char **argv;
{
	color t = blue;

	if (argc > 1)
		t = (color)atoi(argv[1]);

	t = baz(t);
	printf("Color is x%x\n", t);
}
