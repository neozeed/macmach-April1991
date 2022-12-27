
/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	envprocs.c,v $
 * Revision 1.1  90/02/19  21:47:24  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.3  89/05/09  21:56:17  mrt
 * 	Made env_set_port refuse to enter a null port.
 * 	Placed all the standard ports into the port table so that
 * 	they would not be deallocated if a client removed them.
 * 	[89/05/06            mrt]
 * 
 * Revision 1.2  89/05/05  18:30:55  mrt
 *	Cleanup for Mach 2.5
 *
 *    30-Jul-87	Mary Thompson
 *	Allocated tmp_ports as a static page to return the port
 *	list in.  The deallocation of the local tmp_ports was
 *	causing the ports to be deallocated as well
 *
 *    6-May-87	Mary Thompson
 *	Added code to unrestrict the service port
 *
 *   30 Mar 87  Mary Thompson
 * 	Fixed env_set_stlist and env_set_ptlist to deallocate
 *	input arrays of name, ports and values.
 *
 *   10 Feb 87	Mary Thompson
 *	Added calls to remove_dead_port in env_disconnect for the
 *	the port(s) that were deallocated.
 *
 *    4 Feb 87 Mary Thompson
 *	Added initial_conn to ask Service server for environment port.
 *	Added indirection to ports and remove_dead_port routine.
 */
/*
 * Abstract
 *	Environment Manager procedures for Mach
 *
 * Written by:
 *	Mary R. Thompson - Aug. 1986
 */

#include <mach.h>
#include <mach_init.h>
#include <servers/service.h>
#include "emdefs.h"
#include <stdio.h>

#define HASHTABLESIZE 32  /* must be a power of 2 */
#define HASHTABLEMASK  (HASHTABLESIZE -1)
#define MAXCONN  256
#define MAXPORTENT 1000  /* maximum number of ports that may
			    be entered in all contexts */

#define true 1   /* TRUE and FALSE from Mach are boolean_t */
#define false 0
#define Debug false

 typedef enum env_t { envstr, envport} env_t;

 typedef union env_var_t
    { env_str_val_t	str;
      int		pi;
    } env_var_t;
 
 typedef struct port_ent_t
    { port_t		pt;
      int		refcnt;
    } port_ent_t;

 typedef port_ent_t	port_table_t[MAXPORTENT];

 typedef struct hash_entry_t *pHashEntry;

 typedef struct hash_entry_t
   { env_name_t		name;  
     int		context;
     env_t		vartype;
     env_var_t		envval;
     pHashEntry		next;
   }  hash_entry_t;

 /* hash table head, one per context */

 typedef struct hash_head_t	*pHashHead;
 typedef struct hash_head_t
    { int	refcnt;		/* number of contexts using this */
      pHashEntry head[HASHTABLESIZE];
    } hash_head_t;

 /* list of hash table heads for all contexts */

 typedef struct  context_head_t
    { pHashHead		head;
      port_t		rwport;
      port_t		roport;
      boolean_t		inuse;
    } context_head_t;

 typedef context_head_t contexts_t[MAXCONN];

 static contexts_t	contexts;
 static pHashEntry	HashEntryPool;
 static port_table_t	pt_table;
 static port_array_t	tmp_ports;   /* used by env_list_ports */

 void del_port();   /* forward declaration */

#define delta (int)('A') - (int)('a')



/***********************************************************
 * procedure EnvInitialize (newport) 
 *
 * Abstract:
 *   Initializes Environment manager
 * Parameters:
 *   NewPort: Local environment port for first process
 *********************************************************/

void EnvInitialize ()
{
   int j;
   kern_return_t   retcode;

#if Debug
   printf("  (* envprocs:EnvInitialize entered. *)\n");
#endif
   HashEntryPool = (pHashEntry)(NULL);

   for ( j = 0; j < MAXCONN; j++ )
      contexts[j].inuse = FALSE;

   for ( j = 0; j < MAXPORTENT; j++)
     { pt_table[j].refcnt = 0;
       pt_table[j].pt = PORT_NULL;
     }

	/* put the standard ports in the port table so that they
      	   wont't be deallocated if some client removes them */
   j = add_port(environment_port);
   j = add_port(name_server_port);
   j = add_port(service_port);

   retcode = vm_allocate(task_self_,(vm_address_t *)&tmp_ports,
		MAXPORTENT*sizeof(port_t),TRUE);
   if (retcode != KERN_SUCCESS)
	printf("EnvMgr error %d from vm_allocate ports\n",retcode);

#if Debug
   printf("  (* envprocs:EnvInitialize exiting. *)\n");
#endif
}
 

/***********************************************************
 * function StrEqCase (S1,S2) 
 *
 * Abstract:
 *   Compare an Upper cased string (S1) with a mixed case string (S2)
 *   case INsensitively.
 *
 * Design:
 *   Since we don't modify our arguments, make them var.  This also
 *   allows our locals to be referenced more quickly.  It is actually
 *   a lot slower if they aren't vars.  -dwp
 ************************************************************/

