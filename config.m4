dnl $Id$
dnl config.m4 for extension phpltp

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(phpltp, for phpltp support,
Make sure that the comment is aligned:
[  --with-phpltp             Include phpltp support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(phpltp, whether to enable phpltp support,
dnl Make sure that the comment is aligned:
dnl [  --enable-phpltp           Enable phpltp support])

if test "$PHP_PHPLTP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-phpltp -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/phpltp.h"  # you most likely want to change this
  dnl if test -r $PHP_PHPLTP/$SEARCH_FOR; then # path given as parameter
  dnl   PHPLTP_DIR=$PHP_PHPLTP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for phpltp files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PHPLTP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PHPLTP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the phpltp distribution])
  dnl fi

  dnl # --with-phpltp -> add include path
  dnl PHP_ADD_INCLUDE($PHPLTP_DIR/include)

  dnl # --with-phpltp -> check for lib and symbol presence
  dnl LIBNAME=phpltp # you may want to change this
  dnl LIBSYMBOL=phpltp # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PHPLTP_DIR/$PHP_LIBDIR, PHPLTP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PHPLTPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong phpltp lib version or lib not found])
  dnl ],[
  dnl   -L$PHPLTP_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_ADD_LIBRARY_WITH_PATH(ltps, ./, PHPLTP_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(segmentor, ../ltp-3.3.2/lib/, PHPLTP_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(boost_regex, /usr/lib64/, PHPLTP_SHARED_LIBADD)
  PHP_SUBST(PHPLTP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(phpltp, phpltp.c, $ext_shared, , -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
