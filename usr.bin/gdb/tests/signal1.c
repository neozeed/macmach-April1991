#include <sys/time.h>
#include <signal.h>

int ntick;

tick(sig,code,scp)
	struct sigcontext *scp;
{
	printf("Time flies..\n");
	ntick++;
}

struct sigvec st = { tick, 0, 0};

main()
{
	struct itimerval tv;
	int i = 0;

	tv.it_value.tv_sec = 5;
	tv.it_value.tv_usec = 0;
	tv.it_interval.tv_sec = 5;
	tv.it_interval.tv_usec = 0;

	setitimer( 0, &tv, 0);

	sigvec( SIGALRM, &st, 0);
	while (ntick < 3) {
		i++;
		getpid();
	}
	printf("Mr. Time gave us %d cycles\n", i);
}