boolean_t StrEqCase (S1,S2)
char  *S1, *S2;
{
   int  i;
   char tc;

   /* do-it-yourself strlen in case user hands in garbage */
   for ( i=0; i < env_name_size; i++ )
	if (S1[i] == '\0')
	    break;

   if (strlen(S2) != i)
      return(FALSE);

   if (i > 0)
      do {
           tc = S2[i];
           if (tc >= 'a')
              if (tc <= 'z')
                 tc = (char)((int)(tc) + delta);
           if (S1[i] != tc)
              return(FALSE);
           i = i - 1;
         } while (!(i <= 0));

   return(TRUE);
}  /* StrEqCase */


/***********************************************************
 * ConvUpper (S) 
 *
 * Abstract:
 *   Convert a string to all upper-case letters
 *
 ************************************************************/

void ConvUpper(S)
  char  *S;
  {
   int  i;
   char tc;

   for (i =0; S[i] != '\0';i++)
      {
        tc = S[i];
        if (tc >= 'a')
           if (tc <= 'z')
             { tc = (char)((int)(tc) + delta);
               S[i] = tc;
	     }
      } 

  }  /* ConvUpper */

/****************************************************************
 * function NewHashEntry  
 *
 * Abstract:
 *   Allocate a new HashEntry.
 * Returns:
 *   Pointer to a new HashEntry record.
 ****************************************************************/

pHashEntry NewHashEntry ()
{
   pHashEntry result;

   if (HashEntryPool == (pHashEntry)(NULL))
        result = (pHashEntry)malloc(sizeof(hash_entry_t));
   else {
           result = HashEntryPool;
           HashEntryPool = HashEntryPool->next;
        }
#if Debug
  printf("  (* Envmgr:NewHashEntry at mem x%x. *)\n",result);
#endif

   return(result);
}  /* NewHashEntry */

/****************************************************************
 * procedure FreeHashEntry (elem) 
 *
 * Abstract:
 *   Frees a HashEntry record.  Returns it to the pool of free HashEntry
 *   records.
 * Parameters:
 *   elem        pointer to the record to be freed.
 ****************************************************************/

FreeHashEntry (elem)
pHashEntry elem;
{
   elem->next = HashEntryPool;
   HashEntryPool = elem;
}  /* FreeHashEntry */

/****************************************************************
 * function Hash (s) 
 *
 * Abstract:
 *   Hash function for storing env var
 * Parameters:
 *   S:   name of the env var to hash
 * Returns:
 *   Hash value
 ****************************************************************/

int Hash (S)
  char  *S;
  {
   int i, n;

   n = 0;
   for ( i = 0; S[i] != '\0'; i++ )
      n = n + (int)(S[i]);

     /* Hash := n mod HASHTABLESIZE */

   return((n & HASHTABLEMASK));
  }  /* Hash */

/****************************************************************
 * function FindHashEntry (s,p) 
 *
 * Abstract:
 *   Finds hash entry
 * Parameters:
 *   S:   name of the env var to find
 *   P:   context number
 * Returns:
 *   Pointer to hash entry for name, or NULL if not found
 ****************************************************************/
pHashEntry FindHashEntry (_S,P)
  env_name_t  _S;
  int P;
  {
   env_name_t S;
   pHashEntry entry;
   int N;

   strcpy(S,_S);
   ConvUpper(S);
#if Debug
   printf(" envmgrprocs:FindHashEntry (%s,)\n",S);
#endif
/*   entry = contexts[P].LastHE;
   if (entry != (pHashEntry)(NULL))
      {
         if (StrEqCase(S,entry->name))
            {
               return(entry);
            }
      }
#if Debug
   else
     printf("  (* envmgrprocs:FindHashEntry NULL ProcessP[P].LastHE Hint. *)\n");
#endif
*/
   N = Hash(S);
#if Debug
   printf("  (* envmgrprocs:FindHashEntry, string hashes to %d. *)\n",N);
#endif
   entry = contexts[P].head->head[N]; /* head of list */
   while (entry != (pHashEntry)(NULL))
         {
#if Debug
   printf("  (* envmgrprocs:comparing to x%x  [%s]. *)\n",entry,entry->name);
#endif
            if (StrEqCase(S,entry->name))
               {
                  /* ProcessP[P].LastHE = entry;   Reload hint */
                  return(entry);
               }
            entry = entry->next;
         }
   return(entry);  /* NULL */
}


/****************************************************************
 * function CreateHashEntry (s,p) 
 *
 * Abstract:
 *   Creates internal env_var record.
 * Parameters:
 *   S:   name of the env_var to create
 *   P:   context number
 * Returns:
 *   Pointer to new hash entry for env var
 * Side Effects:
 *   New entry added to hash table for context P.
 ****************************************************************/

pHashEntry CreateHashEntry (_S,P)
  env_name_t  _S;
  int P;
 {
   env_name_t S;
   pHashEntry Entry;
   int N;

   strcpy(S,_S);
   ConvUpper(S);

   N = Hash(S);

   Entry = NewHashEntry();
   Entry->next = contexts[P].head->head[N];
   contexts[P].head->head[N] = Entry;
   return(Entry);
}

