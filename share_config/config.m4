dnl $Id$
dnl config.m4 for extension share_config

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(share_config, for share_config support,
Make sure that the comment is aligned:
[  --with-share_config             Include share_config support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(share_config, whether to enable share_config support,
dnl Make sure that the comment is aligned:
dnl [  --enable-share_config           Enable share_config support])

if test "$PHP_SHARE_CONFIG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-share_config -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/share_config.h"  # you most likely want to change this
  dnl if test -r $PHP_SHARE_CONFIG/$SEARCH_FOR; then # path given as parameter
  dnl   SHARE_CONFIG_DIR=$PHP_SHARE_CONFIG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for share_config files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SHARE_CONFIG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SHARE_CONFIG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the share_config distribution])
  dnl fi

  dnl # --with-share_config -> add include path
  dnl PHP_ADD_INCLUDE($SHARE_CONFIG_DIR/include)

  dnl # --with-share_config -> check for lib and symbol presence
  dnl LIBNAME=share_config # you may want to change this
  dnl LIBSYMBOL=share_config # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SHARE_CONFIG_DIR/lib, SHARE_CONFIG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SHARE_CONFIGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong share_config lib version or lib not found])
  dnl ],[
  dnl   -L$SHARE_CONFIG_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SHARE_CONFIG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(share_config, share_config.c, $ext_shared)
  
  ifdef([PHP_ADD_EXTENDION_DEP],
  [
    PHP_ADD_EXTENSION_DEP(share_config, redis)
  ])
  fi
