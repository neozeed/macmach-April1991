/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	nmcontrol.c,v $
 * Revision 1.1  90/02/19  21:58:31  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.8  89/05/02  11:20:22  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 *  2-Oct-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Minor changes to handling of port statistics.
 *
 * 19-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added code to get and dump port statistics.
 *
 * 21-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some error checking.
 *	Use stabsearch to match parameter names.
 *	ls_sendlog returns and old and a current log - write them both out.
 *
 * 13-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Replaced net_recursion_level by trace_recursion_level
 *	and net_trace tracing_on.  Removed net_debug_level.
 *	Look up the name service port in the top-level nameserver.
 *
 * 21-Mar-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 * nmcontrol.c
 *
 *
 */
#ifndef	lint
static char     rcsid[] = "$ Header: nmcontrol.c,v 1.8 89/05/02 11:20:22 dpj Exp $";
#endif not lint
/*
 * Program to control the operation of the network server and generate debugging 
 * information.
 */


#include	"debug.h"
#include	"trace.h"
#include	"nm_defs.h"
#include	"mem.h"

#include	<stdio.h>
#include	<mach.h>
#include	<netdb.h>
#include	"ls_defs.h"
#include	"logstat.h"
#include	<servers/netname.h>

#define readln(p) while (((getc(p))!='\n') && (!feof(p)))
#define NETNAME_NAME	"NEW_NETWORK_NAME_SERVER"

/*
 * Global variables
 */
static char		machine_name[60];	/* name of the machine currently monitored */
static netaddr_t	machine_id;		/* IP address of the machine currently monitored */
static char		service_name[] = "NM_LOGSTAT";	/* logstat service */
static port_t		logstat_port;		/* port for current server */
static log_ptr_t	cur_log_ptr;		/* current log */
static unsigned int	cur_log_ptr_size;	/* size of old log (words) */
static log_ptr_t	old_log_ptr;		/* current log */
static unsigned int	old_log_ptr_size;	/* size of old log (words) */
static stat_ptr_t	stat_ptr;		/* current stat */
static unsigned int	stat_ptr_size;		/* size of current stat (words) */
static debug_ptr_t	debug_ptr;		/* current debug */
static unsigned int	debug_ptr_size;		/* size of current debug (words) */
static param_ptr_t	param_ptr;		/* current param */
static unsigned int	param_ptr_size;		/* size of current param (words) */
static port_stat_ptr_t	port_stat_ptr;		/* current port statistics */
static unsigned int	port_stat_ptr_size;	/* size of current port statistics (words) */

static port_t		nn_port;		/* port to new network server */

extern	char		*stat_names[];
extern	char		*debug_names[];
extern	char		*param_names[];
extern	char		*port_stat_names[];

/*
 * Tracing values.
 */
int	trace_recursion_level = 0;
#undef	tracing_on
int	tracing_on = 0;

/*
 * host_address
 *	Return a host address for a given host name.
 *
 */
long host_address(host_name)
char *host_name;
{
    register struct hostent *hp = gethostbyname(host_name);
    if (hp == 0) return 0;
    else return *(long *)(hp->h_addr);
}

/*
 * input
 *
 * Obtain user input, allowing multiple inputs on the same line.
 *
 * Parameters:
 *
 * prompt: string to use as a prompt.
 * format: specification for data required.
 * OUT var: address where to put the data read.
 *
 * Results:
 *
 * Side effects:
 *
 * May print a prompt on stdout, and start a read on stdin.
 *
 * Design:
 *
 * Note:
 *
 */
void input(prompt, format, var)
char *prompt;
char *format;
char *var;
BEGIN("input")
	char c;
	int fcnt;

	while ((stdin->_cnt > 0) &&
	    ((((c = *(stdin)->_ptr&0377) == ' ') || (c == '\n'))))
		getc(stdin);
	fcnt = stdin->_cnt;

	if (fcnt == 0) {
		fprintf(stdout, "%s", prompt);
		fflush(stdout);
	}
	fscanf(stdin, format, var);
	RET;
END


/*
 * printmenu
 *
 * Parameters: none
 *
 * Results:
 *
 * Side effects:
 *
 * Print a command menu on stdout.
 *
 * Design:
 *
 * Note:
 *
 */
