/*
*  help.c:  The help, version and license screens.
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
static char rcsid[]="$Id: help.c 3.01 2000/06/20 03:00:01 tom Exp $";
#endif



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_CTYPE_H
# include <ctype.h>
#endif
#include "common.h"
#if USE_RC
# include "rc-defs.h"
#endif /* USE_RC */
#include "globals.h"
#include "hd-defs.h"
#include "tty.h"
#include "utils.h"
#include "help.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `help.c'.
*/
static int
gmt_timezone_value __P_((int hour));
static const char *
get_longopt_description __P_((const int  longopt_symbolic,
                              const Bool with_larglist));
static void
my_bug_report_address __P_((FILE *fp));
static void
my_copyright __P_((      FILE *fp,
                   const Bool  with_short_licence));
static void
my_help_head_text __P_((FILE *fp));
static void
my_help_tail_text __P_((FILE *fp));
static void
print_compiler_info __P_((FILE *fp));
__END_DECLARATIONS



/*
*  static variables definitions.
*/
/* Compiler infotext. */
#if USE_DE
static const char *compiler_info[]={"Kompiliert mit %s%s f%sr %s%s%s%s"};
#else /* !USE_DE */
static const char *compiler_info[]={N_("Compiled with %s%s for %s%s%s%s")};
#endif /* !USE_DE */

/*
   The table used to inform about the supported date formats is a vector of
     `Di_struct' terminated by an element containing a `di_info' which is zero!
*/
static const Di_struct  info_date_format[]=
{
/*
  { char *di_info, char di_1format, char di_2format },
*/
#if USE_DE
  { "# Tagesnummer (mu"SZ" definiert werden)",                           DFORMAT_CHAR, DAYNR_CHAR },
  { "# Jahresnummer (mu"SZ" definiert werden)",                          DFORMAT_CHAR, YEARNR_CHAR },
  { "# Wochentagsname (darf definiert werden)",                          DFORMAT_CHAR, WDNAME_CHAR },
  { "Monat-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",        '\0',         '\0'},
  { "# Monatsnummer",                                                    DFORMAT_CHAR, MONTHNR_CHAR },
  { "# Monatsname",                                                      DFORMAT_CHAR, MONTHNAME_CHAR },
  { "Hervorhebung-Gruppe (alle Mitglieder m"UE"ssen definiert werden):", '\0',         '\0'},
  { "Start von Hervorhebungssequenz/Markierungszeichen",                 DFORMAT_CHAR, HLS1S_CHAR },
  { "Ende von Hervorhebungssequenz/Markierungszeichen",                  DFORMAT_CHAR, HLS1E_CHAR },
  { "Zeichenersetzung:",                                                 '\0',         '\0' },
  { "Leerzeichen ('%c')",                                                PSEUDO_BLANK, ' ' },
  { "Unterstrichzeichen ('%c')",                                         QUOTE_CHAR,   PSEUDO_BLANK },
  { "Prozentzeichen ('%c')",                                             QUOTE_CHAR,   DFORMAT_CHAR },
  { "Backslashzeichen ('%c')",                                           QUOTE_CHAR,   QUOTE_CHAR },
  { "Alle mit # markierten Formatelemente d"UE"rfen optional noch",      '\0',         '\0' },
  { "eine Formatanweisung enthalten, deren Schablone wie folgt ist:",    '\0',         '\0' },
  { "[AUSRICHTUNG [VORZEICHEN] [NULL] BREITE [STIL] [ANHANG] FORMAT]",   '\0',         '\0' },
  { "AUSRICHTUNG-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",  '\0',         '\0' },
  { "Feldinhalt linksb"UE"ndig stellen im Feld der Breite BREITE",       FLEFT_CHAR,   ' ' },
  { "Feldinhalt mittig stellen im Feld der Breite BREITE",               FCENTER_CHAR, ' ' },
  { "Feldinhalt rechtsb"UE"ndig stellen im Feld der Breite BREITE",      FRIGHT_CHAR,  ' ' },
  { "VORZEICHEN (darf definiert werden):",                               '\0',         '\0' },
  { "Numerischen Wert mit f"UE"hrendem Vorzeichen ausstatten",           FSIGN_CHAR,   ' ' },
  { "NULL (darf definiert werden):",                                     '\0',         '\0' },
  { "Numerischen Wert mit f"UE"hrenden Nullen auff"UE"llen",             FLZERO_CHAR,  ' ' },
  { "BREITE (mu"SZ" definiert werden):",                                 '\0',         '\0' },
  { "Feld hat die Breite N (%d...%d)",                                   'N',          '\0' },
  { "STIL-Gruppe (genau ein Mitglied darf definiert werden):",           '\0',         '\0' },
  { "Feldinhalt in Gro"SZ"buchstaben umwandeln",                         FUPPER_CHAR,  ' ' },
  { "Feldinhalt in Kleinbuchstaben umwandeln",                           FLOWER_CHAR,  ' ' },
  { "Feldinhalt in Wortgro"SZ"schreibung umwandeln",                     FWORD_CHAR,   ' ' },
  { "ANHANG (darf definiert werden):",                                   '\0',         '\0' },
  { "Numerischen Wert mit Ordnungszahlanhang ausstatten",                FSUFFIX_CHAR, ' ' },
  { "FORMAT-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",       '\0',         '\0' },
  { "Feldinhalt wird nicht nach Position BREITE abgeschnitten",          FVAR_CHAR,    ' ' },
  { "Feldinhalt wird nach Position BREITE abgeschnitten",                FFIX_CHAR,    ' ' },
#else /* !USE_DE */
  { N_("# Day number (must be defined)"),                              DFORMAT_CHAR, DAYNR_CHAR },
  { N_("# Year number (must be defined)"),                             DFORMAT_CHAR, YEARNR_CHAR },
  { N_("# Weekday name (may be defined)"),                             DFORMAT_CHAR, WDNAME_CHAR },
  { N_("Month group (exactly one member must be defined):"),           '\0',         '\0'},
  { N_("# Month number"),                                              DFORMAT_CHAR, MONTHNR_CHAR },
  { N_("# Month name"),                                                DFORMAT_CHAR, MONTHNAME_CHAR },
  { N_("Highlighting group (all members must be defined):"),           '\0',         '\0'},
  { N_("Start of highlighting sequence/marking character"),            DFORMAT_CHAR, HLS1S_CHAR },
  { N_("End of highlighting sequence/marking character"),              DFORMAT_CHAR, HLS1E_CHAR },
  { N_("Character replacement:"),                                      '\0',         '\0' },
  { N_("Space/blank ('%c') character"),                                PSEUDO_BLANK, ' ' },
  { N_("Underscore ('%c') character"),                                 QUOTE_CHAR,   PSEUDO_BLANK },
  { N_("Percent ('%c') character"),                                    QUOTE_CHAR,   DFORMAT_CHAR },
  { N_("Backslash ('%c') character"),                                  QUOTE_CHAR,   QUOTE_CHAR },
  { N_("All format elements marked by # may optionally"),              '\0',         '\0' },
  { N_("contain a format instruction, which template is like:"),       '\0',         '\0' },
  { N_("[ALIGNMENT [SIGN] [ZERO] WIDTH [STYLE] [SUFFIX] FORMAT]"),     '\0',         '\0' },
  { N_("ALIGNMENT group (exactly one member must be defined):"),       '\0',         '\0' },
  { N_("Field contents placed at the left margin using width WIDTH"),  FLEFT_CHAR,   ' ' },
  { N_("Field contents placed in centered manner using width WIDTH"),  FCENTER_CHAR, ' ' },
  { N_("Field contents placed at the right margin using width WIDTH"), FRIGHT_CHAR,  ' ' },
  { N_("SIGN (may be defined):"),                                      '\0',         '\0' },
  { N_("Numerical value is provided with leading sign"),               FSIGN_CHAR,   ' ' },
  { N_("ZERO (may be defined):"),                                      '\0',         '\0' },
  { N_("Numerical value is filled with leading zero(es)"),             FLZERO_CHAR,  ' ' },
  { N_("WIDTH (must be defined):"),                                    '\0',         '\0' },
  { N_("Field has the width N (%d...%d)"),                             'N',          '\0' },
  { N_("STYLE group (exactly one member may be defined):"),            '\0',         '\0' },
  { N_("Field contents is converted to upper-case letters"),           FUPPER_CHAR,  ' ' },
  { N_("Field contents is converted to lower-case letters"),           FLOWER_CHAR,  ' ' },
  { N_("Field contents is converted to capitalized words"),            FWORD_CHAR,   ' ' },
  { N_("SUFFIX (may be defined):"),                                    '\0',         '\0' },
  { N_("Numerical value is provided with an ordinal number suffix"),   FSUFFIX_CHAR, ' ' },
  { N_("FORMAT group (exactly one member must be defined):"),          '\0',         '\0' },
  { N_("Field contents is not cut after position WIDTH"),              FVAR_CHAR,    ' ' },
  { N_("Field contents is cut after position WIDTH"),                  FFIX_CHAR,    ' ' },
#endif /* !USE_DE */
  { NULL, '\0', '\0' }
};

#if USE_RC
/*
   The table used to inform about the characters used in the
     %? inclusive/exclusive day special texts is a vector of
     `Ed_struct' terminated by an element containing an `ed_info' which is zero!
*/
static const Ed_struct  info_exclusive_day[]=
{
/*
  { char ed_id, char *ed_info },
*/
# if USE_DE
  { RC_EX_LHDY_CHAR,      "gesetzlichen Feiertage" },
  { RC_EX_AHDY_CHAR,      "Feiertage" },
  { RC_EX_MON_CHAR,       "Montage" },
  { RC_EX_TUE_CHAR,       "Dienstage" },
  { RC_EX_WED_CHAR,       "Mittwoche" },
  { RC_EX_THU_CHAR,       "Donnerstage" },
  { RC_EX_FRI_CHAR,       "Freitage" },
  { RC_EX_SAT_CHAR,       "Samstage" },
  { RC_EX_SUN_CHAR,       "Sonntage" },
  { RC_EX_MON_2_THU_CHAR, "Montage...Donnerstage" },
  { RC_EX_MON_2_FRI_CHAR, "Montage...Freitage" },
# else /* !USE_DE */
  { RC_EX_LHDY_CHAR,      N_("legal holidays") },
  { RC_EX_AHDY_CHAR,      N_("holidays") },
  { RC_EX_MON_CHAR,       N_("Mondays") },
  { RC_EX_TUE_CHAR,       N_("Tuesdays") },
  { RC_EX_WED_CHAR,       N_("Wednesdays") },
  { RC_EX_THU_CHAR,       N_("Thursdays") },
  { RC_EX_FRI_CHAR,       N_("Fridays") },
  { RC_EX_SAT_CHAR,       N_("Saturdays") },
  { RC_EX_SUN_CHAR,       N_("Sundays") },
  { RC_EX_MON_2_THU_CHAR, N_("Mondays...Thursdays") },
  { RC_EX_MON_2_FRI_CHAR, N_("Mondays...Fridays") },
# endif /* !USE_DE */
  { '\0', NULL }
};
#endif



/*
*  Function implementations.
*/
    void
my_help_on_help (fp, longopt, lopt_ambig, cols)
         FILE *fp;
   const char *longopt;
   const Bool  lopt_ambig;
   const int   cols;
/*
   Prints the "help on help" text to file `fp' using the central output
     function `print_text()', and uses the global text buffer `s1' internally.
     If delivered `lopt_ambig' is TRUE, only display the names of all
     longoptions stored in `lopt' structure member `long_name', which
     are equal to delivered `longopt'.
*/
{
   auto     const Lopt_struct  *ptr_lopt=lopt;
   register const int           filler_blanks=2;
   register const int           len_longopt=(int)strlen(longopt);
   register       int           len_line;
   register       int           len_string;


#if USE_DE
   sprintf(s1, "%s: Benutze `%s %s' mit einem dieser Argumente",
           prgr_name, prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE));
#else /* !USE_DE */
   sprintf(s1, _("%s: Use `%s %s' with one of these arguments"),
           prgr_name, prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE));
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   for (len_line=0 ; len_line < filler_blanks ; len_line++)
     s1[len_line] = ' ';
   s1[len_line] = '\0';
   if (lopt_ambig)
    {
      while (tolower(*ptr_lopt->long_name) < tolower(*longopt))
        ptr_lopt++;
      while (strncasecmp(longopt+1, ptr_lopt->long_name+1, len_longopt-1))
        ptr_lopt++;
    }
   while (ptr_lopt->long_name != NULL)
    {
      len_string = (int)strlen(ptr_lopt->long_name) + 1;
      if ((ptr_lopt+1)->long_name != NULL)
        len_string++;
      if (len_line + len_string >= cols)
       {
         print_text (fp, s1);
         for (len_line=0 ; len_line < filler_blanks ; len_line++)
           s1[len_line] = ' ';
         s1[len_line] = '\0';
       }
      strcat(s1, " ");
      strcat(s1, ptr_lopt->long_name);
      if ((ptr_lopt+1)->long_name != NULL)
       {
         if (   lopt_ambig
             && strncasecmp(longopt, (ptr_lopt+1)->long_name, len_longopt))
           break;
         strcat(s1, ",");
         len_line += len_string;
       }
      ptr_lopt++;
    }
   print_text (fp, s1);
}



    void
my_extended_help (fp, longopt_symbolic)
         FILE *fp;
   const int   longopt_symbolic;
