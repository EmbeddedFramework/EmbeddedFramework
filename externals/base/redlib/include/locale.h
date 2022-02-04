/*
 * <locale.h>             Copyright (C) Codemist 2007
 *
 *                        A C Norman
 */

//*****************************************************************************
//
// Modfied by Code Red Technologies Ltd and NXP
//
//   +--+
//   | ++----+
//   +-++    |
//     |     |
//   +-+--+  |
//   | +--+--+
//   +----+    Copyright 2010-2011 Code Red Technologies Ltd.
//             Copyright 2017, 2019 NXP
//
//*****************************************************************************
#ifndef __LOCALE_H_INCLUDED
#define __LOCALE_H_INCLUDED

#include <sys/redlib_version.h>

#ifndef __CODEMIST
#define __CODEMIST
#endif


/* Handles national characteristics eg. USA month day year UK day month year */

#define LC_COLLATE  1
    /* affects the behaviour of the strcoll function */
#define LC_CTYPE    2
    /* affects the behaviour of the character handling functions */
    /* (isdigit, isspace and isxdigit are not affected) */
#define LC_MONETARY 4
    /* affects the monetary formatting information returned by the */
    /* localeconv function. */
#define LC_NUMERIC  8
    /* affects the decimal-point character for the formatted input/output */
    /* functions and the string conversion functions */
#define LC_TIME    16
    /* affects the behaviour of the strftime function */
#define LC_ALL     31
    /* program's entire locale */

extern char *setlocale(int /*category*/, const char * /*locale*/);
  /*
   * Selects the appropriate piece of the program's locale as specified by the
   * category and locale arguments. The setlocale function may be used to
   * change or query the program's entire current locale or portions thereof.
   * The effect of the category argument for each value is described above.
   * A value of "C" for locale specifies the minimal environment for C
   * translation; a value of "" for locale specifies the implementation-defined
   * native environment. At program startup the equivalent of
   * setlocale(LC_ALL, "C") is executed.
   *
   * Return value:
   * If a pointer to string is given for locale and the selection can be
   * honoured, the string associated with the specified category for the new
   * locale is returned. If the selction can not be honoured, a null pointer
   * is returned and the program's locale is not changed.
   * A null pointer for locale causes the string associated with the category
   * for the program's current locale to be returned and the program's locale
   * is not changed. This enquiry can fail by returning a null pointer only if
   * the category is LC_ALL and the most recent successful locale-setting call
   * used a category other than LC_ALL.
   * The string returned is such that a subsequent call with that string and
   * its associated category will restore that part of the program's locale.
   * The string returned shall not be modified by the program, but may be
   * overwritten by a subsequent call to setlocale.
   */

struct lconv {
  char *decimal_point;
       /* The decimal point character used to format non-monetary quantities */
  char *thousands_sep;
       /* The character used to separate groups of digits to the left of the */
       /* decimal point character in formatted non-monetary quantities.      */
  char *grouping;
       /* A string whose elements indicate the size of each group of digits  */
       /* in formatted non-monetary quantities. See below for more details.  */
  char *int_curr_symbol;
       /* The international currency symbol applicable to the current locale.*/
       /* The first three characters contain the alphabetic international    */
       /* currency symbol in accordance with those specified in ISO 4217     */
       /* Codes for the representation of Currency and Funds. The fourth     */
       /* character (immediately preceding the null character) is the        */
       /* character used to separate the international currency symbol from  */
       /* the monetary quantity.                                             */
  char *currency_symbol;
       /* The local currency symbol applicable to the current locale.        */
  char *mon_decimal_point;
       /* The decimal-point used to format monetary quantities.              */
  char *mon_thousands_sep;
       /* The separator for groups of digits to the left of the decimal-point*/
       /* in formatted monetary quantities.                                  */
  char *mon_grouping;
       /* A string whose elements indicate the size of each group of digits  */
       /* in formatted monetary quantities. See below for more details.      */
  char *positive_sign;
       /* The string used to indicate a nonnegative-valued formatted         */
       /* monetary quantity.                                                 */
  char *negative_sign;
       /* The string used to indicate a negative-valued formatted monetary   */
       /* quantity.                                                          */
  char int_frac_digits;
       /* The number of fractional digits (those to the right of the         */
       /* decimal-point) to be displayed in an internationally formatted     */
       /* monetary quantities.                                               */
  char frac_digits;
       /* The number of fractional digits (those to the right of the         */
       /* decimal-point) to be displayed in a formatted monetary quantity.   */
  char p_cs_precedes;
       /* Set to 1 or 0 if the currency_symbol respectively precedes or      */
       /* succeeds the value for a nonnegative formatted monetary quantity.  */
  char p_sep_by_space;
       /* Set to 1 or 0 if the currency_symbol respectively is or is not     */
       /* separated by a space from the value for a nonnegative formatted    */
       /* monetary quantity.                                                 */
  char n_cs_precedes;
       /* Set to 1 or 0 if the currency_symbol respectively precedes or      */
       /* succeeds the value for a negative formatted monetary quantity.     */
  char n_sep_by_space;
       /* Set to 1 or 0 if the currency_symbol respectively is or is not     */
       /* separated by a space from the value for a negative formatted       */
       /* monetary quantity.                                                 */
  char p_sign_posn;
       /* Set to a value indicating the position of the positive_sign for a  */
       /* nonnegative formatted monetary quantity. See below for more details*/
  char n_sign_posn;
       /* Set to a value indicating the position of the negative_sign for a  */
       /* negative formatted monetary quantity. See below for more details.  */

  /*
   * The elements of grouping amd mon_grouping are interpreted according to
   * the following:
   * CHAR_MAX   No further grouping is to be performed.
   * 0          The previous element is to be repeatedly used for the
   *            remainder of the digits.
   * other      The value is the number of digits that compromise the current
   *            group. The next element is examined to determine the size of
   *            the next group of digits to the left of the current group.
   *
   * The value of p_sign_posn and n_sign_posn is interpreted according to
   * the following:
   * 0          Parentheses surround the quantity and currency_symbol.
   * 1          The sign string preceeds the quantity and currency_symbol.
   * 2          The sign string succeeds the quantity and currency_symbol.
   * 3          The sign string immediately preceeds the currency_symbol.
   * 4          The sign string immediately succeeds the currency_symbol.
   */
};

extern struct lconv *localeconv(void);
  /*
   * Sets the components of an object with type struct lconv with values
   * appropriate for the formatting of numeric quantities (monetary and
   * otherwise) according to the rules of the current locale.
   * The members of the structure with type char * are strings, any of which
   * (except decimal_point) can point to "", to indicate that the value is not
   * available in the current locale or is of zero length. The members with
   * type char are nonnegative numbers, any of which can be CHAR_MAX to
   * indicate that the value is not available in the current locale.
   * The members included are described above.
   *
   * Return value:
   * A pointer to the filled in object. The structure pointed to by the return
   * value shall not be modified by the program, but may be overwritten by a
   * subsequent call to the localeconv function. In addition, calls to the
   * setlocale function with categories LC_ALL, LC_MONETARY, or LC_NUMERIC may
   * overwrite the contents of the structure.
   */

#ifndef NULL
#  define NULL 0
#endif

#endif /* __LOCALE_H_INCLUDED */

/* end of locale.h */