/***************************************************************
 * function DeleteHashEntry (s,p,vt) 
 *
 * Abstract:
 *   Deletes internal env var record.
 * Parameters:
 *   S:   name of the env var record to delete
 *   P:   context number
 *   vt;  type of variable to be deleted
 * Side Effects:
 *   Hash entry deleted, if entry is for a port 
 *   the refcnt in the port table is decremented and
 *   if it is 0, the port is deallocated.
 ***************************************************************/

int DeleteHashEntry (_S,P,vt)
  env_name_t  _S;
  int P;
  env_t vt;
{
   env_name_t S;
   pHashEntry entry, prev;
   int N;
   strcpy(S,_S);
   ConvUpper(S);

   N = Hash(S);
   entry = contexts[P].head->head[N]; /* head of list */
   if (entry == (pHashEntry)(NULL))
	return(ENV_VAR_NOT_FOUND);
   if (StrEqCase(S,entry->name))
        {  if (entry->vartype == vt)
              contexts[P].head->head[N] = entry->next;
	   else return(ENV_WRONG_VAR_TYPE);
        }
   else {
           do {
                prev = entry;
                entry = entry->next;
                if (entry == (pHashEntry)(NULL))
                   return(ENV_VAR_NOT_FOUND); /* entry not there */
              } while (!(StrEqCase(S,entry->name)));
	   if (entry->vartype != vt)
		return(ENV_WRONG_VAR_TYPE);
           prev->next = entry->next;
        }

     /* Entry found and removed from hash table chains */

   if (entry->vartype == envport)
	del_port(entry->envval.pi);
   FreeHashEntry(entry);
   return(ENV_SUCCESS);
}

/***************************************************************
 * procedure CopyHashTable (p) 
 *
 * Abstract:
 *   Copies all entries (and the hash table itself) for a  context from the
 *   hash table it was using to a new hash table.
 * Parameters:
 *   P   context whose hash table is to be copied.
 * Design:
 *   This copy is done when a set_env_var is about to occur on
 *   a context whose refcnt is greater than 1. This is a lazy
 *   evaluation of env_copy_conn.
 ***************************************************************/

CopyHashTable (P)
  int P;
  {
   int j;
   pHashEntry Entry, NewEntry;
   pHashEntry PrevEntry = (pHashEntry)(NULL);
   pHashHead OldHead;

#if Debug
   printf("  (* envmgrprocs:Copying hash table for process %d *)\n",P);
#endif
   /* ProcessP[P].LastHE = (pHashEntry)(NULL);  * would point to entry in old table */

   OldHead = contexts[P].head;
   OldHead->refcnt -=  1;

   contexts[P].head = (pHashHead)malloc(sizeof(hash_head_t));
   {

      contexts[P].head->refcnt = 1;
      for ( j = 0; j <= HASHTABLESIZE - 1; j++ )
         {
                      /* copy old variables */
            Entry = OldHead->head[j];
            contexts[P].head->head[j] = (pHashEntry)(NULL);
            while (Entry != (pHashEntry)(NULL))
                  {
                     NewEntry = NewHashEntry();
                     if (contexts[P].head->head[j] == (pHashEntry)(NULL))
                          contexts[P].head->head[j] = NewEntry;
                     else PrevEntry->next = NewEntry;
                     *NewEntry = *Entry;
                     NewEntry->context = P;
                     if (Entry->vartype == envport)
			pt_table[Entry->envval.pi].refcnt += 1;
                     NewEntry->next = (pHashEntry)(NULL);
                     PrevEntry = NewEntry;
                     Entry = Entry->next;
                  }
         }
   }
}

/****************************************************
 * int PortToContext(ServPort,restrict);
 *
 * Abstract:
 *	Finds the pointer to the context for this port 
 * Parameters:
 * 	ServPort	port call was received on
 *	restrict	set to TRUE if port is read-only
 *			false otherwise
 * Returns:
 *	index of context for this port
 ****************************************************/
int PortToContext(ServPort,restrict)
   port_t	ServPort;
   boolean_t	*restrict;
  {
    int	i;

    for (i=0; i < MAXCONN; i++)
     if (contexts[i].inuse )
      {  if (contexts[i].rwport == ServPort)
	  { *restrict = FALSE;
	    return(i);
	  }
        if (contexts[i].roport == ServPort)
          { *restrict = TRUE;
	    return(i);
	  }
      }
    printf("EnvMgr received message on unknown port: %d\n",
		(int)ServPort);
    return(MAXCONN);
  }  /* PortToContext  */


int add_port(new_port)
   port_t	new_port;
