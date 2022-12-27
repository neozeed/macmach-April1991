#ifndef	_memory_object_default
#define	_memory_object_default

/* Module memory_object_default */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_memory_object_default
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <kern/mig_user_hack.h>

/* SimpleRoutine memory_object_create */
mig_external kern_return_t memory_object_create
#if	defined(LINTLIBRARY)
    (old_memory_object, new_memory_object, new_object_size, new_control_port, new_name, new_page_size)
	memory_object_t old_memory_object;
	memory_object_t new_memory_object;
	vm_size_t new_object_size;
	memory_object_control_t new_control_port;
	memory_object_name_t new_name;
	vm_size_t new_page_size;
{ return memory_object_create(old_memory_object, new_memory_object, new_object_size, new_control_port, new_name, new_page_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t old_memory_object,
	memory_object_t new_memory_object,
	vm_size_t new_object_size,
	memory_object_control_t new_control_port,
	memory_object_name_t new_name,
	vm_size_t new_page_size
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_initialize */
mig_external kern_return_t memory_object_data_initialize
#if	defined(LINTLIBRARY)
    (memory_object, memory_control_port, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control_port;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
{ return memory_object_data_initialize(memory_object, memory_control_port, offset, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control_port,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
);
#else
    ();
#endif
#endif

#endif	_memory_object_default
