#ifndef __GCAL_H
#  define __GCAL_H
/*
*  gcal.h:  Get and process the program options
*           and lauch the extended calendar program header file.
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
*  $Id: gcal.h 3.00 2000/03/04 03:00:00 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `gcal.c'.
*/
EXPORT int
main __P_((int   argc,
           char *argv[]));
EXPORT int
eval_longopt __P_((char *longopt,
                   int  *longopt_symbolic));
__END_DECLARATIONS
#endif /* __GCAL_H */
