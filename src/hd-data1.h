#ifndef __HD_DATA1_H
#  define __HD_DATA1_H
/*
*  hd-data1.h:  All top 20 countries data which is composed into the
*               eternal holiday list header file.
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
*  $Id: hd-data1.h 3.00 2000/03/30 03:00:00 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd_data1.c'.
*/
EXPORT void
au_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
be_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
br_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
ca_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
ch_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
cn_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
de_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
es_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
fr_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
gb_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
il_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
it_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
jp_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
kr_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
mx_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
nl_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
nz_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
pt_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
ru_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
EXPORT void
us_hdy __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
__END_DECLARATIONS
#endif /* __HD_DATA1_H */
