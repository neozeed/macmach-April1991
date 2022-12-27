echo "#! /bin/sh"
echo ""
echo "[ -d include ] || mkdir include"
echo "[ -d include/mac2 ] || mkdir include/mac2"
echo "[ -d include/mach ] || mkdir include/mach"
echo "[ -d include/mach/ca ] || mkdir include/mach/ca"
echo "[ -d include/mach/mac2 ] || mkdir include/mach/mac2"
echo "[ -d include/mach/machine ] || mkdir include/mach/machine"
echo "[ -d include/mach/vax ] || mkdir include/mach/mac2"
echo "[ -d include/mach/sun3 ] || mkdir include/mach/mac2"
echo "[ -d include/mach_debug ] || mkdir include/mach_debug"
echo "[ -d include/servers ] || mkdir include/servers"
echo "[ -d include/sys ] || mkdir include/sys"
echo "[ -d libmach ] || mkdir libmach"
echo ""
echo "echo \"README\""
echo "README" >/dev/tty
echo "cat >README <<'@EOF@'"
cat README
echo "@EOF@"
echo ""
echo "echo \"libmach/libmach.o.make\""
echo "libmach/libmach.o.make" >/dev/tty
echo "cat >libmach/libmach.o.make <<'@EOF@'"
cat libmach.o.make
echo "@EOF@"
echo ""
echo "echo \"include/mach.h\""
echo "include/mach.h" >/dev/tty
echo "cat >include/mach.h <<'@EOF@'"
cat include/mach.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_error.h\""
echo "include/mach_error.h" >/dev/tty
echo "cat >include/mach_error.h <<'@EOF@'"
cat include/mach_error.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_exception.h\""
echo "include/mach_exception.h" >/dev/tty
echo "cat >include/mach_exception.h <<'@EOF@'"
cat include/mach_exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_init.h\""
echo "include/mach_init.h" >/dev/tty
echo "cat >include/mach_init.h <<'@EOF@'"
cat include/mach_init.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_interface.h\""
echo "include/mach_interface.h" >/dev/tty
echo "cat >include/mach_interface.h <<'@EOF@'"
cat include/mach_interface.h
echo "@EOF@"
echo ""
echo "echo \"include/sys/syscall.h\""
echo "include/sys/syscall.h" >/dev/tty
echo "cat >include/sys/syscall.h <<'@EOF@'"
cat include/sys/syscall.h
echo "@EOF@"
echo ""
echo "echo \"include/sys/types.h\""
echo "include/sys/types.h" >/dev/tty
echo "cat >include/sys/types.h <<'@EOF@'"
cat include/sys/types.h
echo "@EOF@"
echo ""
echo "echo \"include/mac2/machparam.h\""
echo "include/mac2/machparam.h" >/dev/tty
echo "cat >include/mac2/machparam.h <<'@EOF@'"
cat include/mac2/machparam.h
echo "@EOF@"
echo ""
echo "echo \"include/mac2/endian.h\""
echo "include/mac2/endian.h" >/dev/tty
echo "cat >include/mac2/endian.h <<'@EOF@'"
cat include/mac2/endian.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/errorlib.h\""
echo "include/servers/errorlib.h" >/dev/tty
echo "cat >include/servers/errorlib.h <<'@EOF@'"
cat include/servers/errorlib.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/netname_defs.h\""
echo "include/servers/netname_defs.h" >/dev/tty
echo "cat >include/servers/netname_defs.h <<'@EOF@'"
cat include/servers/netname_defs.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/netmemory_defs.h\""
echo "include/servers/netmemory_defs.h" >/dev/tty
echo "cat >include/servers/netmemory_defs.h <<'@EOF@'"
cat include/servers/netmemory_defs.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/emdefs.h\""
echo "include/servers/emdefs.h" >/dev/tty
echo "cat >include/servers/emdefs.h <<'@EOF@'"
cat include/servers/emdefs.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/ipcx_types.h\""
echo "include/servers/ipcx_types.h" >/dev/tty
echo "cat >include/servers/ipcx_types.h <<'@EOF@'"
cat include/servers/ipcx_types.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/netname.h\""
echo "include/servers/netname.h" >/dev/tty
echo "cat >include/servers/netname.h <<'@EOF@'"
cat include/servers/netname.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/env_mgr.h\""
echo "include/servers/env_mgr.h" >/dev/tty
echo "cat >include/servers/env_mgr.h <<'@EOF@'"
cat include/servers/env_mgr.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/service.h\""
echo "include/servers/service.h" >/dev/tty
echo "cat >include/servers/service.h <<'@EOF@'"
cat include/servers/service.h
echo "@EOF@"
echo ""
echo "echo \"include/servers/netmemory.h\""
echo "include/servers/netmemory.h" >/dev/tty
echo "cat >include/servers/netmemory.h <<'@EOF@'"
cat include/servers/netmemory.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/boolean.h\""
echo "include/mach/boolean.h" >/dev/tty
echo "cat >include/mach/boolean.h <<'@EOF@'"
cat include/mach/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/device.defs\""
echo "include/mach/device.defs" >/dev/tty
echo "cat >include/mach/device.defs <<'@EOF@'"
cat include/mach/device.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/device_types.defs\""
echo "include/mach/device_types.defs" >/dev/tty
echo "cat >include/mach/device_types.defs <<'@EOF@'"
cat include/mach/device_types.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/device_types.h\""
echo "include/mach/device_types.h" >/dev/tty
echo "cat >include/mach/device_types.h <<'@EOF@'"
cat include/mach/device_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/error.h\""
echo "include/mach/error.h" >/dev/tty
echo "cat >include/mach/error.h <<'@EOF@'"
cat include/mach/error.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/exc.defs\""
echo "include/mach/exc.defs" >/dev/tty
echo "cat >include/mach/exc.defs <<'@EOF@'"
cat include/mach/exc.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/exception.h\""
echo "include/mach/exception.h" >/dev/tty
echo "cat >include/mach/exception.h <<'@EOF@'"
cat include/mach/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/host_info.h\""
echo "include/mach/host_info.h" >/dev/tty
echo "cat >include/mach/host_info.h <<'@EOF@'"
cat include/mach/host_info.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/kern_return.h\""
echo "include/mach/kern_return.h" >/dev/tty
echo "cat >include/mach/kern_return.h <<'@EOF@'"
cat include/mach/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/kernel_event.h\""
echo "include/mach/kernel_event.h" >/dev/tty
echo "cat >include/mach/kernel_event.h <<'@EOF@'"
cat include/mach/kernel_event.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach.defs\""
echo "include/mach/mach.defs" >/dev/tty
echo "cat >include/mach/mach.defs <<'@EOF@'"
cat include/mach/mach.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_extra.h\""
echo "include/mach/mach_extra.h" >/dev/tty
echo "cat >include/mach/mach_extra.h <<'@EOF@'"
cat include/mach/mach_extra.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_host.defs\""
echo "include/mach/mach_host.defs" >/dev/tty
echo "cat >include/mach/mach_host.defs <<'@EOF@'"
cat include/mach/mach_host.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_param.h\""
echo "include/mach/mach_param.h" >/dev/tty
echo "cat >include/mach/mach_param.h <<'@EOF@'"
cat include/mach/mach_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_traps.h\""
echo "include/mach/mach_traps.h" >/dev/tty
echo "cat >include/mach/mach_traps.h <<'@EOF@'"
cat include/mach/mach_traps.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_types.defs\""
echo "include/mach/mach_types.defs" >/dev/tty
echo "cat >include/mach/mach_types.defs <<'@EOF@'"
cat include/mach/mach_types.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/mach_types.h\""
echo "include/mach/mach_types.h" >/dev/tty
echo "cat >include/mach/mach_types.h <<'@EOF@'"
cat include/mach/mach_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine.h\""
echo "include/mach/machine.h" >/dev/tty
echo "cat >include/mach/machine.h <<'@EOF@'"
cat include/mach/machine.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/memory_object.defs\""
echo "include/mach/memory_object.defs" >/dev/tty
echo "cat >include/mach/memory_object.defs <<'@EOF@'"
cat include/mach/memory_object.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/memory_object.h\""
echo "include/mach/memory_object.h" >/dev/tty
echo "cat >include/mach/memory_object.h <<'@EOF@'"
cat include/mach/memory_object.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/memory_object_default.defs\""
echo "include/mach/memory_object_default.defs" >/dev/tty
echo "cat >include/mach/memory_object_default.defs <<'@EOF@'"
cat include/mach/memory_object_default.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/message.h\""
echo "include/mach/message.h" >/dev/tty
echo "cat >include/mach/message.h <<'@EOF@'"
cat include/mach/message.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mig_errors.h\""
echo "include/mach/mig_errors.h" >/dev/tty
echo "cat >include/mach/mig_errors.h <<'@EOF@'"
cat include/mach/mig_errors.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/monitor.defs\""
echo "include/mach/monitor.defs" >/dev/tty
echo "cat >include/mach/monitor.defs <<'@EOF@'"
cat include/mach/monitor.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/msg_type.h\""
echo "include/mach/msg_type.h" >/dev/tty
echo "cat >include/mach/msg_type.h <<'@EOF@'"
cat include/mach/msg_type.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/netport.h\""
echo "include/mach/netport.h" >/dev/tty
echo "cat >include/mach/netport.h <<'@EOF@'"
cat include/mach/netport.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/notify.h\""
echo "include/mach/notify.h" >/dev/tty
echo "cat >include/mach/notify.h <<'@EOF@'"
cat include/mach/notify.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/policy.h\""
echo "include/mach/policy.h" >/dev/tty
echo "cat >include/mach/policy.h <<'@EOF@'"
cat include/mach/policy.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/port.h\""
echo "include/mach/port.h" >/dev/tty
echo "cat >include/mach/port.h <<'@EOF@'"
cat include/mach/port.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/processor_info.h\""
echo "include/mach/processor_info.h" >/dev/tty
echo "cat >include/mach/processor_info.h <<'@EOF@'"
cat include/mach/processor_info.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/std_types.defs\""
echo "include/mach/std_types.defs" >/dev/tty
echo "cat >include/mach/std_types.defs <<'@EOF@'"
cat include/mach/std_types.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach/std_types.h\""
echo "include/mach/std_types.h" >/dev/tty
echo "cat >include/mach/std_types.h <<'@EOF@'"
cat include/mach/std_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/syscall_sw.h\""
echo "include/mach/syscall_sw.h" >/dev/tty
echo "cat >include/mach/syscall_sw.h <<'@EOF@'"
cat include/mach/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/task_info.h\""
echo "include/mach/task_info.h" >/dev/tty
echo "cat >include/mach/task_info.h <<'@EOF@'"
cat include/mach/task_info.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/task_special_ports.h\""
echo "include/mach/task_special_ports.h" >/dev/tty
echo "cat >include/mach/task_special_ports.h <<'@EOF@'"
cat include/mach/task_special_ports.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/thread_info.h\""
echo "include/mach/thread_info.h" >/dev/tty
echo "cat >include/mach/thread_info.h <<'@EOF@'"
cat include/mach/thread_info.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/thread_special_ports.h\""
echo "include/mach/thread_special_ports.h" >/dev/tty
echo "cat >include/mach/thread_special_ports.h <<'@EOF@'"
cat include/mach/thread_special_ports.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/thread_status.h\""
echo "include/mach/thread_status.h" >/dev/tty
echo "cat >include/mach/thread_status.h <<'@EOF@'"
cat include/mach/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/thread_switch.h\""
echo "include/mach/thread_switch.h" >/dev/tty
echo "cat >include/mach/thread_switch.h <<'@EOF@'"
cat include/mach/thread_switch.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/time_value.h\""
echo "include/mach/time_value.h" >/dev/tty
echo "cat >include/mach/time_value.h <<'@EOF@'"
cat include/mach/time_value.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vm_attributes.h\""
echo "include/mach/vm_attributes.h" >/dev/tty
echo "cat >include/mach/vm_attributes.h <<'@EOF@'"
cat include/mach/vm_attributes.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vm_inherit.h\""
echo "include/mach/vm_inherit.h" >/dev/tty
echo "cat >include/mach/vm_inherit.h <<'@EOF@'"
cat include/mach/vm_inherit.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vm_param.h\""
echo "include/mach/vm_param.h" >/dev/tty
echo "cat >include/mach/vm_param.h <<'@EOF@'"
cat include/mach/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vm_prot.h\""
echo "include/mach/vm_prot.h" >/dev/tty
echo "cat >include/mach/vm_prot.h <<'@EOF@'"
cat include/mach/vm_prot.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vm_statistics.h\""
echo "include/mach/vm_statistics.h" >/dev/tty
echo "cat >include/mach/vm_statistics.h <<'@EOF@'"
cat include/mach/vm_statistics.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/boolean.h\""
echo "include/mach/mac2/boolean.h" >/dev/tty
echo "cat >include/mach/mac2/boolean.h <<'@EOF@'"
cat include/mach/mac2/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/exception.h\""
echo "include/mach/mac2/exception.h" >/dev/tty
echo "cat >include/mach/mac2/exception.h <<'@EOF@'"
cat include/mach/mac2/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/frame.h\""
echo "include/mach/mac2/frame.h" >/dev/tty
echo "cat >include/mach/mac2/frame.h <<'@EOF@'"
cat include/mach/mac2/frame.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/kern_return.h\""
echo "include/mach/mac2/kern_return.h" >/dev/tty
echo "cat >include/mach/mac2/kern_return.h <<'@EOF@'"
cat include/mach/mac2/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/reg.h\""
echo "include/mach/mac2/reg.h" >/dev/tty
echo "cat >include/mach/mac2/reg.h <<'@EOF@'"
cat include/mach/mac2/reg.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/syscall_sw.h\""
echo "include/mach/mac2/syscall_sw.h" >/dev/tty
echo "cat >include/mach/mac2/syscall_sw.h <<'@EOF@'"
cat include/mach/mac2/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/thread_status.h\""
echo "include/mach/mac2/thread_status.h" >/dev/tty
echo "cat >include/mach/mac2/thread_status.h <<'@EOF@'"
cat include/mach/mac2/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/vm_param.h\""
echo "include/mach/mac2/vm_param.h" >/dev/tty
echo "cat >include/mach/mac2/vm_param.h <<'@EOF@'"
cat include/mach/mac2/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/mac2/vm_types.h\""
echo "include/mach/mac2/vm_types.h" >/dev/tty
echo "cat >include/mach/mac2/vm_types.h <<'@EOF@'"
cat include/mach/mac2/vm_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/boolean.h\""
echo "include/mach/sun3/boolean.h" >/dev/tty
echo "cat >include/mach/sun3/boolean.h <<'@EOF@'"
cat include/mach/sun3/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/exception.h\""
echo "include/mach/sun3/exception.h" >/dev/tty
echo "cat >include/mach/sun3/exception.h <<'@EOF@'"
cat include/mach/sun3/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/kern_return.h\""
echo "include/mach/sun3/kern_return.h" >/dev/tty
echo "cat >include/mach/sun3/kern_return.h <<'@EOF@'"
cat include/mach/sun3/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/reg.h\""
echo "include/mach/sun3/reg.h" >/dev/tty
echo "cat >include/mach/sun3/reg.h <<'@EOF@'"
cat include/mach/sun3/reg.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/syscall_sw.h\""
echo "include/mach/sun3/syscall_sw.h" >/dev/tty
echo "cat >include/mach/sun3/syscall_sw.h <<'@EOF@'"
cat include/mach/sun3/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/thread_status.h\""
echo "include/mach/sun3/thread_status.h" >/dev/tty
echo "cat >include/mach/sun3/thread_status.h <<'@EOF@'"
cat include/mach/sun3/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/vm_param.h\""
echo "include/mach/sun3/vm_param.h" >/dev/tty
echo "cat >include/mach/sun3/vm_param.h <<'@EOF@'"
cat include/mach/sun3/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/sun3/vm_types.h\""
echo "include/mach/sun3/vm_types.h" >/dev/tty
echo "cat >include/mach/sun3/vm_types.h <<'@EOF@'"
cat include/mach/sun3/vm_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/boolean.h\""
echo "include/mach/vax/boolean.h" >/dev/tty
echo "cat >include/mach/vax/boolean.h <<'@EOF@'"
cat include/mach/vax/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/exception.h\""
echo "include/mach/vax/exception.h" >/dev/tty
echo "cat >include/mach/vax/exception.h <<'@EOF@'"
cat include/mach/vax/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/kern_return.h\""
echo "include/mach/vax/kern_return.h" >/dev/tty
echo "cat >include/mach/vax/kern_return.h <<'@EOF@'"
cat include/mach/vax/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/syscall_sw.h\""
echo "include/mach/vax/syscall_sw.h" >/dev/tty
echo "cat >include/mach/vax/syscall_sw.h <<'@EOF@'"
cat include/mach/vax/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/thread_status.h\""
echo "include/mach/vax/thread_status.h" >/dev/tty
echo "cat >include/mach/vax/thread_status.h <<'@EOF@'"
cat include/mach/vax/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/vm_param.h\""
echo "include/mach/vax/vm_param.h" >/dev/tty
echo "cat >include/mach/vax/vm_param.h <<'@EOF@'"
cat include/mach/vax/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/vax/vm_types.h\""
echo "include/mach/vax/vm_types.h" >/dev/tty
echo "cat >include/mach/vax/vm_types.h <<'@EOF@'"
cat include/mach/vax/vm_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/boolean.h\""
echo "include/mach/ca/boolean.h" >/dev/tty
echo "cat >include/mach/ca/boolean.h <<'@EOF@'"
cat include/mach/ca/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/exception.h\""
echo "include/mach/ca/exception.h" >/dev/tty
echo "cat >include/mach/ca/exception.h <<'@EOF@'"
cat include/mach/ca/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/float.h\""
echo "include/mach/ca/float.h" >/dev/tty
echo "cat >include/mach/ca/float.h <<'@EOF@'"
cat include/mach/ca/float.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/kern_return.h\""
echo "include/mach/ca/kern_return.h" >/dev/tty
echo "cat >include/mach/ca/kern_return.h <<'@EOF@'"
cat include/mach/ca/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/syscall_sw.h\""
echo "include/mach/ca/syscall_sw.h" >/dev/tty
echo "cat >include/mach/ca/syscall_sw.h <<'@EOF@'"
cat include/mach/ca/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/thread_status.h\""
echo "include/mach/ca/thread_status.h" >/dev/tty
echo "cat >include/mach/ca/thread_status.h <<'@EOF@'"
cat include/mach/ca/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/vm_param.h\""
echo "include/mach/ca/vm_param.h" >/dev/tty
echo "cat >include/mach/ca/vm_param.h <<'@EOF@'"
cat include/mach/ca/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/ca/vm_types.h\""
echo "include/mach/ca/vm_types.h" >/dev/tty
echo "cat >include/mach/ca/vm_types.h <<'@EOF@'"
cat include/mach/ca/vm_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/boolean.h\""
echo "include/mach/machine/boolean.h" >/dev/tty
echo "cat >include/mach/machine/boolean.h <<'@EOF@'"
cat include/mach/machine/boolean.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/exception.h\""
echo "include/mach/machine/exception.h" >/dev/tty
echo "cat >include/mach/machine/exception.h <<'@EOF@'"
cat include/mach/machine/exception.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/frame.h\""
echo "include/mach/machine/frame.h" >/dev/tty
echo "cat >include/mach/machine/frame.h <<'@EOF@'"
cat include/mach/machine/frame.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/kern_return.h\""
echo "include/mach/machine/kern_return.h" >/dev/tty
echo "cat >include/mach/machine/kern_return.h <<'@EOF@'"
cat include/mach/machine/kern_return.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/reg.h\""
echo "include/mach/machine/reg.h" >/dev/tty
echo "cat >include/mach/machine/reg.h <<'@EOF@'"
cat include/mach/machine/reg.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/syscall_sw.h\""
echo "include/mach/machine/syscall_sw.h" >/dev/tty
echo "cat >include/mach/machine/syscall_sw.h <<'@EOF@'"
cat include/mach/machine/syscall_sw.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/thread_status.h\""
echo "include/mach/machine/thread_status.h" >/dev/tty
echo "cat >include/mach/machine/thread_status.h <<'@EOF@'"
cat include/mach/machine/thread_status.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/vm_param.h\""
echo "include/mach/machine/vm_param.h" >/dev/tty
echo "cat >include/mach/machine/vm_param.h <<'@EOF@'"
cat include/mach/machine/vm_param.h
echo "@EOF@"
echo ""
echo "echo \"include/mach/machine/vm_types.h\""
echo "include/mach/machine/vm_types.h" >/dev/tty
echo "cat >include/mach/machine/vm_types.h <<'@EOF@'"
cat include/mach/machine/vm_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/callout_statistics.h\""
echo "include/mach_debug/callout_statistics.h" >/dev/tty
echo "cat >include/mach_debug/callout_statistics.h <<'@EOF@'"
cat include/mach_debug/callout_statistics.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/ipc_statistics.h\""
echo "include/mach_debug/ipc_statistics.h" >/dev/tty
echo "cat >include/mach_debug/ipc_statistics.h <<'@EOF@'"
cat include/mach_debug/ipc_statistics.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/mach_debug.defs\""
echo "include/mach_debug/mach_debug.defs" >/dev/tty
echo "cat >include/mach_debug/mach_debug.defs <<'@EOF@'"
cat include/mach_debug/mach_debug.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/mach_debug_types.defs\""
echo "include/mach_debug/mach_debug_types.defs" >/dev/tty
echo "cat >include/mach_debug/mach_debug_types.defs <<'@EOF@'"
cat include/mach_debug/mach_debug_types.defs
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/mach_debug_types.h\""
echo "include/mach_debug/mach_debug_types.h" >/dev/tty
echo "cat >include/mach_debug/mach_debug_types.h <<'@EOF@'"
cat include/mach_debug/mach_debug_types.h
echo "@EOF@"
echo ""
echo "echo \"include/mach_debug/zone_info.h\""
echo "include/mach_debug/zone_info.h" >/dev/tty
echo "cat >include/mach_debug/zone_info.h <<'@EOF@'"
cat include/mach_debug/zone_info.h
echo "@EOF@"
echo ""
echo "echo \"include/mig_errors.h\""
echo "include/mig_errors.h" >/dev/tty
echo "cat >include/mig_errors.h <<'@EOF@'"
cat include/mig_errors.h
echo "@EOF@"
echo ""
echo "echo \"include/exception_all.h\""
echo "include/exception_all.h" >/dev/tty
echo "cat >include/exception_all.h <<'@EOF@'"
cat include/exception_all.h
echo "@EOF@"
echo ""
echo "echo \"libmach/bcopy.c\""
echo "libmach/bcopy.c" >/dev/tty
echo "cat >libmach/bcopy.c <<'@EOF@'"
cat libmach/bcopy.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_init.c\""
echo "libmach/mach_init.c" >/dev/tty
echo "cat >libmach/mach_init.c <<'@EOF@'"
cat libmach/mach_init.c
echo "@EOF@"
echo ""
echo "echo \"libmach/sbrk.c\""
echo "libmach/sbrk.c" >/dev/tty
echo "cat >libmach/sbrk.c <<'@EOF@'"
cat libmach/sbrk.c
echo "@EOF@"
echo ""
echo "echo \"libmach/error_codes.c\""
echo "libmach/error_codes.c" >/dev/tty
echo "cat >libmach/error_codes.c <<'@EOF@'"
cat libmach/error_codes.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_error.c\""
echo "libmach/mach_error.c" >/dev/tty
echo "cat >libmach/mach_error.c <<'@EOF@'"
cat libmach/mach_error.c
echo "@EOF@"
echo ""
echo "echo \"libmach/Ovfork.c\""
echo "libmach/Ovfork.c" >/dev/tty
echo "cat >libmach/Ovfork.c <<'@EOF@'"
cat libmach/Ovfork.c
echo "@EOF@"
echo ""
echo "echo \"libmach/slot_name.c\""
echo "libmach/slot_name.c" >/dev/tty
echo "cat >libmach/slot_name.c <<'@EOF@'"
cat libmach/slot_name.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mig_support.c\""
echo "libmach/mig_support.c" >/dev/tty
echo "cat >libmach/mig_support.c <<'@EOF@'"
cat libmach/mig_support.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mig_strncpy.c\""
echo "libmach/mig_strncpy.c" >/dev/tty
echo "cat >libmach/mig_strncpy.c <<'@EOF@'"
cat libmach/mig_strncpy.c
echo "@EOF@"
echo ""
echo "echo \"libmach/msg.c\""
echo "libmach/msg.c" >/dev/tty
echo "cat >libmach/msg.c <<'@EOF@'"
cat libmach/msg.c
echo "@EOF@"
echo ""
echo "echo \"libmach/XXX_pdp.c\""
echo "libmach/XXX_pdp.c" >/dev/tty
echo "cat >libmach/XXX_pdp.c <<'@EOF@'"
cat libmach/XXX_pdp.c
echo "@EOF@"
echo ""
echo "echo \"libmach/XXX_pds.c\""
echo "libmach/XXX_pds.c" >/dev/tty
echo "cat >libmach/XXX_pds.c <<'@EOF@'"
cat libmach/XXX_pds.c
echo "@EOF@"
echo ""
echo "echo \"libmach/XXX_pdu.c\""
echo "libmach/XXX_pdu.c" >/dev/tty
echo "cat >libmach/XXX_pdu.c <<'@EOF@'"
cat libmach/XXX_pdu.c
echo "@EOF@"
echo ""
echo "echo \"libmach/XXX_plr.c\""
echo "libmach/XXX_plr.c" >/dev/tty
echo "cat >libmach/XXX_plr.c <<'@EOF@'"
cat libmach/XXX_plr.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_exception.c\""
echo "libmach/mach_exception.c" >/dev/tty
echo "cat >libmach/mach_exception.c <<'@EOF@'"
cat libmach/mach_exception.c
echo "@EOF@"
echo ""
echo "echo \"libmach/XXX_new_vawp.c\""
echo "libmach/XXX_new_vawp.c" >/dev/tty
echo "cat >libmach/XXX_new_vawp.c <<'@EOF@'"
cat libmach/XXX_new_vawp.c
echo "@EOF@"
echo ""
echo "echo \"libmach/new_mach_user.c\""
echo "libmach/new_mach_user.c" >/dev/tty
echo "cat >libmach/new_mach_user.c <<'@EOF@'"
cat libmach/new_mach_user.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_traps.a\""
echo "libmach/mach_traps.a" >/dev/tty
echo "cat >libmach/mach_traps.a <<'@EOF@'"
cat mach_traps.a
echo "@EOF@"
echo ""
echo "echo \"libmach/err_kern.sub\""
echo "libmach/err_kern.sub" >/dev/tty
echo "cat >libmach/err_kern.sub <<'@EOF@'"
cat libmach/err_kern.sub
echo "@EOF@"
echo ""
echo "echo \"libmach/err_us.sub\""
echo "libmach/err_us.sub" >/dev/tty
echo "cat >libmach/err_us.sub <<'@EOF@'"
cat libmach/err_us.sub
echo "@EOF@"
echo ""
echo "echo \"libmach/err_server.sub\""
echo "libmach/err_server.sub" >/dev/tty
echo "cat >libmach/err_server.sub <<'@EOF@'"
cat libmach/err_server.sub
echo "@EOF@"
echo ""
echo "echo \"libmach/err_ipc.sub\""
echo "libmach/err_ipc.sub" >/dev/tty
echo "cat >libmach/err_ipc.sub <<'@EOF@'"
cat libmach/err_ipc.sub
echo "@EOF@"
echo ""
echo "echo \"libmach/SYS.h\""
echo "libmach/SYS.h" >/dev/tty
echo "cat >libmach/SYS.h <<'@EOF@'"
cat libmach/SYS.h
echo "@EOF@"
echo ""
echo "echo \"libmach/env_mgrUser.c\""
echo "libmach/env_mgrUser.c" >/dev/tty
echo "cat >libmach/env_mgrUser.c <<'@EOF@'"
cat libmach/env_mgrUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/ipcxUser.c\""
echo "libmach/ipcxUser.c" >/dev/tty
echo "cat >libmach/ipcxUser.c <<'@EOF@'"
cat libmach/ipcxUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/netnameUser.c\""
echo "libmach/netnameUser.c" >/dev/tty
echo "cat >libmach/netnameUser.c <<'@EOF@'"
cat libmach/netnameUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/serviceUser.c\""
echo "libmach/serviceUser.c" >/dev/tty
echo "cat >libmach/serviceUser.c <<'@EOF@'"
cat libmach/serviceUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/excUser.c\""
echo "libmach/excUser.c" >/dev/tty
echo "cat >libmach/excUser.c <<'@EOF@'"
cat libmach/excUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/excServer.c\""
echo "libmach/excServer.c" >/dev/tty
echo "cat >libmach/excServer.c <<'@EOF@'"
cat libmach/excServer.c
echo "@EOF@"
echo ""
echo "echo \"libmach/netmemoryUser.c\""
echo "libmach/netmemoryUser.c" >/dev/tty
echo "cat >libmach/netmemoryUser.c <<'@EOF@'"
cat libmach/netmemoryUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/netmemstatUser.c\""
echo "libmach/netmemstatUser.c" >/dev/tty
echo "cat >libmach/netmemstatUser.c <<'@EOF@'"
cat libmach/netmemstatUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/memory_objectServer.c\""
echo "libmach/memory_objectServer.c" >/dev/tty
echo "cat >libmach/memory_objectServer.c <<'@EOF@'"
cat libmach/memory_objectServer.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_user.c\""
echo "libmach/mach_user.c" >/dev/tty
echo "cat >libmach/mach_user.c <<'@EOF@'"
cat libmach/mach_user.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_debugUser.c\""
echo "libmach/mach_debugUser.c" >/dev/tty
echo "cat >libmach/mach_debugUser.c <<'@EOF@'"
cat libmach/mach_debugUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/monitorUser.c\""
echo "libmach/monitorUser.c" >/dev/tty
echo "cat >libmach/monitorUser.c <<'@EOF@'"
cat libmach/monitorUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_hostUser.c\""
echo "libmach/mach_hostUser.c" >/dev/tty
echo "cat >libmach/mach_hostUser.c <<'@EOF@'"
cat libmach/mach_hostUser.c
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_interface.h\""
echo "libmach/mach_interface.h" >/dev/tty
echo "cat >libmach/mach_interface.h <<'@EOF@'"
cat libmach/mach_interface.h
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_debug_interface.h\""
echo "libmach/mach_debug_interface.h" >/dev/tty
echo "cat >libmach/mach_debug_interface.h <<'@EOF@'"
cat libmach/mach_debug_interface.h
echo "@EOF@"
echo ""
echo "echo \"libmach/exc.h\""
echo "libmach/exc.h" >/dev/tty
echo "cat >libmach/exc.h <<'@EOF@'"
cat libmach/exc.h
echo "@EOF@"
echo ""
echo "echo \"libmach/ipcx.h\""
echo "libmach/ipcx.h" >/dev/tty
echo "cat >libmach/ipcx.h <<'@EOF@'"
cat libmach/ipcx.h
echo "@EOF@"
echo ""
echo "echo \"libmach/mach_host.h\""
echo "libmach/mach_host.h" >/dev/tty
echo "cat >libmach/mach_host.h <<'@EOF@'"
cat libmach/mach_host.h
echo "@EOF@"
echo ""
echo "echo \"libmach/monitor.h\""
echo "libmach/monitor.h" >/dev/tty
echo "cat >libmach/monitor.h <<'@EOF@'"
cat libmach/monitor.h
echo "@EOF@"
echo ""
echo "echo \"libmach/netmemstat.h\""
echo "libmach/netmemstat.h" >/dev/tty
echo "cat >libmach/netmemstat.h <<'@EOF@'"
cat libmach/netmemstat.h
echo "@EOF@"
echo ""
echo "echo \"libmach/env_mgr.h\""
echo "libmach/env_mgr.h" >/dev/tty
echo "cat >libmach/env_mgr.h <<'@EOF@'"
cat libmach/env_mgr.h
echo "@EOF@"
echo ""
echo "echo \"libmach/netmemory_defs.h\""
echo "libmach/netmemory_defs.h" >/dev/tty
echo "cat >libmach/netmemory_defs.h <<'@EOF@'"
cat libmach/netmemory_defs.h
echo "@EOF@"
echo ""
echo "echo \"libmach/netmemory.h\""
echo "libmach/netmemory.h" >/dev/tty
echo "cat >libmach/netmemory.h <<'@EOF@'"
cat libmach/netmemory.h
echo "@EOF@"
echo ""
echo "echo \"libmach/netname.h\""
echo "libmach/netname.h" >/dev/tty
echo "cat >libmach/netname.h <<'@EOF@'"
cat libmach/netname.h
echo "@EOF@"
echo ""
echo "echo \"libmach/netname_defs.h\""
echo "libmach/netname_defs.h" >/dev/tty
echo "cat >libmach/netname_defs.h <<'@EOF@'"
cat libmach/netname_defs.h
echo "@EOF@"
echo ""
echo "echo \"libmach/service.h\""
echo "libmach/service.h" >/dev/tty
echo "cat >libmach/service.h <<'@EOF@'"
cat libmach/service.h
echo "@EOF@"
echo ""
