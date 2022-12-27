/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	afsint.h,v $
 * Revision 2.3  89/06/03  15:34:32  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:10:43  jsb]
 * 
 * Revision 2.2  89/04/22  15:18:34  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */
/* Machine generated file -- Do NOT edit */

#ifndef	_RXGEN_AFSINT_
#define	_RXGEN_AFSINT_

#ifdef	KERNEL
/* The following 'ifndefs' are not a good solution to the vendor's omission of surrounding all system includes with 'ifndef's since it requires that this file is included after the system includes...*/
#include <sys/types.h>
#ifndef	SOCK_DGRAM  /* XXXXX */
#include <sys/socket.h>
#endif
#ifndef	DTYPE_SOCKET  /* XXXXX */
#include <sys/file.h>
#endif
#ifndef	S_IFMT  /* XXXXX */
#include <sys/stat.h>
#endif
#ifndef	IPPROTO_UDP /* XXXXX */
#include <netinet/in.h>
#endif
#ifndef	DST_USA  /* XXXXX */
#include <sys/time.h>
#endif
#include <rpc/types.h>
#ifndef	XDR_GETLONG /* XXXXX */
#include <rpc/xdr.h>
#endif
#include <afsint/rxgen_consts.h>
#include <afs/osi.h>
#include <afsint/rx.h>
#else	KERNEL
#include <sys/types.h>
#include <rx/xdr.h>
#include <rx/rx.h>
#include <afs/rxgen_consts.h>
#endif	KERNEL


struct AFSFid {
	u_long Volume;
	u_long Vnode;
	u_long Unique;
};
typedef struct AFSFid AFSFid;
bool_t xdr_AFSFid();


struct AFSCallBack {
	u_long CallBackVersion;
	u_long ExpirationTime;
	u_long CallBackType;
};
typedef struct AFSCallBack AFSCallBack;
bool_t xdr_AFSCallBack();

#define EXCLUSIVE 1
#define SHARED 2
#define DROPPED 3
#define AFSNAMEMAX 256
#define AFSPATHMAX 1024
#define AFSOPAQUEMAX 1024

typedef struct AFSOpaque {
	u_int AFSOpaque_len;
	char *AFSOpaque_val;
} AFSOpaque;
bool_t xdr_AFSOpaque();

#define AFSCBMAX 50

typedef struct AFSCBFids {
	u_int AFSCBFids_len;
	AFSFid *AFSCBFids_val;
} AFSCBFids;
bool_t xdr_AFSCBFids();


typedef struct AFSCBs {
	u_int AFSCBs_len;
	AFSCallBack *AFSCBs_val;
} AFSCBs;
bool_t xdr_AFSCBs();

#define VICECONNBAD	1234
#define VICETOKENDEAD	1235
#define AFS_LOCKWAIT	(5*60)
#define CALLBACK_VERSION	1

typedef u_long VolumeId;
bool_t xdr_VolumeId();


typedef u_long VolId;
bool_t xdr_VolId();


typedef u_long VnodeId;
bool_t xdr_VnodeId();


typedef u_long Unique;
bool_t xdr_Unique();


typedef u_long UserId;
bool_t xdr_UserId();


typedef u_long FileVersion;
bool_t xdr_FileVersion();


typedef u_long Date;
bool_t xdr_Date();


typedef long ErrorCode;
bool_t xdr_ErrorCode();


typedef long Rights;
bool_t xdr_Rights();


struct CBS {
	long SeqLen;
	char *SeqBody;
};
typedef struct CBS CBS;
bool_t xdr_CBS();


struct BBS {
	long MaxSeqLen;
	long SeqLen;
	char *SeqBody;
};
typedef struct BBS BBS;
bool_t xdr_BBS();


struct AFSAccessList {
	long MaxSeqLen;
	long SeqLen;
	char *SeqBody;
};
typedef struct AFSAccessList AFSAccessList;
bool_t xdr_AFSAccessList();


