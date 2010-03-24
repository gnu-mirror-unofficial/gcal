/*
*  utils.c:  Pool of common functions.
*
*
*  Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken
*  Copyright (c) 2010 Free Software Foundation, Inc.
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 3, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*/



#ifdef RCSID
static char rcsid[]="$Id: utils.c 3.01 2000/06/14 03:00:01 tom Exp $";
#endif



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_CTYPE_H
#  include <ctype.h>
#endif
#if HAVE_ERRNO_H
#  include <errno.h>
#endif
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else /* !TIME_WITH_SYS_TIME */
#  if HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else /* !HAVE_SYS_TIME_H */
#    include <time.h>
#  endif /* !HAVE_SYS_TIME_H */
#endif /* !TIME_WITH_SYS_TIME */
#include "common.h"
#if USE_RC
#  include "rc-defs.h"
#endif /* USE_RC */
#include "globals.h"
#include "hd-defs.h"
#include "hd-use.h"
#if USE_RC
#  include "rc-utils.h"
#endif /* USE_RC */
#include "utils.h"



/*
*  LOCAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `utils.c'.
*/
LOCAL void
gregorian2julian __P_((int *day,
                       int *month,
                       int *year));
LOCAL int
raw_week_number __P_((const int  day,
                      const int  month,
                      const int  year,
                      const Bool is_iso_week,
                      const int  start_day_of_week));
LOCAL const char *
dflt_day_name __P_((const int day));
LOCAL const char *
dflt_month_name __P_((const int month));
__END_DECLARATIONS



/*
*  Function implementations.
*/
   PUBLIC VOID_PTR
my_malloc (amount, exit_status, module_name, module_line, var_name, var_contents)
   const int   amount;
   const int   exit_status;
   const char *module_name;
   const long  module_line;
   const char *var_name;
   const int   var_contents;
/*
   Allocates AMOUNT bytes of memory dynamically, with error checking.
     Calls `my_error()' and terminates the program if any errors occur.
     AMOUNT is limited to `int' range instead of `size_t' range;
     this is wanted!
*/
{
   auto VOID_PTR  ptr_memblock;


   if ((Uint)amount > testval)
     /*
        Error, table size overflow!
     */
     my_error (ERR_INTERNAL_TABLE_CRASH, module_name, module_line, var_name, (int)testval);
   ptr_memblock = (VOID_PTR)malloc((int)amount);
   if (ptr_memblock == (VOID_PTR)NULL)
     /*
        Error, `malloc()' function failed.
     */
     my_error (exit_status, module_name, module_line, var_name, var_contents);

   return(ptr_memblock);
}



   PUBLIC VOID_PTR
my_realloc (ptr_memblock, amount, exit_status, module_name, module_line, var_name, var_contents)
         VOID_PTR  ptr_memblock;
   const int       amount;
   const int       exit_status;
   const char     *module_name;
   const long      module_line;
   const char     *var_name;
   const int       var_contents;
/*
   Changes the size of an allocated block of memory PTR_MEMBLOCK to AMOUNT
     bytes, with error checking.  Calls `my_error()' and terminates the program
     if any errors occur.  AMOUNT is limited to `int' range instead of `size_t'
     range; this is wanted!  If PTR_MEMBLOCK is NULL, `my_malloc()' is called
     instead.
*/
{
   if ((Uint)amount > testval)
     /*
        Error, table size overflow!
     */
     my_error (ERR_INTERNAL_TABLE_CRASH, module_name, module_line, var_name, (int)testval);
   if (ptr_memblock == (VOID_PTR)NULL)
     return(my_malloc (amount, exit_status, module_name, module_line, var_name, var_contents));
   ptr_memblock = (VOID_PTR)realloc(ptr_memblock, (int)amount);
   if (ptr_memblock == (VOID_PTR)NULL)
     /*
        Error, `realloc()' function failed.
     */
     my_error (exit_status, module_name, module_line, var_name, var_contents);

   return(ptr_memblock);
}



   PUBLIC void
allocate_all_strings (amount, module_name, module_line)
   const int   amount;
   const char *module_name;
   const long  module_line;
/*
   Initially allocates AMOUNT bytes of memory dynamically for all string
     vectors used, with error checking.  Calls `my_error()' indirectly and
     terminates the program if any errors occur.  AMOUNT is limited to `int'
     range instead of `size_t' range; this is wanted!  Can only be called once!
*/
{
   static Bool  all_strings_initialized=FALSE;


   if (!all_strings_initialized)
    {
      s1 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s1", 0);
      s2 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s2", 0);
      s3 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s3", 0);
      s4 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s4", 0);
#if USE_RC
      s5 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s5", 0);
      s6 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s6", 0);
      s7 = (char *)my_malloc (amount,
                              ERR_NO_MEMORY_AVAILABLE,
                              module_name, module_line,
                              "s7", 0);
      line_buffer = (char *)my_malloc (amount,
                                       ERR_NO_MEMORY_AVAILABLE,
                                       module_name, module_line,
                                       "line_buffer", 0);
#endif
      all_strings_initialized = TRUE;
    }
}



   PUBLIC void
resize_all_strings (amount, with_line_buffer, module_name, module_line)
   const int   amount;
   const Bool  with_line_buffer;
   const char *module_name;
   const long  module_line;
