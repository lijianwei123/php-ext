dnl $Id$
dnl config.m4 for extension daily

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(daily, for daily support,
dnl Make sure that the comment is aligned:
dnl [  --with-daily             Include daily support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(daily, whether to enable daily support,
Make sure that the comment is aligned:
[  --enable-daily           Enable daily support])

if test "$PHP_DAILY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-daily -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/daily.h"  # you most likely want to change this
  dnl if test -r $PHP_DAILY/$SEARCH_FOR; then # path given as parameter
  dnl   DAILY_DIR=$PHP_DAILY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for daily files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DAILY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DAILY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the daily distribution])
  dnl fi

  dnl # --with-daily -> add include path
  dnl PHP_ADD_INCLUDE($DAILY_DIR/include)

  dnl # --with-daily -> check for lib and symbol presence
  dnl LIBNAME=daily # you may want to change this
  dnl LIBSYMBOL=daily # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DAILY_DIR/lib, DAILY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DAILYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong daily lib version or lib not found])
  dnl ],[
  dnl   -L$DAILY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DAILY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(daily, daily.c basedb.c, $ext_shared)
fi