void printmenu()
BEGIN("printmenu")
	fprintf(stdout, "\n");
	fprintf(stdout, "This programs allows to examine/modify a network server in operation.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Available commands:\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "    H,?.......Print this message\n");
	fprintf(stdout, "    O.........Open a connection to another network server\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Log:\n");
	fprintf(stdout, "       LG.........Get log\n");
	fprintf(stdout, "       LW.........Write log to file\n");
	fprintf(stdout, "       LR.........Reset log\n");
	fprintf(stdout, "       LD.........Dump log on remote server\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Statistics:\n");
	fprintf(stdout, "       SG.........Get statistics\n");
	fprintf(stdout, "       SR.........Reset statistics\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Debug:\n");
	fprintf(stdout, "       DG.........Get debugging flags\n");
	fprintf(stdout, "       DS.........Set debugging flags\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Parameters:\n");
	fprintf(stdout, "       PG.........Get parameters\n");
	fprintf(stdout, "       PS.........Set parameters\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Port Statistics:\n");
	fprintf(stdout, "       MG.........Get port statistics \n");
	fprintf(stdout, "       MW.........Write port statistics to file\n");
	fprintf(stdout, "\n");
	fprintf(stdout, " Memory Usage:\n");
	fprintf(stdout, "       A..........Display memory usage \n");
	fprintf(stdout, "\n");
#ifdef	notdef
	fprintf(stdout, "    K.........Kill network server\n");
#endif	notdef
	fprintf(stdout, "    Q.........Quit\n");
	fprintf(stdout, "\n");
	RET;
END


/*
 * openconnection
 *
 * Open a connection to a network server to control.
 *
 * Parameters:
 *
 * name: name of machine to look for, or NULL.
 *
 * Results:
 *
 * Side effects:
 *
 * May prompt for the name of the machine to control. Sets logstat_port and 
 * machine_name to the appropriate value.
 *
 * Design:
 *
 * Note:
 *
 */

void openconnection(name)
char	*name;
BEGIN("openconnection")
	register kern_return_t	kr;

	/*
	 * Look up the new network server.
	 */
	if ((kr = netname_look_up(name_server_port, "", NETNAME_NAME, &nn_port)) == KERN_SUCCESS) {
		printf("Look up of new network server succeeds, port = %x.\n", nn_port);
	}
	else {
		nn_port = name_server_port;
		printf("Look up of new network server fails, using old server, port = %x.\n", nn_port);
	}

	do {
	    	if (name) {
			strcpy(machine_name,name);
			name = NULL;
		} else {
			input("What machine do you want to control? ", "%79s", machine_name);
		}
		machine_id = host_address(machine_name);
		fprintf(stdout,"Looking up \"%s\" at %s (%x) ...\n",service_name,machine_name,machine_id);
		kr = netname_look_up(nn_port,machine_name,service_name,&logstat_port);
		if (kr == NETNAME_SUCCESS) {
			fprintf(stdout,"    [Successful]\n");
		}
		else {
			fprintf(stdout,"    [Can't find %s!  kr = %1d]\n",machine_name,kr);
			readln(stdin);
		}
		fflush(stdout);
	} 
	while (!(kr == NETNAME_SUCCESS));
	RET;
END


/*
 * writelog
 *
 * Append the old and current log to a file.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * Prompts for the name of the file to write to ("-" for stdout). 
 *
 * Design:
 *
 * Note:
 *
 */
void writelog()
BEGIN("writelog")
	char   outfilename[80];
	FILE * of;			/* file descriptor for the output file */
	int     i;
	char *lptr;

	/* 
	 * Get the file descriptor that we are to write to.
	 */
	input("Output file: ", "%79s", outfilename);
	if ((of = fopen (outfilename, "w")) == NULL) {
		fprintf (stderr, "Cannot open %s\n", outfilename);
                fflush(stderr);
		RET;
	}

	for ( i = 1, lptr = (char *)old_log_ptr; i <= (old_log_ptr_size<<2); i++, lptr++)
		putc(*lptr, of);
	for ( i = 1, lptr = (char *)cur_log_ptr; i <= (cur_log_ptr_size<<2); i++, lptr++)
		putc(*lptr, of);

	fclose(of);
	RET;
END


/*
 * genericlog
 *
 * execute any log-related command.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * May execute a log subcommand. May prompt for the name of the subcommand to
 * execute.
 *
 * Design:
 *
 * Note:
 *
 */
void genericlog()
BEGIN("genericlog")
	char subcmd[2];

	input("Enter Log sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
	case 'G':
	case 'g':
		if (ls_sendlog(logstat_port,&old_log_ptr,&old_log_ptr_size,&cur_log_ptr,&cur_log_ptr_size)
				== KERN_SUCCESS)
			fprintf(stdout,"Received %d bytes of log\n",(old_log_ptr_size+cur_log_ptr_size)<<2);
		else fprintf(stderr, "ls_sendlog fails.\n");
		break;
	case 'W':
	case 'w':
		writelog();
		break;
	case 'R':
	case 'r':
		if (ls_resetlog(logstat_port) == KERN_SUCCESS) fprintf(stdout,"Log reset\n");
		else fprintf(stderr, "ls_resetlog fails.\n");
		break;
	case 'D':
	case 'd':
		if (ls_writelog(logstat_port) == KERN_SUCCESS) fprintf(stdout,"Log dumped\n");
		else fprintf(stderr, "ls_writelog fails.\n");
		break;
	default:
		fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
		readln(stdin);
		break;
	}
	fflush(stdout);
	RET;
END


/*
 * showrec
 *
 * Display the values in a record according to a list of names.
 *
 * Parameters:
 *
 * rec: pointer to the record to display.
 * rec_names: names of the fields.
 *
 * Results:
 *
 * Side effects:
 *
 * Prints stuff on stdout or the specified file.
 *
 * Design:
 *
 * Note:
 *
 */
showrec(rec,rec_names)
int	rec[];
char	*rec_names[];
BEGIN("showrec")
	char	outfilename[80];
	FILE	*of;
	int	curfield;

	/* 
	 * Get the file descriptor that we are to write to.
	 */
	input("Output file: ", "%79s", outfilename);
	if (outfilename[0] == '-')
		of = stdout;
	else {
		if ((of = fopen (outfilename, "w")) == NULL) {
			fprintf (stderr, "Cannot open %s\n", outfilename);
                	fflush(stderr);
			RET;
		}
	}
	
	/*
	 * Loop through all the fields in the record.
	 */
	for ( curfield = 0; rec_names[curfield]; curfield++) {
	    	if (curfield & 0x1)
			fprintf(of,"      %25s %6d\n",rec_names[curfield],rec[curfield]);
		else
			fprintf(of,"%25s %6d",rec_names[curfield],rec[curfield]);
	}

	fprintf(of,"\n");
	if (of != stdout)
		fclose(of);
	RET;
END


/*
 * changerec
 *
 * Change a value in a record.
 *
 * Parameters:
 *
 * rec: pointer to the record to display.
 * rec_names: names of the fields.
 *
 * Results:
 *
 * Side effects:
 *
 * Changes a field in rec. Prompts on stdin for the name of field and the new value.
 *
 * Design:
 *
 * Note:
 *
 */
changerec(rec,rec_names)
int	rec[];
char	*rec_names[];
BEGIN("changerec")
	char	fname[30];
	int	curfield;

	input("Field to change: ", "%29s", fname);
	if ((curfield = stabsearch(fname, rec_names, TRUE)) >= 0) {
	    	input("New value: ", "%d", &rec[curfield]);
	} else {
		fprintf(stdout,"Unknown field\n");
		readln(stdin);
	}

	RET;

END


/*
 * genericstat
 *
 * execute any stat-related command.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * May execute a stat subcommand. May prompt for the name of the subcommand to
 * execute.
 *
 * Design:
 *
 * Note:
 *
 */
void genericstat()
BEGIN("genericstat")
	char subcmd[2];

	input("Enter Stat sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
	case 'G':
	case 'g':
		if (ls_sendstat(logstat_port,&stat_ptr,&stat_ptr_size) == KERN_SUCCESS)
			showrec(stat_ptr,stat_names);
		else fprintf(stderr, "ls_sendstat fails.\n");
		break;
	case 'R':
	case 'r':
		if (ls_resetstat(logstat_port) != KERN_SUCCESS) fprintf(stderr, "ls_resetstat fails.\n");
		fprintf(stdout,"Stat reset\n");
		break;
	default:
		fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
		readln(stdin);
		break;
	}
	fflush(stdout);
	RET;
END


/*
 * genericdebug
 *
 * execute any debug-related command.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * May execute a debug subcommand. May prompt for the name of the subcommand to
 * execute.
 *
 * Design:
 *
 * Note:
 *
 */
void genericdebug()
BEGIN("genericdebug")
	char subcmd[2];

	input("Enter Debug sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
	case 'G':
	case 'g':
		if (ls_senddebug(logstat_port,&debug_ptr,&debug_ptr_size) == KERN_SUCCESS)
			showrec(debug_ptr,debug_names);
		else fprintf(stderr, "ls_senddebug fails.\n");
		break;
	case 'S':
	case 's':
		if (ls_senddebug(logstat_port,&debug_ptr,&debug_ptr_size) == KERN_SUCCESS) {
			changerec(debug_ptr,debug_names);
			if (ls_setdebug(logstat_port,debug_ptr,debug_ptr_size) != KERN_SUCCESS)
				fprintf(stderr, "ls_setdebug fails.\n");
		}
		else fprintf(stderr, "ls_senddebug fails.\n");
		break;
	default:
		fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
		readln(stdin);
		break;
	}
	fflush(stdout);
	RET;
END


/*
 * genericparam
 *
 * execute any param-related command.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * May execute a param subcommand. May prompt for the name of the subcommand to
 * execute.
 *
 * Design:
 *
 * Note:
 *
 */
void genericparam()
BEGIN("genericparam")
	char subcmd[2];

	input("Enter Param sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
	case 'G':
	case 'g':
		if (ls_sendparam(logstat_port,&param_ptr,&param_ptr_size) == KERN_SUCCESS)
			showrec(param_ptr,param_names);
		else fprintf(stderr, "ls_sendparam fails.\n");
		break;
	case 'S':
	case 's':
		if (ls_sendparam(logstat_port,&param_ptr,&param_ptr_size) == KERN_SUCCESS) {
			changerec(param_ptr,param_names);
			if (ls_setparam(logstat_port,param_ptr,param_ptr_size) != KERN_SUCCESS)
				fprintf(stderr, "ls_setparam fails.\n");
		}
		else fprintf(stderr, "ls_sendparam fails.\n");
		break;
	default:
		fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
		readln(stdin);
		break;
	}
	fflush(stdout);
	RET;
END


/*
 * dumpportstat
 *
 * Dumps the port statistics to a file.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * Prompts for the name of the file to write to ("-" for stdout). 
 *
 * Design:
 *
 * Note:
 *
 */
void dumpportstat()
BEGIN("dumpportstat")
	char   outfilename[80];
	FILE * of;			/* file descriptor for the output file */
	int     i;
	char *lptr;

	/* 
	 * Get the file descriptor that we are to write to.
	 */
	input("Output file: ", "%79s", outfilename);
	if ((of = fopen (outfilename, "w")) == NULL) {
		fprintf (stderr, "Cannot open %s\n", outfilename);
                fflush(stderr);
		RET;
	}

	for (i = 0, lptr = (char *)&machine_id; i < sizeof(netaddr_t); i++, lptr++)
		putc(*lptr, of);
	for (i = 0, lptr = (char *)port_stat_ptr; i < (port_stat_ptr_size<<2); i++, lptr++)
		putc(*lptr, of);

	fclose(of);
	RET;
END


/*
 * showportstatrec
 *
 * Display the values in a record according to a list of names.
 *
 * Parameters:
 *
 * of: file to write output to.
 * rec: pointer to the record to display.
 * rec_names: names of the fields.
 *
 * Results:
 *
 * Side Effects:
 *
 * Design:
 *
 * Note:
 *
 */
showportstatrec(of,rec,rec_names)
FILE	*of;
int	rec[];
char	*rec_names[];
BEGIN("showportstatrec")
	int	curfield;

	/*
	 * Loop through all the fields in the record.
	 */
	for ( curfield = 0; rec_names[curfield]; curfield++) {
	    	if (curfield & 0x1)
			fprintf(of,"      %25s %8x\n",rec_names[curfield],rec[curfield]);
		else
			fprintf(of,"%25s %8x",rec_names[curfield],rec[curfield]);
	}

	fprintf(stdout,"\n");
	RET;
END


/*
 * genericportstat
 *
 * execute any port statistics related command.
 *
 * Parameters: none.
 *
 * Results:
 *
 * Side effects:
 *
 * May execute a port stat subcommand.
 * May prompt for the name of the subcommand to execute.
 *
 * Design:
 *
 * Note:
 *
 */
void genericportstat()
BEGIN("genericportstat")
	char	subcmd[2];
	char	outfilename[80];
	FILE	*of;

	input("Enter Port Statistics sub-command: ", "%1s", subcmd);
	switch(subcmd[0]) {
	case 'G':
	case 'g':
		if (ls_sendportstat(logstat_port,&port_stat_ptr,&port_stat_ptr_size) == KERN_SUCCESS) {
			fprintf(stdout, "Received %d port statistics records.\n",
				(port_stat_ptr_size << 2) / sizeof(port_stat_t));
		}
		else fprintf(stderr, "ls_sendportstat fails.\n");
		break;
	case 'W':
	case 'w':
		{
			int		num_records = (port_stat_ptr_size << 2) / sizeof(port_stat_t);
			port_stat_ptr_t	temp_ptr = port_stat_ptr;

			/* 
			 * Get the file descriptor that we are to write to.
			 */
			input("Output file: ", "%79s", outfilename);
			if (outfilename[0] == '-')
				of = stdout;
			else {
				if ((of = fopen (outfilename, "w")) == NULL) {
					fprintf (stderr, "Cannot open %s\n", outfilename);
		                	fflush(stderr);
					RET;
				}
			}
			while (num_records--) showportstatrec(of, temp_ptr++, port_stat_names);
		}
		break;
	case 'D':
	case 'd':
		dumpportstat();
		break;
	default:
		fprintf(stdout, "Invalid subcommand. Type ? for help.\n");
		readln(stdin);
		break;
	}
	fflush(stdout);
	RET;
END


/*
 * memlist
 *
 * Analyze memory allocation status.
 */
void memlist()
BEGIN("memlist")
	char			outfilename[80];
	FILE			*of;
	kern_return_t		kr;
	mem_objrec_ptr_t	class_ptr;
	int			class_ptr_size;
	char			*nam_ptr;
	int			nam_ptr_size;
	int			*bucket_ptr;
	int			bucket_ptr_size;
	int			class_num;
	mem_objrec_ptr_t	cur_class;
	char			*cur_nam;
	int			*cur_bucket;
	int			i;

	/* 
	 * Get the file descriptor that we are to write to.
	 */
	input("Output file: ", "%79s", outfilename);
	if (outfilename[0] == '-')
		of = stdout;
	else {
		if ((of = fopen (outfilename, "w")) == NULL) {
			fprintf (stderr, "Cannot open %s\n", outfilename);
                	fflush(stderr);
			RET;
		}
	}

	/*
	 * Get the list from the server.
	 */
	kr = ls_mem_list(logstat_port,
				&class_ptr,&class_ptr_size,
				&nam_ptr,&nam_ptr_size,
				&bucket_ptr,&bucket_ptr_size);
	if ((kr != KERN_SUCCESS) || (class_ptr_size == 0)) {
		fprintf(stderr,"ls_mem_list failed\n");
		RET;
	}

	/*
	 * Display the info.
	 */
	cur_class = class_ptr;
	cur_nam = nam_ptr;
	cur_bucket = bucket_ptr;
	for (cur_class = class_ptr, class_num = 0;
			class_num < (class_ptr_size / sizeof(mem_objrec_t));
			cur_class++, class_num++) {
		fprintf(of,"**  %s:  full_num = %d, reuse_num = %d\n",
			cur_nam,cur_class->full_num,cur_class->reuse_num);
		fprintf(of,"   obj size = %d, bucket size = %d, wasted = %d\n",
			cur_class->obj_size,cur_class->bucket_size,
			cur_class->bucket_size -
				(cur_class->full_num * cur_class->obj_size));
		fprintf(of,"   cur_buckets = %d, max_buckets = %d\n",
			cur_class->cur_buckets,cur_class->max_buckets);
		fprintf(of,"   Usage: ");
		for (i = 0; i < cur_class->cur_buckets; i++) {
			fprintf(of,"%4d",cur_class->full_num - *cur_bucket);
			cur_bucket++;
		}
		fprintf(of,"\n");
		cur_nam += 80;
	}

	kr = vm_deallocate(task_self(),class_ptr,class_ptr_size);
	kr = vm_deallocate(task_self(),nam_ptr,nam_ptr_size);
	kr = vm_deallocate(task_self(),bucket_ptr,bucket_ptr_size);

	if (of != stdout)
		fclose(of);

	fflush(stdout);

	RET;
END


/*
 * main
 *
 * Main loop.
 *
 * Parameters:
 *
 * Results:
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
main(argc, argv)
int argc;
char **argv;
BEGIN("main")
	int quit;
	char cmd[2];

	if (argc == 1)
		openconnection(NULL);
	else
		openconnection(argv[1]);

	quit = FALSE;
	do {
		input("> ", "%1s", cmd);
		switch (cmd[0]) {
		case 'H':
		case 'h':
		case '?':
			printmenu();
			break;
		case 'O':
		case 'o':
			openconnection(NULL);
			break;
		case 'L':
		case 'l':
			genericlog();
			break;
		case 'S':
		case 's':
			genericstat();
			break;
		case 'D':
		case 'd':
			genericdebug();
			break;
		case 'P':
		case 'p':
			genericparam();
			break;
		case 'M':
		case 'm':
			genericportstat();
			break;
		case 'A':
		case 'a':
			memlist();
			break;
		case 'Q':
		case 'q':
			quit = TRUE;
			break;
		default:
			fprintf(stdout, "Invalid command. Type ? for list.\n");
			readln(stdin);
			break;
		}
	} 
	while (!quit);

	fprintf(stdout, "Terminated.\n");
	RET;
END
