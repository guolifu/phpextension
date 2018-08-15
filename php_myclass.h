/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MYCLASS_H
#define PHP_MYCLASS_H

extern zend_module_entry myclass_module_entry;
#define phpext_myclass_ptr &myclass_module_entry

#define PHP_MYCLASS_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_MYCLASS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MYCLASS_API __attribute__ ((visibility("default")))
#else
#	define PHP_MYCLASS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(myclass)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(myclass)
*/
zend_class_entry *children_ce;
PHP_FUNCTION(hello);
PHP_METHOD(children,__construct);
PHP_METHOD(children, set);
PHP_METHOD(children, test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_children_learn, 0, 0, 1)
    ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()

/* Always refer to the globals in your function as MYCLASS_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define MYCLASS_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(myclass, v)

#if defined(ZTS) && defined(COMPILE_DL_MYCLASS)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_MYCLASS_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */