/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cfs_xdr.c,v $
 * Revision 2.2  90/07/05  11:26:51  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.4  90/05/31  17:02:03  dcs
 * Prepare for merge with facilities kernel.
 * 
 * Revision 1.3  90/03/27  14:15:39  satya
 * Added #ifdef KERNEL around include cfs/cfs.h to
 * 
 * Revision 1.2  90/03/19  16:47:34  dcs
 * Removed the offset field from the cfsreadlinkrtnargs.
 * 
 * Revision 1.1  90/03/15  10:43:42  jjk
 * Initial revision
 * 
 */

#ifdef KERNEL
#include <cfs/cfs.h>
#else
#include "vice.h"
#include "cfs.h"
#endif
#include <netinet/in.h>

#ifdef	CFSDEBUG
char *xdropnames[] = {"encode", "decode", "free"};
#endif

/*
 * These are the XDR routines used to serialize and deserialize
 * the various structures passed as parameters accross the network.
 */

/*
 * File Identifier
 * Used by Coda to identify files, it is composed of a Volume ID, a Coda
 * vnode ID, and a uniquifier.
 */
bool_t
xdr_fid(xdrs, fid)
	XDR *xdrs;
	ViceFid *fid;
{
	if (xdr_u_long(xdrs, &(fid->Volume)) &&
	    xdr_u_long(xdrs, &(fid->Vnode)) &&
	    xdr_u_long(xdrs, &(fid->Unique)))
		return (TRUE);
	return (FALSE);
}

/*
 * handle cred structures
 */
bool_t
xdr_cred(xdrs, cred)
	XDR *xdrs;
	struct ucred *cred;
{
	if (xdr_u_short(xdrs, &cred->id_ref) &&
	    xdr_u_short(xdrs, &cred->id_uid) &&
	    xdr_u_short(xdrs, &cred->id_ruid) &&
	    xdr_u_short(xdrs, &cred->id_gid) &&
	    xdr_u_short(xdrs, &cred->id_rgid) &&
	    xdr_vector(xdrs, &cred->id_groups, NGROUPS, 
			sizeof(u_short), xdr_u_short) &&
	    xdr_long(xdrs, &cred->id_pag))
		return(TRUE);
	return(FALSE);
}

/*
 * fidcred handles ViceFids and Cred structures as arguments
 */
bool_t
xdr_fidcred(xdrs, fca)
	XDR *xdrs;
	struct cfsfidcred *fca;
{
	if (xdr_cred(xdrs, &fca->cred) &&
	    xdr_fid(xdrs, &fca->ViceFid))
		return(TRUE);
	return(FALSE);
}

/*
 * fidreturn handles ViceFids as return parameters
 */
bool_t
xdr_fidreturn(xdrs, fa)
	XDR *xdrs;
	struct cfsfidreturn *fa;
{
	if (xdr_fid(xdrs, &fa->ViceFid) &&
	    xdr_u_long(xdrs, &fa->result))
		return(TRUE);
	return(FALSE);
}

/*
 * fidreturn handles ViceFids as return parameters
 */
bool_t
xdr_fidattrreturn(xdrs, fa)
	XDR *xdrs;
	struct cfsfidattrreturn *fa;
{
	if (xdr_fid(xdrs, &fa->ViceFid) &&
	    xdr_vattr(xdrs, &fa->attr) &&
	    xdr_u_long(xdrs, &fa->result))
		return(TRUE);
	return(FALSE);
}

/*
 * attr is a list of file attributes
 */
bool_t
xdr_vattr(xdrs, aa)
	XDR *xdrs;
	struct vattr *aa;
{
	if (xdr_u_short(xdrs, &(aa->va_mode)) &&
	    xdr_short(xdrs, &(aa->va_uid)) &&
	    xdr_short(xdrs, &(aa->va_gid)) &&
	    xdr_long(xdrs, &(aa->va_fsid)) &&
	    xdr_long(xdrs, &(aa->va_nodeid)) &&
	    xdr_short(xdrs, &(aa->va_nlink)) &&
	    xdr_u_long(xdrs, &(aa->va_size)) &&
	    xdr_long(xdrs, &(aa->va_blocksize)) &&
	    xdr_timeval(xdrs, &(aa->va_atime)) &&
	    xdr_timeval(xdrs, &(aa->va_mtime)) &&
	    xdr_timeval(xdrs, &(aa->va_ctime)) &&
	    xdr_short(xdrs, &(aa->va_rdev)) &&
	    xdr_long(xdrs, &(aa->va_blocks)))
		return(TRUE);
	return(FALSE);
}

