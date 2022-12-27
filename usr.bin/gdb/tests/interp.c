#include <mach.h>
#include <mach/message.h>

char *myname;

main(argc,argv)
	char **argv;
{
	int tpid;
	port_t t, et, n;

	tpid = atoi(argv[1]);
	if (argc < 3)
		myname = "ANON";
	else
		myname = argv[2];

	if (task_by_unix_pid(task_self(), tpid, &t))
		quit(1,"task_by_unix_pid");
	if (task_get_special_port(t, TASK_EXCEPTION_PORT, &et))
		quit(2,"task_get_special_port");
	port_allocate(task_self(), &n);
	if (task_set_special_port(t, TASK_EXCEPTION_PORT, n))
		quit(2,"task_set_special_port");

	mirror(n,et);
	if (task_set_special_port(t, TASK_EXCEPTION_PORT, et))
		printf("task_set_special_port failed, target dead ?\n");
}

mirror(p1,p2)
	port_t p1, p2;
{
	char msgbuf[8196];
	msg_header_t *msg;
	port_set_name_t p;

	port_set_allocate(task_self(), &p);
	port_set_add(task_self(), p, p1);
	port_set_add(task_self(), p, task_notify());

	msg = (msg_header_t *)msgbuf;
	for (;;) {
		msg->msg_size = sizeof msgbuf;
		msg->msg_local_port = p;
		msg_receive(msg, 0, 0);

		printf("%s: sple %d size x%x type %d l x%x r x%x id x%x\n",
			myname,
			msg->msg_simple, msg->msg_size, msg->msg_type,
			msg->msg_local_port, msg->msg_remote_port,
			msg->msg_id);

		if (msg->msg_local_port != p1)
			break;

		msg->msg_local_port = msg->msg_remote_port;
		msg->msg_remote_port = p2;
		msg_send(msg,0,0);
	}
	printf("End interpose.\n");
}
