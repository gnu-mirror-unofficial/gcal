#ifndef __UTILS_H
#  define __UTILS_H
/*
*  utils.h:  Pool of common functions header file.
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
*  $Id: utils.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `utils.c'.
*/
EXPORT VOID_PTR
my_malloc __P_((const int   amount,
                const int   exit_status,
                const char *module_name,
                const long  module_line,
                const char *var_name,
                const int   var_contents));
EXPORT VOID_PTR
my_realloc __P_((      VOID_PTR  ptr_memblock,
                 const int       amount,
                 const int       exit_status,
                 const char     *module_name,
                 const long      module_line,
                 const char     *var_name,
                 const int       var_contents));
EXPORT void
allocate_all_strings __P_((const int   amount,
                           const char *module_name,
                           const long  module_line));
EXPORT void
resize_all_strings __P_((const int   amount,
                         const Bool  with_line_buffer,
                         const char *module_name,
                         const long  module_line));
EXPORT void
my_error __P_((const int   exit_status,
               const char *module_name,
               const long  module_line,
               const char *var_name,
               const int   var_contents));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
EXPORT RETSIGTYPE
handle_signal __P_((int the_signal));
#endif
EXPORT void
my_exit __P_((const int exit_status));
EXPORT int
my_atoi __P_((const char *string));
EXPORT int
my_system __P_((const char *command));
#if !HAVE_STRSTR
EXPORT char *
my_strstr __P_((const char *text,
                const char *pattern));
#endif /* !HAVE_STRSTR */
#if !HAVE_STRCSPN
EXPORT char *
my_strcspn __P_((const char *s1,
                 const char *s2));
#endif /* !HAVE_STRCSPN */
#if !HAVE_STRCASECMP
EXPORT int
my_strcasecmp __P_((const char *s1,
                    const char *s2));
#endif /* !HAVE_STRCASECMP */
#if !HAVE_STRNCASECMP
EXPORT int
my_strncasecmp __P_((const char *s1,
                     const char *s2,
                           int   len));
#endif /* !HAVE_STRNCASECMP */
EXPORT Bool
get_actual_date __P_((void));
EXPORT int
compare_d_m_name __P_((const char       *string,
                       const Cmode_enum  mode));
EXPORT int
asc_sort __P_((const char **a,
               const char **b));
EXPORT int
des_sort __P_((const char **a,
               const char **b));
EXPORT Bool
is_presorted __P_((char **table,
                   int    elems));
EXPORT void
reverse_order __P_((      char **table,
                    const int    elems));
EXPORT const char *
day_suffix __P_((int day));
EXPORT const char *
short3_day_name __P_((const int day));
EXPORT const char *
short_day_name __P_((const int day));
EXPORT const char *
day_name __P_((const int day));
EXPORT const char *
short_month_name __P_((const int month));
EXPORT const char *
month_name __P_((const int month));
EXPORT Ulint
date2num __P_((const int day,
               const int month,
               const int year));
EXPORT void
num2date __P_((Ulint  mjd,
               int   *day,
               int   *month,
               int   *year));
EXPORT Bool
doy2date __P_((      int  doy,
               const int  is_leap_year,
                     int *day,
                     int *month));
EXPORT int
weekday_of_date __P_((const int day,
                      const int month,
                      const int year));
EXPORT int
day_of_year __P_((const int day,
                  const int month,
                  const int year));
EXPORT int
days_of_february __P_((const int year));
EXPORT Bool
valid_date __P_((const int day,
                 const int month,
                 const int year));
EXPORT Bool
prev_date __P_((int *day,
                int *month,
                int *year));
EXPORT Bool
next_date __P_((int *day,
                int *month,
                int *year));
EXPORT int
week_number __P_((const int  day,
                  const int  month,
                  const int  year,
                  const Bool is_iso_week,
                  const int  start_day_of_week));
EXPORT int
weekno2doy __P_((      int  week,
                 const int  year,
                 const Bool is_iso_week,
                 const int  start_day_of_week));
EXPORT int
knuth_easter_formula __P_((const int year));
EXPORT int
julian_gregorian_diff __P_((const int day,
                            const int month,
                            const int year));
__END_DECLARATIONS
#endif /* __UTILS_H */
