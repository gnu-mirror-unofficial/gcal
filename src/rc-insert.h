#ifndef __RC_INSERT_H
#  define __RC_INSERT_H
/*
*  rc-insert.h:  Inserts a line (of a resource file) into `rc_elems_table[]' header file.
*
*
*  Copyright (c) 1994-1997, 2000 Thomas Esken
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 2, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*    Free Software Foundation, Inc.
*    59 Temple Place - Suite 330
*    Boston, MA 02111-1307,  USA
*/



/*
*  $Id: rc-insert.h 3.00 2000/03/04 03:00:00 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
*  The REGEX stuff.
*/
#  if HAVE_RE_COMP
IMPORT int re_exec();
#  endif
#  if HAVE_REGCMP
IMPORT char *regex();
#  endif
/*
************************************************** Defined in `rc-insert.c'.
*/
EXPORT void
insert_line_into_table __P_((      char *line_buffer,
                             const char *filename,
                             const long  line_number,
                                   int  *rc_elems,
                                   int   len_date,
                                   int   print_twice));
__END_DECLARATIONS
#endif /* __RC_INSERT_H */
