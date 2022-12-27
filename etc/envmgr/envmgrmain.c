/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *  HISTORY
 * $Log:	envmgrmain.c,v $
 * Revision 1.1  90/02/19  21:47:21  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.3  89/05/09  21:56:08  mrt
 * 	Changed NameServerPort to name_server_port.
 * 
 * Revision 1.2  89/05/05  18:30:45  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 *  Mar 30 87	Mary Thompson
 *	Changed msgn interface to netname
 *
 *  Feb-10-87	Mary Thompson
 *	Added exit(2) if env_run exits. This keeps
 *	nanny from attempting to restart it.
 *
 *  Feb-5-86  Mary Thompson
 *	Changed env_init to call initial_conn instead
 *	of env_new_conn.
 *
 *  Oct 7 86  Mary Thompson
 *	Changed to include  new netmsgserver interface
 *	that no longer includes accenttype
 */
/*
 * Abstract:
 *   Main loop of Environment Manager
 *
 * Written by: Mary Thompson
 */

#include <stdio.h>

#include <mach.h>
#include <mach/message.h>
#include <mach/notify.h>
#include <servers/netname.h>

extern void 		initial_conn();
extern kern_return_t	env_disconnect();
extern boolean_t	remove_dead_port();
extern void		EnvInitialize();
extern boolean_t	env_mgr_server();

/************************************************************
 *	procedure  env_init (knownenvport) 
 *
 *  Abstract
 *	initializes the env_mgr tables, creates an empty
 *	environment,and returns the port to it.
 ************************************************************/
void env_init (KnownEnvPort)
  port_t 	*KnownEnvPort;
{
   EnvInitialize();
   initial_conn(KnownEnvPort);
   printf("Environment Manager initialized: \nInitial environment port is %d\n",
		(int)*KnownEnvPort);
}

/*********************************************************
 * procedure env_run  
 *
 * Abstract:
 *   Main loop of Environment Manager.  Waits for messages to server,
 *   handles them, and replies to sender.
 *
 *********************************************************/

void env_run ()
{
   typedef int space[1024]; /* Maximum message size */

   typedef struct DumMsg
        {
           msg_header_t 	head;
           space 		body;
        } DumMsg;

   typedef DumMsg *pDumMsg;

   typedef struct OwnPortMsg
        {
           msg_header_t Head;
           msg_type_t 	PortType;
           port_t 	PortIndex;
        } OwnPortMsg;

   typedef OwnPortMsg *pOwnPortMsg;


#define Debug 0

   kern_return_t	retcode;
   msg_return_t		msgcode;
   boolean_t 		ok;
   pDumMsg 		pInMsg, pRepMsg;
   pOwnPortMsg 		pOwnMsg;


   pInMsg = (pDumMsg)malloc(sizeof(DumMsg));
   pRepMsg = (pDumMsg)malloc(sizeof(DumMsg));
   retcode = port_enable(task_self_,task_notify_);
   if (retcode != KERN_SUCCESS)
	printf(" port_enable on task_nofify_ failed; code = %d\n",
		retcode);
#if Debug
   printf(" (* Main loop in envmgrmain entered. *)\n");
#endif
   while (TRUE)
     {
       pInMsg->head.msg_size = (long)(2048); /* bytes */
       pInMsg->head.msg_local_port = PORT_DEFAULT;
#if Debug
         printf(" (* Envmgrmain requesting recieve on AllPts. *)\n");
#endif

       msgcode = msg_receive(&pInMsg->head,MSG_OPTION_NONE,0);
       if (msgcode != RCV_SUCCESS)
           printf("*** error %d in EnvRun at Receive, message will be ignored.\n",msgcode);
       else 
         { if (pInMsg->head.msg_type == MSG_TYPE_EMERGENCY)
           {
#if Debug
             printf(" (* EnvRun received Emergency Msg %d on port %d. *)\n",
			pInMsg->head.msg_id,(int )(pInMsg->head.msg_local_port));
#endif
             if (pInMsg->head.msg_id == NOTIFY_OWNERSHIP_RIGHTS)
                  {   /* A user process has died or explicitly deallocated
                         * a port corresponding to a search list connection. */

                     pOwnMsg = (pOwnPortMsg )(pInMsg);
                        retcode  = env_disconnect(pOwnMsg->PortIndex);
#if Debug
		     printf("return from env_disconnect was %d\n",retcode);
#endif
		  }
             else if (pInMsg->head.msg_id == NOTIFY_PORT_DELETED)
		  {  /*  a port that has been entered may have been deallocated */
                     pOwnMsg = (pOwnPortMsg )(pInMsg);
                        ok   = remove_dead_port(pOwnMsg->PortIndex);
#if Debug
	if (ok)
	   printf("port %d was removed from port table\n",pOwnMsg->PortIndex);
        else 	printf("port %d was not removed from port table\n",pOwnMsg->PortIndex);
#endif
		  }

           }
          else { /* normal message */
              ok = env_mgr_server((char *)pInMsg,(char *)pRepMsg);
#if Debug
              if (!ok)
                  printf(" (* Return code from environment manager is NOT success. *)\n");
              else
                  printf(" (* Envmgrmain done processing server request. *)\n");
#endif
              pRepMsg->head.msg_local_port = pInMsg->head.msg_local_port;
              pRepMsg->head.msg_remote_port = pInMsg->head.msg_remote_port;
#if Debug
              printf(" (* Envmgrmain sending reply message. *)\n");
#endif
              msgcode = msg_send(&pRepMsg->head,MSG_OPTION_NONE,0);
              if ((msgcode != SEND_SUCCESS) && (msgcode != SEND_INVALID_PORT))
                               /* Probably remote process death */
                  printf("*** error %din EnvRun at Send.\n",msgcode);
#if Debug
              else
                  printf(" (* Envmgrmain reply message sent. *)\n");
#endif
                         }
          } /* of message handling */

     }
   /* of main loop */

}


main()
{
   msg_return_t	retcode; 
   int 		CkInTry = 1;
   port_t	EMPort;
   kern_return_t GR;



#if Debug
   printf("\n(* Envmgr: main called. *)\n");
   printf("\n(* Port ... *)\n");
#endif

   env_init(&EMPort);   /* Initialize environment mgr */
#if Debug
   printf("\n(* Envmgr: Initialized. EMPort is %d *)\n",
		(int)EMPort);
#endif

   GR  = netname_check_in(name_server_port,"EnvMgrPort",PORT_NULL,EMPort);
   if (GR != KERN_SUCCESS)
	printf("netname_check_in of EnvMgrPort failed for reason %d\n",GR);
   env_run ();
   printf("(* !!!!! ENVMGR server exited - give it up !!!!! *)\n");
   exit(2);  /* causes nanny to not try to restart it */
}
