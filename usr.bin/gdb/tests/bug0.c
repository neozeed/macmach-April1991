
main(argc, argv)
    char **argv;
{
    F(10, 6.0);
}

F(i, d)
    double d;
{
    if (i > 0) F(i-1, d+1.0);
    if (i == 5) i = *(long*)0;
}

#if 0
(gdb) b F
Breakpoint 1 at 0x40021c: file rec.c, line 9.
(gdb) run
Starting program: /var/home/bothner/tmp/rec

Bpt 1, F (i=0, d=0, 0, 0) (rec.c line 9)
9       {
(gdb) dis 1
(gdb) n
10          if (i > 0) F(i-1, d+1.0);
(gdb)
F (i=0, d=0, 0, 0) (rec.c line 9)
9       {
(gdb)

Oops.
#endif
