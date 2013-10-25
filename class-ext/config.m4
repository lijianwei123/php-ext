dnl $Id$
dnl config.m4 for extension class_ext

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(class_ext, for class_ext support,
Make sure that the comment is aligned:
[  --with-class_ext             Include class_ext support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(class_ext, whether to enable class_ext support,
dnl Make sure that the comment is aligned:
dnl [  --enable-class_ext           Enable class_ext support])

if test "$PHP_CLASS_EXT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-class_ext -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/class_ext.h"  # you most likely want to change this
  dnl if test -r $PHP_CLASS_EXT/$SEARCH_FOR; then # path given as parameter
  dnl   CLASS_EXT_DIR=$PHP_CLASS_EXT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for class_ext files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CLASS_EXT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CLASS_EXT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the class_ext distribution])
  dnl fi

  dnl # --with-class_ext -> add include path
  dnl PHP_ADD_INCLUDE($CLASS_EXT_DIR/include)

  dnl # --with-class_ext -> check for lib and symbol presence
  dnl LIBNAME=class_ext # you may want to change this
  dnl LIBSYMBOL=class_ext # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CLASS_EXT_DIR/lib, CLASS_EXT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CLASS_EXTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong class_ext lib version or lib not found])
  dnl ],[
  dnl   -L$CLASS_EXT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CLASS_EXT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(class_ext, class_ext.c, $ext_shared)
fi