/*
 * Return Arguments for CFS_GETATTR
 */
bool_t
xdr_attr_return(xdrs, ara)
	XDR *xdrs;
	struct cfsattrreturn *ara;
{
	if (xdr_vattr(xdrs, &ara->attr) &&
	    xdr_u_long(xdrs, &ara->result))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_SETATTR
 */
bool_t
xdr_cfssetattr(xdrs, saa)
	XDR *xdrs;
	struct cfssetattrargs *saa;
{
	if (xdr_cred(xdrs, &saa->cred) &&
	    xdr_vattr(xdrs, &saa->attr) &&
	    xdr_fid(xdrs, &saa->ViceFid))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_CREATE
 */
bool_t
xdr_cfscreate(xdrs, ca)
	XDR *xdrs;
	struct cfscreateargs *ca;
{
	if (xdr_cred(xdrs, &ca->cred) &&
	    xdr_fid(xdrs, &ca->ViceFid) &&
	    xdr_string(xdrs, &ca->name, CFS_MAXNAMLEN) &&
	    xdr_vattr(xdrs, &ca->attr) &&
	    xdr_int(xdrs, &ca->excl) &&
	    xdr_int(xdrs, &ca->mode))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for cfs_open, cfs_close, and cfs_select.
 */
bool_t
xdr_fid_int(xdrs, oca)
	XDR *xdrs;
	struct cfsfidintargs *oca;
{
	if (xdr_cred(xdrs, &oca->cred) &&
	    xdr_fid(xdrs, &oca->ViceFid) &&
	    xdr_int(xdrs, &oca->flags))
		return(TRUE);
	return(FALSE);
}

/* 
 * Return arguments for open
 */
bool_t
xdr_cfsopenreturn(xdrs, opa)
	XDR *xdrs;
	struct cfsopenreturnargs *opa;
{
	if (xdr_u_long(xdrs, &opa->inode) &&
	    xdr_short(xdrs, &opa->dev) &&
	    xdr_u_long(xdrs, &opa->result))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for cfs_lookup, cfs_remove, and cfs_rmdir.
 */
bool_t
xdr_fid_name(xdrs, vna)
	XDR *xdrs;
	struct cfsfidnameargs *vna;
{
	if (xdr_cred(xdrs, &vna->cred) &&
	    xdr_fid(xdrs, &vna->ViceFid) &&
	    xdr_string(xdrs, &vna->name, CFS_MAXNAMLEN))
		return(TRUE);
	return(FALSE);
}

/*
 * Return arguments for CFS_LOOKUP
 */
bool_t
xdr_cfslookupreturn(xdrs, lra)
	XDR *xdrs;
	struct cfslookupreturnargs *lra;
{
	if (xdr_fid(xdrs, &lra->ViceFid) &&
	    xdr_u_long(xdrs, &lra->result) &&
	    xdr_int(xdrs, &lra->vtype))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_LINK
 */
bool_t
xdr_cfslink(xdrs, la)
	XDR *xdrs;
	struct cfslinkargs *la;
{
	if (xdr_cred(xdrs, &la->cred) &&
	    xdr_fid(xdrs, &la->sourceFid) &&
	    xdr_fid(xdrs, &la->destFid) &&
	    xdr_string(xdrs, &la->tname, CFS_MAXPATHLEN))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_RENAME
 */
bool_t
xdr_cfsrename(xdrs, ra)
	XDR *xdrs;
	struct cfsrenameargs *ra;
{
	if (xdr_cred(xdrs, &ra->cred) &&
	    xdr_fid(xdrs, &ra->sourceFid) &&
	    xdr_string(xdrs, &ra->srcname, CFS_MAXNAMLEN) &&
	    xdr_fid(xdrs, &ra->destFid) &&
	    xdr_string(xdrs, &ra->destname, CFS_MAXNAMLEN))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_MKDIR
 */
bool_t
xdr_cfsmkdir(xdrs, ma)
	XDR *xdrs;
	struct cfsmkdirargs *ma;
{
	if (xdr_cred(xdrs, &ma->cred) &&
	    xdr_fid(xdrs, &ma->ViceFid) &&
	    xdr_string(xdrs, &ma->name, CFS_MAXNAMLEN) &&
	    xdr_vattr(xdrs, &ma->attr))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_SYMLINK
 */
bool_t
xdr_cfssymlink(xdrs, sa)
	XDR *xdrs;
	struct cfssymlinkargs *sa;
{
	if (xdr_cred(xdrs, &sa->cred) &&
	    xdr_fid(xdrs, &sa->ViceFid) &&
	    xdr_string(xdrs, &sa->srcname, CFS_MAXNAMLEN) &&
	    xdr_vattr(xdrs, &sa->attr) &&
	    xdr_string(xdrs, &sa->tname, CFS_MAXPATHLEN))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_IOCTL
 */
bool_t
xdr_cfsioctl(xdrs, ioa)
	XDR *xdrs;
	struct cfsioctlargs *ioa;
{
	if (xdr_cred(xdrs, &ioa->cred) &&
	    xdr_fid(xdrs, &ioa->ViceFid) &&
	    xdr_int(xdrs, &ioa->cmd) &&
	    xdr_int(xdrs, &ioa->rwflag) &&
	    xdr_bytes(xdrs, &ioa->data, &ioa->len, CFS_MAXARRAYSIZE))
		return(TRUE);
	return(FALSE);
}

/*
 * Return arguments for CFS_IOCTL
 */
bool_t
xdr_cfsioctlreturn(xdrs, iora)
	XDR *xdrs;
	struct cfsioctlreturnargs *iora;
{
	if (xdr_u_long(xdrs, &iora->result) &&
	    xdr_bytes(xdrs, &iora->data, &iora->len, CFS_MAXARRAYSIZE))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_READDIR
 */
bool_t
xdr_cfsreaddir(xdrs, rda)
	XDR *xdrs;
	struct cfsreaddirargs *rda;
{
	if (xdr_cred(xdrs, &rda->cred) &&
	    xdr_fid(xdrs, &rda->ViceFid) &&
	    xdr_int(xdrs, &rda->count) &&
	    xdr_int(xdrs, &rda->offset))
		return(TRUE);
	return(FALSE);
}

/*
 * Return arguments for CFS_READDIR
 */
bool_t
xdr_cfsreaddirrtn(xdrs, rdr)
	XDR *xdrs;
	struct cfsreaddirrtnargs *rdr;
{
	if (xdr_u_long(xdrs, &rdr->result) &&
	    xdr_bytes(xdrs, &rdr->data, &rdr->size, rdr->size))
		return(TRUE);
	return(FALSE);
}

/*
 * Return arguments for CFS_READLINK
 */
bool_t
xdr_cfsreadlinkrtn(xdrs, rlr)
	XDR *xdrs;
	struct cfsreadlinkrtnargs *rlr;
{
	if (xdr_bytes(xdrs, &rlr->data, &rlr->count, CFS_MAXPATHLEN) &&
	    xdr_u_long(xdrs, &rlr->result))
		return(TRUE);
	return(FALSE);
}

/*
 * Arguments for CFS_RDWR
 */
bool_t
xdr_cfsrdwr(xdrs, rwa)
	XDR *xdrs;
	struct cfsrdwrargs *rwa;
{
	if (xdr_cred(xdrs, &rwa->cred) &&
	    xdr_fid(xdrs, &rwa->ViceFid) &&
	    xdr_int(xdrs, &rwa->rwflag) &&
	    ((rwa->rwflag == (int)UIO_READ &&
	      xdr_u_int(xdrs, &rwa->count)) ||
	     (rwa->rwflag == (int)UIO_WRITE &&
	      xdr_bytes(xdrs, &rwa->data, &rwa->count, rwa->count))) &&
	    xdr_int(xdrs, &rwa->offset) &&
	    xdr_int(xdrs, &rwa->ioflag))
		return(TRUE);
	return(FALSE);
}

/*
 * Return arguments for CFS_RDWR
 */
bool_t
xdr_cfsrdwrrtn(xdrs, rwra)
	XDR *xdrs;
	struct cfsrdwrrtnargs *rwra;
{
	if (xdr_u_long(xdrs, &rwra->result) &&
	    xdr_int(xdrs, &rwra->rwflag) &&
	    ((rwra->rwflag == (int)UIO_READ &&
	      xdr_bytes(xdrs, &rwra->data, &rwra->count, rwra->count)) ||
	     (rwra->rwflag == (int)UIO_WRITE &&
	      xdr_u_int(xdrs, &rwra->count))))
		return(TRUE);
	return(FALSE);
}
