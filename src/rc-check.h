#ifndef __RC_CHECK_H
#  define __RC_CHECK_H
/*
*  rc-check.h:  Checks if a line (of a resource file) must be inserted into `rc_elems_table[]' header file.
*
*
*  Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken
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
*  $Id: rc-check.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-check.c'.
*/
EXPORT void
rc_check __P_((      char *line_buffer,
               const char *filename,
               const long  line_number,
               const int   line_length,
                     int  *rc_elems,
               const int   day,
               const int   ed,
               const int   wd));
__END_DECLARATIONS
#endif /* __RC_CHECK_H */