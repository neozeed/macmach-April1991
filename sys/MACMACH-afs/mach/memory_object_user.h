#ifndef	_memory_object
#define	_memory_object

/* Module memory_object */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_memory_object
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <kern/mig_user_hack.h>

/* SimpleRoutine memory_object_init */
mig_external kern_return_t memory_object_init
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, memory_object_name, memory_object_page_size)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	memory_object_name_t memory_object_name;
	vm_size_t memory_object_page_size;
{ return memory_object_init(memory_object, memory_control, memory_object_name, memory_object_page_size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	memory_object_name_t memory_object_name,
	vm_size_t memory_object_page_size
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_terminate */
mig_external kern_return_t memory_object_terminate
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, memory_object_name)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	memory_object_name_t memory_object_name;
{ return memory_object_terminate(memory_object, memory_control, memory_object_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	memory_object_name_t memory_object_name
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_copy */
mig_external kern_return_t memory_object_copy
#if	defined(LINTLIBRARY)
    (old_memory_object, old_memory_control, offset, length, new_memory_object)
	memory_object_t old_memory_object;
	memory_object_control_t old_memory_control;
	vm_offset_t offset;
	vm_size_t length;
	memory_object_t new_memory_object;
{ return memory_object_copy(old_memory_object, old_memory_control, offset, length, new_memory_object); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t old_memory_object,
	memory_object_control_t old_memory_control,
	vm_offset_t offset,
	vm_size_t length,
	memory_object_t new_memory_object
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_request */
mig_external kern_return_t memory_object_data_request
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, length, desired_access)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{ return memory_object_data_request(memory_object, memory_control, offset, length, desired_access); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	vm_prot_t desired_access
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_unlock */
mig_external kern_return_t memory_object_data_unlock
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, length, desired_access)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	vm_prot_t desired_access;
{ return memory_object_data_unlock(memory_object, memory_control, offset, length, desired_access); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	vm_prot_t desired_access
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_write */
mig_external kern_return_t memory_object_data_write
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
{ return memory_object_data_write(memory_object, memory_control, offset, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_lock_completed */
mig_external kern_return_t memory_object_lock_completed
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, length)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
{ return memory_object_lock_completed(memory_object, memory_control, offset, length); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_supply_completed */
mig_external kern_return_t memory_object_supply_completed
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, length, result, error_offset)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t length;
	kern_return_t result;
	vm_offset_t error_offset;
{ return memory_object_supply_completed(memory_object, memory_control, offset, length, result, error_offset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t length,
	kern_return_t result,
	vm_offset_t error_offset
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_return */
mig_external kern_return_t memory_object_data_return
#if	defined(LINTLIBRARY)
    (memory_object, memory_control, offset, data, dataCnt)
	memory_object_t memory_object;
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
{ return memory_object_data_return(memory_object, memory_control, offset, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_t memory_object,
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt
);
#else
    ();
#endif
#endif

#endif	_memory_object
