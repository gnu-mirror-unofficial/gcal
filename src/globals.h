#ifndef __GLOBALS_H
#  define __GLOBALS_H
/*
*  globals.h:  Declarations of all GLOBAL common variables used
*              (except all common variables that depend to the RC section).
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
*  $Id: globals.h 3.00 2000/03/26 03:00:00 tom Exp $
*/



/*
*  GLOBAL variables declarations.
*/
/* The table of supported date formats. */
IMPORT Df_struct  supported_date_format[];

/* Points to the used date format. */
IMPORT Df_struct  *date_format;

/* The table of long-style options. */
IMPORT const Lopt_struct  lopt[];

/* The table of Gregorian Reformation dates. */
IMPORT Greg_struct  greg_reform_date[];

/* Points to the used Gregorian Reformation date. */
IMPORT Greg_struct  *greg;

/* Effective hls 1 end (current day). */
IMPORT Hls_struct  ehls1e;

/* Effective hls 1 start (current day). */
IMPORT Hls_struct  ehls1s;

/* Effective hls 2 end (holiday). */
IMPORT Hls_struct  ehls2e;

/* Effective hls 2 start (holiday). */
IMPORT Hls_struct  ehls2s;

/* Used if a list/range of months/years is given. */
IMPORT Ml_struct  *month_list;

/* Actual size of all string vectors. */
IMPORT Uint  maxlen_max;

/* Amount of days in months. */
IMPORT const int  dvec[];

/* Number of past days of month. */
IMPORT const int  mvec[];

/* Actual day. */
IMPORT int  act_day;

/* Actual hour. */
IMPORT int  act_hour;

/* Actual minute. */
IMPORT int  act_min;

/* Actual month. */
IMPORT int  act_month;

/* Actual second. */
IMPORT int  act_sec;

/* Actual year. */
IMPORT int  act_year;

/* Buffer of actual day. */
IMPORT int  buf_ad;

/* Buffer of actual month. */
IMPORT int  buf_am;

/* Buffer of actual year. */
IMPORT int  buf_ay;

/* True actual day as reported by the operating system. */
IMPORT int  true_day;

/* True actual month as reported by the operating system. */
IMPORT int  true_month;

/* True actual year as reported by the operating system. */
IMPORT int  true_year;

/* Current day. */
IMPORT int  day;

/* Starting month of a fiscal year. */
IMPORT int  fiscal_month;

/* Format length of a standard/special/both day. */
IMPORT int  format_len;

/* Is current year a leap year? */
IMPORT int  is_leap_year;

/* Is output directed to channel 1? */
IMPORT int  is_tty1;

/* Is output directed to channel 2? */
IMPORT int  is_tty2;

/* Is output displayed on a terminal? */
IMPORT int  is_tty;

/* Maximum string length of a textual day_name(). */
IMPORT int  len_dayname_max;

/* Maximum string length of a textual month_name(). */
IMPORT int  len_monthname_max;

/* Maximum string length of a textual day_suffix() [if any]. */
IMPORT int  len_suffix_max;

/* String length of the maximum year able to compute. */
IMPORT int  len_year_max;

/* Current month. */
IMPORT int  month;

/* Number of month columns of a year calendar. */
IMPORT int  out_cols;

/* Number of month rows of a year calendar. */
IMPORT int  out_rows;

/* `-s<0,1...7|day name>'. */
IMPORT int  start_day;

/* `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
IMPORT int  time_hour_offset;

/* `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
IMPORT int  time_min_offset;

/* `--transform-year=BASE_YEAR'. */
IMPORT int  transform_year;

/* `--debug[=0...WARN_LVL_MAX]'. */
IMPORT int  warning_level;

/* Current year. */
IMPORT int  year;

/* Vector of holiday dates (legal days). */
IMPORT char  hd_ldays[];

/* `--cc-holidays=CC[+CC+...]'. */
IMPORT char  *cc;

/* The "DD" text. */
IMPORT char  *dd_lit;

