/*
 *  access.h - account access definitions
 *
 **********************************************************************
 * HISTORY
 * 23-Oct-87  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Fixed codes to be decimal, not octal.
 *
 * 08-Dec-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added 4.2 daemons and RFS to access list.  Changed all
 *	failure access codes to correspond to 4.2 accounting
 *	system.
 *
 * 09-Oct-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added ACCESS_CODE_NOTLOGIN for new failure from oklogin().
 *
 * 03-Jun-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Added ACCESS_CODE_INSECUREPWD for failures after okpassword().
 *
 * 29-Jul-83  Mike Accetta (mja) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 */


/*
 *  Format of a login account access record (in /usr/adm/access)
 */

struct access
{
    char	  acs_name[8];		/* login name accessed */
    short	  acs_uid;		/* from user ID */
    dev_t	  acs_ttydev;		/* on terminal device */
    struct ttyloc acs_ttyloc;		/* with terminal location */
    time_t	  acs_time;		/* at time */
    int		  acs_type:5,		/* from agent (see below) */
		  acs_code:5,		/* with result (see below) */
		  acs_pad:22;		/* reserved for future expansion */
};

/*
 *  Access types
 */
#define	ACCESS_TYPE_LOGIN	1	/* login program */
#define	ACCESS_TYPE_SU		2	/* su program */
#define	ACCESS_TYPE_FTP		3	/* ftp program */
#define	ACCESS_TYPE_CMUFTP	4	/* cmuftp program */
#define	ACCESS_TYPE_SUP		5	/* supfilesrv program */
#define	ACCESS_TYPE_RFS		6	/* remote file system server */
#define ACCESS_TYPE_RLOGIN	7	/* remote login program */
#define ACCESS_TYPE_REXEC	8	/* remote exec program */
#define ACCESS_TYPE_RSH		9	/* remote shell program */

/*
 *  Access codes
 */
#define	ACCESS_CODE_INSECUREPWD	3	/* insecure password */
#define	ACCESS_CODE_NODIR	2	/* no directory */
#define	ACCESS_CODE_DENIED	1	/* accessed denied */
#define	ACCESS_CODE_OK		0	/* unused */
#define	ACCESS_CODE_NOUSER	(-1)	/* unknown user */
#define	ACCESS_CODE_BADPASSWORD (-2)	/* invalid password */
#define	ACCESS_CODE_ACCEXPIRED	(-3)	/* account is expired */
#define	ACCESS_CODE_GRPEXPIRED	(-4)	/* group is expired */
#define	ACCESS_CODE_ACCNOTVALID	(-5)	/* account not valid for login or su */
#define	ACCESS_CODE_MANYDEFACC	(-6)	/* more than one default account */
#define	ACCESS_CODE_NOACCFORGRP	(-7)	/* no account found for group */
#define	ACCESS_CODE_NOGRPFORACC	(-8)	/* no group found for account */
#define	ACCESS_CODE_NOGRPDEFACC	(-9)	/* no default login account group */
#define	ACCESS_CODE_NOTGRPMEMB	(-10)	/* not member of group */
#define	ACCESS_CODE_NOTDEFMEMB	(-11)	/* not member of default login group */
#define	ACCESS_CODE_OOPS	(-12)	/* shouldn't happen */
