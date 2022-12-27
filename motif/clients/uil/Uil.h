#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)Uil.h	3.6 90/07/20";
#endif /* lint */
#endif /* REV_INFO */

/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
*  (c) Copyright 1989, 1990, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
*  ALL RIGHTS RESERVED
*  
*  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
*  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
*  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
*  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
*  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
*  SOFTWARE IS HEREBY TRANSFERRED.
*  
*  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
*  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
*  FOUNDATION, INC. OR ITS THIRD PARTY SUPPLIERS  
*  
*  	OPEN SOFTWARE FOUNDATION, INC. AND ITS THIRD PARTY SUPPLIERS,
*  ASSUME NO RESPONSIBILITY FOR THE USE OR INABILITY TO USE ANY OF ITS
*  SOFTWARE .   OSF SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
*  KIND, AND OSF EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES, INCLUDING
*  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE.
*  
*  Notice:  Notwithstanding any other lease or license that may pertain to,
*  or accompany the delivery of, this computer software, the rights of the
*  Government regarding its use, reproduction and disclosure are as set
*  forth in Section 52.227-19 of the FARS Computer Software-Restricted
*  Rights clause.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.  Unpublished - all
*  rights reserved under the Copyright laws of the United States.
*  
*  RESTRICTED RIGHTS NOTICE:  Use, duplication, or disclosure by the
*  Government is subject to the restrictions as set forth in subparagraph
*  (c)(1)(ii) of the Rights in Technical Data and Computer Software clause
*  at DFARS 52.227-7013.
*  
*  Open Software Foundation, Inc.
*  11 Cambridge Center
*  Cambridge, MA   02142
*  (617)621-8700
*  
*  RESTRICTED RIGHTS LEGEND:  This computer software is submitted with
*  "restricted rights."  Use, duplication or disclosure is subject to the
*  restrictions as set forth in NASA FAR SUP 18-52.227-79 (April 1985)
*  "Commercial Computer Software- Restricted Rights (April 1985)."  Open
*  Software Foundation, Inc., 11 Cambridge Center, Cambridge, MA  02142.  If
*  the contract contains the Clause at 18-52.227-74 "Rights in Data General"
*  then the "Alternate III" clause applies.
*  
*  (c) Copyright 1989, 1990, Open Software Foundation, Inc.
*  ALL RIGHTS RESERVED 
*  
*  
* Open Software Foundation is a trademark of The Open Software Foundation, Inc.
* OSF is a trademark of Open Software Foundation, Inc.
* OSF/Motif is a trademark of Open Software Foundation, Inc.
* Motif is a trademark of Open Software Foundation, Inc.
* DEC is a registered trademark of Digital Equipment Corporation
* DIGITAL is a registered trademark of Digital Equipment Corporation
* X Window System is a trademark of the Massachusetts Institute of Technology
*
*******************************************************************************
******************************************************************************/

/*
**++
**  FACILITY:
**
**      User Interface Language Compiler (UIL)
**
**  ABSTRACT:
**
**      This include file defines the set of definitions for use with
**	UIL compiler callable interface.
**
**--
**/

#ifndef Uil_h
#define Uil_h

/*
**
**  INCLUDE FILES
**
**/



/*
**
**  Definition of Compiler Severity Statuses
**
*/

typedef unsigned int	Uil_status_type;
#define Uil_k_min_status	0
#define Uil_k_success_status	0
#define Uil_k_info_status	1
#define Uil_k_warning_status	2
#define Uil_k_error_status	3
#define Uil_k_severe_status	4
#define Uil_k_max_status	4

/*
**
*/

typedef char (*string_array)[];
#define CEIL(a,b) ((a) < (b) ? (a) : (b))


/*
**  Uil_command_type -- Input which describes how/what to compile.
*/

typedef struct  _Uil_command_type
{
    char	    *source_file;	    /* single source to compile	    */
    char	    *resource_file;	    /* name of output file	    */
    char	    *listing_file;	    /* name of listing file	    */
    unsigned int    include_dir_count;	    /* number of directories in	    */
					    /* include_dir array	    */
    char	    **include_dir;	   /* directory to search for	    */
					    /* includes files		    */
    unsigned	    listing_file_flag: 1;   /* produce a listing	    */
    unsigned	    resource_file_flag: 1;  /* generate UID output	    */
    unsigned	    machine_code_flag : 1;  /* generate machine code	    */
    unsigned	    report_info_msg_flag: 1;/* report info messages	    */
    unsigned	    report_warn_msg_flag: 1;/* report warnings		    */
    unsigned	    parse_tree_flag: 1;	    /* generate parse tree	    */
    unsigned        issue_summary: 1;       /* issue diagnostics summary    */
    unsigned int    status_update_delay;    /* Number of times a status	    */
		    			    /* point is passed before	    */
					    /* calling statusCB routine	    */
					    /* 0 means called every time    */
} Uil_command_type;



/*
** Uil_compile_desc_type -- Output information about the compilation including
** the compiler_version, data_structure_version, parse tree, and error counts.
*/

typedef struct _Uil_comp_desc
{
    unsigned int	compiler_version;   /* version number of Compiler */
    unsigned int	data_version;	    /* version number of structures */
    char		*parse_tree_root;   /* parse tree output */
    unsigned int	message_count[Uil_k_max_status+1];    
					    /* array of severity counts */
} Uil_compile_desc_type;    




/*
**  Uil_continue_type -- A value returned from a Uil callback routine which
**  allows the application to specify whether to terminate or continue the
**  compilation.
*/

typedef unsigned int	Uil_continue_type;
#define Uil_k_terminate		0
#define Uil_k_continue		1



/*
**
**  Entry Points
**
*/

#ifndef _NO_PROTO
#define _ARGUMENTS(arglist) arglist
#else
#define _ARGUMENTS(arglist) ()
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern Uil_status_type Uil
		    _ARGUMENTS((
			Uil_command_type *command_desc ,
			Uil_compile_desc_type *compile_desc ,
			Uil_continue_type (*message_cb )(),
			char *message_data ,
			Uil_continue_type (*status_cb )(),
			char *status_data ));


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#undef _ARGUMENTS


#endif /* Uil_h */
/* DON'T ADD STUFF AFTER THIS #endif */