/* The "ARG" text. */
IMPORT char  *larg_lit;

/* The "MM" text. */
IMPORT char  *mm_lit;

/* Stores the actual program name. */
IMPORT char  *prgr_name;

/* General purpose text buffer 1. */
IMPORT char  *s1;

/* General purpose text buffer 2. */
IMPORT char  *s2;

/* General purpose text buffer 3. */
IMPORT char  *s3;

/* General purpose text buffer 4. */
IMPORT char  *s4;

/* Character for separating HH:MM time values. */
IMPORT char  *time_sep;

/* `--translate-string=CHARACTER_PAIR...'. */
IMPORT char  *translate_string;

/* Pointer to the $TZ (timezone) environment variable. */
IMPORT char  *tz;

/* The "WWW" text. */
IMPORT char  *www_lit;

/* The "YY" text. */
IMPORT char  *yy_lit;

/* The "YYYY" text. */
IMPORT char  *yyyy_lit;

/* `-jb'. */
IMPORT Bool  cal_both_dates_flag;

/* `-j'. */
IMPORT Bool  cal_special_flag;

/* `-K'. */
IMPORT Bool  cal_with_week_number;

/* Use Astronomical holidays. */
IMPORT Bool  hdy_astronomical;

/* Use Bahai calendar holidays. */
IMPORT Bool  hdy_bahai;

/* Use Celtic calendar holidays. */
IMPORT Bool  hdy_celtic;

/* Use Chinese calendar holidays. */
IMPORT Bool  hdy_chinese;

/* Use Chinese flexible calendar holidays. */
IMPORT Bool  hdy_chinese_flexible;

/* Use Christian Western churches calendar holidays. */
IMPORT Bool  hdy_christian;

/* Use Hebrew calendar holidays. */
IMPORT Bool  hdy_hebrew;

/* Use Islamic CIVIL calendar holidays. */
IMPORT Bool  hdy_islamic;

/* Use Japanese calendar holidays. */
IMPORT Bool  hdy_japanese;

/* Use Japanese flexible calendar holidays. */
IMPORT Bool  hdy_japanese_flexible;

/* Use Multicultural New_Year's_Day holidays. */
IMPORT Bool  hdy_multicultural_new_year;

/* Use Orthodox Christian Eastern churches NEW calendar holidays. */
IMPORT Bool  hdy_orthodox_new;

/* Use Orthodox Christian Eastern churches OLD calendar holidays. */
IMPORT Bool  hdy_orthodox_old;

/* Use Persian Jalaali calendar holidays. */
IMPORT Bool  hdy_persian;

/* Use Zodiacal Marker holidays. */
IMPORT Bool  hdy_zodiacal_marker;

/* Use Bahai calendar months. */
IMPORT Bool  mth_bahai;

/* Use Chinese calendar months. */
IMPORT Bool  mth_chinese;

/* Use Chinese flexible calendar months. */
IMPORT Bool  mth_chinese_flexible;

/* Use Coptic calendar months. */
IMPORT Bool  mth_coptic;

/* Use Ethiopic calendar months. */
IMPORT Bool  mth_ethiopic;

/* Use French Revolutionary calendar months. */
IMPORT Bool  mth_french_revolutionary;

/* Use Hebrew calendar months. */
IMPORT Bool  mth_hebrew;

/* Use Indian CIVIL calendar months. */
IMPORT Bool  mth_indian_civil;

/* Use Islamic CIVIL calendar months. */
IMPORT Bool  mth_islamic;

/* Use Japanese calendar months. */
IMPORT Bool  mth_japanese;

/* Use Japanese flexible calendar months. */
IMPORT Bool  mth_japanese_flexible;

/* Use Old-Armenic calendar months. */
IMPORT Bool  mth_old_armenic;

/* Use Old-Egyptic calendar months. */
IMPORT Bool  mth_old_egyptic;

/* Use Persian Jalaali calendar months. */
IMPORT Bool  mth_persian;