/*****************************************************
 * Abstract:
 *    Adds new_port to the pt_table and returns
 *    the new index
 * Returns;
 *    the port table index if successful
 *    -1 if the pt_table is full
 *****************************************************/
  {  int	i,empty;
  
    empty = -1;
    for (i = 0; i < MAXPORTENT; i++)
      { if (pt_table[i].pt == new_port)
	   goto found;
        if ((empty == -1) && (pt_table[i].refcnt == 0))
	   empty = i;
      }
    if (empty == -1)  /* no more room in table */
	return(-1);
    i = empty;
    pt_table[i].pt = new_port;
found:
    pt_table[i].refcnt += 1;
    return(i);
  }

void del_port(port_index)
   int		port_index;
/******************************************************
 * Abstract:
 *   Decrement the refcnt for port_index by one
 *   and deallocte the port if the refcnt is 0
 *****************************************************/
  {
     kern_return_t 	retcode;

     pt_table[port_index].refcnt -= 1;
     if (pt_table[port_index].refcnt == 0 )
      {
   	retcode = port_deallocate(task_self_,pt_table[port_index].pt);
        if (retcode == KERN_SUCCESS)
	    pt_table[port_index].pt = PORT_NULL;
      }
   }

boolean_t remove_dead_port(old_port)
    port_t	old_port;
/****************************************************
 *  Abstract;
 *	If old_port is found in the port table, set its
 *	value to PORT_NULL and return TRUE
 *	Otherwise return FALSE.
 ****************************************************/
   {
    int		i;
      
    for (i = 0; i < MAXPORTENT; i++)
  	if (pt_table[i].pt == old_port)
 	  { pt_table[i].pt = PORT_NULL;
  	    return (TRUE);
	  }
    return(FALSE);
   }

kern_return_t	env_set_string ( ServPort,env_name,env_val)
   port_t	ServPort;
   env_name_t	env_name;
   env_str_val_t	env_val;
/******************************************************
 *  Abstract:
 *	Define a value for a string environment variable
 *	in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_name	name of env variable to be set
 *	env_val		value of env_name
 *  Returns:
 *	ENV_SUCCESS
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 ******************************************************/
{
   int		cx;
   pHashEntry   entry;
   boolean_t	restrict;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (restrict)
 	return(ENV_READ_ONLY);

   if (contexts[cx].head ->refcnt > 1)
	CopyHashTable(cx);
  
   entry = FindHashEntry(env_name,cx);
   if (entry != NULL)
    { if (entry->vartype != envstr)
	return(ENV_WRONG_VAR_TYPE);
      strcpy(entry->envval.str,env_val);
    }
   else
    {  entry = CreateHashEntry(env_name,cx);
       strcpy(entry->name,env_name);
       entry->context = cx;
       entry->vartype = envstr;
       strcpy(entry->envval.str,env_val);
    }

   return(ENV_SUCCESS);
}

kern_return_t	env_get_string ( ServPort,env_name,env_val)
   port_t	ServPort;
   env_name_t	env_name;
   env_str_val_t env_val;
/******************************************************
 *  Abstract:
 *	Return the value of a string environment variable
 *	in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_name	name of env variable to be found
 *	env_val		returned pointing to value of env_name
 *  Returns:
 *	ENV_SUCCESS
 *	WrongEnvVarType	 env_name was found but was 
 *			 not of type env_str.
 *	EnvVariableNotFound  env_name not found.
 ******************************************************/
{
   int		cx;
   pHashEntry   entry;
   boolean_t	restrict;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);
   
   strcpy(env_val,"\0");   /* in case of error */
   entry = FindHashEntry(env_name,cx);
   if (entry == NULL)
      return(ENV_VAR_NOT_FOUND);

   if (entry->vartype != envstr)
     return(ENV_WRONG_VAR_TYPE);

   strcpy(env_val,entry->envval.str);
   return(ENV_SUCCESS);
}

kern_return_t	env_del_string ( ServPort,env_name)
   port_t	ServPort;
   env_name_t	env_name;
/******************************************************
 *  Abstract:
 *	Remove a string variable from the context specified
 *	by ServPort
 *  Parameters:
 * 	ServPort	port identifying the context to use
 *	env_name	name of env variable to be deleted
 *  Returns:
 *	ENV_SUCCESS
 *	WrongEnvVarType	 env_name was found but was 
 *			 not of type env_str.
 *	EnvVariableNotFound  env_name not found.
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 ******************************************************/
{
   int		cx;
   boolean_t	restrict;
   int		retcode;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (restrict)
 	return(ENV_READ_ONLY);

   if (contexts[cx].head ->refcnt > 1)
	CopyHashTable(cx);
   
   retcode = DeleteHashEntry(env_name,cx,envstr);

   return((kern_return_t)retcode);
}

kern_return_t	env_set_port ( ServPort,env_name,env_val)
   port_t	ServPort;
   env_name_t	env_name;
   port_t	env_val;
