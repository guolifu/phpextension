dnl $Id$
dnl config.m4 for extension myclass

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

 PHP_ARG_WITH(myclass, for myclass support,
 Make sure that the comment is aligned:
 [  --with-myclass             Include myclass support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(myclass, whether to enable myclass support,
 Make sure that the comment is aligned:
 [  --enable-myclass           Enable myclass support])

if test "$PHP_MYCLASS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-myclass -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/myclass.h"  # you most likely want to change this
  dnl if test -r $PHP_MYCLASS/$SEARCH_FOR; then # path given as parameter
  dnl   MYCLASS_DIR=$PHP_MYCLASS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for myclass files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MYCLASS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MYCLASS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the myclass distribution])
  dnl fi

  dnl # --with-myclass -> add include path
  dnl PHP_ADD_INCLUDE($MYCLASS_DIR/include)

  dnl # --with-myclass -> check for lib and symbol presence
  dnl LIBNAME=myclass # you may want to change this
  dnl LIBSYMBOL=myclass # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MYCLASS_DIR/$PHP_LIBDIR, MYCLASS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MYCLASSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong myclass lib version or lib not found])
  dnl ],[
  dnl   -L$MYCLASS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MYCLASS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(myclass, myclass.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