/*
   Changes the size of all string vectors used to AMOUNT bytes, with error
     checking.  Calls `my_error()' indirectly and terminates the program
     if any errors occur. AMOUNT is limited to `int' range instead of `size_t'
     range; this is wanted!
*/
{
   if (   ((Uint)amount > testval)
       && (maxlen_max < testval))
     maxlen_max = testval;
   else
     maxlen_max = (Uint)amount;
   s1 = (char *)my_realloc ((VOID_PTR)s1, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s1", maxlen_max);
   s2 = (char *)my_realloc ((VOID_PTR)s2, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s2", maxlen_max);
   s3 = (char *)my_realloc ((VOID_PTR)s3, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s3", maxlen_max);
   s4 = (char *)my_realloc ((VOID_PTR)s4, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s4", maxlen_max);
#if USE_RC
   s5 = (char *)my_realloc ((VOID_PTR)s5, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s5", maxlen_max);
   s6 = (char *)my_realloc ((VOID_PTR)s6, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s6", maxlen_max);
   s7 = (char *)my_realloc ((VOID_PTR)s7, maxlen_max,
                            ERR_NO_MEMORY_AVAILABLE,
                            module_name, module_line,
                            "s7", maxlen_max);
   if (with_line_buffer)
     line_buffer = (char *)my_realloc ((VOID_PTR)line_buffer, maxlen_max,
                                       ERR_NO_MEMORY_AVAILABLE,
                                       module_name, module_line,
                                       "line_buffer", maxlen_max);
#endif
}



   PUBLIC void
my_error (exit_status, module_name, module_line, var_name, var_contents)
   const int   exit_status;
   const char *module_name;
   const long  module_line;
   const char *var_name;
   const int   var_contents;
/*
   Displays a specific error message on STDERR channel
     and terminates the program with status `exit_status'.
*/
{
   S_NEWLINE(stderr);
   if (prgr_name == (char *)NULL)
     prgr_name = PRGR_NAME;
#if USE_DE
   fprintf(stderr, "%s: Abbruch, ", prgr_name);
#else /* !USE_DE */
   fprintf(stderr, _("%s: abort, "), prgr_name);
#endif /* !USE_DE */
   switch(exit_status)
    {
#if USE_DE
      case ERR_INVALID_EASTER_DATE:
        fprintf(stderr, "unzul%sssiges Jahr f%sr Ostertagsberechnung\nJahr mu%s im Bereich (%d...%d) sein",
                AE, UE, SZ, EASTER_MIN, EASTER_MAX);
        break;
      case ERR_NO_MEMORY_AVAILABLE:
        fprintf(stderr, "`%s' Zeile %ld: virtueller Speicher ersch%spft (%s=%d)",
                module_name, module_line, OE, var_name, var_contents);
        break;
#  if USE_RC
      case ERR_INVALID_DATE_FIELD:
        fprintf(stderr, "unzul%sssiger Datumeintrag in Datei `%s'\nZeile %ld: %s",
                AE, module_name, module_line, var_name);
        break;
      case ERR_INVALID_MONTH_FIELD:
        fprintf(stderr, "ung%sltiger Monat(%02d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case ERR_INVALID_DAY_FIELD:
        fprintf(stderr, "ung%sltiger Tag(%02d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case ERR_MALFORMED_INCLUDE:
        fprintf(stderr, "mi%sgebildetes %s in Datei `%s'\nZeile %ld: %s",
                SZ, RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case ERR_CYCLIC_INCLUDE:
        fprintf(stderr, "rekursives/zyklisches %s unzul%sssig in Datei `%s'\nZeile %ld: %s",
                RC_INCL_STMENT, AE, module_name, module_line, var_name);
        break;
      case ERR_FILE_NOT_FOUND:
        fprintf(stderr, "Datei `%s' nicht gefunden", module_name);
        break;
      case ERR_INVALID_NWD_FIELD:
        fprintf(stderr, "ung%sltiges N'ter Wochentagfeld(%d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case ERR_NO_SEPARATOR_CHAR:
        fprintf(stderr, "fehlendes `whitespace' Trennzeichen nach Datumteil in Datei `%s'\nZeile %ld: %s",
                module_name, module_line, var_name);
        break;
#  endif /* USE_RC */
      case ERR_WRITE_FILE:
        fprintf(stderr, "kann Datei `%s' nicht schreiben\nSpeichermedium ist voll!",
                var_name);
        break;
#  ifdef GCAL_EMAIL
      case ERR_EMAIL_SEND_FAILURE:
        fprintf(stderr, "versenden der eMail an <%s> hat versagt", var_name);
#    if HAVE_ERRNO_H
        fprintf(stderr, "\n`%s' Zeile %ld: ", module_name, module_line);
        perror("perror");
#    endif
        break;
#  endif
#  if defined(GCAL_EPAGER) || defined(GCAL_EMAIL) || USE_RC
      case ERR_INTERNAL_C_FUNC_FAILURE:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') `%s%d' hat versagt",
                module_name, module_line, INTERNAL_TXT, var_name, var_contents);
#    if HAVE_ERRNO_H
        perror("\nperror");
#    endif
        break;
#  endif
      case ERR_READ_FILE:
        fprintf(stderr, "Lesefehler in Datei `%s'", var_name);
#  if HAVE_ERRNO_H
        fprintf(stderr, "\n`%s' Zeile %ld: ", module_name, module_line);
        perror("perror");
#  endif
        break;
      case ERR_ILLEGAL_CHAR_IN_FILE:
        fprintf(stderr, "illegales Zeichen in Antwortdatei `%s'\nZeile %ld: %s",
                module_name, module_line, var_name);
        break;
      case ERR_INTERNAL_TABLE_CRASH:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') ung%sltiger Wert f%sr Tabellengr%s%se `sizeof %s>%d'",
                module_name, module_line, INTERNAL_TXT, UE, UE, OE, SZ, var_name, var_contents);
        break;
      case ERR_INVALID_DATE_FORMAT:
        fprintf(stderr, "(`%s') Datumformat `%s' ist ung%sltig", module_name, var_name, UE);
        break;
#  if USE_RC
      case ERR_INVALID_REGEX_PATTERN:
        if (*module_name)
          fprintf(stderr, "%s im Suchmuster `%s'", module_name, var_name);
        else
          fprintf(stderr, "ung%sltiges Suchmuster `%s' angegeben", UE, var_name);
        break;
#  endif
      default:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') unbehandelter Fehler (%d)",
                module_name, module_line, INTERNAL_TXT, exit_status);
#else /* !USE_DE */
      case ERR_INVALID_EASTER_DATE:
        fprintf(stderr, _("invalid year for computing Easter Sundays date\nYear must be in range (%d...%d)"),
                EASTER_MIN, EASTER_MAX);
        break;
      case ERR_NO_MEMORY_AVAILABLE:
        fprintf(stderr, _("`%s' line %ld: virtual memory exhausted (%s=%d)"),
                module_name, module_line, var_name, var_contents);
        break;
#  if USE_RC
      case ERR_INVALID_DATE_FIELD:
        fprintf(stderr, _("invalid date part in file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
      case ERR_INVALID_MONTH_FIELD:
        fprintf(stderr, _("invalid month field(%02d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case ERR_INVALID_DAY_FIELD:
        fprintf(stderr, _("invalid day field(%02d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case ERR_MALFORMED_INCLUDE:
        fprintf(stderr, _("malformed %s in file `%s'\nLine %ld: %s"),
                RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case ERR_CYCLIC_INCLUDE:
        fprintf(stderr, _("invalid recursive/cyclic %s in file `%s'\nLine %ld: %s"),
                RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case ERR_FILE_NOT_FOUND:
        fprintf(stderr, _("file `%s' not found"), module_name);
        break;
      case ERR_INVALID_NWD_FIELD:
        fprintf(stderr, _("invalid N'th weekday field(%d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case ERR_NO_SEPARATOR_CHAR:
        fprintf(stderr, _("missing `whitespace' character after date part in file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
#  endif /* USE_RC */
      case ERR_WRITE_FILE:
        fprintf(stderr, _("file `%s' can't be written\nStorage media full!"),
                var_name);
        break;
#  ifdef GCAL_EMAIL
      case ERR_EMAIL_SEND_FAILURE:
        fprintf(stderr, _("sending eMail to <%s> failed"), var_name);
#    if HAVE_ERRNO_H
        fprintf(stderr, _("\n`%s' line %ld: "), module_name, module_line);
        perror("perror");
#    endif
        break;
#  endif
#  if defined(GCAL_EPAGER) || defined(GCAL_EMAIL) || USE_RC
      case ERR_INTERNAL_C_FUNC_FAILURE:
        fprintf(stderr, _("`%s' line %ld: (`%s') `%s%d' failed"),
                module_name, module_line, _("Internal"), var_name, var_contents);
#    if HAVE_ERRNO_H
        perror("\nperror");
#    endif
        break;
#  endif
      case ERR_READ_FILE:
        fprintf(stderr, _("read error in file `%s'"), var_name);
#  if HAVE_ERRNO_H
        fprintf(stderr, _("\n`%s' line %ld: "), module_name, module_line);
        perror("perror");
#  endif
        break;
      case ERR_ILLEGAL_CHAR_IN_FILE:
        fprintf(stderr, _("illegal character in response file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
      case ERR_INTERNAL_TABLE_CRASH:
        fprintf(stderr, _("`%s' line %ld: (`%s') invalid value for table size `sizeof %s>%d'"),
                module_name, module_line, _("Internal"), var_name, var_contents);
        break;
      case ERR_INVALID_DATE_FORMAT:
        fprintf(stderr, _("(`%s') date format `%s' is invalid"), module_name, var_name);
        break;
#  if USE_RC
      case ERR_INVALID_REGEX_PATTERN:
        if (*module_name)
          fprintf(stderr, _("%s in search pattern `%s'"), module_name, var_name);
        else
          fprintf(stderr, _("invalid search pattern `%s' specified"), var_name);
        break;
#  endif /* USE_RC */
      default:
        fprintf(stderr, _("`%s' line %ld: (`%s') unmanaged error (%d)"),
                module_name, module_line, _("Internal"), exit_status);
#endif /* !USE_DE */
    }
   S_NEWLINE(stderr);
   my_exit (exit_status);
}



#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
   PUBLIC RETSIGTYPE
handle_signal (the_signal)
   int the_signal;
/*
   Signal handler function which displays the numeric ID of the
     received signal on STDERR channel and terminates the program
     with ERR_TERMINATION_BY_SIGNAL exit status.
*/
{
   fflush(stdout);
#if USE_DE
   fprintf(stderr, "\n%s: Programmabbruch durch Signal %d\n", prgr_name, the_signal);
#else /* !USE_DE */
   fprintf(stderr, _("\n%s: program aborted by signal %d\n"), prgr_name, the_signal);
#endif /* !USE_DE */
   my_exit (ERR_TERMINATION_BY_SIGNAL);
}
#endif /* HAVE_SIGNAL && (SIGINT || SIGTERM || SIGHUP) */



   PUBLIC void
my_exit (exit_status)
   const int exit_status;
/*
   Tries to erase all temporary files before without error checking
     and quits the program by calling the systems `exit()' function.
*/
{
#if USE_RC
   if (rc_tvar_tfp != (FILE *)NULL)
     (void)fclose(rc_tvar_tfp);
   if (rc_tvar_tfn != (char *)NULL)
     (void)unlink(rc_tvar_tfn);
   if (rc_here_fp != (FILE *)NULL)
     (void)fclose(rc_here_fp);
   if (rc_here_fn != (char *)NULL)
     (void)unlink(rc_here_fn);
#endif
#ifdef GCAL_EMAIL
   if (tfp != (FILE *)NULL)
     (void)fclose(tfp);
   if (tfn != (char *)NULL)
     (void)unlink(tfn);
#endif
   exit(exit_status);
}



   PUBLIC int
my_atoi (string)
   const char *string;
/*
   Converts given `string' to a positiv integer value
     skipping leading zeroes and returns values in
     range 0...YEAR_MAX only (invalid values are set to 0).
*/
{
   while (*string == '0')
     string++;
   if (   *string == '-'
       || (int)strlen(string) > len_year_max)
     return(0);

   return(atoi(string));
}



   PUBLIC int
my_system (command)
   const char *command;
/*
   Wrapper for the system() function.
*/
{
#if HAVE_SYSTEM
   return(((unsigned int)system(command) >> 8) & 0xff);
#else /* !HAVE_SYSTEM */
   return(-1);
#endif /* !HAVE_SYSTEM */
}



#if !HAVE_STRSTR
   PUBLIC char *
my_strstr (text, pattern)
   const char *text;
   const char *pattern;
/*
   Search the `pattern' needle in haystack `text'   =8^)
     (emulates the ANSI C strstr() function; not very well optimized).
*/
{
   register       int    i=(int)strlen(text);
   register       int    j=(int)strlen(pattern);
   register       int    k;
   auto     const char  *ptr_char=text;


   if (!j)
     return(ptr_char);
   if (i < j)
     return(NULL);
   for (i=0 ; *ptr_char ; i++,ptr_char++)
    {
      for (k=0,j=i ; pat[k] && (text[j]==pattern[k]) ; j++,k++)
        ;
      if (   k
          && !pattern[k])
        return(ptr_char);
    }

   return(NULL);
}
#endif /* !HAVE_STRSTR */



#if !HAVE_STRCSPN
   PUBLIC int
my_strcspn (s1, s2)
   const char *s1;
   const char *s2;
/*
   Find length of initial segment of `s1' consisting entirely
   of characters not from `s2' (emulates the ANSI C strcspn() function).
*/
{
   register       int    count=0;
   auto     const char  *scan1;
   auto     const char  *scan2;


   for (scan1=s1 ; *scan1 ; scan1++)
    {
      for (scan2=s2 ; *scan2 ; )
       if (*scan1 == *scan2++)
         return(count);
      count++;
    }

   return(count);
}
#endif /* !HAVE_STRCSPN */



#if !HAVE_STRCASECMP
   PUBLIC int
my_strcasecmp (s1, s2)
   const char *s1;
   const char *s2;
/*
   Same as the ANSI C `strcmp()' function, but case insensitive.
*/
{
   auto const Uchar  *p1=(const Uchar *)s1;
   auto const Uchar  *p2=(const Uchar *)s2;
   auto       Uchar   c1;
   auto       Uchar   c2;


   if (p1 == p2)
     return(0);
   do
    {
      c1 = tolower(*p1++);
      c2 = tolower(*p2++);
      if (!c1)
        break;
    } while (c1 == c2);

   return(c1-c2);
}
#endif /* !HAVE_STRCASECMP */



#if !HAVE_STRNCASECMP
   PUBLIC int
my_strncasecmp (s1, s2, len)
   const char *s1;
   const char *s2;
         int   len;
/*
   Same as the ANSI C `strncmp()' function, but case insensitive.
*/
{
   auto const Uchar  *p1=(const Uchar *)s1;
   auto const Uchar  *p2=(const Uchar *)s2;
   auto       Uchar   c1;
   auto       Uchar   c2;


   if (   !len
       || p1 == p2)
     return(0);
   if (len < 0)
     len = -len;
   do
    {
      c1 = tolower(*p1++);
      c2 = tolower(*p2++);
      if (   !c1
          || c1 != c2)
        return(c1-c2);
    } while (--len);

   return(c1-c2);
}
#endif /* !HAVE_STRNCASECMP */



   PUBLIC Bool
get_actual_date ()
/*
   Gets the actual local/GMT date and time from the system resp.,
     evaluates the "actual" date from global `rc_adate'-ptr to string.
     Returns TRUE if it's possible to evaluate `rc_adate', otherwise FALSE.
*/
{
   auto   struct tm         *sys_date;
   auto          MY_TIME_T   sys_time;
   static Bool               got_time=FALSE;


   sys_time = time((MY_TIME_T *)NULL);
   sys_date = localtime(&sys_time);
   true_day=act_day = sys_date->tm_mday;
   true_month=act_month = sys_date->tm_mon + 1;
   act_year = sys_date->tm_year;
   if (act_year < CENTURY)
     act_year += CENTURY;
   true_year = act_year;
   if (!got_time)
    {
      act_sec  = sys_date->tm_sec;
      act_min  = sys_date->tm_min;
      act_hour = sys_date->tm_hour;
      buf_ad = act_day;
      buf_am = act_month;
      buf_ay = act_year;
    }
#if USE_RC
   /*
      Actual date modifier %DATE given.
   */
   if (rc_adate != (char *)NULL)
    {
      auto int    d=0;
      auto int    m=0;
      auto int    y=0;
      auto int    n=0;
      auto int    hn=0;
      auto int    hwd=0;
      auto int    i_dummy;
      auto char   hc='\0';
      auto char  *c_dummy=(char *)NULL;
      auto Bool   b_dummy;


      if (   !*rc_adate
          || *rc_adate == RC_HDY_CHAR)
        return(FALSE);
      /*
         Pre-initialize month and year to actual date.
      */
      m = act_month;
      y = act_year;
      /*
         Check if a numeric date is given.
      */
      if (isdigit(*rc_adate))
       {
         /*
            Check if a numeric day in month is given.
         */
         if ((int)strlen(rc_adate) <= 2)
          {
            d = atoi(rc_adate);
            if (!d)
              /*
                 Error, invalid day number given.
              */
              return(FALSE);
          }
         else
#  if USE_DE
           (void)rc_get_date (rc_adate, lptrs3, FALSE, &b_dummy, &d, &m, &y, &n,
                              &i_dummy, &hc, &hn, &hwd, INTERNAL_TXT, -1L, rc_adate, FALSE);
#  else /* !USE_DE */
           (void)rc_get_date (rc_adate, lptrs3, FALSE, &b_dummy, &d, &m, &y, &n,
                              &i_dummy, &hc, &hn, &hwd, _("Internal"), -1L, rc_adate, FALSE);
#  endif /* !USE_DE */
       }
      else
       {
         /*
            Check if textual weekday name is given.
         */
         d = compare_d_m_name (rc_adate, DAy);
         if (d)
          {
            /*
               If a digit (1...5, 9) trails the textual weekday name,
                 generate "N'th weekday of month" date.
            */
            c_dummy = rc_adate;
            while (isalpha(*c_dummy))
              c_dummy++;
            if (*c_dummy)
             {
               n = atoi(c_dummy);
               if (!n)
                 /*
                    Error, invalid "N'th weekday of month" given (must be 1...5, 9).
                 */
                 return(FALSE);
               /*
                  Check if the digit is trailed by anything further...
               */
               c_dummy++;
               if (*c_dummy)
                 /*
                    Error, invalid "N'th weekday of month" given.
                 */
                 return(FALSE);
             }
            else
             {
               /*
                  Only textual weekday name found, so compute absolute day
                    in month of that weekday (of current week).
               */
               hn = act_day;
               i_dummy = weekday_of_date (act_day, act_month, act_year);
               if (d > i_dummy)
                 do
                  {
                    (void)next_date (&hn, &m, &y);
                  } while (d != weekday_of_date (hn, m, y));
               else
                 if (d < i_dummy)
                   do
                    {
                      (void)prev_date (&hn, &m, &y);
                    } while (d != weekday_of_date (hn, m, y));
               d = hn;
             }
          }
         else
          {
            /*
               Check if short month name is given.
            */
            m = compare_d_m_name (rc_adate, MOnth);
            if (!m)
              /*
                 Error, invalid month name given.
              */
              return(FALSE);
            /*
               Check if a day number trails textual a month name.
            */
            c_dummy = rc_adate;
            while (   isupper(*c_dummy)
#  if USE_DE
#    if USE_EASC
                   || *c_dummy == *AE
                   || *c_dummy == *OE
                   || *c_dummy == *UE
                   || *c_dummy == *AAE
                   || *c_dummy == *OOE
                   || *c_dummy == *UUE
#    else /* !USE_EASC */
                   || *c_dummy == '"'
#    endif /* !USE_EASC */
#  endif /* USE_DE */
                   || islower(*c_dummy))
              c_dummy++;
            if (*c_dummy)
             {
               d = atoi(c_dummy);
               if (!d)
                 /*
                    Error, invalid trailing day number given.
                 */
                 return(FALSE);
             }
            else
              d = act_day;
          }
       }
      if (y >= 0)
       {
         if (!y)
           y = act_year;
         if (!m)
           m = act_month;
         if (n)
          {
            if (d > DAY_MAX)
              return(FALSE);
            if (!d)
              d = weekday_of_date (act_day, m, y);
            if (m == 2)
              i_dummy = days_of_february (y);
            else
              i_dummy = dvec[m-1];
            if (n == 9)
              d = eval_holiday (i_dummy, m, y, d, FALSE);
            else
              if (   (n >= 1)
                  && (n <= 5))
               {
                 d = eval_holiday (DAY_MIN, m, y, d, TRUE);
                 d += (DAY_MAX * (n - 1));
                 /*
                    The "N'th weekday of month" doesn't occur in month:
                      generate no "actual" date.
                 */
                 if (d > i_dummy)
                   return(FALSE);
               }
              else
                return(FALSE);
          }
         else
           if (hc)
            {
              /*
                 If no explicit year is given in the actual date modifier %DATE,
                   like %0@e|t|DVAR[[-]N[WW[W]]] resp., %0*d|wN[WW[W]], compute
                   the date respecting the displacement, which was returned by
                   the function `rc_get_date()' in variable `&hn' and `&hwd'.
              */
              switch (hc)
               {
                 case RC_EASTER_CHAR:
                 case RC_TODAY_CHAR:
                   if (!precomp_date (hn, hwd, &d, &m, y,
                       (hc == RC_EASTER_CHAR) ? EAster : TOday))
                     return(FALSE);
                   break;
                 case 'D':
                 case 'W':
                   if (!precomp_nth_wd (hn, hwd, &n, &d, &m, &y,
                                        ((hc == 'D') ? DAy : WEek)))
                     return(FALSE);
                   break;
                 default:
                   if (!islower(hc))
                     return(FALSE);
                   else
                     if (!precomp_date (hn, hwd, &d, &m, y, DVar))
                       return(FALSE);
               }
            }
           else
             if (!d)
               d = act_day;
             else
               if (d == 99)
                {
                  /*
                     Assume last day of month.
                  */
                  if (m == 2)
                    d = days_of_february (y);
                  else
                    d = dvec[m-1];
                }
         if (valid_date (d, m, y))
          {
            act_day = d;
            act_month = m;
            act_year = y;
          }
         else
           return(FALSE);
       }
      else
        return(FALSE);
    }
#endif /* USE_RC */
   /*
      If the period of the Gregorian Reformation is past the actual local
        (Gregorian) system date, convert it to the proper Julian date.
   */
   gregorian2julian (&act_day, &act_month, &act_year);
   if (!got_time)
    {
      got_time = TRUE;
#if USE_RC
      /*
         Get the actual GMT and date.
      */
      sys_date = gmtime(&sys_time);
      gmt_min  = sys_date->tm_min;
      gmt_hour = sys_date->tm_hour;
      buf_gd   = sys_date->tm_mday;
      buf_gm   = sys_date->tm_mon + 1;
      buf_gy   = sys_date->tm_year;
      if (buf_gy < CENTURY)
        buf_gy += CENTURY;
      /*
         If the period of the Gregorian Reformation is past the actual GMT
           (Gregorian) system date, convert it to the proper Julian date.
      */
      gregorian2julian (&buf_gd, &buf_gm, &buf_gy);
      /*
         Compute the day difference between the actual GMT date
           and the local time date.
      */
      gmt_loc_diff = (int)d_between (buf_ad, buf_am, buf_ay, buf_gd, buf_gm, buf_gy);
#endif /* USE_RC */
      buf_ad = act_day;
      buf_am = act_month;
      buf_ay = act_year;
    }

   return(TRUE);
}



   PUBLIC int
compare_d_m_name (string, mode)
   const char       *string;
   const Cmode_enum  mode;
/*
   Compares the given day/month name `string' with built-in names and
     returns (1...7|1...12) either if `string' matches partly (until `\0'
     or first digit found in `string') or `string' matches complete.
     If `string' doesn't match or the length of `string' is less 2 for a day
     name or less 3 for a month name (we need such lengths to distinguish in a
     save way!), the function returns 0 always.
     This function checks first if a native language day/month name is given,
     if such a name can't be found, it checks next whether an "default" English
     day/month name is given.
   Depending on mode:
     mode==DAy:
       Compares delivered day name `string' with the built-in day names
         and returns: 1...7 == dayname 1...7 found ; 0 otherwise
     mode==MOnth:
       Compares delivered month name `string' with the built-in month names
         and returns: 1...12 == monthname 1...12 found ; 0 otherwise
*/
{
   register       int    len=(int)strlen(string);
   register const int    len_min=(mode == DAy) ? TXTLEN_DAY : TXTLEN_MONTH;
   register       int    i;
   register       int    imax=(mode == DAy) ? DAY_MAX : MONTH_MAX;
   register       int    j=0;
   register       int    checks;
   auto     const char  *ptr_char=string;


#if USE_DE
   checks = 2;
#else /* !USE_DE */
#  ifdef GCAL_NLS
   if (is_en)
     checks = 1;
   else
     checks = 2;
#  else /* !GCAL_NLS */
   checks = 1;
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
   if (len >= TXTLEN_DAY)
    {
      while (   *(ptr_char + j)
#if USE_RC
             && !isdigit(*(ptr_char + j))
             && (*(ptr_char + j) != RC_REPEAT_CHAR)
             && (*(ptr_char + j) != RC_APPEARS_CHAR)
#endif
            )
        j++;
      if (j >= len_min)
        do
         {
           for (i=(mode == DAy) ? DAY_MIN : MONTH_MIN ; i <= imax ; i++)
            {
              if (checks == 2)
                ptr_char = (mode == DAy) ? day_name (i) : month_name (i);
              else
                ptr_char = (mode == DAy) ? dflt_day_name (i) : dflt_month_name (i);
              j = 0;
              while (   *(ptr_char + j)
                     && string[j])
#if USE_DE && USE_EASC
                if (   (   (   string[j] == *AE
                            || string[j] == *AAE)
                        && (   *(ptr_char + j) == *AE
                            || *(ptr_char + j) == *AAE))
                    || (   (   string[j] == *OE
                            || string[j] == *OOE)
                        && (   *(ptr_char + j) == *OE
                            || *(ptr_char + j) == *OOE))
                    || (   (   string[j] == *UE
                            || string[j] == *UUE)
                        && (   *(ptr_char + j) == *UE
                            || *(ptr_char + j) == *UUE)))
                  j++;
                else
#endif
                  if (tolower(*(ptr_char+j)) == tolower(string[j]))
                    j++;
                  else
                    break;
              if (   j == len
#if USE_RC
                  || (   j
                      && (   isdigit(string[j])
                          || string[j] == RC_REPEAT_CHAR
                          || string[j] == RC_APPEARS_CHAR))
#endif
                  || !*(ptr_char + j))
                return(i);
            }
         } while (--checks);
    }

   return(0);
}



   PUBLIC int
asc_sort (a, b)
   const char **a;
   const char **b;
/*
   The (q)sort compare function; ascending order.
*/
{
   return(strcmp(*a, *b));
}



   PUBLIC int
des_sort (a, b)
   const char **a;
   const char **b;
/*
   The (q)sort compare function; descending order.
*/
{
   return(strcmp(*b, *a));
}



   PUBLIC Bool
is_presorted (table, elems)
   char **table;
   int    elems;
/*
   Checks whether the textual entries in `&table[]' are presorted in
      ascending sort order.  Returns TRUE if the entries in `table'
      are presorted, otherwise FALSE.
*/
{
   if (--elems > 0)
    {
      register int  i=0;
      register int  j=0;


      while (elems--)
       {
         j++;
         if (*table[i] >= *table[j])
           if (strcmp(table[i], table[j]) > 0)
             return(FALSE);
         i++;
       }
    }

  return(TRUE);
}



   PUBLIC void
reverse_order (table, elems)
         char **table;
   const int    elems;
/*
   Rearranges (reverts) the sort order of the textual entries in
     `&table[]' from  ascending sort order to descending sort order
     by swapping the `table' pointers.
*/
{
   register int  right=elems-1;


   if (right > 0)
    {
      register int    left=0;
      auto     char  *ptr_elem;


      while (left < right)
       {
         ptr_elem = table[left];
         table[left++] = table[right];
         table[right--] = ptr_elem;
       }
    }
}



   PUBLIC const char *
day_suffix (day)
   int day;
/*
   Returns the ordinal suffix (st, nd, rd or th) which is added to a single day number.
*/
{
#if USE_DE
   static   const char  *suffix[]={"'ter", "'ter", "'ter", "'ter"};
#else /* !USE_DE */
   static   const char  *suffix[]={
   /*
      *** Translators, this text should be a proper abbreviation of "fourth...".
   */
                                    N_("th"),
   /*
      *** Translators, this text should be a proper abbreviation of "first".
   */
                                    N_("st"),
   /*
      *** Translators, this text should be a proper abbreviation of "second".
   */
                                    N_("nd"),
   /*
      *** Translators, this text should be a proper abbreviation of "third".
   */
                                    N_("rd")
                                  };
#endif /* !USE_DE */
   register       int    i=0;


   if (day < 0)
     day = -day;
   if (day > 10000)
     day %= 10000;
   if (day > 1000)
     day %= 1000;
   if (day > 100)
     day %= 100;
   if (   day < 11
       || day > 13)
     i = day % 10;
   if (i > 3)
     i = 0;

   return(_(suffix[i]));
}



   PUBLIC const char *
short3_day_name (day)
   const int day;
/*
   Returns the short name of the day using the `printf()' format "%-3s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Mon", "Die", "Mit", "Don",
                                "Fre", "Sam", "Son"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Monday".
   */
                                N_("Mon"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Tuesday".
   */
                                N_("Tue"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Wednesday".
   */
                                N_("Wed"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Thursday".
   */
                                N_("Thu"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Friday".
   */
                                N_("Fri"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Saturday".
   */
                                N_("Sat"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "Sunday".
   */
                                N_("Sun")
                              };
#endif /* !USE_DE */

   return(((day < DAY_MIN) || (day > DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
short_day_name (day)
   const int day;
/*
   Returns the short name of the day using the `printf()' format "%-2s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Monday".
   */
                                N_("Mo"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Tuesday".
   */
                                N_("Tu"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Wednesday".
   */
                                N_("We"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Thursday".
   */
                                N_("Th"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Friday".
   */
                                N_("Fr"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Saturday".
   */
                                N_("Sa"),
   /*
      *** Translators, please translate this as a fixed 2-character text.
      *** This text should be a proper abbreviation of "Sunday".
   */
                                N_("Su")
                              };
#endif /* !USE_DE */

   return(((day < DAY_MIN) || (day > DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
day_name (day)
   const int day;
/*
   Returns the complete name of the day.
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Montag",  "Dienstag", "Mittwoch", "Donnerstag",
                                "Freitag", "Samstag",  "Sonntag"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
                                N_("Monday"), N_("Tuesday"),  N_("Wednesday"), N_("Thursday"),
                                N_("Friday"), N_("Saturday"), N_("Sunday")
                              };
#endif /* !USE_DE */

   return(((day < DAY_MIN) || (day > DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
short_month_name (month)
   const int month;
/*
   Returns the short name of the month using the `printf()' format "%-3s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Monat",
#  else /* !USE_EASC */
                                "ungueltiger Monat",
#  endif /* !USE_EASC */
                                "Jan", "Feb",
#  if USE_EASC
                                "M"AE"r",
#  else /* !USE_EASC */
                                "Mae",
#  endif /* !USE_EASC */
                                "Apr", "Mai", "Jun", "Jul", "Aug",
                                "Sep", "Okt", "Nov", "Dez"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid month"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "January".
   */
                                N_("Jan"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "February".
   */
                                N_("Feb"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "March".
   */
                                N_("Mar"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "April".
   */
                                N_("Apr"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "May".
   */
                                N_("May"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "June".
   */
                                N_("Jun"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "July".
   */
                                N_("Jul"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "August".
   */
                                N_("Aug"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "September".
   */
                                N_("Sep"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "October".
   */
                                N_("Oct"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "November".
   */
                                N_("Nov"),
   /*
      *** Translators, please translate this as a fixed 3-character text.
      *** This text should be a proper abbreviation of "December".
   */
                                N_("Dec")
                              };
#endif /* !USE_DE */

   return(((month < MONTH_MIN) || (month > MONTH_MAX)) ? _(name[0]) : _(name[month]));
}



   PUBLIC const char *
month_name (month)
   const int month;
/*
   Returns the complete name of the month.
*/
{
#if USE_DE
   static char  *name[]={
#  if USE_EASC
                          "ung"UE"ltiger Monat",
#  else /* !USE_EASC */
                          "ungueltiger Monat",
#  endif /* !USE_EASC */
                          "Januar",  "Februar",
#  if USE_EASC
                          "M"AE"rz",
#  else /* !USE_EASC */
                          "Maerz",
#  endif /* !USE_EASC */
                          "April",   "Mai",      "Juni",
                          "Juli",    "August",   "September",
                          "Oktober", "November", "Dezember"
                        };
#else /* !USE_DE */
   static char  *name[]={
                          N_("invalid month"),
                          N_("January"), N_("February"),
                          N_("March"),   N_("April"),
   /*
      *** Translators, please translate this as the complete month name "May".
      *** This text contains a final BLANK character, otherwise it is not
      *** distinguished from the 3-letter abbreviation of the complete month
      *** name "May", which is also "May".
      *** Your translation of this text should NOT contain any final BLANK
      *** characters, even the abbreviated and complete month names are
      *** the same in the language you are translating to.
   */
                          N_("May "),
                          N_("June"),    N_("July"),
                          N_("August"),  N_("September"),
                          N_("October"), N_("November"), N_("December")
                        };


   if (month == 5)
    {
      static char  *mayname;
      static Bool   is_initialized=FALSE;


      /*
         Now let's remove the trailing BLANK character
           from the complete text for the month MAY.
      */
      if (!is_initialized)
       {
         register int    len;
         auto     char  *s=_(name[month]);


         len = (int)strlen(s) - 1;
         mayname = (char *)my_malloc (len,
                                      ERR_NO_MEMORY_AVAILABLE,
                                      __FILE__, ((long)__LINE__)-2L,
                                      "mayname", 0);
         strcpy(mayname, s);
         while (*(mayname + len) == ' ')
           *(mayname + len--) = '\0';
         is_initialized = TRUE;
       }

      return(mayname);
    }
#endif /* !USE_DE */

   return(((month < MONTH_MIN) || (month > MONTH_MAX)) ? _(name[0]) : _(name[month]));
}



   PUBLIC Ulint
date2num (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the absolute number of days of the given date since
     00010101(==YYYYMMDD) respecting the missing period of the
     Gregorian Reformation.
*/
{
   auto Ulint  mjd=(Ulint)((year-1)*(Ulint)(DAY_LAST)+((year-1)>>2));


   if (   year > greg->year
       || (   (year == greg->year)
           && (   month > greg->month
               || (   (month == greg->month)
                   && (day > greg->last_day)))))
     mjd -= (Ulint)(greg->last_day - greg->first_day + 1);
   if (year > greg->year)
    {
      mjd += (((year - 1) / 400) - (greg->year / 400));
      mjd -= (((year - 1) / 100) - (greg->year / 100));
      if (   !(greg->year % 100)
          && (greg->year % 400))
        mjd--;
    }
   mjd += (Ulint)mvec[month-1];
   mjd += day;
   if (   (days_of_february (year) == 29)
       && (month > 2))
     mjd++;

   return(mjd);
}



   PUBLIC void
num2date (mjd, day, month, year)
   Ulint  mjd;
   int   *day;
   int   *month;
   int   *year;
/*
   Converts a delivered absolute number of days `mjd' to a standard
     date (since 00010101(==YYYYMMDD), returned in `&day', `&month' and `&year')
     respecting the missing period of the Gregorian Reformation.
*/
{
   auto     double  x;
   auto     Ulint   jdays=date2num (greg->first_day-1, greg->month, greg->year);
   register int     i;


   if (mjd > jdays)
     mjd += (Ulint)(greg->last_day - greg->first_day + 1);
   x = (double)mjd / (DAY_LAST + 0.25);
   i = (int)x;
   if ((double)i != x)
     *year = i + 1;
   else
    {
      *year = i;
      i--;
    }
   if (mjd > jdays)
    {
      /*
         Correction for Gregorian years.
      */
      mjd -= (Ulint)((*year / 400) - (greg->year / 400));
      mjd += (Ulint)((*year / 100) - (greg->year / 100));
      x = (double)mjd / (DAY_LAST + 0.25);
      i = (int)x;
      if ((double)i != x)
        *year = i + 1;
      else
       {
         *year = i;
         i--;
       }
      if (   (*year % 400)
          && !(*year % 100))
        mjd--;
    }
   i = (int)(mjd - (Ulint)(i * (DAY_LAST + 0.25)));
   /*
      Correction for Gregorian centuries.
   */
   if (   (*year > greg->year)
       && (*year % 400)
       && !(*year % 100)
       && (i < ((*year/100)-(greg->year/100))-((*year/400)-(greg->year/400))))
     i++;
   (void)doy2date (i, (days_of_february (*year)==29), day, month);
}



   PUBLIC Bool
doy2date (doy, is_leap_year, day, month)
         int  doy;
   const int  is_leap_year;
         int *day;
         int *month;
/*
   Converts a given number of days of a year to a standard date
     (returned in `&day' and `&month') and returns:
       TRUE in case the `day_of_year' number is valid;
       FALSE otherwise.
*/
{
   register int   i;
   auto     Bool  decrement_date;


   if (   doy > DAY_LAST+is_leap_year
       || doy < DAY_MIN)
     return(FALSE);
   decrement_date = (Bool)(   is_leap_year
                           && (doy > mvec[2]));
   if (decrement_date)
     doy--;
   for (i=MONTH_MIN ; i < MONTH_MAX ; i++)
    {
      doy -= dvec[i-1];
      if (doy <= 0)
       {
         doy += dvec[i-1];
         break;
       }
    }
   *month = i;
   *day = doy;
   if (   decrement_date
       && (*month == 2)
       && (*day == 28))
     (*day)++;

   return(TRUE);
   /*
      Alternative floating-point algorithm for managing this stuff:
      ...
      if (doy-is_leap_year > mvec[2])
       {
         month = (int)((63 + doy - is_leap_year) / 30.61) - 1;
         day = (int)(63 + doy - is_leap_year - abs((int)((month + 1) * 30.61)));
       }
      else
       {
         month = (int)((428 + doy) / 30.61) - 13;
         day = (int)(428 + doy - abs((int)((month + 13) * 30.61)));
       }
      ...
   */
}



   PUBLIC int
weekday_of_date (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the weekday of a Gregorian/Julian calendar date
     (month must be 1...12) and returns 1...7 (1==mo, 2==tu...7==su).
*/
{
   auto Ulint  mjd=date2num (day, month, year)%DAY_MAX;

   return((mjd > 2) ? (int)mjd-2 : (int)mjd+5);
}



   PUBLIC int
day_of_year (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the day of the year of a Gregorian or Julian calendar date
     (month must be 1...12) and returns 1...365|366.
*/
{
   register int  i;


   if (month < 3)
     i = mvec[month-1] + day;
   else
     i = mvec[month-1] + day + (days_of_february (year) == 29);

   return(i);
}



   PUBLIC int
days_of_february (year)
   const int year;
/*
   Returns the number of days in February --- respecting the Gregorian
     Reformation period likewise the leap year rule as used by the
     Eastern Orthodox churches.
*/
{
   register int  day;


   if (   (year > greg->year)
       || (   (year == greg->year)
           && (   greg->month == 1
               || (   (greg->month == 2)
                   && (greg->last_day >= 28)))))
    {
      if (orthodox_calendar)
        day = (year & 3) ? 28 : ((!(year % 100)) ? (((year % 9) == 2 || (year % 9) == 6) ? 29 : 28) : 29);
      else
        day = (year & 3) ? 28 : ((!(year % 100) && (year % 400)) ? 28 : 29);
    }
   else
     day = (year & 3) ? 28 : 29;
   /*
      Exception, the year 4 AD was historically NO leap year!
   */
   if (year == 4)
     day--;

   return(day);
}



   PUBLIC Bool
valid_date (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Checks whether a delivered date is valid.
*/
{
   if (   day < 0
       || month < MONTH_MIN
       || month > MONTH_MAX
       || (   (month != 2)
           && (day > dvec[month-1]))
       || (   (month == 2)
           && (day > days_of_february (year))))
     return(FALSE);

   return(TRUE);
}



   PUBLIC Bool
prev_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets a delivered date back by one day (to yesterday's date)
     respecting the missing period of the Gregorian Reformation.
     Returns FALSE in case a date was within the missing period
       of Gregorian Reformation, otherwise TRUE;
*/
{
   auto Bool  no_missing_date=TRUE;


   if (   (*day <= greg->last_day+1)
       && (*day >= greg->first_day)
       && (*month == greg->month)
       && (*year == greg->year))
    {
      no_missing_date = FALSE;
      *day = greg->first_day - 1;
    }
   else
    {
      (*day)--;
      if (   !*day
          || !valid_date (*day, *month, *year))
       {
         (*month)--;
         if (*month < MONTH_MIN)
          {
            *month = MONTH_MAX;
            (*year)--;
          }
         if (*month == 2)
           *day = days_of_february (*year);
         else
           *day = dvec[*month-1];
       }
    }

   return(no_missing_date);
}



   PUBLIC Bool
next_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets the delivered date forwards by one day (to tomorrow's date)
     respecting the missing period of the Gregorian Reformation.
     Returns FALSE in case a date was in the missing period
       of Gregorian Reformation, otherwise TRUE.
*/
{
   auto Bool  no_missing_date=TRUE;


   if (   (*day >= greg->first_day-1)
       && (*day <= greg->last_day)
       && (*month == greg->month)
       && (*year == greg->year))
    {
      no_missing_date = FALSE;
      *day = greg->last_day + 1;
    }
   else
    {
      (*day)++;
      if (!valid_date (*day, *month, *year))
       {
         *day = DAY_MIN;
         if (*month == MONTH_MAX)
          {
            *month = MONTH_MIN;
            (*year)++;
          }
         else
           (*month)++;
       }
    }

   return(no_missing_date);
}



   PUBLIC int
week_number (day, month, year, is_iso_week, start_day_of_week)
   const int  day;
   const int  month;
   const int  year;
   const Bool is_iso_week;
   const int  start_day_of_week;
/*
   Returns either a ISO-8601:1988 standard week number of the given date
     if the `is_iso_week' variable is TRUE, or a special value for marking
     a special event, which can be managed by the caller in a special way
     if needed!  Note that an ISO week starts on a Monday(=1) and ends on
     a Sunday(=7), and the first week of a year is the one which includes
     the first Thursday; equivalently, the one which includes 4th January.
     If the `is_iso_week' variable is FALSE, either return a non-ISO week
     number of the given date, or a special or a special value for marking
     a special event, which can be managed by the caller in a special way
     if needed!  Note that a non-ISO week can start on any weekday, and the
     first week of a year is the one which includes the first starting day
     of the week. The function return values are:
     -(WEEK_MAX+1) : Date appears both in 53rd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
     -WEEK_MAX     : Date appears both in 52nd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
     -(WEEK_MAX-1) : Date appears both in 51rd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
                       This special event only happens in the December of
                       the year of Gregorian Reformation or in the January of
                       the year after the year of Gregorian Reformation!
      0            : Date appears both in 53rd (=last) week of previous year
                       and first days of year, AND these first days of year
                       are IN the first week of year.
      1...WEEK_MAX : Date appears in the year.
*/
{
   register int  i=raw_week_number (day, month, year, is_iso_week, start_day_of_week);
   register int  j;
   register int  wmax=WEEK_MAX;


   if (   (i > 1)
       && (year == greg->year))
     wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
            - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
   if (i >= wmax)
    {
      if (month == MONTH_MIN)
       {
         j = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1, is_iso_week, start_day_of_week);
         if (i == j)
           i = -i;
         else
           i = 0;
       }
      else
       {
         j = raw_week_number (DAY_MIN, MONTH_MIN, year+1, is_iso_week, start_day_of_week);
         if (i == j)
           i = -i;
         else
           if (   (j == 1)
               && (i == wmax+1))
             i = 0;
       }
    }
   else
     if (i == 1)
      {
        if (   (   is_iso_week
                && (weekday_of_date (DAY_MIN, MONTH_MIN, year) <= 4))
            || (   !is_iso_week
                && (weekday_of_date (DAY_MIN, MONTH_MIN, year) <= start_day_of_week)))
         {
           j = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1, is_iso_week, start_day_of_week);
           if (year-1 == greg->year)
             wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
                    - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
           if (j != wmax)
             i = 0;
         }
      }
     else
       if (   (i == wmax-1)
           && (month == MONTH_MIN)
           && (year == greg->year+1))
        i = -i;

   return(i);
}



   PUBLIC int
weekno2doy (week, year, is_iso_week, start_day_of_week)
         int  week;
   const int  year;
   const Bool is_iso_week;
   const int  start_day_of_week;
/*
   Returns the "day_of_year" number of a Julian or Gregorian calendar year,
     the given week number (either ISO-8601:1988 or non-ISO) starts at.
     Week number may be:
       (a)  0           == Returns day_of_year number of first week of year.
                             resp., that dates, which occur both in last week
                             of previous year and first week of year
                             (in this case, the function return value is
                              -5...0 indicating how many days+1 are in
                              the previous year).
       (b)  1...52      == Returns day_of_year number always.
       (c) 53           == Returns day_of_year_number or if year has
                             NO 53rd week, returns -WEEK_MAX [=special value]).
       (d) 99           == Returns day_of_year number of last week of year.
     Return values are:
           -WEEK_MAX    == Event (c) has occurred and year has NO 53rd week.
           -5...0       == In case event (a) has occurred and the first days
                             of year occur both in last week of previous year
                             and first week of year.
            1...365|366 == Events (b), (c) and (d).
*/
{
   register int  wd=weekday_of_date (DAY_MIN, MONTH_MIN, year);
   register int  ww=week_number (DAY_MIN, MONTH_MIN, year, is_iso_week, start_day_of_week);
   register int  wm=week_number (dvec[MONTH_MAX-1], MONTH_MAX, year, is_iso_week, start_day_of_week);
   register int  wmax=WEEK_MAX;
   register int  i=DAY_MIN;


   if (wm < 0)
     wm = -wm;
   if (   ww == -(wmax - 1)
       || year == greg->year)
     wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
            - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
   if (ww < 0)
     ww = -ww;
   if (ww > 1)
     i += SDAY(DAY_MAX-wd+1, start_day_of_week);
   else
     if (!ww)
       i -= SDAY(wd-1, start_day_of_week);
   if (   wm
       && (wm != wmax+1)
       && (week == wmax+1))
     return(-WEEK_MAX);
   else
     if (week == 99)
      {
        week = wm;
        if (!wm)
          week = wmax + 1;
        i += (((week - 1) * DAY_MAX));
      }
     else
      {
        i += (((week - 1) * DAY_MAX));
        if (   !week
            && (i <= -(DAY_MAX-1)))
          i += DAY_MAX;
      }
   /*
      Correction in case week occurs during Gregorian Reformation period.
   */
   if (year == greg->year)
     if (i+(greg->last_day-greg->first_day+1) > day_of_year (greg->last_day, greg->month, greg->year))
       i += (greg->last_day - greg->first_day + 1);
   if (i > DAY_LAST+(days_of_february (year)==29))
     return(-WEEK_MAX);

   return(i);
}



   PUBLIC int
knuth_easter_formula (year)
   const int year;
/*
   This procedure calculates the day and month of Easter giving the year.
   It returns the "actual day_of_year date" of Western Eastern (not the
   Eastern Easter of the Eastern Orthodox churches) after AD 463.
   ***
   *** ANNOTATION by Thomas Esken <esken@uni-muenster.de>, 12-DEC-1996
   ***
   *** But in fact, this procedure works correctly for
   *** calculating the Easter Sunday's date after AD 29.
   *** (Ref.: Hans Lietzmann, Zeitrechnung, De gryter 1984, 4te Auflage)
   ***
   `golden_number' is the number of the year in the Metonic cycle, used
   to determine the position of the calendar Moon.
   `gregorian_correction' is the number of preceding years like 1700, 1800,
   1900 when leap year was not held.
   `clavian_correction' is a correction for the Metonic cycle of about
   8 days every 2500 years.
   `epact' is the age of the calendar Moon at the beginning of the year.
   `extra_days' specifies when Sunday occurs in March.  `epact' specifies
   when Full Moon occurs.  Easter is the first Sunday following the first
   Full Moon which occus on or after March 21.  (Ref.: De Morgan, A Budget
   of Paradoxes)
   This basic Algorithm was given by Don Knuth in CACM 5(1962), 209-210,
   and is altered accordingly for managing different Gregorian Reformation
   periods.
*/
{
   auto     long  extra_days;
   register int   golden_number=(year%19)+1;
   register int   century;
   register int   gregorian_correction;
   register int   clavian_correction;
   register int   epact;
   register int   day;
   register int   month;
   register int   easter;
   auto     Bool  was_julian=FALSE;


   if (   (year > greg->year)
       || (   (year == greg->year)
           && (   greg->month < 3
               || (   (greg->month == 3)
                   && (greg->last_day < 22)))))
    {
      /*
         Computation for Gregorian years.
      */
LABEL_compute_gregorian:
      century = year / 100 + 1;
      gregorian_correction = (3 * century / 4) - 12;
      clavian_correction = ((8 * century + 5) / 25) - 5;
      extra_days = (5L * year / 4) - gregorian_correction - 10;
      epact = (11 * golden_number + 20 + clavian_correction - gregorian_correction) % 30;
      /*
         BTW, the earliest year the calculated epact is negative or null
           is the year 1710!
         (see chapter 1.3.2 "THE MIX ASSEMBLY LANGUAGE", page 155,
         question 15 of Don Knuth's "The Art of Computer Programming", Vol. 1)
      */
      if (epact <= 0)
        epact += 30;
      if (   (   (epact == 25)
              && (golden_number > 11))
          || epact == 24)
        epact++;
    }
   else
    {
      /*
         Computation for Julian calendar years.
      */
      was_julian = TRUE;
      extra_days = (5L * year) / 4;
      epact = ((11 * golden_number - 4) % 30) + 1;
    }
   day = 44 - epact;
   if (day < 21)
     day += 30;
   day += DAY_MAX - (int)((extra_days + day) % DAY_MAX);
   if (day > MONTH_LAST)
    {
      day -= MONTH_LAST;
      month = 4;
    }
   else
     month = 3;
   easter = day_of_year (day, month, year);
   /*
      Correction for potential Reformation period which occurred
        during the period Easter Sunday may take place (22-MAR...25-APR).
        I assume if such a Reformation periods happens, the proper
        solution for computing Easter Sunday's date has to be
        done for the Gregorian year instead of the Julian year.
   */
   if (   was_julian
       && (year == greg->year)
       && (   greg->month == 3
           || (   (greg->month == 4)
               && (greg->last_day <= 25))))
    {
      was_julian = FALSE;
      if (   (easter >= day_of_year (greg->first_day, greg->month, greg->year))
          && (easter <= day_of_year (greg->last_day, greg->month, greg->year)))
        goto LABEL_compute_gregorian;
    }

   return(easter);
}



   PUBLIC int
julian_gregorian_diff (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the day difference between the given Gregorian calendar date
     and the according Julian calendar date, i.e. the amount of days,
     the Julian calendar is past the Gregorian calendar.
*/
{
   auto Ulint  mjd=(Ulint)((year-1)*(Ulint)(DAY_LAST)+((year-1)>>2));


   mjd += (Ulint)(mvec[month-1] + day);
   if (   (days_of_february (year) == 29)
       && (month > 2))
     mjd++;

   return((int)(mjd-date2num (day, month, year)));
}



   LOCAL void
gregorian2julian (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Converts a Gregorian date to a Julian date.
*/
{
   if (   *year < greg->year
       || (   (*year == greg->year)
           && (   *month < greg->month
               || (   (*month == greg->month)
                   && (*day < greg->first_day)))))
    {
      auto     Greg_struct  tmp_greg;
      auto     Ulint        mjd;
      register int          diff;


      /*
         So first we need to buffer the Gregorian Reformation period actually used.
      */
      tmp_greg.first_day = greg->first_day;
      tmp_greg.last_day = greg->last_day;
      tmp_greg.month = greg->month;
      tmp_greg.year = greg->year;
      /*
         Then we set the Gregorian Reformation period to the date
           it has happened historically (this was 05-Oct-1582...14-Oct-1582).
      */
      greg->first_day = 5;
      greg->last_day = 14;
      greg->month = 10;
      greg->year = 1582;
      /*
         Now let's adjust the date.
      */
      mjd = date2num (*day, *month, *year);
      diff = julian_gregorian_diff (*day, *month, *year);
      num2date (mjd-diff, day, month, year);
      /*
         And restore the buffered Gregorian Reformation date actually used.
      */
      greg->year = tmp_greg.year;
      greg->month = tmp_greg.month;
      greg->last_day = tmp_greg.last_day;
      greg->first_day = tmp_greg.first_day;
    }
}



   LOCAL int
raw_week_number (day, month, year, is_iso_week, start_day_of_week)
   const int  day;
   const int  month;
   const int  year;
   const Bool is_iso_week;
   const int  start_day_of_week;
/*
   Returns either the raw ISO-8601:1988 standard week number of the given
     date if the `is_iso_week' variable is TRUE.  Note that an ISO week
     starts on a Monday(=1) and ends on a Sunday(=7), and the first week
     of a year is the one which includes the first Thursday; equivalently,
     the one which includes 4th January.
     If the `is_iso_week' variable is FALSE, return a raw non-ISO week
     number of the given date.  Note that a non-ISO week can start on any
     weekday, and the first week of a year is the one which includes the
     first starting day of the week.
*/
{
   register int  jd=day_of_year (day, month, year);
   register int  sd=weekday_of_date (DAY_MIN, MONTH_MIN, year);
   register int  ww;


   if (   (year == greg->year)
       && (   month > greg->month
           || (   (month == greg->month)
               && (day > greg->last_day))))
     jd -= (greg->last_day - greg->first_day + 1);
   if (is_iso_week)
    {
      /*
         Correct the computed day_of_year number of delivered date.
      */
      if (sd > 4)
        /*
           Starting day of year is Friday...Sunday.
        */
        jd -= ((DAY_MAX - sd) + 1);
      else
        /*
           Starting day of year is Monday...Thursday.
        */
        jd += (sd - 1);
    }
   else
    {
      /*
         Correct the computed day_of_year number of delivered date.
      */
      if (sd > start_day_of_week)
        /*
           Starting day of year is greater the starting day of week.
        */
        jd -= (DAY_MAX - (sd - start_day_of_week));
      else
        /*
           Starting day of year is less than the starting day of week.
        */
        jd += (sd - start_day_of_week);
    }
   /*
      Compute week number of delivered date.
   */
   if (jd > 0)
     /*
        Date has consecutive week number of current year: Compute it.
     */
     ww = (jd - 1) / DAY_MAX + 1;
   else
     /*
        Date has week number of last week of previous year:
          Detect that week number by calling this function recursivly  ;)
     */
     ww = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1, is_iso_week, start_day_of_week);

   return(ww);
}



   LOCAL const char *
dflt_day_name (day)
   const int day;
/*
   Returns the complete default (==English) name of the day
     (needed by the `compare_d_m_name()' function if another native language
     other than English must be supported).
*/
{
   static const char  *name[]={
                                "invalid day",
                                "Monday", "Tuesday",  "Wednesday", "Thursday",
                                "Friday", "Saturday", "Sunday"
                              };

   return(((day < DAY_MIN) || (day > DAY_MAX)) ? name[0] : name[day]);
}



   LOCAL const char *
dflt_month_name (month)
   const int month;
/*
   Returns the complete default (==English) name of the month
     (needed by the `compare_d_m_name()' function if another native lanuage
     other than English must be supported).
*/
{
   static const char  *name[]={
                                "invalid month",
                                "January", "February", "March",
                                "April",   "May",      "June",
                                "July",    "August",   "September",
                                "October", "November", "December"
                              };

   return(((month < MONTH_MIN) || (month > MONTH_MAX)) ? name[0] : name[month]);
}