/******************************************************
 *  Abstract:
 *	Define a value for a port environment variable
 *	in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_name	name of env variable to be set
 *	env_val		value of env_name
 *  Returns:
 *	ENV_SUCCESS
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 *	ENV_PORT_NULL 	Attempting to enter a null port.
 ******************************************************/
{
   int		cx;
   pHashEntry   entry;
   boolean_t	restrict;
   int		pi;

   if (env_val == PORT_NULL)
	return(ENV_PORT_NULL);

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (restrict)
 	return(ENV_READ_ONLY);

   if (contexts[cx].head ->refcnt > 1)
	CopyHashTable(cx);
   
   entry = FindHashEntry(env_name,cx);
   if (entry != NULL)
    { if (entry->vartype != envport)
	return(ENV_WRONG_VAR_TYPE);
      if (pt_table[entry->envval.pi].pt == env_val)
	return(ENV_SUCCESS);

      pi = add_port(env_val);
      if (pi != -1)
        { del_port(entry->envval.pi);
	  entry->envval.pi = pi;
	  return(ENV_SUCCESS);
	}
      return(ENV_PORT_TABLE_FULL);
    }
   else
    {  pi = add_port(env_val);
       if (pi == -1)
	  return(ENV_PORT_TABLE_FULL);
       entry = CreateHashEntry(env_name,cx);
       strcpy(entry->name,env_name);
       entry->context = cx;
       entry->vartype = envport;
       entry->envval.pi = pi;
    }

   return(ENV_SUCCESS);
}

kern_return_t	env_get_port ( ServPort,env_name,env_val)
   port_t	ServPort;
   env_name_t	env_name;
   port_t	*env_val;
/******************************************************
 *  Abstract:
 *	Return the value of a port environment variable
 *	in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_name	name of env variable to be found
 *	env_val		returned pointing to value of env_name
 *  Returns:
 *	ENV_SUCCESS
 *	WrongEnvVarType	 env_name was found but was 
 *			 not of type port_t
 *	EnvVariableNotFound  env_name not found.
 ******************************************************/
{
   int		cx;
   pHashEntry   entry;
   boolean_t	restrict;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);
   
   *env_val = PORT_NULL;   /* in case of error */
   entry = FindHashEntry(env_name,cx);
   if (entry == NULL)
      return(ENV_VAR_NOT_FOUND);

   if (entry->vartype != envport)
     return(ENV_WRONG_VAR_TYPE);

   if (pt_table[entry->envval.pi].pt == PORT_NULL)
     {  
#if Debug
	printf("removing obsolete port from hash table\n");
#endif
	DeleteHashEntry(env_name,cx,envport);
	return(ENV_VAR_NOT_FOUND);
     }
   *env_val = pt_table[entry->envval.pi].pt; 
   return(ENV_SUCCESS);
}

kern_return_t	env_del_port ( ServPort,env_name)
port_t	ServPort;
env_name_t	env_name;
/******************************************************
 *  Abstract:
 *	Remove a port variable from the context specified
 *	by ServPort
 *  Parameters:
 * 	ServPort	port identifying the context to use
 *	env_name	name of env variable to be deleted
 *  Returns:
 *	ENV_SUCCESS
 *	WrongEnvVarType	 env_name was found but was 
 *			 not of type port_t
 *	EnvVariableNotFound  env_name not found.
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 ******************************************************/
{
   int		cx;
   boolean_t	restrict;
   int		retcode;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (restrict)
 	return(ENV_READ_ONLY);
   
   if (contexts[cx].head ->refcnt > 1)
	CopyHashTable(cx);
 	
   retcode = DeleteHashEntry(env_name,cx,envport);

   return((kern_return_t)retcode);
}

kern_return_t	env_list_strings ( ServPort,env_names,env_names_Cnt,env_string_vals,env_string_vals_Cnt)
   port_t	ServPort;
   env_name_list *env_names;
   int		*env_names_Cnt;
   env_str_list	*env_string_vals;
   int		*env_string_vals_Cnt;
/******************************************************
 *  Abstract:
 *	Return the names and values of all the String
 *	variables in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_names	returned pointing to a list of env variable names
 *	env_names_Cnt	number of names returned
 *	env_string_val	values of the names returned
 *	env_string_vals_Cnt	same as env_names_Cnt
 *  Returns;
 *	ENV_SUCCESS
 *
 ******************************************************/
{
   int		cx;
   pHashEntry   entry;
   boolean_t	restrict;
   int		hi,vc;
   kern_return_t retcode;
   env_name_list tmp_env_names;  /* pointer to names */
   env_str_list  tmp_env_strings; /* pointer to strings */

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   retcode = ENV_SUCCESS;
   vc = 0;  /* count the number of string vars */
   for (hi=0; hi < HASHTABLESIZE; hi++)
     {  entry = contexts[cx].head->head[hi];
	while (entry != NULL) 
	  { if (entry->vartype == envstr)
	      vc += 1;
	    entry = entry->next;
	  }
     }  /* end counting loop */
   
   tmp_env_names = NULL;
   tmp_env_strings = NULL;
   if (vc == 0)
	goto ex1;
   retcode = vm_allocate(task_self_,(vm_address_t *)&tmp_env_names,
		vc*sizeof(env_name_t),TRUE);
   if (retcode != KERN_SUCCESS)
	{ printf("EnvMgr error %d from vm_allocate names\n",retcode);
	  vc = 0;
	  goto ex1;
	}

   retcode = vm_allocate(task_self_,(vm_address_t *)&tmp_env_strings,
		vc*sizeof(env_str_val_t),TRUE);
   if (retcode != KERN_SUCCESS)
	{ printf("EnvMgr error %d from vm_allocate strings\n",retcode);
	  vc = 0;
	  goto ex1;
	}
   vc = 0;

   for (hi=0; hi < HASHTABLESIZE; hi++)
     {  entry = contexts[cx].head->head[hi];
	while (entry != NULL) 
	  { if (entry->vartype == envstr)
	    { strcpy(tmp_env_names[vc],entry->name);
	      strcpy(tmp_env_strings[vc],entry->envval.str);
	      vc += 1;
	    }
	    entry = entry->next;
	  }
     }  /* loop over all hash table entries */
ex1: *env_names = tmp_env_names;
   *env_string_vals = tmp_env_strings;
   *env_names_Cnt = vc;
   *env_string_vals_Cnt = vc;
   return(retcode);
}

