#include <cthreads.h>
#include <mach/message.h>

main()
{
	port_t          some_port;
	int             another_thread();

	cthread_detach(cthread_fork(another_thread, 0));

	port_allocate(task_self(), &some_port);
	while (1) {
		msg_header_t    msg;

printf(" th0\n");
		msg.msg_local_port = some_port;
		msg.msg_size = sizeof(msg);
		msg_receive(&msg, RCV_TIMEOUT, 1000);
	}
}

another_thread()
{
	port_t          some_port;
	int             yet_another_thread();

	cthread_detach(cthread_fork(yet_another_thread, 0));

	cthread_set_name(cthread_self(), "The_other_thread");
	port_allocate(task_self(), &some_port);
	while (1) {
		msg_header_t    msg;

printf(" th1\n");
		msg.msg_local_port = some_port;
		msg.msg_size = sizeof(msg);
		msg_receive(&msg, RCV_TIMEOUT, 1000);
	}
}

yet_another_thread()
{
	port_t          some_port;

	cthread_set_name(cthread_self(), "tre");
	port_allocate(task_self(), &some_port);
	while (1) {
		msg_header_t    msg;

printf(" th2\n");
		msg.msg_local_port = some_port;
		msg.msg_size = sizeof(msg);
		msg_receive(&msg, RCV_TIMEOUT, 1000);
	}
}