struct ViceFid {
	u_long Volume;
	u_long Vnode;
	u_long Unique;
};
typedef struct ViceFid ViceFid;
bool_t xdr_ViceFid();


typedef long ViceDataType;
bool_t xdr_ViceDataType();

#define Invalid 0
#define File 1 
#define Directory 2 
#define SymbolicLink 3 

struct BD {
	long host;
	long portal;
	long session;
};
typedef struct BD BD;
bool_t xdr_BD();


struct AFSVolSync {
	u_long spare1;
	u_long spare2;
	u_long spare3;
	u_long spare4;
	u_long spare5;
	u_long spare6;
};
typedef struct AFSVolSync AFSVolSync;
bool_t xdr_AFSVolSync();


struct AFSOldFetchStatus {
	u_long InterfaceVersion;
	u_long FileType;
	u_long LinkCount;
	u_long Length;
	u_long DataVersion;
	u_long Author;
	u_long Owner;
	u_long CallerAccess;
	u_long AnonymousAccess;
	u_long UnixModeBits;
	u_long ParentVnode;
	u_long ParentUnique;
	u_long SegSize;
	u_long ClientModTime;
	u_long ServerModTime;
	u_long Group;
};
typedef struct AFSOldFetchStatus AFSOldFetchStatus;
bool_t xdr_AFSOldFetchStatus();


struct AFSFetchStatus {
	u_long InterfaceVersion;
	u_long FileType;
	u_long LinkCount;
	u_long Length;
	u_long DataVersion;
	u_long Author;
	u_long Owner;
	u_long CallerAccess;
	u_long AnonymousAccess;
	u_long UnixModeBits;
	u_long ParentVnode;
	u_long ParentUnique;
	u_long SegSize;
	u_long ClientModTime;
	u_long ServerModTime;
	u_long Group;
	u_long SyncCounter;
	u_long spare1;
	u_long spare2;
	u_long spare3;
	u_long spare4;
};
typedef struct AFSFetchStatus AFSFetchStatus;
bool_t xdr_AFSFetchStatus();


struct AFSStoreStatus {
	u_long Mask;
	u_long ClientModTime;
	u_long Owner;
	u_long Group;
	u_long UnixModeBits;
	u_long SegSize;
};
typedef struct AFSStoreStatus AFSStoreStatus;
bool_t xdr_AFSStoreStatus();

#define	AFS_SETMODTIME	1
#define	AFS_SETOWNER	2
#define	AFS_SETGROUP		4
#define	AFS_SETMODE		8
#define	AFS_SETSEGSIZE	16

typedef long ViceVolumeType;
bool_t xdr_ViceVolumeType();

#define ReadOnly 0
#define ReadWrite 1
#define AFS_DISKNAMESIZE 32

typedef char DiskName[AFS_DISKNAMESIZE];
bool_t xdr_DiskName();


struct ViceDisk {
	long BlocksAvailable;
	long TotalBlocks;
	DiskName Name;
};
typedef struct ViceDisk ViceDisk;
bool_t xdr_ViceDisk();