kern_return_t	env_list_ports ( ServPort,env_names,env_names_Cnt,env_port_vals,env_port_vals_Cnt)
   port_t	ServPort;
   env_name_list *env_names;
   int		*env_names_Cnt;
   port_array_t	*env_port_vals;
   int		*env_port_vals_Cnt;
/******************************************************
 *  Abstract:
 *	Return the names and values of all the port
 *	variables in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_names	returned pointing to a list of env variable names
 *	env_names_Cnt	number of names returned
 *	env_port_vals	values of the names returned
 *	env_port_vals_Cnt	same as env_names_Cnt
 *  Returns;
 *	ENV_SUCCESS
 *
 *
 ******************************************************/
{
   int		cx;
   pHashEntry   entry,nextentry;
   boolean_t	restrict;
   int		hi,vc;
   kern_return_t retcode;
   env_name_list tmp_env_names;  /* pointer to names */

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);
  
   retcode = ENV_SUCCESS;
   vc = 0;  /* count the number of port vars */
   for (hi=0; hi < HASHTABLESIZE; hi++)
     {  entry = contexts[cx].head->head[hi];
	while (entry != NULL) 
	  { if (entry->vartype == envport)
	     { if (pt_table[entry->envval.pi].pt == PORT_NULL)
		{
#if Debug
		   printf("removing obsolete port from hash table\n");
#endif
 		   nextentry = entry->next;  /* get it before we delete entry */
 		   retcode=DeleteHashEntry(entry->name,cx,envport);
		   entry = nextentry;
		   continue;  /* skip the updating of entry */
		 }
	      else vc += 1;
	     }
	    entry = entry->next;
	  }
     }  /* end counting loop */

   tmp_env_names = NULL;
   if (vc == 0)
   {	*env_port_vals = NULL;
	goto ex2;
   }
   retcode = vm_allocate(task_self_,(vm_address_t *)&tmp_env_names,
		vc*sizeof(env_name_t),TRUE);
   if (retcode != KERN_SUCCESS)
	{ printf("EnvMgr error %d from vm_allocate names\n",retcode);
	  vc = 0;
	  goto ex2;
	}

   vc = 0;
   for (hi=0; hi < HASHTABLESIZE; hi++)
     {  entry = contexts[cx].head->head[hi];
	while (entry != NULL) 
	  { if (entry->vartype == envport)
	    { strcpy(tmp_env_names[vc],entry->name);
	      tmp_ports[vc] = pt_table[entry->envval.pi].pt;
	      vc += 1;
	    }
	    entry = entry->next;
	  }
     }  /* loop over all hash table entries */
   *env_port_vals = tmp_ports;

ex2: *env_names = tmp_env_names;
   *env_names_Cnt = vc;
   *env_port_vals_Cnt = vc;
   return (retcode);
}

kern_return_t	env_set_stlist ( ServPort,env_names,env_names_Cnt,env_string_vals,env_string_vals_Cnt)
   port_t	ServPort;
   env_name_list env_names;
   int		env_names_Cnt;
   env_str_list	env_string_vals;
   int		env_string_vals_Cnt;
/******************************************************
 *  Abstract:
 *	Enters a set of string environment
 *	variables in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_names	a list of env variable names
 *	env_names_Cnt	number of names
 *	env_string_val	values of the names
 *	env_string_vals_Cnt	same as env_names_Cnt
 *  Returns;
 *	ENV_SUCCESS
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 *  Side effects;
 *	vm_deallocates the list of name and string values	
 ******************************************************/
{
    int	  vc;
    kern_return_t  retcode,kretcode;

    for (vc = 0; vc < env_names_Cnt; vc++)
      {  retcode = env_set_string(ServPort,env_names[vc],env_string_vals[vc]);
         if (retcode != ENV_SUCCESS)
		break;
      }

    kretcode = vm_deallocate(task_self(),(vm_address_t)env_names,
				env_names_Cnt*sizeof(env_name_t));
    kretcode = vm_deallocate(task_self(),(vm_address_t)env_string_vals,
				env_string_vals_Cnt*sizeof(env_str_val_t));
    return(retcode);
}