/* Must we emulate the highlighting sequences? */
IMPORT Bool  emu_hls;

/* `-jnb'. */
IMPORT Bool  hd_both_dates_flag;

/* `-N'. */
IMPORT Bool  hd_legal_days_only;

/* `-n|N-'. */
IMPORT Bool  hd_sort_des_flag;

/* `-jn'. */
IMPORT Bool  hd_special_flag;

/* `-G'. */
IMPORT Bool  hd_suppr_list_sep_flag;

/* `-X'. */
IMPORT Bool  hd_title_flag;

/* `-H<yes>|<no>'. */
IMPORT Bool  highlight_flag;

/* `-n|N'. */
IMPORT Bool  holiday_flag;

/* Argument is `..' -> current quarter of actual year. */
IMPORT Bool  is_3month_mode2;

/* Argument is `.' or `.+' or `.-'. */
IMPORT Bool  is_3month_mode;

/* Is an extended list of months/years given? */
IMPORT Bool  is_ext_list;

/* Is an extended range of months/years given? */
IMPORT Bool  is_ext_range;

/* Is an extended list/range of years given? */
IMPORT Bool  is_ext_year;

/* `:' char found in argument (MM:YYYY). */
IMPORT Bool  is_fiscal_year;

/* Is a special range of a selected month of years given? */
IMPORT Bool  is_special_range;

/* Is a special range of selected months of years given? */
IMPORT Bool  is_multi_range;

/* `--iso-week-number=<yes>|<no>'. */
IMPORT Bool  iso_week_number;

/* `-O' (compute leap years as done by Eastern churches). */
IMPORT Bool  orthodox_calendar;

/* `-i'. */
IMPORT Bool  special_calsheet_flag;

/* `-u'. */
IMPORT Bool  suppr_cal_flag;

/* Day number leaded with zeroes format specifier given in date format? */
IMPORT Bool  use_day_zeroleaded;

/* 3 char day name format specifier given in date format? */
IMPORT Bool  use_short3_day_name;

/* Year number leaded with zeroes format specifier given in date format? */
IMPORT Bool  use_year_zeroleaded;



#  ifdef DJG
/* Set to SHRT_MAX for checking the maximum table range. */
IMPORT Usint  testval;
#  else /* !DJG */
/* Set to INT_MAX for checking the maximum table range. */
IMPORT Uint  testval;
#  endif /* !DJG */



#  ifdef GCAL_EMAIL
/* Temporary file which is send by the mailer. */
IMPORT FILE  *tfp;

/* Name of tempfile used by the mailer. */
IMPORT char  *tfn;

/* Email address Gcal's output is send to. */
IMPORT char  *email_adr;
#  endif /* GCAL_EMAIL */



#  ifdef GCAL_EPAGER
/* Name of external pager program. */
IMPORT char  *ext_pager;
#  endif /* GCAL_EPAGER */



#  ifdef GCAL_NLS
/* Support of English language? */
IMPORT Bool  is_en;
#  endif /* GCAL_NLS */



#  if USE_PAGER
/* Number of tty columns. */
IMPORT int  tty_cols;

/* Number of tty rows. */
IMPORT int  tty_rows;

/* `-p'. */
IMPORT Bool  pager_flag;
#  endif /* USE_PAGER */



#  if (USE_PAGER || USE_HLS) && defined(GCAL_TCAP)
#    if MUST_DCL_OSPEED && USE_HLS
/* Terminal output baud rate. */
IMPORT short  ospeed;

/*
   On Solaris2, sys/types.h #includes sys/reg.h, which #defines PC.
   Unfortunately, PC is a global variable used by the Termcap library.
*/
#      ifdef PC
#        undef PC
#      endif

/* Padding character. */
IMPORT char  PC;
#    endif /* MUST_DCL_OSPEED && USE_HLS */
#  endif /* (USE_PAGER || USE_HLS) && GCAL_TCAP */
#endif /* __GLOBALS_H */
