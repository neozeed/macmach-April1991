#ifndef	_mac
#define	_mac

/* Module mac */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_mac
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <mac2emul/macserver_types.h>

/* Routine mac_create */
mig_external kern_return_t mac_create
#if	defined(LINTLIBRARY)
    (server_port, thread_port, request_port)
	port_t server_port;
	port_t thread_port;
	port_t *request_port;
{ return mac_create(server_port, thread_port, request_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t server_port,
	port_t thread_port,
	port_t *request_port
);
#else
    ();
#endif
#endif

/* Routine mac_emulate */
mig_external kern_return_t mac_emulate
#if	defined(LINTLIBRARY)
    (request_port, reset_vector, os_vector, os_map, tbox_vector, tbox_map)
	port_t request_port;
	vm_address_t reset_vector;
	vm_address_t os_vector;
	mac_os_map_t os_map;
	vm_address_t tbox_vector;
	mac_tbox_map_t tbox_map;
{ return mac_emulate(request_port, reset_vector, os_vector, os_map, tbox_vector, tbox_map); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	vm_address_t reset_vector,
	vm_address_t os_vector,
	mac_os_map_t os_map,
	vm_address_t tbox_vector,
	mac_tbox_map_t tbox_map
);
#else
    ();
#endif
#endif

/* SimpleRoutine mac_phys_insert */
mig_external kern_return_t mac_phys_insert
#if	defined(LINTLIBRARY)
    (request_port, memory_object, offset, size, perm, physical)
	port_t request_port;
	memory_object_t memory_object;
	vm_offset_t offset;
	vm_size_t size;
	vm_prot_t perm;
	vm_offset_t physical;
{ return mac_phys_insert(request_port, memory_object, offset, size, perm, physical); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	memory_object_t memory_object,
	vm_offset_t offset,
	vm_size_t size,
	vm_prot_t perm,
	vm_offset_t physical
);
#else
    ();
#endif
#endif

/* Routine mac_interrupt_vector */
mig_external kern_return_t mac_interrupt_vector
#if	defined(LINTLIBRARY)
    (request_port, which_intr, new_vector)
	port_t request_port;
	int which_intr;
	vm_address_t new_vector;
{ return mac_interrupt_vector(request_port, which_intr, new_vector); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	vm_address_t new_vector
);
#else
    ();
#endif
#endif

/* Routine mac_interrupt_enable */
mig_external kern_return_t mac_interrupt_enable
#if	defined(LINTLIBRARY)
    (request_port, which_intr, enable)
	port_t request_port;
	int which_intr;
	boolean_t enable;
{ return mac_interrupt_enable(request_port, which_intr, enable); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	boolean_t enable
);
#else
    ();
#endif
#endif

/* Routine mac_interrupt_post */
mig_external kern_return_t mac_interrupt_post
#if	defined(LINTLIBRARY)
    (request_port, which_intr, post)
	port_t request_port;
	int which_intr;
	boolean_t post;
{ return mac_interrupt_post(request_port, which_intr, post); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int which_intr,
	boolean_t post
);
#else
    ();
#endif
#endif

/* Routine mac_input_event */
mig_external kern_return_t mac_input_event
#if	defined(LINTLIBRARY)
    (request_port, event)
	port_t request_port;
	mac_input_event_t *event;
{ return mac_input_event(request_port, event); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	mac_input_event_t *event
);
#else
    ();
#endif
#endif

/* Routine mac_vm_map_compat */
mig_external kern_return_t mac_vm_map_compat
#if	defined(LINTLIBRARY)
    (request_port, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	port_t request_port;
	vm_address_t *address;
	vm_size_t size;
	vm_address_t mask;
	boolean_t anywhere;
	memory_object_t memory_object;
	vm_offset_t offset;
	boolean_t copy;
	vm_prot_t cur_protection;
	vm_prot_t max_protection;
	vm_inherit_t inheritance;
{ return mac_vm_map_compat(request_port, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	vm_address_t *address,
	vm_size_t size,
	vm_address_t mask,
	boolean_t anywhere,
	memory_object_t memory_object,
	vm_offset_t offset,
	boolean_t copy,
	vm_prot_t cur_protection,
	vm_prot_t max_protection,
	vm_inherit_t inheritance
);
#else
    ();
#endif
#endif
#include <mac2emul/macether_types.h>

/* Routine mac_ether_send */
mig_external kern_return_t mac_ether_send
#if	defined(LINTLIBRARY)
    (request_port, packet, packetCnt)
	port_t request_port;
	ether_packet_t packet;
	unsigned int packetCnt;
{ return mac_ether_send(request_port, packet, packetCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_packet_t packet,
	unsigned int packetCnt
);
#else
    ();
#endif
#endif

/* Routine mac_ether_recv */
mig_external kern_return_t mac_ether_recv
#if	defined(LINTLIBRARY)
    (request_port, packet, packetCnt)
	port_t request_port;
	ether_packet_t packet;
	unsigned int *packetCnt;
{ return mac_ether_recv(request_port, packet, packetCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_packet_t packet,
	unsigned int *packetCnt
);
#else
    ();
#endif
#endif

/* Routine mac_ether_add_protocol */
mig_external kern_return_t mac_ether_add_protocol
#if	defined(LINTLIBRARY)
    (request_port, protocol)
	port_t request_port;
	short protocol;
{ return mac_ether_add_protocol(request_port, protocol); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	short protocol
);
#else
    ();
#endif
#endif

/* Routine mac_ether_delete_protocol */
mig_external kern_return_t mac_ether_delete_protocol
#if	defined(LINTLIBRARY)
    (request_port, protocol)
	port_t request_port;
	short protocol;
{ return mac_ether_delete_protocol(request_port, protocol); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	short protocol
);
#else
    ();
#endif
#endif

/* Routine mac_ether_address */
mig_external kern_return_t mac_ether_address
#if	defined(LINTLIBRARY)
    (request_port, address)
	port_t request_port;
	ether_address_t address;
{ return mac_ether_address(request_port, address); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
);
#else
    ();
#endif
#endif

/* Routine mac_ether_add_multicast */
mig_external kern_return_t mac_ether_add_multicast
#if	defined(LINTLIBRARY)
    (request_port, address)
	port_t request_port;
	ether_address_t address;
{ return mac_ether_add_multicast(request_port, address); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
);
#else
    ();
#endif
#endif

/* Routine mac_ether_delete_multicast */
mig_external kern_return_t mac_ether_delete_multicast
#if	defined(LINTLIBRARY)
    (request_port, address)
	port_t request_port;
	ether_address_t address;
{ return mac_ether_delete_multicast(request_port, address); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	ether_address_t address
);
#else
    ();
#endif
#endif

#endif	_mac
