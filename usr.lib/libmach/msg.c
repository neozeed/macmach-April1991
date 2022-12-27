/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	msg.c,v $
 * Revision 1.4  90/02/23  18:46:23  mrt
 * 	Changed types of timeout args to msg_timeout_t and size
 * 	to msg_size_t
 * 	[90/02/08            mrt]
 * 
 * Revision 1.3  89/05/05  18:46:21  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 *  1-Feb-89  Bennet Yee (bsy) at Carnegie-Mellon University
 *	Fixed msg_{receive,send} if-if-else bug.
 *
 * 21-Oct-88  Richard Draves (rpd) at Carnegie-Mellon University
 *	Added msg_send wrapper, which handles SEND_INTERRUPT.
 *	Fixed bug in msg_rpc wrapper; it gave the wrong size to	msg_receive_.
 *	Converted to first try the new *_trap calls and fall back on
 *	the (renamed) *_old calls if they don't work.
 *
 * 19-May-87  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Fixed the test for interupts in msg_rpc_.
 *	(Copied from mwyoung's version.)
 */

#include <mach/kern_return.h>
#include <mach/message.h>

extern msg_return_t msg_send_trap();
extern msg_return_t msg_receive_trap();
extern msg_return_t msg_rpc_trap();

msg_return_t	msg_send(header, option, timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_timeout_t	timeout;
{
	register
	msg_return_t	result;


	result = msg_send_trap(header, option, header->msg_size, timeout);
	if (result == SEND_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
		while ((result = msg_send_old(header, option, timeout))
		       			== SEND_INTERRUPTED)
			if (option & SEND_INTERRUPT)
				break;
	} else if ((result == SEND_INTERRUPTED) &&
		 !(option & SEND_INTERRUPT))
		do
			result = msg_send_trap(header, option,
					       header->msg_size, timeout);
		while (result == SEND_INTERRUPTED);

	return result;
}

msg_return_t	msg_receive(header, option, timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_timeout_t	timeout;
{
	register
	msg_return_t	result;

	result = msg_receive_trap(header, option, header->msg_size,
				  header->msg_local_port, timeout);
	if (result == RCV_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
		while ((result = msg_receive_old(header, option, timeout))
					== RCV_INTERRUPTED)
			if (option & RCV_INTERRUPT)
				break;
	} else if ((result == RCV_INTERRUPTED) &&
		 !(option & RCV_INTERRUPT))
		do
			result = msg_receive_trap(header, option,
						  header->msg_size,
						  header->msg_local_port,
						  timeout);
		while (result == RCV_INTERRUPTED);

	return result;
}

msg_return_t	msg_rpc(header, option, rcv_size, send_timeout, rcv_timeout)
	msg_header_t	*header;
	msg_option_t	option;
	msg_size_t	rcv_size;
	msg_timeout_t	send_timeout;
	msg_timeout_t	rcv_timeout;
{
	register
	msg_return_t	result;
	
	result = msg_rpc_trap(header, option, header->msg_size,
			      rcv_size, send_timeout, rcv_timeout);
	if (result == RPC_SUCCESS)
		return result;

	if (result == KERN_INVALID_ARGUMENT) {
		while ((result = msg_rpc_old(header, option, rcv_size,
					      send_timeout, rcv_timeout))
					== SEND_INTERRUPTED)
			if (option & SEND_INTERRUPT)
				return result;
		if ((result == RCV_INTERRUPTED) &&
		    !(option & RCV_INTERRUPT)) {
			header->msg_size = rcv_size;
			do
				result = msg_receive_old(header, option,
							 rcv_timeout);
			while (result == RCV_INTERRUPTED);
		}

		return result;
	} else if ((result == SEND_INTERRUPTED) &&
		   !(option & SEND_INTERRUPT)) {
		do
			result = msg_rpc_trap(header, option,
					      header->msg_size, rcv_size,
					      send_timeout, rcv_timeout);
		while (result == SEND_INTERRUPT);
	}

	if ((result == RCV_INTERRUPTED) &&
	    !(option & RCV_INTERRUPT))
		do
			result = msg_receive_trap(header, option, rcv_size,
						  header->msg_local_port,
						  rcv_timeout);
		while (result == RCV_INTERRUPTED);

	return result;
}
