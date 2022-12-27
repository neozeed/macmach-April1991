main()
{
	float f = 0.3;
	double g = 0.3333;
	register float r;

	f = f * g + 3.14;
	r = f - 2;
	printf("%f %f\n", f, g, r);
	return (int)r;
}

float f_ret(a,b,c,d,e)
	float a,b,c,d,e;
{
	return (a+b+c+d+e);
}

double d_ret(a,b,c,d,e)
	double a,b,c,d,e;
{
	return (a+b+c+d+e);
}

float s_ret()
{
	return 2.2;
}

double sd_ret()
{
	return (double)3.3;
}