struct ViceStatistics {
	u_long CurrentMsgNumber;
	u_long OldestMsgNumber;
	u_long CurrentTime;
	u_long BootTime;
	u_long StartTime;
	long CurrentConnections;
	u_long TotalViceCalls;
	u_long TotalFetchs;
	u_long FetchDatas;
	u_long FetchedBytes;
	long FetchDataRate;
	u_long TotalStores;
	u_long StoreDatas;
	u_long StoredBytes;
	long StoreDataRate;
	u_long TotalRPCBytesSent;
	u_long TotalRPCBytesReceived;
	u_long TotalRPCPacketsSent;
	u_long TotalRPCPacketsReceived;
	u_long TotalRPCPacketsLost;
	u_long TotalRPCBogusPackets;
	long SystemCPU;
	long UserCPU;
	long NiceCPU;
	long IdleCPU;
	long TotalIO;
	long ActiveVM;
	long TotalVM;
	long EtherNetTotalErrors;
	long EtherNetTotalWrites;
	long EtherNetTotalInterupts;
	long EtherNetGoodReads;
	long EtherNetTotalBytesWritten;
	long EtherNetTotalBytesRead;
	long ProcessSize;
	long WorkStations;
	long ActiveWorkStations;
	long Spare1;
	long Spare2;
	long Spare3;
	long Spare4;
	long Spare5;
	long Spare6;
	long Spare7;
	long Spare8;
	ViceDisk Disk1;
	ViceDisk Disk2;
	ViceDisk Disk3;
	ViceDisk Disk4;
	ViceDisk Disk5;
	ViceDisk Disk6;
	ViceDisk Disk7;
	ViceDisk Disk8;
	ViceDisk Disk9;
	ViceDisk Disk10;
};
typedef struct ViceStatistics ViceStatistics;
bool_t xdr_ViceStatistics();


struct VolumeStatus {
	long Vid;
	long ParentId;
	char Online;
	char InService;
	char Blessed;
	char NeedsSalvage;
	long Type;
	long MinQuota;
	long MaxQuota;
	long BlocksInUse;
	long PartBlocksAvail;
	long PartMaxBlocks;
};
typedef struct VolumeStatus VolumeStatus;
bool_t xdr_VolumeStatus();


struct AFSFetchVolumeStatus {
	long Vid;
	long ParentId;
	char Online;
	char InService;
	char Blessed;
	char NeedsSalvage;
	long Type;
	long MinQuota;
	long MaxQuota;
	long BlocksInUse;
	long PartBlocksAvail;
	long PartMaxBlocks;
};
typedef struct AFSFetchVolumeStatus AFSFetchVolumeStatus;
bool_t xdr_AFSFetchVolumeStatus();


struct AFSStoreVolumeStatus {
	long Mask;
	long MinQuota;
	long MaxQuota;
};
typedef struct AFSStoreVolumeStatus AFSStoreVolumeStatus;
bool_t xdr_AFSStoreVolumeStatus();

#define AFS_SETMINQUOTA 1
#define AFS_SETMAXQUOTA 2

struct AFSVolumeInfo {
	u_long Vid;
	long Type;
	u_long Type0;
	u_long Type1;
	u_long Type2;
	u_long Type3;
	u_long Type4;
	u_long ServerCount;
	u_long Server0;
	u_long Server1;
	u_long Server2;
	u_long Server3;
	u_long Server4;
	u_long Server5;
	u_long Server6;
	u_long Server7;
	u_short Port0;
	u_short Port1;
	u_short Port2;
	u_short Port3;
	u_short Port4;
	u_short Port5;
	u_short Port6;
	u_short Port7;
};
typedef struct AFSVolumeInfo AFSVolumeInfo;
bool_t xdr_AFSVolumeInfo();


struct VolumeInfo {
	u_long Vid;
	long Type;
	u_long Type0;
	u_long Type1;
	u_long Type2;
	u_long Type3;
	u_long Type4;
	u_long ServerCount;
	u_long Server0;
	u_long Server1;
	u_long Server2;
	u_long Server3;
	u_long Server4;
	u_long Server5;
	u_long Server6;
	u_long Server7;
};
typedef struct VolumeInfo VolumeInfo;
bool_t xdr_VolumeInfo();


typedef long ViceLockType;
bool_t xdr_ViceLockType();

#define LockRead		0
#define LockWrite		1
#define LockExtend	2
#define LockRelease	3

typedef long ViceOfflineType;
bool_t xdr_ViceOfflineType();

#define NoSalvage 0
#define Salvage 1

/* Opcode-related useful stats for package: RXAFS_ */
#define RXAFS_LOWEST_OPCODE   130
#define RXAFS_HIGHEST_OPCODE	154
#define RXAFS_NUMBER_OPCODES	25

#endif	_RXGEN_AFSINT_