kern_return_t	env_set_ptlist ( ServPort,env_names,env_names_Cnt,env_port_vals,env_port_vals_Cnt)
   port_t	ServPort;
   env_name_list env_names;
   int		env_names_Cnt;
   port_array_t	env_port_vals;
   int		env_port_vals_Cnt;
/******************************************************
 *  Abstract:
 *	Return the names and values of all the port
 *	variables in the context specified by ServPort
 *  Parameters:
 *	ServPort	port identifying the context to use
 *	env_names	a list of env variable names
 *	env_names_Cnt	number of names
 *	env_port_vals	values of the names 
 *	env_port_vals_Cnt	same as env_names_Cnt
 *  Returns;
 *	ENV_SUCCESS
 *	ENV_READ_ONLY	ServPort only allows reading of
 *			the environment.
 ******************************************************/
{
    int	  vc;
    kern_return_t  retcode,kretcode;

    for (vc = 0; vc < env_names_Cnt; vc++)
      {  retcode = env_set_port(ServPort,env_names[vc],env_port_vals[vc]);
         if (retcode != ENV_SUCCESS)
		break;
      }
    kretcode = vm_deallocate(task_self(),(vm_address_t)env_names,
				env_names_Cnt*sizeof(env_name_t));
    kretcode = vm_deallocate(task_self(),(vm_address_t)env_port_vals,
				env_port_vals_Cnt*sizeof(port_t));
    return(retcode);
}


kern_return_t	env_copy_conn ( ServPort,new_env_port)
   port_t	ServPort;
   port_t	*new_env_port;
/******************************************************
 * Abstract:
 *	Copies the context specified by ServPort and
 * 	allocates a  new port which will be used to
 * 	reference the new context.
 *  Parameters:
 *	ServPort	port identifying the context to copy
 *	new_env_port	returns a port to the new copy of the context
 *  Returns:
 *	ENV_SUCCESS
 *  Design:
 *	The context is marked as having been copied, but
 *	the acutal copying is done only when a variable
 *	in one of the two logical copies is changed.
 *	This is important since the shell will normally
 *	pass a copy of its environment to any task it
 *	starts and normaly the child task will never
 *	modify its environment.
 ******************************************************/
{
   int		cx,ncx;
   boolean_t	restrict;
   kern_return_t retcode;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   for (ncx = 0; (ncx < MAXCONN)  && (contexts[ncx].inuse); ncx++);
   if (ncx == MAXCONN)
      return(ENV_NO_MORE_CONN);

   retcode = port_allocate(task_self_,new_env_port);
   if (retcode != KERN_SUCCESS)
	return(retcode);
   (void) port_unrestrict(task_self(),*new_env_port);

   contexts[cx].head->refcnt += 1;

   contexts[ncx].head = contexts[cx].head;
   contexts[ncx].rwport = *new_env_port;
   contexts[ncx].roport = PORT_NULL;
   contexts[ncx].inuse = TRUE;

   return(ENV_SUCCESS);
}

kern_return_t	env_new_conn ( ServPort,new_env_port)
   port_t	ServPort;
   port_t	*new_env_port;
/******************************************************
 * Abstract:
 *	Creates a new empty context
 * 	allocates a  new port which will be used to
 * 	reference the new context.
 *  Parameters:
 *	ServPort	not used
 *	new_env_port	returns a port to the new copy of the context
 *  Returns:
 *	ENV_SUCCESS
 ******************************************************/
{
   int		i,ncx;
   pHashHead    HashHead;
   kern_return_t retcode;

   for (ncx = 0; (ncx < MAXCONN)  && (contexts[ncx].inuse); ncx++);
   if (ncx == MAXCONN)
      return(ENV_NO_MORE_CONN);

   retcode = port_allocate(task_self_,new_env_port);
   if (retcode != KERN_SUCCESS)
	return(retcode);
   (void) port_unrestrict(task_self(),*new_env_port);

   /* create new, empty hash table */

   HashHead =  (pHashHead)malloc(sizeof(hash_head_t));
   HashHead->refcnt = 1;
   for (i=0; i< HASHTABLESIZE; i++)
	HashHead->head[i] = NULL;

   contexts[ncx].head = HashHead;
   contexts[ncx].rwport = *new_env_port;
   contexts[ncx].roport = PORT_NULL;
   contexts[ncx].inuse = TRUE;

   return(ENV_SUCCESS);

}

kern_return_t	env_restrict_conn ( ServPort,new_env_port)
   port_t	ServPort;
   port_t	*new_env_port;
