dnl $Id$
dnl config.m4 for extension daily-ext

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(daily-ext, for daily-ext support,
Make sure that the comment is aligned:
[  --with-daily-ext             Include daily-ext support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(daily-ext, whether to enable daily-ext support,
dnl Make sure that the comment is aligned:
dnl [  --enable-daily-ext           Enable daily-ext support])

if test "$PHP_DAILY-EXT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-daily-ext -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/daily-ext.h"  # you most likely want to change this
  dnl if test -r $PHP_DAILY-EXT/$SEARCH_FOR; then # path given as parameter
  dnl   DAILY-EXT_DIR=$PHP_DAILY-EXT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for daily-ext files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DAILY-EXT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DAILY-EXT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the daily-ext distribution])
  dnl fi

  dnl # --with-daily-ext -> add include path
  dnl PHP_ADD_INCLUDE($DAILY-EXT_DIR/include)

  dnl # --with-daily-ext -> check for lib and symbol presence
  dnl LIBNAME=daily-ext # you may want to change this
  dnl LIBSYMBOL=daily-ext # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DAILY-EXT_DIR/lib, DAILY-EXT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DAILY-EXTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong daily-ext lib version or lib not found])
  dnl ],[
  dnl   -L$DAILY-EXT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DAILY-EXT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(daily-ext, daily-ext.c, $ext_shared)
fi
