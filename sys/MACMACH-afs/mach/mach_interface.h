#ifndef	_mach
#define	_mach

/* Module mach */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_mach
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <mach/mach_extra.h>

/* Routine xxx_port_allocate */
mig_external kern_return_t xxx_port_allocate
#if	defined(LINTLIBRARY)
    (target_task, my_port)
	task_t target_task;
	port_t *my_port;
{ return xxx_port_allocate(target_task, my_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t *my_port
);
#else
    ();
#endif
#endif

/* Routine xxx_port_deallocate */
mig_external kern_return_t xxx_port_deallocate
#if	defined(LINTLIBRARY)
    (target_task, my_port)
	task_t target_task;
	port_t my_port;
{ return xxx_port_deallocate(target_task, my_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port
);
#else
    ();
#endif
#endif

/* Routine xxx_port_enable */
mig_external kern_return_t xxx_port_enable
#if	defined(LINTLIBRARY)
    (target_task, my_port)
	task_t target_task;
	port_t my_port;
{ return xxx_port_enable(target_task, my_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port
);
#else
    ();
#endif
#endif

/* Routine xxx_port_disable */
mig_external kern_return_t xxx_port_disable
#if	defined(LINTLIBRARY)
    (target_task, my_port)
	task_t target_task;
	port_t my_port;
{ return xxx_port_disable(target_task, my_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port
);
#else
    ();
#endif
#endif

/* Routine xxx_port_select */
mig_external kern_return_t xxx_port_select
#if	defined(LINTLIBRARY)
    (target_task, port_list, port_listCnt)
	task_t target_task;
	port_array_t *port_list;
	unsigned int *port_listCnt;
{ return xxx_port_select(target_task, port_list, port_listCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_array_t *port_list,
	unsigned int *port_listCnt
);
#else
    ();
#endif
#endif

/* Routine xxx_port_set_backlog */
mig_external kern_return_t xxx_port_set_backlog
#if	defined(LINTLIBRARY)
    (target_task, my_port, backlog)
	task_t target_task;
	port_t my_port;
	int backlog;
{ return xxx_port_set_backlog(target_task, my_port, backlog); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port,
	int backlog
);
#else
    ();
#endif
#endif

/* Routine xxx_port_status */
mig_external kern_return_t xxx_port_status
#if	defined(LINTLIBRARY)
    (target_task, my_port, enabled, num_msgs, backlog, ownership, receive_rights)
	task_t target_task;
	port_t my_port;
	boolean_t *enabled;
	int *num_msgs;
	int *backlog;
	boolean_t *ownership;
	boolean_t *receive_rights;
{ return xxx_port_status(target_task, my_port, enabled, num_msgs, backlog, ownership, receive_rights); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_t my_port,
	boolean_t *enabled,
	int *num_msgs,
	int *backlog,
	boolean_t *ownership,
	boolean_t *receive_rights
);
#else
    ();
#endif
#endif

/* Routine task_create */
mig_external kern_return_t task_create
#if	defined(LINTLIBRARY)
    (target_task, inherit_memory, child_task)
	task_t target_task;
	boolean_t inherit_memory;
	task_t *child_task;
{ return task_create(target_task, inherit_memory, child_task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	boolean_t inherit_memory,
	task_t *child_task
);
#else
    ();
#endif
#endif

/* Routine task_terminate */
mig_external kern_return_t task_terminate
#if	defined(LINTLIBRARY)
    (target_task)
	task_t target_task;
{ return task_terminate(target_task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task
);
#else
    ();
#endif
#endif

/* Routine task_threads */
mig_external kern_return_t task_threads
#if	defined(LINTLIBRARY)
    (target_task, thread_list, thread_listCnt)
	task_t target_task;
	thread_array_t *thread_list;
	unsigned int *thread_listCnt;
{ return task_threads(target_task, thread_list, thread_listCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	thread_array_t *thread_list,
	unsigned int *thread_listCnt
);
#else
    ();
#endif
#endif

/* Routine thread_terminate */
mig_external kern_return_t thread_terminate
#if	defined(LINTLIBRARY)
    (target_thread)
	thread_t target_thread;
{ return thread_terminate(target_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread
);
#else
    ();
#endif
#endif

/* Routine vm_allocate */
mig_external kern_return_t vm_allocate
#if	defined(LINTLIBRARY)
    (target_task, address, size, anywhere)
	vm_task_t target_task;
	vm_address_t *address;
	vm_size_t size;
	boolean_t anywhere;
{ return vm_allocate(target_task, address, size, anywhere); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t *address,
	vm_size_t size,
	boolean_t anywhere
);
#else
    ();
#endif
#endif

/* Routine vm_deallocate */
mig_external kern_return_t vm_deallocate
#if	defined(LINTLIBRARY)
    (target_task, address, size)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
{ return vm_deallocate(target_task, address, size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	vm_size_t size
);
#else
    ();
#endif
#endif

/* Routine vm_protect */
mig_external kern_return_t vm_protect
#if	defined(LINTLIBRARY)
    (target_task, address, size, set_maximum, new_protection)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
	boolean_t set_maximum;
	vm_prot_t new_protection;
{ return vm_protect(target_task, address, size, set_maximum, new_protection); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	vm_size_t size,
	boolean_t set_maximum,
	vm_prot_t new_protection
);
#else
    ();
#endif
#endif

/* Routine vm_inherit */
mig_external kern_return_t vm_inherit
#if	defined(LINTLIBRARY)
    (target_task, address, size, new_inheritance)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
	vm_inherit_t new_inheritance;
{ return vm_inherit(target_task, address, size, new_inheritance); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	vm_size_t size,
	vm_inherit_t new_inheritance
);
#else
    ();
#endif
#endif

/* Routine vm_read */
mig_external kern_return_t vm_read
#if	defined(LINTLIBRARY)
    (target_task, address, size, data, dataCnt)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
	pointer_t *data;
	unsigned int *dataCnt;
{ return vm_read(target_task, address, size, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	vm_size_t size,
	pointer_t *data,
	unsigned int *dataCnt
);
#else
    ();
#endif
#endif

/* Routine vm_write */
mig_external kern_return_t vm_write
#if	defined(LINTLIBRARY)
    (target_task, address, data, dataCnt)
	vm_task_t target_task;
	vm_address_t address;
	pointer_t data;
	unsigned int dataCnt;
{ return vm_write(target_task, address, data, dataCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	pointer_t data,
	unsigned int dataCnt
);
#else
    ();
#endif
#endif

/* Routine vm_copy */
mig_external kern_return_t vm_copy
#if	defined(LINTLIBRARY)
    (target_task, source_address, size, dest_address)
	vm_task_t target_task;
	vm_address_t source_address;
	vm_size_t size;
	vm_address_t dest_address;
{ return vm_copy(target_task, source_address, size, dest_address); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t source_address,
	vm_size_t size,
	vm_address_t dest_address
);
#else
    ();
#endif
#endif

/* Routine vm_region */
mig_external kern_return_t vm_region
#if	defined(LINTLIBRARY)
    (target_task, address, size, protection, max_protection, inheritance, is_shared, object_name, offset)
	vm_task_t target_task;
	vm_address_t *address;
	vm_size_t *size;
	vm_prot_t *protection;
	vm_prot_t *max_protection;
	vm_inherit_t *inheritance;
	boolean_t *is_shared;
	memory_object_name_t *object_name;
	vm_offset_t *offset;
{ return vm_region(target_task, address, size, protection, max_protection, inheritance, is_shared, object_name, offset); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t *address,
	vm_size_t *size,
	vm_prot_t *protection,
	vm_prot_t *max_protection,
	vm_inherit_t *inheritance,
	boolean_t *is_shared,
	memory_object_name_t *object_name,
	vm_offset_t *offset
);
#else
    ();
#endif
#endif

/* Routine vm_statistics */
mig_external kern_return_t vm_statistics
#if	defined(LINTLIBRARY)
    (target_task, vm_stats)
	vm_task_t target_task;
	vm_statistics_data_t *vm_stats;
{ return vm_statistics(target_task, vm_stats); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_statistics_data_t *vm_stats
);
#else
    ();
#endif
#endif

/* Routine task_by_unix_pid */
mig_external kern_return_t task_by_unix_pid
#if	defined(LINTLIBRARY)
    (target_task, process_id, result_task)
	task_t target_task;
	int process_id;
	task_t *result_task;
{ return task_by_unix_pid(target_task, process_id, result_task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	int process_id,
	task_t *result_task
);
#else
    ();
#endif
#endif

/* Routine mach_ports_register */
mig_external kern_return_t mach_ports_register
#if	defined(LINTLIBRARY)
    (target_task, init_port_set, init_port_setCnt)
	task_t target_task;
	port_array_t init_port_set;
	unsigned int init_port_setCnt;
{ return mach_ports_register(target_task, init_port_set, init_port_setCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_array_t init_port_set,
	unsigned int init_port_setCnt
);
#else
    ();
#endif
#endif

/* Routine mach_ports_lookup */
mig_external kern_return_t mach_ports_lookup
#if	defined(LINTLIBRARY)
    (target_task, init_port_set, init_port_setCnt)
	task_t target_task;
	port_array_t *init_port_set;
	unsigned int *init_port_setCnt;
{ return mach_ports_lookup(target_task, init_port_set, init_port_setCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	port_array_t *init_port_set,
	unsigned int *init_port_setCnt
);
#else
    ();
#endif
#endif

/* Routine unix_pid */
mig_external kern_return_t unix_pid
#if	defined(LINTLIBRARY)
    (target_task, process_id)
	task_t target_task;
	int *process_id;
{ return unix_pid(target_task, process_id); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	int *process_id
);
#else
    ();
#endif
#endif

/* Routine netipc_listen */
mig_external kern_return_t netipc_listen
#if	defined(LINTLIBRARY)
    (request_port, src_addr, dst_addr, src_port, dst_port, protocol, ipc_port)
	port_t request_port;
	int src_addr;
	int dst_addr;
	int src_port;
	int dst_port;
	int protocol;
	port_t ipc_port;
{ return netipc_listen(request_port, src_addr, dst_addr, src_port, dst_port, protocol, ipc_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	int src_addr,
	int dst_addr,
	int src_port,
	int dst_port,
	int protocol,
	port_t ipc_port
);
#else
    ();
#endif
#endif

/* Routine netipc_ignore */
mig_external kern_return_t netipc_ignore
#if	defined(LINTLIBRARY)
    (request_port, ipc_port)
	port_t request_port;
	port_t ipc_port;
{ return netipc_ignore(request_port, ipc_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t request_port,
	port_t ipc_port
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_provided */
mig_external kern_return_t memory_object_data_provided
#if	defined(LINTLIBRARY)
    (memory_control, offset, data, dataCnt, lock_value)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
	vm_prot_t lock_value;
{ return memory_object_data_provided(memory_control, offset, data, dataCnt, lock_value); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt,
	vm_prot_t lock_value
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_unavailable */
mig_external kern_return_t memory_object_data_unavailable
#if	defined(LINTLIBRARY)
    (memory_control, offset, size)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t size;
{ return memory_object_data_unavailable(memory_control, offset, size); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t size
);
#else
    ();
#endif
#endif

/* Routine memory_object_get_attributes */
mig_external kern_return_t memory_object_get_attributes
#if	defined(LINTLIBRARY)
    (memory_control, object_ready, may_cache, copy_strategy)
	memory_object_control_t memory_control;
	boolean_t *object_ready;
	boolean_t *may_cache;
	memory_object_copy_strategy_t *copy_strategy;
{ return memory_object_get_attributes(memory_control, object_ready, may_cache, copy_strategy); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t *object_ready,
	boolean_t *may_cache,
	memory_object_copy_strategy_t *copy_strategy
);
#else
    ();
#endif
#endif

/* Routine vm_set_default_memory_manager */
mig_external kern_return_t vm_set_default_memory_manager
#if	defined(LINTLIBRARY)
    (host, default_manager)
	host_priv_t host;
	memory_object_t *default_manager;
{ return vm_set_default_memory_manager(host, default_manager); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	host_priv_t host,
	memory_object_t *default_manager
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_lock_request */
mig_external kern_return_t memory_object_lock_request
#if	defined(LINTLIBRARY)
    (memory_control, offset, size, should_clean, should_flush, lock_value, reply_to)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t size;
	boolean_t should_clean;
	boolean_t should_flush;
	vm_prot_t lock_value;
	port_t reply_to;
{ return memory_object_lock_request(memory_control, offset, size, should_clean, should_flush, lock_value, reply_to); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t size,
	boolean_t should_clean,
	boolean_t should_flush,
	vm_prot_t lock_value,
	port_t reply_to
);
#else
    ();
#endif
#endif

/* SimpleRoutine xxx_pager_cache */
mig_external kern_return_t xxx_pager_cache
#if	defined(LINTLIBRARY)
    (memory_control, should_cache)
	memory_object_control_t memory_control;
	boolean_t should_cache;
{ return xxx_pager_cache(memory_control, should_cache); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t should_cache
);
#else
    ();
#endif
#endif

/* Routine xxx_host_info */
mig_external kern_return_t xxx_host_info
#if	defined(LINTLIBRARY)
    (target_task, info)
	port_t target_task;
	machine_info_data_t *info;
{ return xxx_host_info(target_task, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t target_task,
	machine_info_data_t *info
);
#else
    ();
#endif
#endif

/* Routine xxx_slot_info */
mig_external kern_return_t xxx_slot_info
#if	defined(LINTLIBRARY)
    (target_task, slot, info)
	task_t target_task;
	int slot;
	machine_slot_data_t *info;
{ return xxx_slot_info(target_task, slot, info); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	int slot,
	machine_slot_data_t *info
);
#else
    ();
#endif
#endif

/* Routine xxx_cpu_control */
mig_external kern_return_t xxx_cpu_control
#if	defined(LINTLIBRARY)
    (target_task, cpu, running)
	task_t target_task;
	int cpu;
	boolean_t running;
{ return xxx_cpu_control(target_task, cpu, running); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	int cpu,
	boolean_t running
);
#else
    ();
#endif
#endif

/* Routine task_suspend */
mig_external kern_return_t task_suspend
#if	defined(LINTLIBRARY)
    (target_task)
	task_t target_task;
{ return task_suspend(target_task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task
);
#else
    ();
#endif
#endif

/* Routine task_resume */
mig_external kern_return_t task_resume
#if	defined(LINTLIBRARY)
    (target_task)
	task_t target_task;
{ return task_resume(target_task); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task
);
#else
    ();
#endif
#endif

/* Routine task_get_special_port */
mig_external kern_return_t task_get_special_port
#if	defined(LINTLIBRARY)
    (task, which_port, special_port)
	task_t task;
	int which_port;
	port_t *special_port;
{ return task_get_special_port(task, which_port, special_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	int which_port,
	port_t *special_port
);
#else
    ();
#endif
#endif

/* Routine task_set_special_port */
mig_external kern_return_t task_set_special_port
#if	defined(LINTLIBRARY)
    (task, which_port, special_port)
	task_t task;
	int which_port;
	port_t special_port;
{ return task_set_special_port(task, which_port, special_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	int which_port,
	port_t special_port
);
#else
    ();
#endif
#endif

/* Routine task_info */
mig_external kern_return_t task_info
#if	defined(LINTLIBRARY)
    (target_task, flavor, task_info_out, task_info_outCnt)
	task_t target_task;
	int flavor;
	task_info_t task_info_out;
	unsigned int *task_info_outCnt;
{ return task_info(target_task, flavor, task_info_out, task_info_outCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t target_task,
	int flavor,
	task_info_t task_info_out,
	unsigned int *task_info_outCnt
);
#else
    ();
#endif
#endif

/* Routine thread_create */
mig_external kern_return_t thread_create
#if	defined(LINTLIBRARY)
    (parent_task, child_thread)
	task_t parent_task;
	thread_t *child_thread;
{ return thread_create(parent_task, child_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t parent_task,
	thread_t *child_thread
);
#else
    ();
#endif
#endif

/* Routine thread_suspend */
mig_external kern_return_t thread_suspend
#if	defined(LINTLIBRARY)
    (target_thread)
	thread_t target_thread;
{ return thread_suspend(target_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread
);
#else
    ();
#endif
#endif

/* Routine thread_resume */
mig_external kern_return_t thread_resume
#if	defined(LINTLIBRARY)
    (target_thread)
	thread_t target_thread;
{ return thread_resume(target_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread
);
#else
    ();
#endif
#endif

/* Routine thread_abort */
mig_external kern_return_t thread_abort
#if	defined(LINTLIBRARY)
    (target_thread)
	thread_t target_thread;
{ return thread_abort(target_thread); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread
);
#else
    ();
#endif
#endif

/* Routine thread_get_state */
mig_external kern_return_t thread_get_state
#if	defined(LINTLIBRARY)
    (target_thread, flavor, old_state, old_stateCnt)
	thread_t target_thread;
	int flavor;
	thread_state_t old_state;
	unsigned int *old_stateCnt;
{ return thread_get_state(target_thread, flavor, old_state, old_stateCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread,
	int flavor,
	thread_state_t old_state,
	unsigned int *old_stateCnt
);
#else
    ();
#endif
#endif

/* Routine thread_set_state */
mig_external kern_return_t thread_set_state
#if	defined(LINTLIBRARY)
    (target_thread, flavor, new_state, new_stateCnt)
	thread_t target_thread;
	int flavor;
	thread_state_t new_state;
	unsigned int new_stateCnt;
{ return thread_set_state(target_thread, flavor, new_state, new_stateCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread,
	int flavor,
	thread_state_t new_state,
	unsigned int new_stateCnt
);
#else
    ();
#endif
#endif

/* Routine thread_get_special_port */
mig_external kern_return_t thread_get_special_port
#if	defined(LINTLIBRARY)
    (thread, which_port, special_port)
	thread_t thread;
	int which_port;
	port_t *special_port;
{ return thread_get_special_port(thread, which_port, special_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	int which_port,
	port_t *special_port
);
#else
    ();
#endif
#endif

/* Routine thread_set_special_port */
mig_external kern_return_t thread_set_special_port
#if	defined(LINTLIBRARY)
    (thread, which_port, special_port)
	thread_t thread;
	int which_port;
	port_t special_port;
{ return thread_set_special_port(thread, which_port, special_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t thread,
	int which_port,
	port_t special_port
);
#else
    ();
#endif
#endif

/* Routine thread_info */
mig_external kern_return_t thread_info
#if	defined(LINTLIBRARY)
    (target_thread, flavor, thread_info_out, thread_info_outCnt)
	thread_t target_thread;
	int flavor;
	thread_info_t thread_info_out;
	unsigned int *thread_info_outCnt;
{ return thread_info(target_thread, flavor, thread_info_out, thread_info_outCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	thread_t target_thread,
	int flavor,
	thread_info_t thread_info_out,
	unsigned int *thread_info_outCnt
);
#else
    ();
#endif
#endif

/* Routine port_names */
mig_external kern_return_t port_names
#if	defined(LINTLIBRARY)
    (task, port_names_p, port_names_pCnt, port_types, port_typesCnt)
	task_t task;
	port_name_array_t *port_names_p;
	unsigned int *port_names_pCnt;
	port_type_array_t *port_types;
	unsigned int *port_typesCnt;
{ return port_names(task, port_names_p, port_names_pCnt, port_types, port_typesCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_array_t *port_names_p,
	unsigned int *port_names_pCnt,
	port_type_array_t *port_types,
	unsigned int *port_typesCnt
);
#else
    ();
#endif
#endif

/* Routine port_type */
mig_external kern_return_t port_type
#if	defined(LINTLIBRARY)
    (task, port_name, port_type_p)
	task_t task;
	port_name_t port_name;
	port_type_t *port_type_p;
{ return port_type(task, port_name, port_type_p); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name,
	port_type_t *port_type_p
);
#else
    ();
#endif
#endif

/* Routine port_rename */
mig_external kern_return_t port_rename
#if	defined(LINTLIBRARY)
    (task, old_name, new_name)
	task_t task;
	port_name_t old_name;
	port_name_t new_name;
{ return port_rename(task, old_name, new_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t old_name,
	port_name_t new_name
);
#else
    ();
#endif
#endif

/* Routine port_allocate */
mig_external kern_return_t port_allocate
#if	defined(LINTLIBRARY)
    (task, port_name)
	task_t task;
	port_name_t *port_name;
{ return port_allocate(task, port_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t *port_name
);
#else
    ();
#endif
#endif

/* Routine port_deallocate */
mig_external kern_return_t port_deallocate
#if	defined(LINTLIBRARY)
    (task, port_name)
	task_t task;
	port_name_t port_name;
{ return port_deallocate(task, port_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name
);
#else
    ();
#endif
#endif

/* Routine port_set_backlog */
mig_external kern_return_t port_set_backlog
#if	defined(LINTLIBRARY)
    (task, port_name, backlog)
	task_t task;
	port_name_t port_name;
	int backlog;
{ return port_set_backlog(task, port_name, backlog); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name,
	int backlog
);
#else
    ();
#endif
#endif

/* Routine port_status */
mig_external kern_return_t port_status
#if	defined(LINTLIBRARY)
    (task, port_name, enabled, num_msgs, backlog, ownership, receive_rights)
	task_t task;
	port_name_t port_name;
	port_set_name_t *enabled;
	int *num_msgs;
	int *backlog;
	boolean_t *ownership;
	boolean_t *receive_rights;
{ return port_status(task, port_name, enabled, num_msgs, backlog, ownership, receive_rights); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name,
	port_set_name_t *enabled,
	int *num_msgs,
	int *backlog,
	boolean_t *ownership,
	boolean_t *receive_rights
);
#else
    ();
#endif
#endif

/* Routine port_set_allocate */
mig_external kern_return_t port_set_allocate
#if	defined(LINTLIBRARY)
    (task, set_name)
	task_t task;
	port_set_name_t *set_name;
{ return port_set_allocate(task, set_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_set_name_t *set_name
);
#else
    ();
#endif
#endif

/* Routine port_set_deallocate */
mig_external kern_return_t port_set_deallocate
#if	defined(LINTLIBRARY)
    (task, set_name)
	task_t task;
	port_set_name_t set_name;
{ return port_set_deallocate(task, set_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_set_name_t set_name
);
#else
    ();
#endif
#endif

/* Routine port_set_add */
mig_external kern_return_t port_set_add
#if	defined(LINTLIBRARY)
    (task, set_name, port_name)
	task_t task;
	port_set_name_t set_name;
	port_name_t port_name;
{ return port_set_add(task, set_name, port_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_set_name_t set_name,
	port_name_t port_name
);
#else
    ();
#endif
#endif

/* Routine port_set_remove */
mig_external kern_return_t port_set_remove
#if	defined(LINTLIBRARY)
    (task, port_name)
	task_t task;
	port_name_t port_name;
{ return port_set_remove(task, port_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name
);
#else
    ();
#endif
#endif

/* Routine port_set_status */
mig_external kern_return_t port_set_status
#if	defined(LINTLIBRARY)
    (task, set_name, members, membersCnt)
	task_t task;
	port_set_name_t set_name;
	port_name_array_t *members;
	unsigned int *membersCnt;
{ return port_set_status(task, set_name, members, membersCnt); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_set_name_t set_name,
	port_name_array_t *members,
	unsigned int *membersCnt
);
#else
    ();
#endif
#endif

/* Routine port_insert_send */
mig_external kern_return_t port_insert_send
#if	defined(LINTLIBRARY)
    (task, my_port, his_name)
	task_t task;
	port_t my_port;
	port_name_t his_name;
{ return port_insert_send(task, my_port, his_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_t my_port,
	port_name_t his_name
);
#else
    ();
#endif
#endif

/* Routine port_extract_send */
mig_external kern_return_t port_extract_send
#if	defined(LINTLIBRARY)
    (task, his_name, his_port)
	task_t task;
	port_name_t his_name;
	port_t *his_port;
{ return port_extract_send(task, his_name, his_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t his_name,
	port_t *his_port
);
#else
    ();
#endif
#endif

/* Routine port_insert_receive */
mig_external kern_return_t port_insert_receive
#if	defined(LINTLIBRARY)
    (task, my_port, his_name)
	task_t task;
	port_t my_port;
	port_name_t his_name;
{ return port_insert_receive(task, my_port, his_name); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_t my_port,
	port_name_t his_name
);
#else
    ();
#endif
#endif

/* Routine port_extract_receive */
mig_external kern_return_t port_extract_receive
#if	defined(LINTLIBRARY)
    (task, his_name, his_port)
	task_t task;
	port_name_t his_name;
	port_t *his_port;
{ return port_extract_receive(task, his_name, his_port); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t his_name,
	port_t *his_port
);
#else
    ();
#endif
#endif

/* Routine vm_map */
mig_external kern_return_t vm_map
#if	defined(LINTLIBRARY)
    (target_task, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	vm_task_t target_task;
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
{ return vm_map(target_task, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
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

/* SimpleRoutine memory_object_data_error */
mig_external kern_return_t memory_object_data_error
#if	defined(LINTLIBRARY)
    (memory_control, offset, size, error_value)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	vm_size_t size;
	kern_return_t error_value;
{ return memory_object_data_error(memory_control, offset, size, error_value); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	vm_size_t size,
	kern_return_t error_value
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_set_attributes */
mig_external kern_return_t memory_object_set_attributes
#if	defined(LINTLIBRARY)
    (memory_control, object_ready, may_cache, copy_strategy)
	memory_object_control_t memory_control;
	boolean_t object_ready;
	boolean_t may_cache;
	memory_object_copy_strategy_t copy_strategy;
{ return memory_object_set_attributes(memory_control, object_ready, may_cache, copy_strategy); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	boolean_t object_ready,
	boolean_t may_cache,
	memory_object_copy_strategy_t copy_strategy
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_destroy */
mig_external kern_return_t memory_object_destroy
#if	defined(LINTLIBRARY)
    (memory_control, reason)
	memory_object_control_t memory_control;
	kern_return_t reason;
{ return memory_object_destroy(memory_control, reason); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	kern_return_t reason
);
#else
    ();
#endif
#endif

/* SimpleRoutine memory_object_data_supply */
mig_external kern_return_t memory_object_data_supply
#if	defined(LINTLIBRARY)
    (memory_control, offset, data, dataCnt, dealloc_data, lock_value, precious, reply_to)
	memory_object_control_t memory_control;
	vm_offset_t offset;
	pointer_t data;
	unsigned int dataCnt;
	boolean_t dealloc_data;
	vm_prot_t lock_value;
	boolean_t precious;
	port_t reply_to;
{ return memory_object_data_supply(memory_control, offset, data, dataCnt, dealloc_data, lock_value, precious, reply_to); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	memory_object_control_t memory_control,
	vm_offset_t offset,
	pointer_t data,
	unsigned int dataCnt,
	boolean_t dealloc_data,
	vm_prot_t lock_value,
	boolean_t precious,
	port_t reply_to
);
#else
    ();
#endif
#endif

/* Routine port_set_backup */
mig_external kern_return_t port_set_backup
#if	defined(LINTLIBRARY)
    (task, port_name, backup, previous)
	task_t task;
	port_name_t port_name;
	port_t backup;
	port_t *previous;
{ return port_set_backup(task, port_name, backup, previous); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	task_t task,
	port_name_t port_name,
	port_t backup,
	port_t *previous
);
#else
    ();
#endif
#endif

/* Routine vm_machine_attribute */
mig_external kern_return_t vm_machine_attribute
#if	defined(LINTLIBRARY)
    (target_task, address, size, attribute, value)
	vm_task_t target_task;
	vm_address_t address;
	vm_size_t size;
	vm_machine_attribute_t attribute;
	vm_machine_attribute_val_t *value;
{ return vm_machine_attribute(target_task, address, size, attribute, value); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	vm_task_t target_task,
	vm_address_t address,
	vm_size_t size,
	vm_machine_attribute_t attribute,
	vm_machine_attribute_val_t *value
);
#else
    ();
#endif
#endif

#endif	_mach