/******************************************************
 * Abstract:
 * 	Allocates a  new port through which the current
 *	environment variables may be read but not modified.
 *	A read-only port allows one task to allow a child
 *	task to read, but not modify its environment.
 *  Parameters:
 *	ServPort	port identifying the current context
 *	new_env_port	returns a read only port to the the context
 *  Returns:
 *	ENV_SUCCESS
 *  Design:
 *	There is only one read/write and zero or one read-only port
 *	to each context. If a read only port has already
 *	been created for this context it is returned.
 ******************************************************/
{
   int		cx;
   boolean_t	restrict;
   kern_return_t retcode;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (contexts[cx].roport != PORT_NULL)
     { *new_env_port = contexts[cx].roport;
        return(ENV_SUCCESS);
     }

   retcode = port_allocate(task_self_,new_env_port);
   if (retcode != KERN_SUCCESS)
	return(retcode);
   (void) port_unrestrict(task_self(),*new_env_port);

   contexts[cx].roport = *new_env_port;
   return(ENV_SUCCESS);
}

void	initial_conn ( KnownEnvPort)
   port_t	*KnownEnvPort;
/******************************************************
 * Abstract:
 *	Asks the Service server for the environment_port
 *	Creates a new empty context and associates with
 * 	this port. If Service server won't give us the
 *	enviroment_port, it creates a new port and context
 *  Parameters:
 *	KnownEnvPort	returns a port to the new copy of the context
 *  Returns:
 *	ENV_SUCCESS
 ******************************************************/
{
   int		i;
   pHashHead    HashHead;
   kern_return_t retcode;
   port_t		new_environment_port;


   /* First, ask the service server if there is already an established port. */
   init_service(PORT_NULL);
   if ((service_checkin(service_port, environment_port, &new_environment_port) == KERN_SUCCESS) &&
	    (environment_port == new_environment_port))
   {
	*KnownEnvPort = environment_port;	
	(void) port_unrestrict(task_self(),*KnownEnvPort);

	/* create new, empty hash table */
	HashHead =  (pHashHead)malloc(sizeof(hash_head_t));
	HashHead->refcnt = 1;
	for (i=0; i< HASHTABLESIZE; i++)
	    HashHead->head[i] = NULL;

	contexts[0].head = HashHead;
	contexts[0].rwport = environment_port;
	contexts[0].roport = PORT_NULL;
	contexts[0].inuse = TRUE;
   }
   else /* may be the second instantation of EnvMgr */
	retcode = env_new_conn(PORT_NULL,KnownEnvPort);
}

kern_return_t	env_disconnect ( ServPort)
   port_t	ServPort;
/******************************************************
 *  Abstract:
 *	Destroys the context for ServPort
 *  Parameters:
 *	ServPort	specified the context to be destroyed.
 *  Returns:
 *	ENV_SUCCESS
 *      ENV_READ_ONLY	ServPort only allowed reading of the context
 *  SideEffects:
 *	Deallocates both the read-only and read/write ports
 *	for this context.
 ******************************************************/
{
   int		i,cx;
   boolean_t	restrict,ok;
   kern_return_t retcode;
   pHashEntry	entry,tempentry;
   pHashHead    Head;

   cx = PortToContext(ServPort,&restrict);
   if (cx == MAXCONN)
    	return(ENV_UNKNOWN_PORT);

   if (restrict)
     { if (contexts[cx].roport != ServPort)
	  { printf ("impossible condition in env_disconnect \n");
	    return(ENV_READ_ONLY);
	  }
       retcode = port_deallocate(task_self_,ServPort);
       if (retcode != KERN_SUCCESS)
           printf("EnvMgr:EnvDisconnect got error %d deallocting port %d \n",
		(int)retcode,(int)contexts[cx].roport);
       ok=remove_dead_port(contexts[cx].roport);
       contexts[cx].roport = PORT_NULL;
       return(ENV_SUCCESS);
     }

   Head = contexts[cx].head;
   Head->refcnt -= 1;
   if (Head->refcnt == 0 )
     {     /* dispose of the hash table */
	for (i=0; i > HASHTABLESIZE; i++)
	  {  entry = Head->head[i];
	     while (entry != NULL)
	      {	if (entry->vartype == envport)
		   del_port(entry->envval.pi);
		tempentry = entry->next;
		FreeHashEntry(entry);
		entry = tempentry;
	      }
	  }
        free((char *)Head);
     }  /* end dispose of hash table */

   contexts[cx].inuse = FALSE;
#if Debug
   printf("deallocating port %d\n",(int)contexts[cx].rwport);
#endif
   retcode = port_deallocate(task_self_,contexts[cx].rwport);
   if (retcode != KERN_SUCCESS)
      printf("EnvMgr:EnvDisconnect got error %d deallocting port %d \n",
		(int)retcode,(int)contexts[cx].rwport);
   ok=remove_dead_port(contexts[cx].rwport);
   contexts[cx].rwport = PORT_NULL;

   if (contexts[cx].roport != PORT_NULL)
    {  retcode = port_deallocate(task_self_,contexts[cx].roport);
       if (retcode != KERN_SUCCESS)
           printf("EnvMgr:EnvDisconnect got error %d deallocting port %d \n",
		(int)retcode,(int)contexts[cx].roport);
       ok=remove_dead_port(contexts[cx].roport);
       contexts[cx].roport = PORT_NULL;
    }
   contexts[cx].head = NULL;
   return(ENV_SUCCESS);
}

