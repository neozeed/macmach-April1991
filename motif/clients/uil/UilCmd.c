#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilCmd.c	3.6 91/01/10";
#endif /* lint */
#endif /* REV_INFO */

/******************************************************************************
*******************************************************************************
*
*  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
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
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.  Unpublished - all
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
*  (c) Copyright 1989, 1990, 1991 Open Software Foundation, Inc.
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
**      Command line interpreter for the
**
**--
**/

/*
**
**  INCLUDE FILES
**
**/


#include "UilDef.h"
#include "UilCmdDef.h"


/*
**
**  GLOBAL DECLARATIONS
**
**/

externaldef(uil_comp_glbl) cmd_command_line_type Uil_cmd_z_command;



/*
**++
**  FUNCTIONAL DESCRIPTION:
**
**      This procedure parses the command line and places the
**	results of the parse in the global structure "Uil_cmd_z_command".
**
**  FORMAL PARAMETERS:
**
**      l_arg_count:	number of command arguments
**	rac_arg_value:	array of pointers to null terminated character strings
**			each of which is one of the command line arguments
**
**  IMPLICIT INPUTS:
**
**      none
**
**  IMPLICIT OUTPUTS:
**
**      Uil_cmd_z_command:	respository for command line info
**
**  FUNCTION VALUE:
**
**      void
**
**  SIDE EFFECTS:
**
**      Uil_cmd_z_command is set
**
**--
**/
void	cmd_decode_command_line( l_arg_count, rac_arg_value )
int 	l_arg_count;
char 	*rac_arg_value[ ];

{
    static char	    *include_list	[Uil_k_max_include_dir_count];
    int	i;

    Uil_cmd_z_command.ac_source_file = NULL;
    Uil_cmd_z_command.ac_resource_file = NULL;
    Uil_cmd_z_command.ac_listing_file = NULL;
    Uil_cmd_z_command.include_dir_count = 0;
    Uil_cmd_z_command.ac_include_dir = (char **)include_list;
    Uil_cmd_z_command.v_resource_file = TRUE;
    Uil_cmd_z_command.v_listing_file = FALSE;
    Uil_cmd_z_command.v_show_machine_code = FALSE;
    Uil_cmd_z_command.v_report_info_msg = TRUE;
    Uil_cmd_z_command.v_report_warn_msg = TRUE;
    Uil_cmd_z_command.v_parse_tree = FALSE;
    Uil_cmd_z_command.status_update_delay = 0;
    Uil_cmd_z_command.message_cb = NULL;
    Uil_cmd_z_command.status_cb = NULL;


#if debug_version
    uil_v_dump_tokens = FALSE;
    uil_v_dump_symbols = FALSE;
#endif

    /* traverse the options on the command line */

    for (i = 1;  i < l_arg_count;  i++)
    {
	/* check for an output file  */

	if ( strcmp("-o", rac_arg_value[ i ]) == 0 )
	{
	    /* the next argument is the output file name  */

	    /* check next field is not an option */

            if (((i+1) >= l_arg_count) ||
                ( '-' == rac_arg_value[ i+1 ][ 0 ] ))
	    {
		diag_issue_diagnostic
			( d_miss_opt_arg, 
			  diag_k_no_source, diag_k_no_column,
			  rac_arg_value[ i ],
			  "output file"
			);
		continue;
	    }

	    if (Uil_cmd_z_command.ac_resource_file == NULL)
	        Uil_cmd_z_command.ac_resource_file = rac_arg_value[ i+1 ];
	    else
		diag_issue_diagnostic
		    ( d_dupl_opt, 
		      diag_k_no_source, diag_k_no_column,
		      rac_arg_value[ i ]
		    );
	    i = i + 1;
	}
	    
	/* check for an listing file */

	else if ( strcmp("-v", rac_arg_value[ i ]) == 0 )
	{
	    /* the next argument is the listing file name  */

	    /* check next field is not an option */

            if (((i+1) >= l_arg_count) ||
                ( '-' == rac_arg_value[ i+1 ][ 0 ] ))
	    {
		diag_issue_diagnostic
			( d_miss_opt_arg, 
			  diag_k_no_source, diag_k_no_column,
			  rac_arg_value[ i ],
			  "listing file"
			);
		continue;
	    }

	    if (!Uil_cmd_z_command.v_listing_file)
	    {
	        Uil_cmd_z_command.v_listing_file = TRUE;
	        Uil_cmd_z_command.ac_listing_file = rac_arg_value[ i+1 ];
	    }
	    else
		diag_issue_diagnostic
		    ( d_dupl_opt, 
		      diag_k_no_source, diag_k_no_column,
		      rac_arg_value[ i ]
		    );
	    i = i + 1;
	}
	    
	/* check for the machine code option */

	else if ( strcmp("-m", rac_arg_value[ i ]) == 0 )
	{
	        Uil_cmd_z_command.v_show_machine_code = TRUE;
	}
	    
	/* check if warnings are to be supressed */

	else if ( strcmp("-w", rac_arg_value[ i ]) == 0 )
	{
	        Uil_cmd_z_command.v_report_info_msg = FALSE;
	        Uil_cmd_z_command.v_report_warn_msg = FALSE;
	}


	    
	/* check for an unexpected option */

	else if ( '-' == rac_arg_value[ i ][ 0 ] )
	{

	/* check for an include directory */

	    if ( 'I' == rac_arg_value[ i ][ 1 ] )
	    {
		if (Uil_cmd_z_command.include_dir_count < Uil_k_max_include_dir_count)
		    
		    include_list[Uil_cmd_z_command.include_dir_count++] = 
			& rac_arg_value[i] [2];
		else
		    diag_issue_diagnostic
		        ( d_too_many_dirs, 
		          diag_k_no_source, diag_k_no_column,
		          rac_arg_value[ i ], Uil_k_max_include_dir_count
		        );
	    } else
	    {
		diag_issue_diagnostic
		    ( d_unknown_opt, 
		      diag_k_no_source, diag_k_no_column,
		      rac_arg_value[ i ]
		    );
	    }
	}

	/* assume it is a UIL source file specification
	 * validation of the file spec is done when file is opened */

	else
	{
	    if (Uil_cmd_z_command.ac_source_file == NULL)
	       Uil_cmd_z_command.ac_source_file = rac_arg_value[ i ];
	    else
		diag_issue_diagnostic
		    ( d_add_source, 
		      diag_k_no_source, diag_k_no_column,
		      rac_arg_value[ i ]
		    );
	}
    }

    /*
    **	Check for a source file - otherwise issue a diagnostic.
    */

    if (Uil_cmd_z_command.ac_source_file == NULL)
	diag_issue_diagnostic
	    ( d_no_source, diag_k_no_source, diag_k_no_column );

    if (Uil_cmd_z_command.ac_resource_file == NULL)
	{
        Uil_cmd_z_command.ac_resource_file = XtMalloc (strlen ("a.uid") + 1);
        strcpy (Uil_cmd_z_command.ac_resource_file,"a.uid");
        }

    /*
    **	Machine code listing only makes sense if listing is set.
    */
    
    Uil_cmd_z_command.v_show_machine_code =
        ( Uil_cmd_z_command.v_listing_file & 
	  Uil_cmd_z_command.v_show_machine_code);
}