/*
   Prints the "extended help" text to file `fp' using the central
     output function `print_text()', and uses the global text buffers
     `s1' and `s2' internally.
     If `longopt_symbolic' is set to SYM_NIL, the complete text
     is shown, otherwise the text corresponding to SYM_??? only!
*/
{
#if USE_RC
   auto const Ed_struct    *ptr_info_exclusive_day=info_exclusive_day;
#endif
   auto const Cc_struct    *ptr_cc_holidays=cc_holidays;
   auto       Df_struct    *ptr_date_format=supported_date_format;
   auto const Di_struct    *ptr_info_date_format=info_date_format;
   auto       Greg_struct  *ptr_greg=greg_reform_date;
   auto       char         *ptr_env;


   if (longopt_symbolic == SYM_NIL)
    {
      my_help_head_text (fp);
      print_text (fp, s1);
      strcpy(s1, _("OPTION"));
      print_text (fp, s1);
    }
#if USE_DE
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HELP:
        sprintf(s1, "%s?        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%sh        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Hilfetext ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LONG_HELP1:
      case SYM_LONG_HELP2:
        sprintf(s1, "%s??       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%shh       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LONG_HELP2, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Erweiterten Hilfetext ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LICENSE1:
      case SYM_LICENSE2:
      case SYM_LICENSE3:
        sprintf(s1, "%sL        %s", SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE2, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE3, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Softwarelizenz ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_VERSION:
        sprintf(s1, "%sV        %s", SWITCH, get_longopt_description (SYM_VERSION, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Versionsinformation ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXIT_STAT_HELP_NON_ZERO:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXIT_STAT_HELP_NON_ZERO, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Setze Programm EXIT Status auf %d bei `%s' etc.",
                ERR_EXIT_INFO_TEXTS_NON_ZERO, get_longopt_description (SYM_HELP, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_RESPONSE_FILE:
        sprintf(s1, "%sR %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_RESPONSE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Antwortdatei f%sr die `%cDATEI' Option erzeugen", UE, RSP_CHAR);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Schreibe Argumente der Kommandozeile in Datei %s", larg_lit, larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# ifdef GCAL_SHELL
      case SYM_SCRIPT_FILE:
        sprintf(s1, "%sS %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_SCRIPT_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Shell script mit den Argumenten der Kommandozeile erzeugen");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Dateiname des shell scripts", larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
# if USE_RC
      case SYM_DATE_VARIABLE:
        sprintf(s1, "%sv %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_DATE_VARIABLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Globale Datumvariable(n) \"DVAR->a...d|f...s|u...|z\" definieren");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = \"DVAR%s%s%s\" Definitionen getrennt durch `%s' Zeichen",
                larg_lit, RC_VAR_ASSIGN, mm_lit, dd_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, "            Z.B.  %sv a%s1127%sb%s054   Setze `a' auf 27 Nov und `b' auf 4 Mai",
                SWITCH, RC_VAR_ASSIGN, SEP, RC_VAR_ASSIGN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_extern_static_DVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_extern_static_DVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Lokale Datumvariablen von Datei zu Datei exportieren");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TEXT_VARIABLE:
        sprintf(s1, "%sr %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_TEXT_VARIABLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Globale Textvariable(n) \"TVAR->%ca...%cz\" definieren",
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = \"TVAR%sTEXT\" Definitonen getrennt durch `%s' Zeichen",
                larg_lit, RC_VAR_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, "            Z.B.  %sr %ca%sfoo%s%cb%sbar   Setze `%ca' auf `foo' und `%cb' auf `bar'",
                SWITCH, RC_TVAR_CHAR, RC_VAR_ASSIGN, SEP, RC_TVAR_CHAR, RC_VAR_ASSIGN,
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, "          = \"TVAR[%s|%c%s]KOMMANDO\" Definitionen getrennt durch `%s' Zeichen",
                RC_TVAR_ICMD_ASSIGN, QUOTE_CHAR, RC_TVAR_UCMD_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, "            Z.B.  %sr %ca%sfoo   Ausgabe von Kommando `foo' an `%ca' zuweisen",
                SWITCH, RC_TVAR_CHAR, RC_TVAR_ICMD_ASSIGN, RC_TVAR_CHAR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_extern_static_TVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_extern_static_TVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Lokale Textvariablen von Datei zu Datei exportieren");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_DAY:
        sprintf(s1, "%sD %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, deren Datum %s nicht ausschlie%st", larg_lit, SZ);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Eines oder mehrere der folgenden Zeichen.  Wenn Zeichen ein", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "            KLEINBUCHSTABE ist, so bedeutet das einen Nichtausschlu%s!", SZ);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "            %c = Ausschlu%s aller %s",
                   ptr_info_exclusive_day->ed_id, SZ, ptr_info_exclusive_day->ed_info);
           print_text (fp, s1);
         } while ((++ptr_info_exclusive_day)->ed_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_PERIOD:
        sprintf(s1, "%sP %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_PERIOD, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, deren Datum %s nicht ausschlie%st", larg_lit, SZ);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Liste von `%c[DATUM][%c[DATUM]]' und/oder `%c[DATUM][%c[DATUM]]'",
                larg_lit, RC_IDATE_CHAR, RC_DRANGE_CHAR, RC_EDATE_CHAR, RC_DRANGE_CHAR);
        print_text (fp, s1);
        sprintf(s1, "            Ausdr%scken, die durch `%s' Zeichen getrennt sind",
                UE, SPLIT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_TEXT:
        sprintf(s1, "%sI %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, in denen %s im Text enthalten ist", larg_lit);
        print_text (fp, s1);
#  if NO_REGEX
        sprintf(s1, "   %-3s    = Jeder beliebige Text, Metazeichen werden nicht ber%scksichtigt",
                larg_lit, UE);
#  else /* !NO_REGEX */
        sprintf(s1, "   %-3s    = Jeder beliebige Text bzw. \"regul%sre Ausdruck\"",
                larg_lit, AE);
#  endif /* !NO_REGEX */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_IGNORE_CASE:
        sprintf(s1, "          %s", get_longopt_description (SYM_IGNORE_CASE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Unterschied zwischen Gro%s- und Kleinschreibung", SZ);
        print_text (fp, s1);
        sprintf(s1, "          bei Benutzung der `%s' Option ignorieren",
                get_longopt_description (SYM_FILTER_TEXT, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_REVERT_MATCH:
        sprintf(s1, "          %s", get_longopt_description (SYM_REVERT_MATCH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Den Sinn des Vergleichs der `%s' Option umkehren",
                get_longopt_description (SYM_FILTER_TEXT, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* USE_RC */
      case SYM_DEBUG:
        sprintf(s1, "          %s", get_longopt_description (SYM_DEBUG, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Einige Debug-Informationen ausgeben");
        print_text (fp, s1);
        sprintf(s1, "  [%-3s]   = internal    Informationen bei Erreichen programminterner Maxima", larg_lit);
        print_text (fp, s1);
        strcpy(s1, "          = handled     Wie `internal' und zudem gehandhabte Dateinamen");
        print_text (fp, s1);
        strcpy(s1, "          = unhandled   Wie `internal' und zudem nicht gehandhabte Dateinamen");
        print_text (fp, s1);
        strcpy(s1, "          = all         Wie `handled' und `unhandled' zusammen");
        print_text (fp, s1);
        strcpy(s1, "          = abort       Wie `all' und Abbruch bei ungehandhabten Dateinamen");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TYPE_OF_CALENDAR:
        sprintf(s1, "%si[MOD]   %s", SWITCH, get_longopt_description (SYM_TYPE_OF_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Spezial-Kalenderformat f%sr Kalenderblatt verwenden", UE);
        print_text (fp, s1);
        sprintf(s1, "  [MOD]   = -   Standard-Kalenderformat f%sr Kalenderblatt verwenden", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_CALENDAR:
        sprintf(s1, "%sO        %s", SWITCH, get_longopt_description (SYM_ORTHODOX_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Schaltjahrregel der %sstlichen orthodoxen Kirchen verwenden", OE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CALENDAR_WITH_WEEK_NUMBER:
        sprintf(s1, "%sK        %s", SWITCH, get_longopt_description (SYM_CALENDAR_WITH_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Kalenderblatt mit Wochennummern ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISO_WEEK_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISO_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Art der Wochennummern bestimmen (aktuell:  %s)",
                (iso_week_number) ? "ISO-8601:1988" : "Standard");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = yes   ISO-8601:1988 Wochennummern", larg_lit);
        print_text (fp, s1);
        strcpy(s1, "          = no    Standard-Wochennummern");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_CALENDAR:
        sprintf(s1, "%su        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ausgabe des Kalenderblatts explizit unterdr%scken", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_PAGER
      case SYM_PAGER:
        sprintf(s1, "%sp        %s", SWITCH, get_longopt_description (SYM_PAGER, TRUE));
        print_text (fp, s1);
#  ifdef GCAL_EPAGER
        if (ext_pager != (char *)NULL)
          sprintf(s1, "          Ausgabe durch externen `%s' Paginierer lenken",
                  (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
        else
          strcpy(s1, "          Ausgabe durch einfachen internen Paginierer lenken");
#  else /* !GCAL_EPAGER */
        strcpy(s1, "          Ausgabe durch einfachen internen Paginierer lenken");
#  endif /* !GCAL_EPAGER */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* USE_PAGER */
      case SYM_DISABLE_HIGHLIGHTING:
        sprintf(s1, "%sH no     %s", SWITCH, get_longopt_description (SYM_DISABLE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=no",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, "          Text, Feiertag bzw. aktuelles Datum ohne Hervorhebung ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FORCE_HIGHLIGHTING:
        sprintf(s1, "%sH yes    %s", SWITCH, get_longopt_description (SYM_FORCE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=yes",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        sprintf(s1, "          Bel%s%st Hervorhebungssequenzen in Ausgabe bei Ausgabeumlenkung", AE, SZ);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HIGHLIGHTING:
        sprintf(s1, "%sH %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Setze Hervorhebungssequenz-Paare 1 (=aktueller Tag) und 2 (=Feiertag)");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Durch `%s' Zeichen getrennte Hervorhebungssequenz-Paare",
                larg_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, "            Z.B.  %sH \\x2%s\\xAE   Benutze Hex-Wert 2 und AE f%sr Sequenz 1",
                SWITCH, SEP, UE);
        print_text (fp, s1);
        sprintf(s1, "            Z.B.  %sH %s%s*%s*      Benutze Zeichen `*' und `*' f%sr Sequenz 2",
                SWITCH, SEP, SEP, SEP, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BLOCKS:
        sprintf(s1, "%sb %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_BLOCKS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Format des Jahreskalenders festlegen");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = 1|2|3|4|6|12   Anzahl der Bl%scke (aktuell:  %d)",
                larg_lit, OE, out_rows);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CALENDAR_DATES:
        sprintf(s1, "%sj[MOD]   %s", SWITCH, get_longopt_description (SYM_CALENDAR_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres im Kalenderblatt verwenden");
        print_text (fp, s1);
        strcpy(s1, "  [MOD]   = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HOLIDAY_DATES:
        sprintf(s1, "%sjn[MOD]  %s", SWITCH, get_longopt_description (SYM_HOLIDAY_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres in ewiger Feiertagsliste verwenden");
        print_text (fp, s1);
        strcpy(s1, "   [MOD]  = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_FIXED_DATES:
        sprintf(s1, "%sjc[MOD]  %s", SWITCH, get_longopt_description (SYM_FIXED_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres in Terminliste verwenden");
        print_text (fp, s1);
        strcpy(s1, "   [MOD]  = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
      case SYM_STARTING_DAY:
        sprintf(s1, "%ss %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_STARTING_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Starttag der Woche festlegen (aktuell:  %s)",
                (use_short3_day_name) ? short3_day_name (start_day) : short_day_name (start_day));
        print_text (fp, s1);
        if (use_short3_day_name)
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short3_day_name (DAY_MIN),
                  DAY_MIN+1, short3_day_name (DAY_MIN+1),
                  DAY_MAX, short3_day_name (DAY_MAX));
        else
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short_day_name (DAY_MIN),
                  DAY_MIN+1, short_day_name (DAY_MIN+1),
                  DAY_MAX, short_day_name (DAY_MAX));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# ifdef GCAL_EMAIL
      case SYM_MAIL:
        sprintf(s1, "          %s", get_longopt_description (SYM_MAIL, TRUE));
        print_text (fp, s1);
#  if !defined(AMIGA) || defined(__GNUC__)
        ptr_env = getenv(ENV_VAR_MAILPROG);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = MAIL_PRGR;
         }
        else
#  endif /* AMIGA && !__GNUC__ */
          ptr_env = MAIL_PRGR;
        sprintf(s1, "          Versenden der Ausgabe via `%s' Programm an Benutzer", ptr_env);
        print_text (fp, s1);
        if (email_adr != (char *)NULL)
          ptr_env = email_adr;
#  if !defined(AMIGA) || defined(__GNUC__)
        else
          ptr_env = getenv(ENV_VAR_MAILTO);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = getenv(ENV_VAR_USER);
         }
        else
          ptr_env = getenv(ENV_VAR_USER);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = getenv(ENV_VAR_LOGNAME);
         }
        else
          ptr_env = getenv(ENV_VAR_LOGNAME);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = (char *)NULL;
         }
#  endif /* AMIGA && !__GNUC__ */
        if (ptr_env == (char *)NULL)
          ptr_env = "UNBEKANNT";
        sprintf(s1, "  [%-3s]   = eMail Adresse, sonst eMail-Versand an Benutzer `%s'",
                larg_lit, ptr_env);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* GCAL_EMAIL */
      case SYM_HOLIDAY_LIST:
      case SYM_DESC_HOLIDAY_LIST:
        sprintf(s1, "%sn|N[MOD] %s", SWITCH, get_longopt_description (SYM_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste ausgeben (Jahr im Bereich: %d...%d)",
                EASTER_MIN, EASTER_MAX);
        print_text (fp, s1);
        strcpy(s1, " n        = Gesetzliche Feiertage und Gedenktage");
        print_text (fp, s1);
        strcpy(s1, "   N      = Nur gesetzliche Feiertage");
        print_text (fp, s1);
        sprintf(s1, "    [MOD] = %s   Absteigend sortiert", DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_HDLIST_SEP:
        sprintf(s1, "%sG        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_HDLIST_SEP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste ohne anf%shrende Leerzeile ausgeben", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXCLUDE_HD_TITLE:
        sprintf(s1, "%sX        %s", SWITCH, get_longopt_description (SYM_EXCLUDE_HD_TITLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste ohne %sberschrift ausgeben", UUE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ASTRONOMICAL_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ASTRONOMICAL_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit astronomischen Daten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BAHAI_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_BAHAI_HDY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit Bah%s'i Feiertagen ausstatten", ARA);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BAHAI_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_BAHAI_MTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit Bah%s'i Monaten ausstatten", ARA);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CELTIC_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CELTIC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit keltischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_FLEXIBLE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_FLEXIBLE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit flexiblen chinesischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_FLEXIBLE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_FLEXIBLE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit flexiblen chinesischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit chinesischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit chinesischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHRISTIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHRISTIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit christlichen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_COPTIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_COPTIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit koptischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ETHIOPIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_ETHIOPIC_MTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit %sthiopischen Monaten ausstatten", AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FRENCH_REVOLUTIONARY_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_FRENCH_REVOLUTIONARY_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit franz. Revolutionskalendermonaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HEBREW_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEBREW_HDY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit hebr%sischen Feiertagen ausstatten", AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HEBREW_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEBREW_MTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit hebr%sischen Monaten ausstatten", AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INDIAN_CIVIL_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_INDIAN_CIVIL_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit indischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISLAMIC_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISLAMIC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit islamischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISLAMIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISLAMIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit islamischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_FLEXIBLE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_FLEXIBLE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit flexiblen japanischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_FLEXIBLE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_FLEXIBLE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit flexiblen japanischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit japanischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit japanischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MULTICULTURAL_NEW_YEAR_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_MULTICULTURAL_NEW_YEAR_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit multikulturellen Neujahren ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OLD_ARMENIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_OLD_ARMENIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit alt-armenischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OLD_EGYPTIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_OLD_EGYPTIC_MTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste mit alt-%sgyptischen Monaten ausstatten", AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_NEW_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ORTHODOX_NEW_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit neu-orthodoxen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_OLD_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ORTHODOX_OLD_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit alt-orthodoxen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PERSIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERSIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit persischen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PERSIAN_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERSIAN_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit persischen Monaten ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ZODIACAL_MARKER_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZODIACAL_MARKER_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit Tierkreiszeichen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CC_HDY:
        sprintf(s1, "%sq %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_CC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit landesspezifischen Feiertagen ausstatten");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Einzelner Landes- oder Territoriumskode bzw. Liste dieser,", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "            die durch `%s' Zeichen verbunden werden. Alle mit # markierten", CONNECT_SEP);
        print_text (fp, s1);
        sprintf(s1, "            L%snder/Territorien haben nur unvollst%sndig erfasste Feiertage", AE, AE);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "            %-*s = Feiertage in %s",
# if HD_TOP20CC
                   2,
# else /* !HD_TOP20CC */
                   5,
# endif /* !HD_TOP20CC */
                   ptr_cc_holidays->cc_id, ptr_cc_holidays->cc_info);
           print_text (fp, s1);
         } while ((++ptr_cc_holidays)->cc_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_ADJUST_VALUE:
        sprintf(s1, "          %s", get_longopt_description (SYM_ADJUST_VALUE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Referenzwert f%sr Auf-/Untergangszeit bzw. Schattenl%snge festlegen",
                UE, AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Winkelwert bzw. Faktor im Bereich:  %+.1f...%+.1f",
                larg_lit, -DEGS_PER_06_HOURS, DEGS_PER_06_HOURS);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ATMOSPHERE:
        sprintf(s1, "          %s", get_longopt_description (SYM_ATMOSPHERE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Basisdaten der Erdatmosph%sre festlegen", AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Luftdruck und Temperatur getrennt durch `%s' Zeichen",
                larg_lit, SPLIT_SEP);
        print_text (fp, s1);
        sprintf(s1, "            Luftdruck in Millibar (aktuell:  %.3f)", atm_pressure/100.0);
        print_text (fp, s1);
        sprintf(s1, "            Lufttemperatur in Grad Celsius (aktuell:  %+.3f)",
                atm_temperature);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LIMIT:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIMIT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Auf-/Untergangszeiten der Sonne auf den Tag beschr%snken", AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PRECISE:
        sprintf(s1, "          %s", get_longopt_description (SYM_PRECISE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Astronomische Zeiten und Daten mit h%schster Genauigkeit darstellen", OE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXECUTE_COMMAND:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXECUTE_COMMAND, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          `%c%c[%s]' Shell-Kommandos und",
                RC_SPECIAL_TEXT_CHAR, RC_SHELL_ESC_CHAR, larg_lit);
        print_text (fp, s1);
        sprintf(s1, "          \"TVAR[%s|%c%s]KOMMANDO\" Zuweisungen ausf%shren",
                RC_TVAR_ICMD_ASSIGN, QUOTE_CHAR, RC_TVAR_UCMD_ASSIGN, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
# endif
      case SYM_TIME_OFFSET:
        sprintf(s1, "          %s", get_longopt_description (SYM_TIME_OFFSET, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Basiszeit der astronomischen Funktionen %sndern" , AE);
        print_text (fp, s1);
# if USE_RC
        sprintf(s1, "          beziehungsweise Zyklus-Startzeit %sndern" , AE);
        print_text (fp, s1);
# endif
        if (time_hour_offset)
          sprintf(s2, "%+d", gmt_timezone_value (time_hour_offset));
        else
          *s2 = '\0';
# if USE_RC
        sprintf(s1, "   %-3s    = %c|%c|[%c|%c][%s|%s]MMMM|HH%s[MM]   Zeitversatzwert (aktuell:  %s%02d%s%02d == GMT%s)",
                larg_lit, RC_TIME_CHAR, RC_GMTIME_CHAR, RC_TIME_CHAR, RC_GMTIME_CHAR, ASC_LIT, DES_LIT, time_sep,
                (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
                abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
# else /* !USE_RC */
        sprintf(s1, "   %-3s    = %c|[%c][%s|%s]MMMM|HH%s[MM]   Zeitversatzwert (aktuell:  %s%02d%s%02d == GMT%s)",
                larg_lit, staticTIME_CHAR, staticTIME_CHAR, ASC_LIT, DES_LIT, time_sep,
                (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
                abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
# endif /* !USE_RC */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_CYCLE_END:
        sprintf(s1, "          %s", get_longopt_description (SYM_CYCLE_END, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Zyklus-Endezeit %sndern" , AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = %c|%c|[%c|%c][%s|%s]MMMM|HH%s[MM]   Endezeitwert (aktuell:  %02d%s%02d)",
                larg_lit, RC_TIME_CHAR, RC_GMTIME_CHAR, RC_TIME_CHAR, RC_GMTIME_CHAR,
                ASC_LIT, DES_LIT, time_sep, (int)MM2HH(loop_end), time_sep,
                (loop_end>SPECIAL_VALUE) ? loop_end%MINS_PER_HOUR : 0);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CYCLE_STEP:
        sprintf(s1, "          %s", get_longopt_description (SYM_CYCLE_STEP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Zyklus-Schrittweite %sndern" , AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = MMMM|HH%s[MM]   Schrittweitenwert (aktuell:  %02d%s%02d)",
                larg_lit, time_sep, (int)MM2HH(loop_step), time_sep, loop_step%MINS_PER_HOUR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
      case SYM_TRANSFORM_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_TRANSFORM_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Basisjahr des Kalenders %sndern", AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = [%s|%s]JJJJ   Jahrversatzwert (aktuell:  %d)",
                larg_lit, ASC_LIT, DES_LIT, (transform_year) ? transform_year : YEAR_MIN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_GREG_REFORM:
        sprintf(s1, "          %s", get_longopt_description (SYM_GREG_REFORM, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Zeitraum der Gregorianischen Reformation setzen");
        print_text (fp, s1);
        sprintf(s1, "          Aktuell ber%scksichtigter Zeitraum:  %02d-%02d %s %0*d", UE,
                greg->first_day, greg->last_day, short_month_name (greg->month),
                len_year_max, greg->year);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "   %-*d%*s = Setze Zeitraum auf:  %02d-%02d %s %0*d",
                   len_year_max, ptr_greg->year, 6-len_year_max, "",
                   ptr_greg->first_day, ptr_greg->last_day,
                   short_month_name (ptr_greg->month), len_year_max, ptr_greg->year);
           print_text (fp, s1);
         } while ((++ptr_greg)->year);
        sprintf(s1, "   %-3s    = %s%c%s%s%s%s%s%s   Setze Zeitraum explizit",
                larg_lit, yyyy_lit, *yyyy_lit, SPLIT_SEP, mm_lit, SPLIT_SEP, dd_lit, SPLIT_SEP, dd_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_DATE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_DATE_FORMAT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Anordnung der Datumelemente mit dem Formattext %s festlegen", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "          Aktuell:  (%s) `%s' (%s)",
                (date_format->df_id == (char *)NULL) ? "Selbstdefiniert" : date_format->df_id,
                date_format->df_format, date_format->df_info);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "   %-6s = Format ist:  `%s' (%s)", ptr_date_format->df_id,
                   ptr_date_format->df_format, ptr_date_format->df_info);
           print_text (fp, s1);
         } while ((++ptr_date_format)->df_info != (char *)NULL);
        sprintf(s1, "   %-3s    = Eigenes Format definieren. Ber%scksichtigte Formatelemente sind:",
                larg_lit, UE);
        print_text (fp, s1);
        do
         {
           if (!ptr_info_date_format->di_1format)
             sprintf(s1, "               %s", ptr_info_date_format->di_info);
           else
            {
              if (ptr_info_date_format->di_1format != DFORMAT_CHAR)
               {
                 if (ptr_info_date_format->di_2format)
                   sprintf(s2, ptr_info_date_format->di_info, ptr_info_date_format->di_2format);
                 else
                   sprintf(s2, ptr_info_date_format->di_info, FWIDTH_MIN, FWIDTH_MAX);
                 sprintf(s1, "            %c%c = %s", ptr_info_date_format->di_1format,
                         (ptr_info_date_format->di_2format) ? ptr_info_date_format->di_2format : ' ', s2);
               }
              else
                sprintf(s1, "            %c%c = %s", ptr_info_date_format->di_1format,
                        ptr_info_date_format->di_2format, ptr_info_date_format->di_info);
            }
           print_text (fp, s1);
         } while ((++ptr_info_date_format)->di_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TRANSLATE_STRING:
        sprintf(s1, "          %s", get_longopt_description (SYM_TRANSLATE_STRING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Zu %sbersetzende landesspezifische Sonderzeichenpaare definieren", UE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = GRO%sBUCHSTABE\"\"KLEINBUCHSTABE... Definitionen",
                larg_lit, SZ);
        print_text (fp, s1);
        sprintf(s1, "            Z.B. bewirkt ein `%s%s%s%s%s%s%s%s' %-3s die korrekte Umsetzung vorstehender",
                AAE, AE, OOE, OE, UUE, UE, SZ, SZ, larg_lit);
        print_text (fp, s1);
        strcpy(s1, "            Sonderzeichen in einem selbstdefinierten Datumformat, welches eine");
        print_text (fp, s1);
        strcpy(s1, "            STIL-Formatanweisungskomponente aufweist, und zwar so, wie sie von");
        print_text (fp, s1);
        strcpy(s1, "            dem in Deutschland benutzten Zeichensatz verwendet werden");
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
# if USE_RC
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, "+++ TERMINE +++");
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_TODAY:
      case SYM_FIXED_DATES_LIST:
      case SYM_DESC_FIXED_DATES_LIST:
        sprintf(s1, "%sc|C[MOD] %s", SWITCH, get_longopt_description (SYM_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_TODAY, TRUE));
        print_text (fp, s1);
#  ifdef GCAL_SHELL
        sprintf(s1, "          Benutze Standard-Ressourcendatei `.%s%s' f%sr Terminliste",
                PRGR_NAME, RC_SUFFIX, UE);
#  else /* !GCAL_SHELL */
        sprintf(s1, "          Benutze Standard-Ressourcendatei `%s%s' f%sr Terminliste",
                PRGR_NAME, RC_SUFFIX, UE);
#  endif /* !GCAL_SHELL */
        print_text (fp, s1);
        strcpy(s1, "          Impliziert Zeitraum:  Heute");
        print_text (fp, s1);
        strcpy(s1, " c        = Nur die Daten ausgeben, zu denen Termine vorliegen");
        print_text (fp, s1);
        strcpy(s1, "   C      = Auch die Daten ausgeben, zu denen keine Termine vorliegen");
        print_text (fp, s1);
        sprintf(s1, "    [MOD] = %s   Absteigend sortiert", DES_LIT);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, "    [MOD] = Einer oder mehr der folgenden Modifizierer; hier markiert mit #");
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HEADING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEADING_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "            %-3s          = Benutzter Text f%sr Terminlisten-%sberschrift",
                larg_lit, UE, UUE);
        print_text (fp, s1);
        sprintf(s1, "                           Aktuell:  `%s'", rc_heading_text+2);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_GROUPING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_GROUPING_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # g[MOD]       = Benutze Trenntext MOD f%sr tageweise Gruppierung", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, "          1. Darstellung des Textes");
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_BIORHYTHM_AXIS_LEN:
        sprintf(s1, "          %s", get_longopt_description (SYM_BIORHYTHM_AXIS_LEN, TRUE));
        print_text (fp, s1);
        strcpy(s1, "                         = Breite der Biorhythmus-Textgrafik festlegen");
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = %d...%d   L%snge einer einzelnen Achse",
                larg_lit, BIO_AXIS_MIN, BIO_AXIS_MAX, AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MOONIMAGE_LINES:
        sprintf(s1, "          %s", get_longopt_description (SYM_MOONIMAGE_LINES, TRUE));
        print_text (fp, s1);
        sprintf(s1, "                         = H%she der Mondphasen-Textgrafik festlegen", OE);
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = %d...%d   Gesamtanzahl der Zeilen",
                larg_lit, MOONIMAGE_MIN, MOONIMAGE_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_FILENAME:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_FILENAME, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # a            = Herkunft des Termins ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ALTERNATIVE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_ALTERNATIVE_FORMAT, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # A            = Alternatives Listenformat verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_HOLIDAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_HOLIDAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # e            = Gesetzliche Feiertage und Gedenktage einf%sgen", UE);
        print_text (fp, s1);
        sprintf(s1, "          # E            = Nur gesetzliche Feiertage einf%sgen", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_WEEK_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # k            = Wochennummer ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OMIT_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_OMIT_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # o            = Datumfeld nur einmal ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_FDLIST_SEP:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_FDLIST_SEP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # Q            = Terminliste ohne anf%shrende Leerzeile ausgeben", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # U            = Datumfeld nicht ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_TEXT_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_TEXT_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # J            = Textfeld nicht ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXCLUDE_RC_TITLE:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXCLUDE_RC_TITLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # x            = Terminliste ohne %sberschrift ausgeben", UUE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_CONS_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_CONS_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # z            = Fortlaufende Postennummer ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ZERO_DATES:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZERO_DATES, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # Z            = Nur Daten ausgeben, f%sr die keine Termine vorliegen", UE);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, "          2. Ber%scksichtigter Zeitraum", UE);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_LEAP_DAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_LEAP_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "                         = Den `29-FEBRUAR' in Nichtschaltjahren handhaben");
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = february   Am `28-FEBRUAR' ber%scksichtigen",
                larg_lit, UE);
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = march      Am `01-M%sRZ' ber%scksichtigen",
                larg_lit, AAE, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_TODAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_TODAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # d            = Aktuelles Datum einschliessen bei Datumliste");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LIST_MODE:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIST_MODE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # l            = Datumliste anstelle Einzeldatum");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_PERIOD:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERIOD, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # Nd           = Einzelner absoluter Tag N");
        print_text (fp, s1);
        sprintf(s1, "          # %cdN          = Einzelner absoluter Tag N", RC_NWD_CHAR);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, "          # NMOD         = Einzelner Tag N relativ zu heute");
        print_text (fp, s1);
        sprintf(s1, "             MOD           = %s   Vorw%srts", ASC_LIT, AE);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   R%sckw%srts", DES_LIT, UE, AE);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, "          # Nw           = Gesamte Woche N");
        print_text (fp, s1);
        strcpy(s1, "                           N = 0        1'te Woche / letzte Woche im Vorjahr");
        print_text (fp, s1);
        strcpy(s1, "                           N = 1...52   1'te...52'te Woche (stets)");
        print_text (fp, s1);
        strcpy(s1, "                           N = 53       53'te Woche (manchmal)");
        print_text (fp, s1);
        strcpy(s1, "                           N = 99       Letzte Woche");
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %s%s         = Einzelner Tag %s im Monat %s",
                mm_lit, dd_lit, dd_lit, mm_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %s%sN       = Einzelner N'ter Wochentag %s im Monat %s",
                mm_lit, www_lit, www_lit, mm_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...4   1'ter...4'ter Wochentag %s (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 5       5'ter Wochentag %s (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 9       Letzter Wochentag %s", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %cdN%s       = Einzelner N'ter Wochentag %s",
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...51   1'ter...51'ter Wochentag %s (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 52|53    52|53'ter Wochentag %s (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 99       Letzter Wochentag %s", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %cwN%s       = Einzelner Wochentag %s der N'ten Woche",
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 0        %s der nicht in der 1'ten Woche ist", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...51   %s der 1'ten...51'ten Woche (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 52|53    %s der 52|53'ten Woche (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 99       %s der letzten Woche", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[[%s|%s]N]   = Einzelner Tag N relativ zu Ostersonntag",
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[%s|%s]N%s  = Einzelner N'ter Wochentag relativ zu Ostersonntag",
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[[%s|%s]N]   = Einzelner Tag N relativ zu heute",
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[%s|%s]N%s  = Einzelner N'ter Wochentag relativ zu heute",
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c?[[%s|%s]N]   = Einzelner Tag N relativ zur Datumvariablen",
                RC_HDY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c?[%s|%s]N%s  = Einzelner N'ter Wochentag relativ zur Datumvariablen",
                RC_HDY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
      case SYM_TOMORROW:
        sprintf(s1, "          %s", get_longopt_description (SYM_TOMORROW, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # t|T          = Datumliste von morgen");
        print_text (fp, s1);
        if (longopt_symbolic == SYM_TOMORROW)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_WEEK:
      case SYM_END_OF_WEEK:
      case SYM_START_OF_WEEK:
        sprintf(s1, "          %s", get_longopt_description (SYM_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_WEEK, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # w|W[MOD]     = Datumliste der gesamten Woche");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende der Woche",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang der Woche",
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_WEEK
            || longopt_symbolic == SYM_END_OF_WEEK
            || longopt_symbolic == SYM_START_OF_WEEK)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MONTH:
      case SYM_END_OF_MONTH:
      case SYM_START_OF_MONTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_MONTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # m|M[MOD]     = Datumliste des gesamten Monats");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende des Monats",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang der Monats",
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_MONTH
            || longopt_symbolic == SYM_END_OF_MONTH
            || longopt_symbolic == SYM_START_OF_MONTH)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_YEAR:
      case SYM_END_OF_YEAR:
      case SYM_START_OF_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_YEAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # y|Y[MOD]     = Datumliste des gesamten Jahres");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende des Jahres",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang des Jahres",
                DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_RESOURCE_FILE:
        sprintf(s1, "%sf|F %-3s  %s", SWITCH, larg_lit, get_longopt_description (SYM_RESOURCE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Benutze alternative Datei(en) anstelle Standard-Ressourcendatei");
        print_text (fp, s1);
        strcpy(s1, "          Impliziert Zeitraum:  Heute");
        print_text (fp, s1);
        strcpy(s1, " f        = Nur die Daten ausgeben, zu denen Termine vorliegen");
        print_text (fp, s1);
        strcpy(s1, "   F      = Auch die Daten ausgeben, zu denen keine Termine vorliegen");
        print_text (fp, s1);
        sprintf(s1, "     %-3s  = Einzelne Datei oder durch `%s' Zeichen verbundene Liste von Dateien",
                larg_lit, CONNECT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HERE_FILE:
        sprintf(s1, "%s# %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HERE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Zus%stzliche Ressourcendateizeile definieren", AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Jede m%sgliche Zeile einer Ressourcendatei", larg_lit, OE);
        print_text (fp, s1);
      default:
          ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "%cDATUM", RC_ADATE_CHAR);
      print_text (fp, s1);
      strcpy(s1, "Vorgegebenes `DATUM' anstelle heutiges Datum verwenden");
      print_text (fp, s1);
      sprintf(s1, "Datumformat:  %s[%s[%s|%s[N]]], %s%c%c|%c|DVAR[[%s|%s]N[%s]], %s%cdN[%s]",
              yyyy_lit, mm_lit, dd_lit, www_lit, yyyy_lit, RC_HDY_CHAR, RC_EASTER_CHAR, RC_TODAY_CHAR,
              ASC_LIT, DES_LIT, www_lit, yyyy_lit, RC_NWD_CHAR, www_lit);
      print_text (fp, s1);
      sprintf(s1, "              %s%cwN[%s], Monatsname[%s], Wochentagsname[N] oder %s",
              yyyy_lit, RC_NWD_CHAR, www_lit, dd_lit, dd_lit);
      print_text (fp, s1);
    }
# endif /* USE_RC */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "%cDATEI", RSP_CHAR);
      print_text (fp, s1);
      sprintf(s1, "L%sdt Optionen und Kommandos aus `DATEI' vor", AE);
      print_text (fp, s1);
      print_text (fp, s1);
      print_text (fp, s1);
      strcpy(s1, "KOMMANDO");
      print_text (fp, s1);
      sprintf(s1, "%s       = Monat im Bereich:  %d...%d", mm_lit, MONTH_MIN, MONTH_MAX);
      print_text (fp, s1);
      sprintf(s1, "                       oder:  Monatsname | %s | %s%s | %s%s | %s%s",
              MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, ASC_LIT, MONTH3_LIT, DES_LIT);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "           Liste:    %s[%s%s]%s...%s%s[%s%s]",
              mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit, YEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, "           Bereich:  %s[%s%s]%s%s[%s%s] | %s%s%s%s%s | %s %s%s%s",
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, mm_lit, YEAR_SEP, yyyy_lit,
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, yyyy_lit,
              mm_lit, yyyy_lit, MRANGE_SEP, yyyy_lit);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "%s%*s   = Jahr  im Bereich:  %d...%d",
              yyyy_lit, 6-len_year_max, "", YEAR_MIN, YEAR_MAX);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "           Liste:    [%s%s]%s%s...%s[%s%s]%s",
              mm_lit, FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, "           Bereich:  [%s%s]%s%s[%s%s]%s | %s%s%s %s%s%s",
              mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit,
              mm_lit, MRANGE_SEP, mm_lit, yyyy_lit, YRANGE_SEP, yyyy_lit);
    }
#else /* !USE_DE */
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HELP:
        sprintf(s1, "%s?        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%sh        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display help text and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LONG_HELP1:
      case SYM_LONG_HELP2:
        sprintf(s1, "%s??       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%shh       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LONG_HELP2, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display extended help text and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LICENSE1:
      case SYM_LICENSE2:
      case SYM_LICENSE3:
        sprintf(s1, "%sL        %s", SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE2, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE3, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display software license and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_VERSION:
        sprintf(s1, "%sV        %s", SWITCH, get_longopt_description (SYM_VERSION, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display version information and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXIT_STAT_HELP_NON_ZERO:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXIT_STAT_HELP_NON_ZERO, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Set EXIT status of program to %d on `%s' etc."),
                ERR_EXIT_INFO_TEXTS_NON_ZERO, get_longopt_description (SYM_HELP, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_RESPONSE_FILE:
        sprintf(s1, "%sR %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_RESPONSE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Create response file for the `%cFILE' option"), RSP_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Store arguments of command line in file %s"), larg_lit, larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# ifdef GCAL_SHELL
      case SYM_SCRIPT_FILE:
        sprintf(s1, "%sS %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_SCRIPT_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Create shell script which contains the arguments of command line"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = File name of the shell script"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
# if USE_RC
      case SYM_DATE_VARIABLE:
        sprintf(s1, "%sv %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_DATE_VARIABLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Define global date variable(s) \"DVAR->a...d|f...s|u...|z\""));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = \"DVAR%s%s%s\" definitions separated by `%s' characters"),
                larg_lit, RC_VAR_ASSIGN, mm_lit, dd_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            E.g.  %sv a%s1127%sb%s054   Set `a' to Nov 27 and `b' to May 4"),
                SWITCH, RC_VAR_ASSIGN, SEP, RC_VAR_ASSIGN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_extern_static_DVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_extern_static_DVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Export local date variables from file to file"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TEXT_VARIABLE:
        sprintf(s1, "%sr %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_TEXT_VARIABLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Define global text variable(s) \"TVAR->%ca...%cz\""),
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = \"TVAR%sTEXT\" definitions separated by `%s' characters"),
                larg_lit, RC_VAR_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            E.g.  %sr %ca%sfoo%s%cb%sbar   Set `%ca' to `foo' and `%cb' to `bar'"),
                SWITCH, RC_TVAR_CHAR, RC_VAR_ASSIGN, SEP, RC_TVAR_CHAR, RC_VAR_ASSIGN,
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("          = \"TVAR[%s|%c%s]COMMAND\" definitions separated by `%s' characters"),
                RC_TVAR_ICMD_ASSIGN, QUOTE_CHAR, RC_TVAR_UCMD_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            E.g.  %sr %ca%sfoo   Assign output of command `foo' to `%ca'"),
                SWITCH, RC_TVAR_CHAR, RC_TVAR_ICMD_ASSIGN, RC_TVAR_CHAR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_extern_static_TVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_extern_static_TVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Export local text variables from file to file"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_DAY:
        sprintf(s1, "%sD %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose date is not excluded by %s"),
                larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = One or more of the following characters.  If character"), larg_lit);
        print_text (fp, s1);
        strcpy(s1, _("            is a LOWER-CASE-LETTER, it means a non-exclusion!"));
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("            %c = Exclusion of all %s"),
                   ptr_info_exclusive_day->ed_id, _(ptr_info_exclusive_day->ed_info));
           print_text (fp, s1);
         } while ((++ptr_info_exclusive_day)->ed_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_PERIOD:
        sprintf(s1, "%sP %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_PERIOD, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose date is not excluded by %s"),
                larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = List of `%c[DATE][%c[DATE]]' and/or `%c[DATE][%c[DATE]]'"),
                larg_lit, RC_IDATE_CHAR, RC_DRANGE_CHAR, RC_EDATE_CHAR, RC_DRANGE_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("            expressions separated by `%s' characters"), SPLIT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FILTER_TEXT:
        sprintf(s1, "%sI %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose text is matched by %s"), larg_lit);
        print_text (fp, s1);
#  if NO_REGEX
        sprintf(s1, _("   %-3s    = Any text you like, metacharacters are not respected"), larg_lit);
#  else /* !NO_REGEX */
        sprintf(s1, _("   %-3s    = Any text respectively \"regular expression\" you like"), larg_lit);
#  endif /* !NO_REGEX */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_IGNORE_CASE:
        sprintf(s1, "          %s", get_longopt_description (SYM_IGNORE_CASE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Ignore case distinctions if `%s' option is given"),
                get_longopt_description (SYM_FILTER_TEXT, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_REVERT_MATCH:
        sprintf(s1, "          %s", get_longopt_description (SYM_REVERT_MATCH, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Revert the sense of matching of the `%s' option"),
                get_longopt_description (SYM_FILTER_TEXT, FALSE));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* USE_RC */
      case SYM_DEBUG:
        sprintf(s1, "          %s", get_longopt_description (SYM_DEBUG, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display some debug information"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `internal',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("  [%-3s]   = internal    Information if program internal maximums are reached"),
                larg_lit);
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the words `handled' and `internal',
           *** because they are used textually AS IS as an option argument.
        */
        strcpy(s1, _("          = handled     Like `internal' and file names which are handled"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the words `unhandled' and `internal',
           *** because they are used textually AS IS as an option argument.
        */
        strcpy(s1, _("          = unhandled   Like `internal' and file names which are unhandled"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the words `all', `handled' and `unhandled',
           *** because they are used textually AS IS as an option argument.
        */
        strcpy(s1, _("          = all         Like `handled' and `unhandled' together"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the words `abort' and `all',
           *** because they are used textually AS IS as an option argument.
        */
        strcpy(s1, _("          = abort       Like `all' and abort if file name can't be handled"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TYPE_OF_CALENDAR:
        sprintf(s1, "%si[MOD]   %s", SWITCH, get_longopt_description (SYM_TYPE_OF_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use special format for calendar sheet"));
        print_text (fp, s1);
        strcat(s1, _("  [MOD]   = -   Use standard format for calendar sheet"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_CALENDAR:
        sprintf(s1, "%sO        %s", SWITCH, get_longopt_description (SYM_ORTHODOX_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use leap year rule of Eastern Orthodox churches"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CALENDAR_WITH_WEEK_NUMBER:
        sprintf(s1, "%sK        %s", SWITCH, get_longopt_description (SYM_CALENDAR_WITH_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide calendar sheet with week numbers"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISO_WEEK_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISO_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Determine type of week numbers (actual:  %s)"),
                (iso_week_number) ? "ISO-8601:1988" : _("Standard"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `yes',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("   %-3s    = yes   ISO-8601:1988 week numbers"), larg_lit);
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `no',
           *** because it is used textually AS IS as an option argument.
        */
        strcpy(s1, _("          = no    Standard week numbers"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_CALENDAR:
        sprintf(s1, "%su        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Suppress output of calendar sheet explicitly"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_PAGER
      case SYM_PAGER:
        sprintf(s1, "%sp        %s", SWITCH, get_longopt_description (SYM_PAGER, TRUE));
        print_text (fp, s1);
#  ifdef GCAL_EPAGER
        if (ext_pager != (char *)NULL)
          sprintf(s1, _("          Direct output through external `%s' pager"),
                  (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
        else
          strcpy(s1, _("          Direct output through simple internal pager"));
#  else /* !GCAL_EPAGER */
        strcpy(s1, _("          Direct output through simple internal pager"));
#  endif /* !GCAL_EPAGER */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* USE_PAGER */
      case SYM_DISABLE_HIGHLIGHTING:
        sprintf(s1, "%sH no     %s", SWITCH, get_longopt_description (SYM_DISABLE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=no",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, _("          Disable highlighting of text, holiday resp., actual day"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FORCE_HIGHLIGHTING:
        sprintf(s1, "%sH yes    %s", SWITCH, get_longopt_description (SYM_FORCE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=yes",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, _("          Forces highlighting sequences if output is redirected/piped"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HIGHLIGHTING:
        sprintf(s1, "%sH %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set highlighting sequence pairs 1 (=actual day) and 2 (=holiday)"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Highlighting sequence pairs separated by `%s' characters"),
                larg_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            E.g.  %sH \\x2%s\\xAE   Use hex values 2 and AE for sequence 1"),
                SWITCH, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            E.g.  %sH %s%s*%s*      Use characters `*' and `*' for sequence 2"),
                SWITCH, SEP, SEP, SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BLOCKS:
        sprintf(s1, "%sb %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_BLOCKS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Modify format of year calendar"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = 1|2|3|4|6|12   Number of blocks (actual:  %d)"),
                larg_lit, out_rows);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CALENDAR_DATES:
        sprintf(s1, "%sj[MOD]   %s", SWITCH, get_longopt_description (SYM_CALENDAR_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in calendar sheet"));
        print_text (fp, s1);
        strcpy(s1, _("  [MOD]   = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HOLIDAY_DATES:
        sprintf(s1, "%sjn[MOD]  %s", SWITCH, get_longopt_description (SYM_HOLIDAY_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in eternal holiday list"));
        print_text (fp, s1);
        strcpy(s1, _("   [MOD]  = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_FIXED_DATES:
        sprintf(s1, "%sjc[MOD]  %s", SWITCH, get_longopt_description (SYM_FIXED_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in fixed date list"));
        print_text (fp, s1);
        strcpy(s1, _("   [MOD]  = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
      case SYM_STARTING_DAY:
        sprintf(s1, "%ss %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_STARTING_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Set starting day of week (actual:  %s)"),
                (use_short3_day_name) ? short3_day_name (start_day) : short_day_name (start_day));
        print_text (fp, s1);
        if (use_short3_day_name)
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short3_day_name (DAY_MIN),
                  DAY_MIN+1, short3_day_name (DAY_MIN+1),
                  DAY_MAX, short3_day_name (DAY_MAX));
        else
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short_day_name (DAY_MIN),
                  DAY_MIN+1, short_day_name (DAY_MIN+1),
                  DAY_MAX, short_day_name (DAY_MAX));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# ifdef GCAL_EMAIL
      case SYM_MAIL:
        sprintf(s1, "          %s", get_longopt_description (SYM_MAIL, TRUE));
        print_text (fp, s1);
#  if !defined(AMIGA) || defined(__GNUC__)
        ptr_env = getenv(ENV_VAR_MAILPROG);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = MAIL_PRGR;
         }
        else
#  endif /* AMIGA && !__GNUC__ */
          ptr_env = MAIL_PRGR;
        sprintf(s1, _("          Send output via `%s' program to user"), ptr_env);
        print_text (fp, s1);
        if (email_adr != (char *)NULL)
          ptr_env = email_adr;
#  if !defined(AMIGA) || defined(__GNUC__)
        else
          ptr_env = getenv(ENV_VAR_MAILTO);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = getenv(ENV_VAR_USER);
         }
        else
          ptr_env = getenv(ENV_VAR_USER);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = getenv(ENV_VAR_LOGNAME);
         }
        else
          ptr_env = getenv(ENV_VAR_LOGNAME);
        if (ptr_env != (char *)NULL)
         {
           if (!*ptr_env)
             ptr_env = (char *)NULL;
         }
#  endif /* AMIGA && !__GNUC__ */
        if (ptr_env == (char *)NULL)
          ptr_env = _("UNKNOWN");
        sprintf(s1, _("  [%-3s]   = Email address, otherwise eMail is send to user `%s'"),
                larg_lit, ptr_env);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif /* GCAL_EMAIL */
      case SYM_HOLIDAY_LIST:
      case SYM_DESC_HOLIDAY_LIST:
        sprintf(s1, "%sn|N[MOD] %s", SWITCH, get_longopt_description (SYM_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display eternal holiday list (Year in range: %d...%d)"),
                EASTER_MIN, EASTER_MAX);
        print_text (fp, s1);
        strcpy(s1, _(" n        = Legal days and memorial days"));
        print_text (fp, s1);
        strcpy(s1, _("   N      = Legal days only"));
        print_text (fp, s1);
        sprintf(s1, _("    [MOD] = %s   Descending sort order"), DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_HDLIST_SEP:
        sprintf(s1, "%sG        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_HDLIST_SEP, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Suppress leading blank line of eternal holiday list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXCLUDE_HD_TITLE:
        sprintf(s1, "%sX        %s", SWITCH, get_longopt_description (SYM_EXCLUDE_HD_TITLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Exclude title of eternal holiday list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ASTRONOMICAL_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ASTRONOMICAL_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with astronomical data"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BAHAI_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_BAHAI_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Bahá'i calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_BAHAI_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_BAHAI_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Bahá'i calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CELTIC_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CELTIC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Celtic calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_FLEXIBLE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_FLEXIBLE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Chinese flexible calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_FLEXIBLE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_FLEXIBLE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Chinese flexible calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Chinese calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHINESE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHINESE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Chinese calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CHRISTIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHRISTIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Christian Western calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_COPTIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_COPTIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Coptic calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ETHIOPIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_ETHIOPIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Ethiopic calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_FRENCH_REVOLUTIONARY_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_FRENCH_REVOLUTIONARY_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with French Revolutionary calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HEBREW_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEBREW_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Hebrew calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HEBREW_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEBREW_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Hebrew calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INDIAN_CIVIL_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_INDIAN_CIVIL_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Indian civil-calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISLAMIC_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISLAMIC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Islamic civil-calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ISLAMIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_ISLAMIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Islamic civil-calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_FLEXIBLE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_FLEXIBLE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Japanese flexible calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_FLEXIBLE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_FLEXIBLE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Japanese flexible calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Japanese calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_JAPANESE_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_JAPANESE_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Japanese calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MULTICULTURAL_NEW_YEAR_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_MULTICULTURAL_NEW_YEAR_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with multicultural New Year holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OLD_ARMENIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_OLD_ARMENIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Old-Armenic calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OLD_EGYPTIC_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_OLD_EGYPTIC_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Old-Egyptic calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_NEW_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ORTHODOX_NEW_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Orthodox new-calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ORTHODOX_OLD_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ORTHODOX_OLD_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Orthodox old-calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PERSIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERSIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Persian Jalaali-calendar holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PERSIAN_MTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERSIAN_MTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Persian Jalaali-calendar months"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ZODIACAL_MARKER_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZODIACAL_MARKER_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with zodiacal marker data"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CC_HDY:
        sprintf(s1, "%sq %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_CC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with country specific holidays"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Single country resp., territory code or a list of these,"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("            which are connected by `%s' characters. Countries/territories,"), CONNECT_SEP);
        print_text (fp, s1);
        strcpy(s1, _("            marked by # have an incomplete recording of holidays"));
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("            %-*s = Holidays in %s"),
# if HD_TOP20CC
                   2,
# else /* !HD_TOP20CC */
                   5,
# endif /* !HD_TOP20CC */
                   ptr_cc_holidays->cc_id, _(ptr_cc_holidays->cc_info));
           print_text (fp, s1);
         } while ((++ptr_cc_holidays)->cc_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_ADJUST_VALUE:
        sprintf(s1, "          %s", get_longopt_description (SYM_ADJUST_VALUE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set reference value for rise/set time respectively shadow length"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Angular value respectively factor in range:  %+.1f...%+.1f"),
                larg_lit, -DEGS_PER_06_HOURS, DEGS_PER_06_HOURS);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ATMOSPHERE:
        sprintf(s1, "          %s", get_longopt_description (SYM_ATMOSPHERE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set base data of Earth's atmosphere"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Air pressure and temperature separated by `%s' character"),
                larg_lit, SPLIT_SEP);
        print_text (fp, s1);
        sprintf(s1, _("            Air pressure in Millibar (actual:  %.3f)"), atm_pressure/100.0);
        print_text (fp, s1);
        sprintf(s1, _("            Air temperature in degrees Celsius (actual:  %+.3f)"),
                atm_temperature);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LIMIT:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIMIT, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Limit rise/set times of Sun to the day"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_PRECISE:
        sprintf(s1, "          %s", get_longopt_description (SYM_PRECISE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Represent astronomical times and data with utmost precision"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXECUTE_COMMAND:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXECUTE_COMMAND, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Execute `%c%c[%s]' shell commands"),
                RC_SPECIAL_TEXT_CHAR, RC_SHELL_ESC_CHAR, larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("          and \"TVAR[%s|%c%s]COMMAND\" assignments"),
                RC_TVAR_ICMD_ASSIGN, QUOTE_CHAR, RC_TVAR_UCMD_ASSIGN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
      case SYM_TIME_OFFSET:
        sprintf(s1, "          %s", get_longopt_description (SYM_TIME_OFFSET, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Change base time of astronomical functions"));
        print_text (fp, s1);
# if USE_RC
        strcpy(s1, _("          respectively, change cycle-starting time"));
        print_text (fp, s1);
# endif
        if (time_hour_offset)
          sprintf(s2, "%+d", gmt_timezone_value (time_hour_offset));
        else
          *s2 = '\0';
# if USE_RC
        sprintf(s1, _("   %-3s    = %c|%c|[%c|%c][%s|%s]MMMM|HH%s[MM]   Time offset value (actual:  %s%02d%s%02d == GMT%s)"),
                larg_lit, RC_TIME_CHAR, RC_GMTIME_CHAR, RC_TIME_CHAR, RC_GMTIME_CHAR, ASC_LIT, DES_LIT, time_sep,
                (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
                abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
# else /* !USE_RC */
        sprintf(s1, _("   %-3s    = %c|[%c][%s|%s]MMMM|HH%s[MM]   Time offset value (actual:  %s%02d%s%02d == GMT%s)"),
                larg_lit, staticTIME_CHAR, staticTIME_CHAR, ASC_LIT, DES_LIT, time_sep,
                (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
                abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
# endif /* !USE_RC */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# if USE_RC
      case SYM_CYCLE_END:
        sprintf(s1, "          %s", get_longopt_description (SYM_CYCLE_END, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Change cycle-ending time"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = %c|%c|[%c|%c][%s|%s]MMMM|HH%s[MM]   Ending time value (actual:  %02d%s%02d)"),
                larg_lit, RC_TIME_CHAR, RC_GMTIME_CHAR, RC_TIME_CHAR, RC_GMTIME_CHAR,
                ASC_LIT, DES_LIT, time_sep, (int)MM2HH(loop_end), time_sep,
                (loop_end>SPECIAL_VALUE) ? loop_end%MINS_PER_HOUR : 0);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_CYCLE_STEP:
        sprintf(s1, "          %s", get_longopt_description (SYM_CYCLE_STEP, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Change cycle-timestep"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = MMMM|HH%s[MM]   Timestep value (actual:  %02d%s%02d)"),
                larg_lit, time_sep, (int)MM2HH(loop_step), time_sep, loop_step%MINS_PER_HOUR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
# endif
      case SYM_TRANSFORM_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_TRANSFORM_YEAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Change base year of calendar"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = [%s|%s]YYYY   Year offset value (actual:  %d)"),
                larg_lit, ASC_LIT, DES_LIT, (transform_year) ? transform_year : YEAR_MIN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_GREG_REFORM:
        sprintf(s1, "          %s", get_longopt_description (SYM_GREG_REFORM, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set period of Gregorian Reformation"));
        print_text (fp, s1);
        sprintf(s1, _("          Actual respected period:  %02d-%02d %s %0*d"),
                greg->first_day, greg->last_day, short_month_name (greg->month),
                len_year_max, greg->year);
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("   %-*d%*s = Set period to:  %02d-%02d %s %0*d"),
                   len_year_max, ptr_greg->year, 6-len_year_max, "",
                   ptr_greg->first_day, ptr_greg->last_day,
                   short_month_name (ptr_greg->month), len_year_max, ptr_greg->year);
           print_text (fp, s1);
         } while ((++ptr_greg)->year);
        sprintf(s1, _("   %-3s    = %s%c%s%s%s%s%s%s   Set period explicitly"),
                larg_lit, yyyy_lit, *yyyy_lit, SPLIT_SEP, mm_lit, SPLIT_SEP, dd_lit, SPLIT_SEP, dd_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_DATE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_DATE_FORMAT, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Set order of date elements using the %s format text"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("          Actual:  (%s) `%s' (%s)"),
                (date_format->df_id == (char *)NULL) ? _("self-defined") : date_format->df_id,
                date_format->df_format, _(date_format->df_info));
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("   %-6s = Format is:  `%s' (%s)"), ptr_date_format->df_id,
                   ptr_date_format->df_format, _(ptr_date_format->df_info));
           print_text (fp, s1);
         } while ((++ptr_date_format)->df_info != (char *)NULL);
        sprintf(s1, _("   %-3s    = Define individual format. Respected format elements are:"),
                larg_lit);
        print_text (fp, s1);
        do
         {
           if (!ptr_info_date_format->di_1format)
             sprintf(s1, "               %s", _(ptr_info_date_format->di_info));
           else
            {
              if (ptr_info_date_format->di_1format != DFORMAT_CHAR)
               {
                 if (ptr_info_date_format->di_2format)
                   sprintf(s2, _(ptr_info_date_format->di_info), ptr_info_date_format->di_2format);
                 else
                   sprintf(s2, _(ptr_info_date_format->di_info), FWIDTH_MIN, FWIDTH_MAX);
                 sprintf(s1, "            %c%c = %s", ptr_info_date_format->di_1format,
                         (ptr_info_date_format->di_2format) ? ptr_info_date_format->di_2format : ' ', s2);
               }
              else
                sprintf(s1, "            %c%c = %s", ptr_info_date_format->di_1format,
                        ptr_info_date_format->di_2format, _(ptr_info_date_format->di_info));
            }
           print_text (fp, s1);
         } while ((++ptr_info_date_format)->di_info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_TRANSLATE_STRING:
        sprintf(s1, "          %s", get_longopt_description (SYM_TRANSLATE_STRING, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Define translatable country specific special character pairs"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = UPPER-CASE-LETTER\"\"LOWER-CASE-LETTER... definitions"),
                larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("            E.g. a `%s%s%s%s%s%s%s%s' %-3s causes the correct conversion of"),
                AAE, AE, OOE, OE, UUE, UE, SZ, SZ, larg_lit);
        print_text (fp, s1);
        strcpy(s1, _("            the preceding special characters in an individual date format,"));
        print_text (fp, s1);
        strcpy(s1, _("            which has a STYLE format instruction component, and that, how"));
        print_text (fp, s1);
        strcpy(s1, _("            they are used by the character set used in Germany"));
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
# if USE_RC
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, _("+++ FIXED DATES +++"));
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_TODAY:
      case SYM_FIXED_DATES_LIST:
      case SYM_DESC_FIXED_DATES_LIST:
        sprintf(s1, "%sc|C[MOD] %s", SWITCH, get_longopt_description (SYM_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_TODAY, TRUE));
        print_text (fp, s1);
#  ifdef GCAL_SHELL
        sprintf(s1, _("          Use standard resource file `.%s%s' for fixed date list"),
                PRGR_NAME, RC_SUFFIX);
#  else /* !GCAL_SHELL */
        sprintf(s1, _("          Use standard resource file `%s%s' for fixed date list"),
                PRGR_NAME, RC_SUFFIX);
#  endif /* !GCAL_SHELL */
        print_text (fp, s1);
        strcpy(s1, _("          Implies period:  Today"));
        print_text (fp, s1);
        strcpy(s1, _(" c        = Display only those dates, for which fixed dates exists"));
        print_text (fp, s1);
        strcpy(s1, _("   C      = Display those dates, for which fixed dates doesn't exist, too"));
        print_text (fp, s1);
        sprintf(s1, _("    [MOD] = %s   Descending sort order"), DES_LIT);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("    [MOD] = One or more of the following modifiers which are marked by #"));
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HEADING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_HEADING_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("            %-3s         = Heading/Title text used for fixed date list"),
                larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("                          Actual:  `%s'"), rc_heading_text+2);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_GROUPING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_GROUPING_TEXT, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # g[MOD]       = Group fixed dates by day using text MOD"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("          1. Representation of text"));
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_BIORHYTHM_AXIS_LEN:
        sprintf(s1, "          %s", get_longopt_description (SYM_BIORHYTHM_AXIS_LEN, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Set width of the biorhythm text graphics"));
        print_text (fp, s1);
        sprintf(s1, _("                           %-3s = %d...%d   Length of a single axis"),
                larg_lit, BIO_AXIS_MIN, BIO_AXIS_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MOONIMAGE_LINES:
        sprintf(s1, "          %s", get_longopt_description (SYM_MOONIMAGE_LINES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Set height of the Moon phase text graphics"));
        print_text (fp, s1);
        sprintf(s1, _("                           %-3s = %d...%d   Total number of lines"),
                larg_lit, MOONIMAGE_MIN, MOONIMAGE_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_FILENAME:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_FILENAME, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # a            = Display origin of fixed date"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ALTERNATIVE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_ALTERNATIVE_FORMAT, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # A            = Display using alternative list format"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_HOLIDAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_HOLIDAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # e            = Include legal days and memorial days"));
        print_text (fp, s1);
        strcpy(s1, _("          # E            = Include legal days only"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_WEEK_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_WEEK_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # k            = Display week number"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_OMIT_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_OMIT_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # o            = Omit repeating date part of fixed dates"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_FDLIST_SEP:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_FDLIST_SEP, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # Q            = Suppress leading blank line of fixed dates list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # U            = Suppress date part of fixed dates"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_SUPPRESS_TEXT_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_TEXT_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # J            = Suppress text part of fixed dates"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_EXCLUDE_RC_TITLE:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXCLUDE_RC_TITLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # x            = Exclude title of fixed date list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_CONS_NUMBER:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_CONS_NUMBER, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # z            = Display consecutive number of fixed date"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_ZERO_DATES:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZERO_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # Z            = Display dates, for which fixed dates doesn't exist"));
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("          2. Respected period"));
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_LEAP_DAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_LEAP_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Manage `29-FEBRUARY' in non-leap years"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `february',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("                           %-3s = february   Respect at `28-FEBRUARY'"), larg_lit);
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `march',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("                           %-3s = march      Respect at `01-MARCH'"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_INCLUDE_TODAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_TODAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # d            = Include actual date if list of periods is generated"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_LIST_MODE:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIST_MODE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # l            = Generate list of periods instead of a single period"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_PERIOD:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERIOD, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # Nd           = Single absolute day N"));
        print_text (fp, s1);
        sprintf(s1, _("          # %cdN          = Single absolute day N"), RC_NWD_CHAR);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, _("          # NMOD         = Single day N relative to today"));
        print_text (fp, s1);
        sprintf(s1, _("             MOD           = %s   Forwards"), ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   Backwards"), DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, _("          # Nw           = Complete week N"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 0        1st week / last week of previous year"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 1...52   1st...52nd week (always)"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 53       53rd week (sometimes)"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 99       Last week"));
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %s%s         = Single day %s of month %s"),
                mm_lit, dd_lit, dd_lit, mm_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %s%sN       = Single N'th weekday %s of month %s"),
                mm_lit, www_lit, www_lit, mm_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...4   1st...4th weekday %s (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 5       5th weekday %s (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 9       Last weekday %s"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %cdN%s       = Single N'th weekday %s"),
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...51   1st...51st weekday %s (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 52|53    52|53rd weekday %s (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 99       Last weekday %s"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %cwN%s       = Single weekday %s of N'th week"),
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 0        %s which isn't located in 1st week"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...51   %s of 1st...51st week (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 52|53    %s of 52|53rd week (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 99       %s of last week"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[[%s|%s]N]   = Single day N relative to Easter Sunday"),
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[%s|%s]N%s  = Single N'th weekday relative to Easter Sunday"),
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[[%s|%s]N]   = Single day N relative to today's date"),
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[%s|%s]N%s  = Single N'th weekday relative to today's date"),
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c?[[%s|%s]N]   = Single day N relative to date variable"),
                RC_HDY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c?[%s|%s]N%s  = Single N'th weekday relative to date variable"),
                RC_HDY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
      case SYM_TOMORROW:
        sprintf(s1, "          %s", get_longopt_description (SYM_TOMORROW, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # t|T          = List tomorrow"));
        print_text (fp, s1);
        if (longopt_symbolic == SYM_TOMORROW)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_WEEK:
      case SYM_END_OF_WEEK:
      case SYM_START_OF_WEEK:
        sprintf(s1, "          %s", get_longopt_description (SYM_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_WEEK, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # w|W[MOD]     = List complete week"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of week"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of week"),
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_WEEK
            || longopt_symbolic == SYM_END_OF_WEEK
            || longopt_symbolic == SYM_START_OF_WEEK)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_MONTH:
      case SYM_END_OF_MONTH:
      case SYM_START_OF_MONTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_MONTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # m|M[MOD]     = List complete month"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of month"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of month"),
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_MONTH
            || longopt_symbolic == SYM_END_OF_MONTH
            || longopt_symbolic == SYM_START_OF_MONTH)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_YEAR:
      case SYM_END_OF_YEAR:
      case SYM_START_OF_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_YEAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # y|Y[MOD]     = List complete year"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of year"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of year"),
                DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_RESOURCE_FILE:
        sprintf(s1, "%sf|F %-3s  %s", SWITCH, larg_lit, get_longopt_description (SYM_RESOURCE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use alternative file(s) instead of standard resource file"));
        print_text (fp, s1);
        strcpy(s1, _("          Implies period:  Today"));
        print_text (fp, s1);
        strcpy(s1, _(" f        = Display only those dates, for which fixed dates exists"));
        print_text (fp, s1);
        strcpy(s1, _("   F      = Display those dates, for which fixed dates doesn't exist, too"));
        print_text (fp, s1);
        sprintf(s1, _("     %-3s  = Single file or list of files connected by `%s' characters"),
                larg_lit, CONNECT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        /* Fallthrough. */
        print_text (fp, s1);
      case SYM_HERE_FILE:
        sprintf(s1, "%s# %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HERE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Define additional resource file line"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Any possible resource file line"), larg_lit);
        print_text (fp, s1);
      default:
          ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("%cDATE"), RC_ADATE_CHAR);
      print_text (fp, s1);
      strcpy(s1, _("Use given `DATE' instead of today's date"));
      print_text (fp, s1);
      sprintf(s1, _("Date format:  %s[%s[%s|%s[N]]], %s%c%c|%c|DVAR[[%s|%s]N[%s]], %s%cdN[%s]"),
              yyyy_lit, mm_lit, dd_lit, www_lit, yyyy_lit, RC_HDY_CHAR, RC_EASTER_CHAR, RC_TODAY_CHAR,
              ASC_LIT, DES_LIT, www_lit, yyyy_lit, RC_NWD_CHAR, www_lit);
      print_text (fp, s1);
      sprintf(s1, _("              %s%cwN[%s], month name[%s], weekday name[N] or %s"),
              yyyy_lit, RC_NWD_CHAR, www_lit, dd_lit, dd_lit);
      print_text (fp, s1);
    }
# endif /* USE_RC */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("%cFILE"), RSP_CHAR);
      print_text (fp, s1);
      strcpy(s1, _("Preload options and commands from `FILE'"));
      print_text (fp, s1);
      print_text (fp, s1);
      print_text (fp, s1);
      strcpy(s1, _("COMMAND"));
      print_text (fp, s1);
      sprintf(s1, _("%s        = Month in range:  %d...%d"), mm_lit, MONTH_MIN, MONTH_MAX);
      print_text (fp, s1);
      sprintf(s1, _("                        or:  month name | %s | %s%s | %s%s | %s%s"),
              MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, ASC_LIT, MONTH3_LIT, DES_LIT);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("            List:   %s[%s%s]%s...%s%s[%s%s]"),
              mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit, YEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, _("            Range:  %s[%s%s]%s%s[%s%s] | %s%s%s%s%s | %s %s%s%s"),
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, mm_lit, YEAR_SEP, yyyy_lit,
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, yyyy_lit,
              mm_lit, yyyy_lit, MRANGE_SEP, yyyy_lit);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("%s%*s    = Year  in range:  %d...%d"),
              yyyy_lit, 6-len_year_max, "", YEAR_MIN, YEAR_MAX);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("            List:   [%s%s]%s%s...%s[%s%s]%s"),
              mm_lit, FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, _("            Range:  [%s%s]%s%s[%s%s]%s | %s%s%s %s%s%s"),
              mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit,
              mm_lit, MRANGE_SEP, mm_lit, yyyy_lit, YRANGE_SEP, yyyy_lit);
    }
#endif /* !USE_DE */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      my_help_tail_text (fp);
    }
}



    void
my_basic_help (fp)
   FILE *fp;
/*
   Prints the "basic help" text to file `fp' using the central
     output function `print_text()' and uses global text buffer
     `s1' internally.
*/
{
   my_help_head_text (fp);
#if USE_DE
   sprintf(s1, "  %sh,  %s          Diesen Hilfetext ausgeben und Programm beenden",
           SWITCH, get_longopt_description (SYM_HELP, TRUE));
   print_text (fp, s1);
   sprintf(s1, "  %shh, %s   Erweiterten Hilfetext ausgeben und Programm beenden",
           SWITCH, get_longopt_description (SYM_LONG_HELP2, TRUE));
   print_text (fp, s1);
   sprintf(s1, "  %sL,  %s       Softwarelizenz ausgeben und Programm beenden",
           SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
   print_text (fp, s1);
   sprintf(s1, "  %sV,  %s       Versionsinformation ausgeben und Programm beenden",
           SWITCH, get_longopt_description (SYM_VERSION, TRUE));
   print_text (fp, s1);
# if USE_PAGER
#  ifdef GCAL_EPAGER
   if (ext_pager != (char *)NULL)
     sprintf(s1, "  %sp,  %s         Ausgabe durch externen `%s' Paginierer lenken",
             SWITCH, get_longopt_description (SYM_PAGER, TRUE),
             (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
   else
     sprintf(s1, "  %sp,  %s         Ausgabe durch einfachen internen Paginierer lenken",
             SWITCH, get_longopt_description (SYM_PAGER, TRUE));
#  else /* !GCAL_EPAGER */
   sprintf(s1, "  %sp,  %s         Ausgabe durch einfachen internen Paginierer lenken",
           SWITCH, get_longopt_description (SYM_PAGER, TRUE));
#  endif /* !GCAL_EPAGER */
# endif /* USE_PAGER */
#else /* !USE_DE */
   sprintf(s1, _("  %sh,  %s          Display this help text and quit program"),
           SWITCH, get_longopt_description (SYM_HELP, TRUE));
   print_text (fp, s1);
   sprintf(s1, _("  %shh, %s   Display extended help text and quit program"),
           SWITCH, get_longopt_description (SYM_LONG_HELP2, TRUE));
   print_text (fp, s1);
   sprintf(s1, _("  %sL,  %s       Display software license and quit program"),
           SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
   print_text (fp, s1);
   sprintf(s1, _("  %sV,  %s       Display version information and quit program"),
           SWITCH, get_longopt_description (SYM_VERSION, TRUE));
   print_text (fp, s1);
# if USE_PAGER
#  ifdef GCAL_EPAGER
   if (ext_pager != (char *)NULL)
     sprintf(s1, _("  %sp,  %s         Direct output through external `%s' pager"),
             SWITCH, get_longopt_description (SYM_PAGER, TRUE),
             (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
   else
     sprintf(s1, _("  %sp,  %s         Direct output through simple internal pager"),
             SWITCH, get_longopt_description (SYM_PAGER, TRUE));
#  else /* !GCAL_EPAGER */
   sprintf(s1, _("  %sp,  %s         Direct output through simple internal pager"),
           SWITCH, get_longopt_description (SYM_PAGER, TRUE));
#  endif /* !GCAL_EPAGER */
# endif /* USE_PAGER */
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, lopt_msg ());
   print_text (fp, s1);
   my_bug_report_address (fp);
}



    void
my_license (fp)
   FILE *fp;
/*
   Prints the program-id and the license text to file `fp' using
     the central output function `print_text()', and uses the global
     text buffer `s1' internally.
*/
{
   my_copyright (fp, FALSE);
#if USE_DE
   sprintf(s1, "Diese Software erhebt keinen Anspruch auf Vollst%sndigkeit,", AE);
   print_text (fp, s1);
   sprintf(s1, "Verwendungsf%shigkeit oder Korrektheit.  F%sr JEDWEDE Sch%sden oder", AE, UE, AE);
   print_text (fp, s1);
   strcpy(s1, "Verluste (implizit oder explizit), die aus Nutzung oder Handhabung");
   print_text (fp, s1);
   sprintf(s1, "meiner Software hervorgehen, wird grunds%stzlich keine Haftung", AE);
   print_text (fp, s1);
   sprintf(s1, "%sbernommen.  Bei Verwendung dieser Software erkl%srt sich der Benutzer", UE, AE);
   print_text (fp, s1);
   strcpy(s1, "mit dieser Vereinbarung in vollem Umfang einverstanden.");
   print_text (fp, s1);
   print_text (fp, s1);
   sprintf(s1, "Dieses Programm ist freie Software; Sie k%snnen es weitervertreiben", OE);
   print_text (fp, s1);
   sprintf(s1, "und/oder ver%sndern innerhalb der Bestimmungen der `GNU General Public", AE);
   print_text (fp, s1);
   strcpy(s1, "License'; herausgegeben von der `Free Software Foundation';");
   print_text (fp, s1);
   sprintf(s1, "in der Version 2 oder jeder sp%steren Version.", AE);
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, "Sie sollten eine Kopie der `GNU General Public License' zusammen mit");
   print_text (fp, s1);
   strcpy(s1, "diesem Programm erhalten haben; falls nicht, schreiben Sie an die:");
#else /* !USE_DE */
   strcpy(s1, _("This software doesn't claim completeness, correctness or usability."));
   print_text (fp, s1);
   strcpy(s1, _("On principle I will not be liable for ANY damages or losses (implicit"));
   print_text (fp, s1);
   strcpy(s1, _("or explicit), which result from using or handling my software."));
   print_text (fp, s1);
   strcpy(s1, _("If you use this software, you agree without any exception to this"));
   print_text (fp, s1);
   strcpy(s1, _("agreement, which binds you LEGALLY !!"));
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, _("This program is free software; you can redistribute it and/or modify"));
   print_text (fp, s1);
   strcpy(s1, _("it under the terms of the `GNU General Public License' as published by"));
   print_text (fp, s1);
   strcpy(s1, _("the `Free Software Foundation'; either version 3, or (at your option)"));
   print_text (fp, s1);
   strcpy(s1, _("any later version."));
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, _("You should have received a copy of the `GNU General Public License'"));
   print_text (fp, s1);
   strcpy(s1, _("along with this program; if not, write to the:"));
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, "   Free Software Foundation, Inc.");
   print_text (fp, s1);
}



    void
my_version (fp)
   FILE *fp;
/*
   Prints the program-id and all compilation flags to file `fp'
     using the central output function `print_text()', and
     uses the global text buffers `s1' and `s2' internally.
*/
{
   auto     const Cc_struct  *ptr_cc_holidays=cc_holidays;
   register       int         i=0;
#if USE_RC
   auto           char       *ptr_env;
# if !defined(AMIGA) || defined(__GNUC__)
   auto           char       *ptr_home;
# endif
#endif


   my_copyright (fp, TRUE);
   print_compiler_info (fp);
   print_text (fp, s1);
#if USE_DE
   strcpy(s1, "Kompilations-Optionen:");
#else /* !USE_DE */
   strcpy(s1, _("Compilation options:"));
#endif /* !USE_DE */
#ifdef GCAL_NLS
   strcat(s1, " NLS");
   i++;
#endif
#if !HAVE_LIBM
   strcat(s1, " LIBM_EMU");
   i++;
#endif
#if USE_EASC
   strcat(s1, " USE_EASC");
   i++;
#endif
#if USE_DE
   strcat(s1, " USE_DE");
   i++;
#endif
#if USE_HLS
   strcat(s1, " USE_HLS");
   i++;
#endif
#if USE_PAGER
   strcat(s1, " USE_PAGER");
   i++;
#endif
#if USE_RC
   strcat(s1, " USE_RC");
   i++;
# if HAVE_GNU_RE_COMPILE_PATTERN
   strcat(s1, " GNU-REGEX");
   i++;
# endif
# if HAVE_POSIX_REGCOMP
   strcat(s1, " POSIX-REGEX");
   i++;
# endif
# if HAVE_RE_COMP
   strcat(s1, " BSD-REGEX");
   i++;
# endif
# if HAVE_REGCMP
   strcat(s1, " SysV-REGEX");
   i++;
# endif
# if HAVE_V8_REGCOMP
   strcat(s1, " V8-REGEX");
   i++;
# endif
# if NO_REGEX
   strcat(s1, " NO-REGEX");
   i++;
# endif
#endif /* USE_RC */
   if (i)
     print_text (fp, s1);
   print_text (fp, s1);
#if USE_RC
# if !defined(AMIGA) || defined(__GNUC__)
   ptr_env = getenv(ENV_VAR_SYS_DATADIR);
   if (ptr_env != (char *)NULL)
    {
      if (!*ptr_env)
        ptr_env = GCAL_SYS_DATADIR;
    }
   else
# endif /* !AMIGA || __GNUC__ */
     ptr_env = GCAL_SYS_DATADIR;
   sprintf(s1, "GCAL_SYS_DATADIR=%s", ptr_env);
   print_text (fp, s1);
# if !defined(AMIGA) || defined(__GNUC__)
   ptr_env = getenv(ENV_VAR_USR_DATADIR);
   if (ptr_env == (char *)NULL)
     ptr_env = GCAL_USR_DATADIR;
   else
     if (!*ptr_env)
       ptr_env = GCAL_USR_DATADIR;
   i = 1;
   ptr_home = getenv(ENV_VAR_HOME);
   if (ptr_home == (char *)NULL)
     i--;
   else
     if (!*ptr_home)
       i--;
   if (i)
    {
      if (*(ptr_home+(strlen(ptr_home)-1)) == *DIR_SEP)
        sprintf(s1, "GCAL_USR_DATADIR=%s%s", ptr_home, ptr_env);
      else
        sprintf(s1, "GCAL_USR_DATADIR=%s%s%s", ptr_home, DIR_SEP, ptr_env);
    }
   else
     sprintf(s1, "GCAL_USR_DATADIR=$%s%s%s", ENV_VAR_HOME, DIR_SEP, ptr_env);
# else /* AMIGA && !__GNUC__ */
   sprintf(s1, "GCAL_USR_DATADIR=%s", GCAL_USR_DATADIR);
# endif /* AMIGA && !__GNUC__ */
   print_text (fp, s1);
   print_text (fp, s1);
#endif /* USE_RC */
#if USE_DE
   sprintf(s1, "Starttag der Woche: %s", day_name (start_day));
#else /* !USE_DE */
   sprintf(s1, _("Starting day of week: %s"), day_name (start_day));
#endif /* USE_DE */
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Art der Wochennummern: %s",
           (iso_week_number) ? "ISO-8601:1988" : "Standard");
#else /* !USE_DE */
   sprintf(s1, _("Type of week numbers: %s"),
           (iso_week_number) ? "ISO-8601:1988" : _("Standard"));
#endif /* USE_DE */
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Basisjahr des Kalenders: %d",
           (transform_year) ? transform_year : YEAR_MIN);
#else /* !USE_DE */
   sprintf(s1, _("Base year of calendar: %d"),
           (transform_year) ? transform_year : YEAR_MIN);
#endif /* USE_DE */
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Gregorianische Reformation: %02d-%02d %s %0*d", greg->first_day,
           greg->last_day, short_month_name (greg->month), len_year_max, greg->year);
#else /* !USE_DE */
   sprintf(s1, _("Gregorian Reformation: %02d-%02d %s %0*d"), greg->first_day,
           greg->last_day, short_month_name (greg->month), len_year_max, greg->year);
#endif /* !USE_DE */
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Datumformat: (%s) `%s' (%s)",
           (date_format->df_id == (char *)NULL) ? "Selbstdefiniert" : date_format->df_id,
           date_format->df_format, date_format->df_info);
#else /* !USE_DE */
   sprintf(s1, _("Date format: (%s) `%s' (%s)"),
           (date_format->df_id == (char *)NULL) ? _("self-defined") : date_format->df_id,
           date_format->df_format, _(date_format->df_info));
#endif /* !USE_DE */
   print_text (fp, s1);
   i = 1;
   while ((++ptr_cc_holidays)->cc_info != (char *)NULL)
     i++;
#if USE_DE
   sprintf(s1, "Ber%scksichtigte L%snder/Territorien: %d", UE, AE, i);
#else /* !USE_DE */
   sprintf(s1, _("Respected countries/territories: %d"), i);
#endif /* !USE_DE */
   print_text (fp, s1);
   if (time_hour_offset)
     sprintf(s2, "%+d", gmt_timezone_value (time_hour_offset));
   else
     *s2 = '\0';
#if USE_DE
   sprintf(s1, "Zeitversatzwert der astronomischen Funktionen: %s%02d%s%02d == GMT%s",
           (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
           abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
#else /* !USE_DE */
   sprintf(s1, _("Time offset value of astronomical functions: %s%02d%s%02d == GMT%s"),
           (time_hour_offset+time_min_offset < 0) ? DES_LIT : ASC_LIT,
           abs(time_hour_offset), time_sep, abs(time_min_offset), s2);
#endif /* !USE_DE */
   print_text (fp, s1);
   my_bug_report_address (fp);
}



    char *
usage_msg ()
/*
   Creates the usage text `usg_text' and includes the actual program name.
*/
{
   static char  *usg_text;
   static Bool   is_initialized=FALSE;


   if (!is_initialized)
    {
      usg_text = (char *)my_malloc (LEN_SINGLE_LINE, ERR_NO_MEMORY_AVAILABLE,
                                    __FILE__, ((long)__LINE__)-1L,
                                    "usg_text", 0);
#if USE_DE
      sprintf(usg_text, "Aufruf: %s [%s|%s{[?|h|??|hh|L|V]|{%sHKNOR%sXb%sijn%sq%ssu%s}}] [[%s] [%s]]",
              prgr_name, SWITCH, SWITCH2, USAGE_RC1, USAGE_SHELL, USAGE_RC2, USAGE_PAGER,
              USAGE_RC3, USAGE_RC4, mm_lit, yyyy_lit);
#else /* !USE_DE */
      sprintf(usg_text, _("Usage:  %s [%s|%s{[?|h|??|hh|L|V]|{%sHKNOR%sXb%sijn%sq%ssu%s}}] [[%s] [%s]]"),
              prgr_name, SWITCH, SWITCH2, USAGE_RC1, USAGE_SHELL, USAGE_RC2, USAGE_PAGER,
              USAGE_RC3, USAGE_RC4, mm_lit, yyyy_lit);
#endif /* !USE_DE */
      is_initialized = TRUE;
    }

   return(usg_text);
}



    char *
lopt_msg ()
/*
   Returns the `lopt_text' description text of the long-style options
     including the actual program name, and uses the global text
     buffer `s3' internally.
*/
{
   static char  *lopt_text;
   static Bool   is_initialized=FALSE;


   if (!is_initialized)
    {
      lopt_text = (char *)my_malloc (LEN_SINGLE_LINE, ERR_NO_MEMORY_AVAILABLE,
                                     __FILE__, ((long)__LINE__)-1L,
                                     "lopt_text", 0);
#if USE_DE
      sprintf(lopt_text, "Benutze `%s %s|[%s?]",
              prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE), LARG_SEP);
#else /* !USE_DE */
      sprintf(lopt_text, _("Use `%s %s|[%s?]"),
              prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE), LARG_SEP);
#endif /* !USE_DE */
#if USE_PAGER
      sprintf(s3, " %s' ", get_longopt_description (SYM_PAGER, TRUE));
      strcat(lopt_text, s3);
#else /* !USE_PAGER */
      strcat(lopt_text, "' ");
#endif /* !USE_PAGER */
#if USE_DE
      sprintf(s3, "f%sr mehr Information.", UE);
      strcat(lopt_text, s3);
#else /* !USE_DE */
      strcat(lopt_text, _("for more information."));
#endif /* !USE_DE */
      is_initialized = TRUE;
    }

   return(lopt_text);
}



   static int
gmt_timezone_value (hour)
   int hour;
/*
   Returns the given HOUR offset value as an offset value N
     within the range -12...-1...[0]...+1...+12, which is
     based on the Greenwich Mean Time timezone `GMT[[+|-]N]'.
*/
{
   register int  sign=-SGN(hour);


   hour = abs(hour);
   while (hour > HOURS_PER_DAY)
     hour -= HOURS_PER_DAY;
   if (hour > HOURS_PER_HALF_DAY)
     hour -= (HOURS_PER_DAY + 1);

   return(hour*sign);
}



   static const char *
get_longopt_description (longopt_symbolic, with_larglist)
   const int  longopt_symbolic;
   const Bool with_larglist;
/*
   Returns a description text of given `longopt_symbolic' long-style option
     in global text buffer `s2'.  If `with_larglist' is TRUE, the returned
     description text contains the argument list text (if any) too, otherwise
     the returned description text contains the complete name of the
     long option only!
*/
{
   auto     const Lopt_struct  *ptr_lopt=lopt;
   register       int           i;


   while (ptr_lopt->long_name != NULL)
    {
      if (ptr_lopt->symbolic_name == longopt_symbolic)
        break;
      ptr_lopt++;
    }
   if (ptr_lopt->long_name != NULL)
    {
      i = 0;
      sprintf(s2, "%s%s%s", SWITCH, SWITCH, ptr_lopt->long_name);
      if (   with_larglist
          && (ptr_lopt->larg_mode != LARG_NO))
       {
         if (ptr_lopt->larg_mode == LARG_NO_OR_ONE)
          {
            strcat(s2, "[");
            strcat(s2, LARG_SEP);
          }
         else
           strcat(s2, LARG_SEP);
         if (   (ptr_lopt->larg_mode == LARG_ONE)
             && (ptr_lopt->largs[0] == NULL))
           strcat(s2, larg_lit);
         else
          {
            if (ptr_lopt->largs[0] == NULL)
              strcat(s2, larg_lit);
            else
              while (ptr_lopt->largs[i] != NULL)
               {
                 strcat(s2, ptr_lopt->largs[i]);
                 if (ptr_lopt->largs[i+1] != NULL)
                   strcat(s2, "|");
                 i++;
               }
          }
         if (ptr_lopt->larg_mode == LARG_NO_OR_ONE)
           strcat(s2, "]");
       }
    }
   else
     *s2 = '\0';

   return(s2);
}



   static void
my_bug_report_address (fp)
   FILE *fp;
/*
   Prints the bug report address to file `fp' using the central output
   function `print_text()', and uses the global text buffer `s1' internally.
*/
{
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Fehlerberichte via eMail an <%s>", BUG_REPORT_ADR1);
   print_text (fp, s1);
   sprintf(s1, "oder (falls das fehlschl%sgt) an <%s>.", AE, BUG_REPORT_ADR2);
#else /* !USE_DE */
   sprintf(s1, _("Email bug reports to <%s>"), BUG_REPORT_ADR1);
   print_text (fp, s1);
   sprintf(s1, _("or (if this fails) to <%s>."), BUG_REPORT_ADR2);
#endif /* !USE_DE */
   print_text (fp, s1);
}



   static void
my_copyright (fp, with_short_license)
         FILE *fp;
   const Bool  with_short_license;
/*
   Prints the program-id and copyright text to file `fp' using
     the central output function `print_text()', and uses the global
     text buffer `s1' internally.
*/
{
   sprintf(s1, "%s (GNU cal) %s", prgr_name, VERSION_NO);
   print_text (fp, s1);
   if (!with_short_license)
     print_text (fp, s1);
   strcpy(s1, COPYRIGHT_TXT);
   print_text (fp, s1);
   if (with_short_license)
    {
#if USE_DE
      strcpy(s1, "Dies ist freie Software; in den Quellen befindet sich die Lizenz-");
      print_text (fp, s1);
      sprintf(s1, "und Kopierbedingung.  Es gibt KEINERLEI Garantie, nicht einmal f%sr", UE);
      print_text (fp, s1);
      strcpy(s1, "die TAUGLICHKEIT oder die VERWENDBARKEIT ZU EINEM ANGEGEBENEN ZWECK.");
#else /* !USE_DE */
      strcpy(s1, _("This is free software; see the source for copying conditions."));
      print_text (fp, s1);
      strcpy(s1, _("There is NO warranty, without even the implied warranty of"));
      print_text (fp, s1);
      strcpy(s1, _("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."));
#endif /* !USE_DE */
      print_text (fp, s1);
    }
   print_text (fp, s1);
}



   static void
my_help_head_text (fp)
   FILE *fp;
/*
   Prints the help heading text to file `fp' using the central
     output function `print_text()', and uses the global text buffer
     `s1' internally.
*/
{
#if USE_DE
   sprintf(s1, "%s: Das Gregorianische Kalenderprogramm (GNU cal) %s",
           prgr_name, VERSION_NO);
#else /* !USE_DE */
   sprintf(s1, _("%s: The Gregorian calendar program (GNU cal) %s"),
           prgr_name, VERSION_NO);
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   strcat(s1, COPYRIGHT_TXT);
   print_text (fp, s1);
   print_text (fp, s1);
#if USE_RC
# if USE_DE
   sprintf(s1, "Aufruf: %s [[OPTION...] [%cDATUM] [%cDATEI...]] [KOMMANDO]",
           prgr_name, RC_ADATE_CHAR, RSP_CHAR);
# else /* !USE_DE */
   sprintf(s1, _("Usage:  %s [[OPTION...] [%cDATE] [%cFILE...]] [COMMAND]"),
           prgr_name, RC_ADATE_CHAR, RSP_CHAR);
# endif /* !USE_DE */
#else /* !USE_RC */
# if USE_DE
   sprintf(s1, "Aufruf: %s [[OPTION...] [%cDATEI...]] [KOMMANDO]", prgr_name, RSP_CHAR);
# else /* !USE_DE */
   sprintf(s1, _("Usage:  %s [[OPTION...] [%cFILE...]] [COMMAND]"), prgr_name, RSP_CHAR);
# endif /* !USE_DE */
#endif /* !USE_RC */
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, usage_msg ());
   print_text (fp, s1);
   print_text (fp, s1);
}



   static void
my_help_tail_text (fp)
   FILE *fp;
/*
   Prints the help tail text to file `fp' using the central output function
     `print_text()', and uses the global text buffer `s1' internally.
*/
{
   *s1 = '\0';
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "   +++  Bei zweistelliger Jahresangabe wird NICHT von %2d%s ausgegangen  +++",
           act_year/100, yy_lit);
   print_text (fp, s1);
   sprintf(s1, "   +++  `%s' ist freie Software, viel Spa%s damit   =8^)  +++", prgr_name, SZ);
#else /* !USE_DE */
   sprintf(s1, _("   +++  If you specify two digits for the year I DON'T assume %2d%s  +++"),
           act_year/100, yy_lit);
   print_text (fp, s1);
   sprintf(s1, _("   +++  `%s' is free software, enjoy   =8^)  +++"), prgr_name);
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, "------------------------oOO      \\\\\\_''/      OOo---------------------------");
   print_text (fp, s1);
   strcpy(s1, "Thomas Esken               O     (/o-o\\)     O  eMail: ");
   strcat(s1, BUG_REPORT_ADR1);
   print_text (fp, s1);
   strcpy(s1, "Im Hagenfeld 84                 ((  ^  ))       Phone: +49 251 232585");
   print_text (fp, s1);
   strcpy(s1, "D-48147 Muenster; Germany    \\____) ~ (____/    MotD : 2old2live, 2young2die");
   print_text (fp, s1);
}



/*
   Writes an informational text to "fp" using the central output function
     showing the date, compiler and operating system used.  Uses the global
     text buffer `s1' internally.  The code is taken from `zip21' source
     package and adjusted to Gcal's needs.
   Special thanks to the Info-ZIP group (`http://quest.jpl.nasa.gov/Info-ZIP/'),
     which merely granted the permission to use their `version_local()' function
     of `zip-2.1' in a modified way for Gcal (the `print_compiler_info()' function
     is based on copyrighted code by the Info-ZIP group).
*/
#if defined(MSDOS)
   static void
print_compiler_info (fp)
   FILE *fp;
{
# if defined(__DJGPP__) || defined(__WATCOMC__) || (defined(_MSC_VER) && (_MSC_VER != 800))
   char buf[80];
# endif


   sprintf(s1, _(*compiler_info),

# ifdef __GNUC__
#  ifdef __DJGPP__
   (sprintf(buf, "djgpp v%d / gcc ", __DJGPP__), buf),
#  else
#   ifdef __GO32__           /* __GO32__ is defined as "1" only (sigh) */
   "djgpp v1.x / gcc ",
#   else
#    ifdef __EMX__            /* ...so is __EMX__ (double sigh) */
   "emx+gcc ",
#    else
   "gcc ",
#    endif
#   endif
#  endif
   __VERSION__,
# else
#  ifdef __WATCOMC__
#   if (__WATCOMC__ % 10 > 0)
/* We do this silly test because __WATCOMC__ gives two digits for the  */
/* minor version, but Watcom packaging prefers to show only one digit. */
   (sprintf(buf, "Watcom C/C++ %d.%02d", __WATCOMC__ / 100,
            __WATCOMC__ % 100), buf), "",
#   else
   (sprintf(buf, "Watcom C/C++ %d.%d", __WATCOMC__ / 100,
            (__WATCOMC__ % 100) / 10), buf), "",
#   endif
#  else
#   ifdef __TURBOC__
#    ifdef __BORLANDC__
   "Borland C++",
#     if (__BORLANDC__ < 0x0200)
   " 1.0",
#     else
#      if (__BORLANDC__ == 0x0200)   /* James:  __TURBOC__ = 0x0297 */
   " 2.0",
#      else
#       if (__BORLANDC__ == 0x0400)
   " 3.0",
#       else
#        if (__BORLANDC__ == 0x0410)   /* __BCPLUSPLUS__ = 0x0310 */
   " 3.1",
#        else
#         if (__BORLANDC__ == 0x0452)   /* __BCPLUSPLUS__ = 0x0320 */
#          if USE_DE
   " 4.0 oder 4.02",
#          else /* !USE_DE */
   _(" 4.0 or 4.02"),
#          endif /* !USE_DE */
#         else
#          if (__BORLANDC__ == 0x0460)   /* __BCPLUSPLUS__ = 0x0340 */
   " 4.5",
#          else
#           if (__BORLANDC__ == 0x0500)   /* __TURBOC__ = 0x0500 */
   " 5.0",
#           else
#            if USE_DE
   " neuer als 5.0",
#            else /* !USE_DE */
   _(" later than 5.0"),
#            endif /* !USE_DE */
#           endif
#          endif
#         endif
#        endif
#       endif
#      endif
#     endif
#    else
   "Turbo C",
#     if (__TURBOC__ >= 0x0400)     /* Kevin:  3.0 -> 0x0401 */
#      if USE_DE
   "++ 3.0 oder neuer",
#      else /* !USE_DE */
   _("++ 3.0 or later"),
#      endif /* !USE_DE */
#     else
#      if (__TURBOC__ == 0x0295)     /* [661] vfy'd by Kevin */
   "++ 1.0",
#      else
#       if ((__TURBOC__ >= 0x018d) && (__TURBOC__ <= 0x0200))  /* James: 0x0200 */
   " 2.0",
#       else
#        if (__TURBOC__ > 0x0100)
   " 1.5",                    /* James:  0x0105? */
#        else
   " 1.0",                    /* James:  0x0100 */
#        endif
#       endif
#      endif
#     endif
#    endif
#   else
#    ifdef MSC
   "Microsoft C ",
#     ifdef _MSC_VER
#      if (_MSC_VER == 800)
   "8.0/8.0c (Visual C++ 1.0/1.5)",
#      else
   (sprintf(buf, "%d.%02d", _MSC_VER/100, _MSC_VER%100), buf),
#      endif
#     else
#      if USE_DE
   "5.1 oder aelter",
#      else /* !USE_DE */
   _("5.1 or earlier"),
#      endif /* !USE_DE */
#     endif
#    else
#     if USE_DE
   "unbekannter Kompilierer", "",
#     else /* !USE_DE */
   _("unknown compiler"), "",
#     endif /* !USE_DE */
#    endif /* MSC */
#   endif /* __TURBOC__ */
#  endif /* __WATCOMC__ */
# endif /* __GNUC__ */

# if USE_DE
   UE,
# endif
   "MS-DOS",

# if (defined(__GNUC__) || (defined(__WATCOMC__) && defined(__386__)))
   " (32-bit)",
# else
#  if defined(M_I86HM) || defined(__HUGE__)
   " (16-bit, huge)",
#  else
#   if defined(M_I86LM) || defined(__LARGE__)
   " (16-bit, large)",
#   else
#    if defined(M_I86MM) || defined(__MEDIUM__)
   " (16-bit, medium)",
#    else
#     if defined(M_I86CM) || defined(__COMPACT__)
   " (16-bit, compact)",
#     else
#      if defined(M_I86SM) || defined(__SMALL__)
   " (16-bit, small)",
#      else
#       if defined(M_I86TM) || defined(__TINY__)
   " (16-bit, tiny)",
#       else
   " (16-bit)",
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif

# if USE_DE
   " am ", __DATE__
# else /* !USE_DE */
   _(" on "), __DATE__
# endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#else /* !MSDOS */
# if defined(WIN32)
   static void
print_compiler_info (fp)
   FILE *fp;
{
#  if (defined(_MSC_VER) || defined(__WATCOMC__))
   char buf[80];
#  endif


   sprintf(s1, _(*compiler_info),

#  ifdef _MSC_VER  /* MSC == MSVC++, including the SDK compiler */
   (sprintf(buf, "Microsoft C %d.%02d ", _MSC_VER/100, _MSC_VER%100), buf),
#   if (_MSC_VER == 800)
   "(Visual C++ v1.1)",
#   else
#    if (_MSC_VER == 850)
   "(Windows NT v3.5 SDK)",
#    else
#     if (_MSC_VER == 900)
   "(Visual C++ v2.0/v2.1)",
#     else
#      if (_MSC_VER == 1000)
   "(Visual C++ v4.0)",
#      else
#       if (_MSC_VER == 1010)
   "(Visual C++ v4.1)",
#       else
#        if (_MSC_VER > 800)
   "(Visual C++)",
#        else
#         if USE_DE
   "(fehlerhafte Version)",
#         else /* !USE_DE */
   _("(bad version)"),
#         endif /* !USE_DE */
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif /* _MSC_VER */

#  ifdef __WATCOMC__
#   if (__WATCOMC__ % 10 > 0)
/* We do this silly test because __WATCOMC__ gives two digits for the  */
/* minor version, but Watcom packaging prefers to show only one digit. */
   (sprintf(buf, "Watcom C/C++ %d.%02d", __WATCOMC__ / 100,
            __WATCOMC__ % 100), buf), "",
#   else
   (sprintf(buf, "Watcom C/C++ %d.%d", __WATCOMC__ / 100,
            (__WATCOMC__ % 100) / 10), buf), "",
#   endif /* __WATCOMC__ % 10 > 0 */
#  endif /* __WATCOMC__ */

#  ifdef __TURBOC__
#   ifdef __BORLANDC__
   "Borland C++",
#    if (__BORLANDC__ == 0x0452)   /* __BCPLUSPLUS__ = 0x0320 */
#     if USE_DE
   " 4.0 oder 4.02",
#     else /* !USE_DE */
   _(" 4.0 or 4.02"),
#     endif /* !USE_DE */
#    else
#     if (__BORLANDC__ == 0x0460)   /* __BCPLUSPLUS__ = 0x0340 */
   " 4.5",
#     else
#      if (__BORLANDC__ == 0x0500)   /* __TURBOC__ = 0x0500 */
   " 5.0",
#      else
#       if USE_DE
   " neuer als 5.0",
#       else /* !USE_DE */
   _(" later than 5.0"),
#       endif /* !USE_DE */
#      endif
#     endif
#    endif
#   else /* !__BORLANDC__ */
   "Turbo C",
#    if (__TURBOC__ >= 0x0400)     /* Kevin:  3.0 -> 0x0401 */
#     if USE_DE
   "++ 3.0 oder neuer",
#     else /* !USE_DE */
   _("++ 3.0 or later"),
#     endif /* !USE_DE */
#    else
#     if (__TURBOC__ == 0x0295)     /* [661] vfy'd by Kevin */
   "++ 1.0",
#     endif
#    endif
#   endif /* __BORLANDC__ */
#  endif /* __TURBOC__ */

#  if !defined(__TURBOC__) && !defined(__WATCOMC__) && !defined(_MSC_VER)
#   if USE_DE
   "unbekannter Kompilierer (SDK?)", "",
#   else /* !USE_DE */
   _("unknown compiler (SDK?)"), "",
#   endif /* !USE_DE */
#  endif

#  if USE_DE
   UE,
#  endif
   "\n\tWindows 95 / Windows NT", " (32-bit)",

#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
   );

   print_text (fp, s1);
}
# else /* !WIN32 */
#  if defined(OS2)
   static void
print_compiler_info (fp)
   FILE *fp;
{
#   if defined(__IBMC__) || defined(__WATCOMC__) || defined(_MSC_VER)
   char buf[80];
#   endif


   sprintf(s1, _(*compiler_info),

#   ifdef __GNUC__
#    ifdef __EMX__  /* __EMX__ is defined as "1" only (sigh) */
   "emx+gcc ", __VERSION__,
#    else
   "gcc/2 ", __VERSION__,
#    endif
#   else
#    ifdef __IBMC__
   "IBM ",
#     if (__IBMC__ < 200)
   (sprintf(buf, "C Set/2 %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#     else
#      if (__IBMC__ < 300)
   (sprintf(buf, "C Set++ %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#      else
   (sprintf(buf, "Visual Age C++ %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#      endif
#     endif
#    else
#     ifdef __WATCOMC__
   "Watcom C", (sprintf(buf, " (__WATCOMC__ = %d)", __WATCOMC__), buf),
#     else
#      ifdef __TURBOC__
#       ifdef __BORLANDC__
   "Borland C++",
#        if (__BORLANDC__ < 0x0200)
   " 1.0",
#        else
#         if (__BORLANDC__ == 0x0200)
   " 2.0",
#         else
#          if (__BORLANDC__ == 0x0400)
   " 3.0",
#          else
#           if (__BORLANDC__ == 0x0410)
   " 3.1",
#           else
#            if (__BORLANDC__ == 0x0452)
   " 4.0",
#            else                    /* these two are guesses based on DOS version */
#             if (__BORLANDC__ == 0x0460)
   " 4.5",
#             else
#              if USE_DE
   " neuer als 4.5",
#              else /* !USE_DE */
   _(" later than 4.5"),
#              endif /* !USE_DE */
#             endif
#            endif
#           endif
#          endif
#         endif
#        endif
#       else
   "Turbo C",
#        if (__TURBOC__ >= 661)
#         if USE_DE
   "++ 1.0 oder neuer",
#         else /* USE_DE */
   _("++ 1.0 or later"),
#         endif /* USE_DE */
#        else
#         if (__TURBOC__ == 661)
   " 3.0?",
#         else
#          if (__TURBOC__ == 397)
   " 2.0",
#          else
#           if USE_DE
   " 1.0 oder 1.5?",
#           else /* !USE_DE */
   _(" 1.0 or 1.5?"),
#           endif /* !USE_DE */
#          endif
#         endif
#        endif
#       endif
#      else
#       ifdef MSC
   "Microsoft C ",
#        ifdef _MSC_VER
   (sprintf(buf, "%d.%02d", _MSC_VER/100, _MSC_VER%100), buf),
#        else
#         if USE_DE
   "5.1 oder aelter",
#         else /* !USE_DE */
   _("5.1 or earlier"),
#         endif /* !USE_DE */
#        endif
#       else
#        if USE_DE
   "unbekannter Kompilierer", "",
#        else /* !USE_DE */
   _("unknown compiler"), "",
#        endif /* !USE_DE */
#       endif /* MSC */
#      endif /* __TURBOC__ */
#     endif /* __WATCOMC__ */
#    endif /* __IBMC__ */
#   endif /* __GNUC__ */

#   if USE_DE
   UE,
#   endif
   "OS/2",

/* GRR:  does IBM C/2 identify itself as IBM rather than Microsoft? */
#   if (defined(MSC) || (defined(__WATCOMC__) && !defined(__386__)))
#    if defined(M_I86HM) || defined(__HUGE__)
   " (16-bit, huge)",
#    else
#     if defined(M_I86LM) || defined(__LARGE__)
   " (16-bit, large)",
#     else
#      if defined(M_I86MM) || defined(__MEDIUM__)
   " (16-bit, medium)",
#      else
#       if defined(M_I86CM) || defined(__COMPACT__)
   " (16-bit, compact)",
#       else
#        if defined(M_I86SM) || defined(__SMALL__)
   " (16-bit, small)",
#        else
#         if defined(M_I86TM) || defined(__TINY__)
   " (16-bit, tiny)",
#         else
   " (16-bit)",
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   else
   " 2.x (32-bit)",
#   endif

#   if USE_DE
   " am ", __DATE__
#   else /* !USE_DE */
   _(" on "), __DATE__
#   endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#  else /* !OS2 */
#   if defined(ACORN)
   static void
print_compiler_info (fp)
   FILE *fp;
{
   sprintf(s1, _(*compiler_info),

#    ifdef __GNUC__
   "gcc ", __VERSION__,
#    else
#     ifdef __CC_NORCROFT
   "Norcroft ", "cc",
#     else
   "cc", "",
#     endif
#    endif

#    if USE_DE
   UE,
#    endif

   "RISC OS",

   " (Acorn Computers Ltd)",

#    if USE_DE
   " am ", __DATE__
#    else /* !USE_DE */
   _(" on "), __DATE__
#    endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#   else /* !ACORN */
#    if defined(AMIGA)

/* NOTE:  the following include depends upon the environment
 *        variable $Workbench to be set correctly.  (Set by
 *        default, by kickstart during startup)
 */
int WBversion = (int)
/*
#include "ENV:Workbench"
*/
10
;

   static void
print_compiler_info (fp)
   FILE *fp;
{
/* Define buffers. */

   char buf1[40];  /* compiler name */
   char buf2[40];  /* revstamp */
   char buf3[40];  /* OS version*/

/* format "with" name strings */

#     ifdef __SASC
   strcpy(buf1, "SAS/C ");
#     else
#      ifdef LATTICE
   strcpy(buf1, "Lattice C ");
#      else
#       ifdef AZTEC_C
   strcpy(buf1, "Manx Aztec C ");
#       else
#        ifdef __GNUC__
   strcpy(buf1, "gcc ");
#        else
#         if USE_DE
   strcpy(buf1, "unbekannt ");
#         else /* !USE_DE */
   strcpy(buf1, _("unknown "));
#         endif /* !USE_DE */
#        endif
#       endif
#      endif
#     endif

/* Define revision, date, and time strings.
 * NOTE:  Do not calculate run time, be sure to use time compiled.
 * Pass these strings via your makefile if undefined.
 */

#     if defined(__VERSION__) && defined(__REVISION__)
#      if USE_DE
   sprintf(buf2, "Version %d.%d", __VERSION__, __REVISION__);
#      else /* !USE_DE */
   sprintf(buf2, _("version %d.%d"), __VERSION__, __REVISION__);
#      endif /* !USE_DE */
#     else
#      ifdef __VERSION__
#       if USE_DE
   sprintf(buf2, "Version %d", __VERSION__);
#       else /* !USE_DE */
   sprintf(buf2, _("version %d"), __VERSION__);
#       endif /* !USE_DE */
#      else
#       if USE_DE
   sprintf(buf2, "unbekannte Version");
#       else /* !USE_DE */
   sprintf(buf2, _("unknown version"));
#       endif /* !USE_DE */
#      endif
#     endif

/* "under" */
/*
   strcpy(buf3, "AmigaDOS ");
*/
   sprintf(buf3, "v%d", WBversion);

#     if USE_DE
   sprintf(s1, _(*compiler_info), buf1, buf2, UE, "AmigaDOS", buf3, " am ", __DATE__);
#     else /* !USE_DE */
   sprintf(s1, _(*compiler_info), buf1, buf2, "AmigaDOS", buf3, _(" on "), __DATE__);
#     endif /* !USE_DE */

   print_text (fp, s1);
}
#    else /* !AMIGA */
#     if defined(ATARI)
   static void
print_compiler_info (fp)
   FILE *fp;
{
#      ifdef __TURBOC__
   char buf[40];
#      endif


   sprintf(s1, _(*compiler_info),

#      ifdef __GNUC__
   "gcc ", __VERSION__,
#      else
#       ifdef __any_other_unix_like_compiler__   /* !!! FIXME */
#        if USE_DE
   "cc ", (sprintf(buf, "Version %d", _RELEASE), buf),
#        else /* !USE_DE */
   "cc ", (sprintf(buf, _("version %d"), _RELEASE), buf),
#        endif /* !USE_DE */
#       else
#        ifdef __TURBOC__
   "Turbo C", (sprintf(buf, " (0x%04x = %d)", __TURBOC__, __TURBOC__), buf),
#        else
#         if USE_DE
   "unbekannter Kompilierer", "",
#         else /* !USE_DE */
   _("unknown compiler"), "",
#         endif /* !USE_DE */
#        endif
#       endif
#      endif

#      if USE_DE
   UE,
#      endif

#      ifdef __MINT__
   "Atari TOS/MiNT",
#      else
   "Atari TOS",
#      endif

   " (Atari ST/TT/Falcon030)",

#      if USE_DE
   " am ", __DATE__
#      else /* !USE_DE */
   _(" on "), __DATE__
#      endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#     else /* !ATARI */
#      if defined(__50SERIES)   /* Prime/PRIMOS */
   static void
print_compiler_info (fp)
   FILE *fp;
{
   sprintf(s1, _(*compiler_info),

#       ifdef __GNUC__
   "gcc ", __VERSION__,
#       else
   "cc", "",
#       endif

#       if USE_DE
   UE,
#       endif

   "PRIMOS",

   " (Prime Computer Inc)",

#       if USE_DE
   " am ", __DATE__
#       else /* !USE_DE */
   _(" on "), __DATE__
#       endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#      else /* !__50SERIES */
#       if defined(VAXC) || defined(VMS)   /* DEC Vax */
   static void
print_compiler_info (fp)
   FILE *fp;
{
#        ifdef VMS_VERSION
   char buf[40];
#        endif
#        ifdef __DECC_VER
   char buf2[40];
   int  vtyp;
#        endif

/*  DEC C in ANSI mode does not like "#ifdef MACRO" inside another
    macro when MACRO is equated to a value (by "#define MACRO 1").   */

   sprintf(s1, _(*compiler_info),

#        ifdef __GNUC__
   "gcc ", __VERSION__,
#        else
#         if defined(DECC) || defined(__DECC) || defined (__DECC__)
   "DEC C",
#          ifdef __DECC_VER
   (sprintf(buf2, " %c%d.%d-%03d",
            ((vtyp = (__DECC_VER / 10000) % 10) == 6 ? 'T' :
             (vtyp == 8 ? 'S' : 'V')),
            __DECC_VER / 10000000,
            (__DECC_VER % 10000000) / 100000, __DECC_VER % 1000), buf2),
#          else
   "",
#          endif
#         else
#          ifdef VAXC
   "VAX C", "",
#          else
#           if USE_DE
   "unbekannter Kompilierer", "",
#           else /* !USE_DE */
   _("unknown compiler"), "",
#           endif /* !USE_DE */
#          endif
#         endif
#        endif

#        if USE_DE
   UE,
#        endif

#        ifdef VMS_VERSION
#         if defined(__alpha)
   "OpenVMS",   /* version has trailing spaces ("V6.1   "), so truncate: */
#          if USE_DE
   (sprintf(buf, " (%.4s f%sr Alpha)", VMS_VERSION), buf, UE),
#          else /* !USE_DE */
   (sprintf(buf, _(" (%.4s for Alpha)"), VMS_VERSION), buf),
#          endif /* !USE_DE */
#         else /* VAX */
   (VMS_VERSION[1] >= '6') ? "OpenVMS" : "VMS",
#          if USE_DE
   (sprintf(buf, " (%.4s f%sr VAX)", VMS_VERSION), buf, UE),
#          else /* !USE_DE */
   (sprintf(buf, _(" (%.4s for VAX)"), VMS_VERSION), buf),
#          endif /* !USE_DE */
#         endif
#        else
   "VMS", "",
#        endif /* ?VMS_VERSION */

#        if USE_DE
   " am ", __DATE__
#        else /* !USE_DE */
   _(" on "), __DATE__
#        endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#       else /* default: UNIX */
#        if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__386BSD__) || defined(__bsdi__)
#         include <sys/param.h> /* for the BSD4_4 define */
#        endif

   static void
print_compiler_info (fp)
   FILE *fp;
{
#        if defined(CRAY) || defined(NX_CURRENT_COMPILER_RELEASE) || defined(NetBSD)
   char buf1[40];
#         if defined(CRAY) || defined(NX_CURRENT_COMPILER_RELEASE)
   char buf2[40];
#         endif
#        endif

    /* Pyramid, NeXT have problems with huge macro expansion, too:  no Info() */
    sprintf(s1, _(*compiler_info),

#        ifdef __GNUC__
#         ifdef NX_CURRENT_COMPILER_RELEASE
   (sprintf(buf1, "NeXT DevKit %d.%02d ", NX_CURRENT_COMPILER_RELEASE/100,
    NX_CURRENT_COMPILER_RELEASE%100), buf1),
    (strlen(__VERSION__) > 8) ? "(gcc)" :
     (sprintf(buf2, "(gcc %s)", __VERSION__), buf2),
#         else
   "gcc ", __VERSION__,
#         endif
#        else
#         if defined(CRAY) && defined(_RELEASE)
#          if USE_DE
   "cc ", (sprintf(buf1, "Version %d", _RELEASE), buf1),
#          else /* !USE_DE */
   "cc ", (sprintf(buf1, _("version %d"), _RELEASE), buf1),
#          endif /* !USE_DE */
#         else
#          ifdef __VERSION__
   "cc ", __VERSION__,
#          else
   "cc", "",
#          endif
#         endif
#        endif

#        if USE_DE
   UE,
#        endif

   "Unix",

#        if defined(sgi) || defined(__sgi)
   " (Silicon Graphics IRIX)",
#        else
#         ifdef sun
#          ifdef sparc
#           ifdef __SVR4
   " (Sun Sparc/Solaris)",
#           else /* may or may not be SunOS */
   " (Sun Sparc)",
#           endif
#          else
#           if defined(sun386) || defined(i386)
   " (Sun 386i)",
#           else
#            if defined(mc68020) || defined(__mc68020__)
   " (Sun 3)",
#            else /* mc68010 or mc68000:  Sun 2 or earlier */
   " (Sun 2)",
#            endif
#           endif
#          endif
#         else
#          ifdef __hpux
   " (HP/UX)",
#          else
#           ifdef __osf__
   " (DEC OSF/1)",
#           else
#            ifdef _AIX
   " (IBM AIX)",
#            else
#             ifdef aiws
   " (IBM RT/AIX)",
#             else
#              if defined(CRAY) || defined(cray)
#               ifdef _UNICOS
   (sprintf(buf2, " (Cray UNICOS release %d)", _UNICOS), buf2),
#               else
   " (Cray UNICOS)",
#               endif
#              else
#               if defined(uts) || defined(UTS)
   " (Amdahl UTS)",
#               else
#                ifdef NeXT
#                 ifdef mc68000
   " (NeXTStep/black)",
#                 else
   " (NeXTStep for Intel)",
#                 endif
#                else              /* the next dozen or so are somewhat order-dependent */
#                 if defined(linux) || defined(__linux__)
#                  ifdef __ELF__
   " (Linux ELF)",
#                  else
   " (Linux a.out)",
#                  endif
#                 else
#                  ifdef MINIX
   " (Minix)",
#                  else
#                   ifdef M_UNIX
   " (SCO Unix)",
#                   else
#                    ifdef M_XENIX
   " (SCO Xenix)",
#                    else
#                     ifdef __NetBSD__
#                      ifdef NetBSD0_8
#                       if NetBSD0_8 == 1
   " (NetBSD 0.8)",
#                       else
   (sprintf(buf1, " (NetBSD 0.8%c)", (char) (NetBSD0_8 + '@')), buf1),
#                       endif
#                      else
#                       ifdef NetBSD0_9
#                        if NetBSD0_9 == 1
   " (NetBSD 0.9)",
#                        else
   (sprintf(buf1, " (NetBSD 0.9%c)", (char)(NetBSD0_9 + 'A' - 2)), buf1),
#                        endif
#                       else
#                        ifdef NetBSD1_0
#                         if NetBSD1_0 == 1
   " (NetBSD 1.0)",
#                         else
   (sprintf(buf1, " (NetBSD 1.0%c)", (char)(NetBSD1_0 + 'A' - 2)), buf1),
#                         endif
#                        else
#                         ifdef NetBSD1_1
#                          if NetBSD1_1 == 1
   " (NetBSD 1.1)",
#                          else
   (sprintf(buf1, " (NetBSD 1.1%c)", (char)(NetBSD1_1 + 'A' - 2)), buf1),
#                          endif
#                         else
#                          if USE_DE
   (BSD4_4 == 0.5) ? " (NetBSD vor 0.9)" : " (NetBSD 1.2 oder neuer)",
#                          else /* !USE_DE */
   (BSD4_4 == 0.5) ? _(" (NetBSD before 0.9)") : _(" (NetBSD 1.2 or later)"),
#                          endif /* !USE_DE */
#                         endif
#                        endif
#                       endif
#                      endif
#                     else
#                      ifdef __FreeBSD__
#                       if USE_DE
   (BSD4_4 == 0.5) ? " (FreeBSD 1.x)" : " (FreeBSD 2.0 oder neuer)",
#                       else /* !USE_DE */
   (BSD4_4 == 0.5) ? " (FreeBSD 1.x)" : _(" (FreeBSD 2.0 or later)"),
#                       endif /* !USE_DE */
#                      else
#                       ifdef __bsdi__
#                        if USE_DE
   (BSD4_4 == 0.5) ? " (BSD/386 1.0)" : " (BSD/386 1.1 oder neuer)",
#                        else /* !USE_DE */
   (BSD4_4 == 0.5) ? " (BSD/386 1.0)" : _(" (BSD/386 1.1 or later)"),
#                        endif /* !USE_DE */
#                       else
#                        ifdef __386BSD__
   (BSD4_4 == 1) ? " (386BSD, post-4.4 release)" : " (386BSD)",
#                        else
#                         if defined(i486) || defined(__i486) || defined(__i486__)
   " (Intel 486)",
#                         else
#                          if defined(i386) || defined(__i386) || defined(__i386__)
   " (Intel 386)",
#                          else
#                           ifdef pyr
   " (Pyramid)",
#                           else
#                            if defined(ultrix) || defined(__ultrix)
#                             if defined(mips) || defined(__mips)
   " (DEC/MIPS)",
#                             else
#                              if defined(vax) || defined(__vax)
   " (DEC/VAX)",
#                              else /* __alpha? */
   " (DEC/Alpha)",
#                              endif
#                             endif
#                            else
#                             ifdef gould
   " (Gould)",
#                             else
#                              ifdef MTS
   " (MTS)",
#                              else
#                               ifdef __convexc__
   " (Convex)",
#                               else
   "",
#                               endif /* Convex */
#                              endif /* MTS */
#                             endif /* Gould */
#                            endif /* DEC */
#                           endif /* Pyramid */
#                          endif /* 386 */
#                         endif /* 486 */
#                        endif /* 386BSD */
#                       endif /* BSDI BSD/386 */
#                      endif /* NetBSD */
#                     endif /* FreeBSD */
#                    endif /* SCO Xenix */
#                   endif /* SCO Unix */
#                  endif /* Minix */
#                 endif /* Linux */
#                endif /* NeXT */
#               endif /* Amdahl */
#              endif /* Cray */
#             endif /* RT/AIX */
#            endif /* AIX */
#           endif /* OSF/1 */
#          endif /* HP/UX */
#         endif /* Sun */
#        endif /* SGI */

#        if USE_DE
   " am ", __DATE__
#        else /* !USE_DE */
   _(" on "), __DATE__
#        endif /* !USE_DE */
   );

   print_text (fp, s1);
}
#       endif /* default: UNIX */
#      endif /* !__50SERIES */
#     endif /* !ATARI */
#    endif /* !AMIGA */
#   endif /* !ACORN */
#  endif /* !OS2 */
# endif /* !WIN32 */
#endif /* !MSDOS */
