typedef enum {red, yellow, green} colors;

colors baz(a)
{
/*5*/	if (a) 
		return red;
	return yellow;
}

main()
/*11*/{
	int i,j,k;

	i = 0;
	j = 2;
	k = 3;
	foo(i,j,k);
}

foo(a,b,c)
/*21*/{
	if (a == b)
		if (baz(a) == red)
			return 3;
		else
			return 2;
	return c-b-a;
}

