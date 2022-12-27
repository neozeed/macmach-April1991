/* -*-C-*-
********************************************************************************
*
* File:         osptrs.h
* RCS:          $Header: osptrs.h,v 1.2 89/11/25 05:10:18 mayer Exp $
* Description:  system specific function pointers
* Author:       David Michael Betz
* Created:      
* Modified:     Sat Nov 25 05:10:11 1989 (Niels Mayer) mayer@hplnpm
* Language:     C
* Package:      N/A
* Status:       X11r4 contrib tape release
*
* WINTERP 1.0 Copyright 1989 Hewlett-Packard Company (by Niels Mayer).
* XLISP version 2.1, Copyright (c) 1989, by David Betz.
*
* Permission to use, copy, modify, distribute, and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice appear in all copies and that both that
* copyright notice and this permission notice appear in supporting
* documentation, and that the name of Hewlett-Packard and David Betz not be
* used in advertising or publicity pertaining to distribution of the software
* without specific, written prior permission.  Hewlett-Packard and David Betz
* make no representations about the suitability of this software for any
* purpose. It is provided "as is" without express or implied warranty.
*
* HEWLETT-PACKARD AND DAVID BETZ DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
* IN NO EVENT SHALL HEWLETT-PACKARD NOR DAVID BETZ BE LIABLE FOR ANY SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
* OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* See ./winterp/COPYRIGHT for information on contacting the authors.
* 
* Please send modifications, improvements and bugfixes to mayer@hplabs.hp.com
* Post XLISP-specific questions/information to the newsgroup comp.lang.lisp.x
*
********************************************************************************
*/

#ifdef FWR_STUFF

/* DB version 1.0 Copyright (c) 1990 FWR Group */
/* TTY version 1.0 Copyright (c) 1990 FWR Group */

{"DB-OPEN",	S, xdb_open	},
{"DB-LOAD",	S, xdb_load	},
{"DB-GROUP",	S, xdb_group	},
{"DB-DATA",	S, xdb_data	},
{"DB-NAME",	S, xdb_name	},
{"DB-UPDATE",	S, xdb_update	},
{"DB-STATUS",	S, xdb_status	},
{"DB-MENU",	S, xdb_menu	},
{"TTY-ENABLE",	S, xtty_enable	},
{"TTY-MENU",	S, xtty_menu	},
{"TTY-YESNO",	S, xtty_yesno	},

#endif /* FWR_STUFF */
