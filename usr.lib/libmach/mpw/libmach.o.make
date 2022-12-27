# libmach.o.make

# note: define MacOS_drvr to make a libmach for Macintosh drivers

OBJECTS = ¶
		env_mgrUser.c.o ¶
		error_codes.c.o ¶
		excServer.c.o ¶
		excUser.c.o ¶
		ipcxUser.c.o ¶
		mach_debugUser.c.o ¶
		mach_error.c.o ¶
		mach_exception.c.o ¶
		mach_hostUser.c.o ¶
		mach_init.c.o ¶
		mach_traps.a.o ¶
		memory_objectServer.c.o ¶
		mig_strncpy.c.o ¶
		mig_support.c.o ¶
		monitorUser.c.o ¶
		msg.c.o ¶
		netmemoryUser.c.o ¶
		netmemstatUser.c.o ¶
		netnameUser.c.o ¶
		new_mach_user.c.o ¶
		serviceUser.c.o ¶
		slot_name.c.o ¶
		XXX_new_vawp.c.o ¶
		XXX_pdp.c.o ¶
		XXX_pds.c.o ¶
		XXX_pdu.c.o ¶
		XXX_plr.c.o ¶
		bcopy.c.o



libmach.o ÄÄ libmach.o.make {OBJECTS}
	Lib -p {OBJECTS} -o libmach.o
env_mgrUser.c.o Ä libmach.o.make env_mgrUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 env_mgrUser.c
error_codes.c.o Ä libmach.o.make error_codes.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 error_codes.c
excServer.c.o Ä libmach.o.make excServer.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 excServer.c
excUser.c.o Ä libmach.o.make excUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 excUser.c
ipcxUser.c.o Ä libmach.o.make ipcxUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 ipcxUser.c
mach_debugUser.c.o Ä libmach.o.make mach_debugUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mach_debugUser.c
mach_error.c.o Ä libmach.o.make mach_error.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mach_error.c
mach_exception.c.o Ä libmach.o.make mach_exception.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mach_exception.c
mach_hostUser.c.o Ä libmach.o.make mach_hostUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mach_hostUser.c
mach_init.c.o Ä libmach.o.make mach_init.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mach_init.c
mach_traps.a.o Ä libmach.o.make mach_traps.a
	 Asm mach_traps.a
memory_objectServer.c.o Ä libmach.o.make memory_objectServer.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 memory_objectServer.c
mig_strncpy.c.o Ä libmach.o.make mig_strncpy.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mig_strncpy.c
mig_support.c.o Ä libmach.o.make mig_support.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 mig_support.c
monitorUser.c.o Ä libmach.o.make monitorUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 monitorUser.c
msg.c.o Ä libmach.o.make msg.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 msg.c
netmemoryUser.c.o Ä libmach.o.make netmemoryUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 netmemoryUser.c
netmemstatUser.c.o Ä libmach.o.make netmemstatUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 netmemstatUser.c
netnameUser.c.o Ä libmach.o.make netnameUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 netnameUser.c
new_mach_user.c.o Ä libmach.o.make new_mach_user.c mach_user.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 new_mach_user.c
serviceUser.c.o Ä libmach.o.make serviceUser.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 serviceUser.c
slot_name.c.o Ä libmach.o.make slot_name.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 slot_name.c
XXX_new_vawp.c.o Ä libmach.o.make XXX_new_vawp.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 XXX_new_vawp.c
XXX_pdp.c.o Ä libmach.o.make XXX_pdp.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 XXX_pdp.c
XXX_pds.c.o Ä libmach.o.make XXX_pds.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 XXX_pds.c
XXX_pdu.c.o Ä libmach.o.make XXX_pdu.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 XXX_pdu.c
XXX_plr.c.o Ä libmach.o.make XXX_plr.c
	 C -d mac2 -d MacOS -d MacOS_DRVR -sym on -mc68020 XXX_plr.c
bcopy.c.o Ä libmach.o.make bcopy.c
	 C -sym on -mc68020 bcopy.c

