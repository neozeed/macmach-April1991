# /usr/src/man/man2/Makefile

MDIR=	/usr/man/man2
SRCS=	accept.2 access.2 acct.2 adjtime.2 bind.2 brk.2 chdir.2 chmod.2 \
	chown.2 chroot.2 close.2 connect.2 cpu_control.2 creat.2 dup.2 \
	execve.2 exit.2 fcntl.2 flock.2 fork.2 fsync.2 getdirentries.2 \
	getdomainname.2 getdtablesize.2 getgid.2 getgroups.2 gethostid.2 \
	gethostname.2 getitimer.2 getpagesize.2 getpeername.2 getpgrp.2 \
	getpid.2 getpriority.2 getrlimit.2 getrusage.2 getsockname.2 \
	getsockopt.2 gettimeofday.2 getuid.2 host_info.2 \
	host_ipc_statistics.2 host_kernel_version.2 \
	host_processor_set_priv.2 host_processor_sets.2 host_processors.2 \
	host_self.2 intro.2 ioctl.2 kern_timestamp.2 kill.2 killpg.2 link.2 \
	listen.2 lseek.2 mach_ports.2 mach_ports_lookup.2 \
	mach_ports_register.2 map_fd.2 mapfrcounter.2 memory_object_copy.2 \
	memory_object_create.2 memory_object_data_error.2 \
	memory_object_data_initialize.2 memory_object_data_provided.2 \
	memory_object_data_request.2 memory_object_data_unavailable.2 \
	memory_object_data_unlock.2 memory_object_data_write.2 \
	memory_object_destroy.2 memory_object_get_attributes.2 \
	memory_object_init.2 memory_object_lock_completed.2 \
	memory_object_lock_request.2 memory_object_server.2 \
	memory_object_set_attributes.2 memory_object_terminate.2 \
	mkdir.2 mknod.2 monitor.2 monitor_create.2 monitor_read.2 \
	monitor_resume.2 monitor_suspend.2 monitor_terminate.2 mount.2 \
	msg_receive.2 msg_rpc.2 msg_send.2 open.2 pipe.2 port_allocate.2 \
	port_deallocate.2 port_extract.2 port_extract_receive.2 \
	port_extract_send.2 port_insert.2 port_insert_receive.2 \
	port_insert_send.2 port_messages.2 port_names.2 port_rename.2 \
	port_restrict.2 port_set_add.2 port_set_allocate.2 \
	port_set_backlog.2 port_set_backup.2 port_set_deallocate.2 \
	port_set_remove.2 port_set_status.2 port_status.2 port_type.2 \
	processor_assign.2 processor_control.2 processor_info.2 \
	processor_set_create.2 processor_set_default.2 \
	processor_set_destroy.2 processor_set_info.2 \
	processor_set_max_priority.2 processor_set_policy_enable.2 \
	processor_set_tasks.2 processor_set_threads.2 profil.2 ptrace.2 \
	quota.2 read.2 readlink.2 reboot.2 recv.2 rename.2 rmdir.2 rpause.2 \
	select.2 send.2 setgroups.2 setpgrp.2 setquota.2 setregid.2 \
	setreuid.2 shutdown.2 sigblock.2 sigpause.2 sigreturn.2 sigsetmask.2 \
	sigstack.2 sigvec.2 slot_info.2 socket.2 socketpair.2 stat.2 \
	statfs.2 swapon.2 symlink.2 sync.2 syscall.2 table.2 task_assign.2 \
	task_create.2 task_get_special_port.2 task_info.2 task_notify.2 \
	task_priority.2 task_resume.2 task_self.2 task_set_special_port.2 \
	task_special_ports.2 task_suspend.2 task_terminate.2 task_threads.2 \
	thread_abort.2 thread_assign.2 thread_create.2 \
	thread_get_special_port.2 thread_get_state.2 thread_info.2 \
	thread_monitor.2 thread_policy.2 thread_priority.2 thread_reply.2 \
	thread_resume.2 thread_self.2 thread_set_special_port.2 \
	thread_set_state.2 thread_special_ports.2 thread_state.2 \
	thread_suspend.2 thread_switch.2 thread_terminate.2 \
	thread_unmonitor.2 thread_wire.2 truncate.2 umask.2 unlink.2 \
	unmount.2 utimes.2 vfork.2 vfsmount.2 vhangup.2 vm_allocate.2 \
	vm_copy.2 vm_deallocate.2 vm_inherit.2 vm_machine_attribute.2 \
	vm_map.2 vm_protect.2 vm_read.2 vm_region.2 \
	vm_set_default_memory_manager.2 vm_statistics.2 vm_wire.2 \
	vm_write.2 wait.2 write.2 

LINKS=	dup2.2 sbrk.2 fstat.2 lstat.2 readv.2 recvfrom.2 recvmsg.2 \
	errno.2 sendmsg.2 sendto.2 wait3.2 fchmod.2 fchown.2 \
	umount.2 writev.2 getegid.2 geteuid.2 getppid.2 ftruncate.2 \
	sethostid.2 setitimer.2 setrlimit.2 setsockopt.2 sethostname.2 \
	setpriority.2 settimeofday.2 seek.2

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s dup.2 ${DESTDIR}${MDIR}/dup2.2
	ln -s brk.2 ${DESTDIR}${MDIR}/sbrk.2
	ln -s intro.2 ${DESTDIR}${MDIR}/errno.2
	ln -s stat.2 ${DESTDIR}${MDIR}/fstat.2
	ln -s stat.2 ${DESTDIR}${MDIR}/lstat.2
	ln -s read.2 ${DESTDIR}${MDIR}/readv.2
	ln -s recv.2 ${DESTDIR}${MDIR}/recvfrom.2
	ln -s recv.2 ${DESTDIR}${MDIR}/recvmsg.2
	ln -s send.2 ${DESTDIR}${MDIR}/sendmsg.2
	ln -s send.2 ${DESTDIR}${MDIR}/sendto.2
	ln -s wait.2 ${DESTDIR}${MDIR}/wait3.2
	ln -s chmod.2 ${DESTDIR}${MDIR}/fchmod.2
	ln -s chown.2 ${DESTDIR}${MDIR}/fchown.2
	ln -s mount.2 ${DESTDIR}${MDIR}/umount.2
	ln -s write.2 ${DESTDIR}${MDIR}/writev.2
	ln -s getgid.2 ${DESTDIR}${MDIR}/getegid.2
	ln -s getuid.2 ${DESTDIR}${MDIR}/geteuid.2
	ln -s getpid.2 ${DESTDIR}${MDIR}/getppid.2
	ln -s truncate.2 ${DESTDIR}${MDIR}/ftruncate.2
	ln -s gethostid.2 ${DESTDIR}${MDIR}/sethostid.2
	ln -s getitimer.2 ${DESTDIR}${MDIR}/setitimer.2
	ln -s getrlimit.2 ${DESTDIR}${MDIR}/setrlimit.2
	ln -s getsockopt.2 ${DESTDIR}${MDIR}/setsockopt.2
	ln -s gethostname.2 ${DESTDIR}${MDIR}/sethostname.2
	ln -s getpriority.2 ${DESTDIR}${MDIR}/setpriority.2
	ln -s gettimeofday.2 ${DESTDIR}${MDIR}/settimeofday.2
	ln -s lseek.2 ${DESTDIR}${MDIR}/seek.2

FRC:
