#ifdef REV_INFO
#ifndef lint
static char SCCSID[] = "OSF/Motif: @(#)UilSymGl.h	3.1 90/03/28";
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
**      This include file contains external declarations of all
**	global data defining the language accepted by the Uil compiler.
**	This is exactly the global data generated by WML, plus any
**	invariant matching definitions.
**
**--
**/

#ifndef UilSymGl_h
#define UilSymGl_h

/*
 * Defined in files included in UilData.c
 */
externalref unsigned char		*constraint_tab;
externalref key_keytable_entry_type	*key_table;
externalref int				key_k_keyword_max_length;
externalref int				key_k_keyword_count;
externalref key_keytable_entry_type	*key_table_case_ins;
externalref unsigned char		**allowed_argument_table;
externalref unsigned char		*argument_type_table;
externalref char			**charset_xmstring_names_table;
externalref unsigned char		*charset_writing_direction_table;
externalref unsigned char		*charset_parsing_direction_table;
externalref unsigned char		*charset_character_size_table;
externalref char			**charset_lang_names_table;
externalref unsigned short int		*charset_lang_codes_table;
externalref unsigned short int		charset_lang_table_max;
externalref unsigned char		**allowed_control_table;
externalref UilEnumSetDescDef		*enum_set_table;
externalref unsigned short int		*argument_enumset_table;
externalref int				*enumval_values_table;
externalref int				uil_max_object;
externalref char			**uil_widget_names;
externalref int				uil_max_arg;
externalref char			**uil_argument_names;
externalref int				uil_max_reason;
externalref char			**uil_reason_names;
externalref int				uil_max_enumset;
externalref int				uil_max_enumval;
externalref char			**uil_enumval_names;
externalref int				uil_max_charset;
externalref char			**uil_charset_names;
externalref unsigned short int		*related_argument_table;
externalref unsigned char		**allowed_reason_table;
externalref char			**tok_token_name_table;
externalref int				tok_num_tokens;
externalref char			**uil_widget_funcs;
externalref unsigned short int		*uil_gadget_variants;
externalref unsigned short int		*uil_urm_nondialog_class;
externalref unsigned short int		*uil_urm_subtree_resource;
externalref char			**uil_argument_toolkit_names;
externalref char			**uil_reason_toolkit_names;

externalref unsigned short int		uil_sym_user_defined_object;
externalref unsigned short int		uil_sym_default_charset;
externalref unsigned short int		uil_sym_isolatin1_charset;

externalref int				uil_max_value;
externalref char			*uil_datatype_names[];


#endif /* UilSymGl_h */
/* DON'T ADD STUFF AFTER THIS #endif */