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
#define MYCLASS_NS "Myclass"
/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(myclass)
	double timer;					// 计时器
	zend_ulong renderDepth;			// 渲染嵌套层数
	zend_string *environ;			// 运行环境
	zend_string *locale;			// 语言区域
	//azalea_bootstrap_t bootstrap;	// Azalea\Bootstrap 实例变量
	zend_bool registeredTemplateFunctions;	// 是否已注册模板函数
	zend_bool startSession;			// 是否开启回话
	zval instances;					// 实例缓存变量
	//azalea_request_t request;		// Azalea\Request 实例变量
	zval config;					// 配置项变量
	zval translations;				// 翻译字符变量
	zval viewTagFunctionsUser;		// 视图标签变量 (用户定义)

	zend_string *docRoot;			// 入口文件根目录
	zend_string *appRoot;			// 系统文件根目录
	zend_string *uri;
	zend_string *baseUri;
	zend_string *ip;
	zend_string *host;
	zend_string *controllersPath;
	zend_string *modelsPath;
	zend_string *viewsPath;

	zval paths;
	zend_string *folderName;
	zend_string *controllerName;
	zend_string *actionName;
	zval pathArgs;
ZEND_END_MODULE_GLOBALS(myclass)
extern ZEND_DECLARE_MODULE_GLOBALS(myclass);
#define MYCLASS_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(myclass , v)

ZEND_BEGIN_MODULE_GLOBALS(myclass_internal)
	int moduleNumber;			// PHP 模块序号
	zend_string *stringWeb;		// 字符串 "WEB"
	zend_string *stringEn;		// 字符串 "en_US"
	zend_string *stringSlash;	// 字符串 "/"
	zend_array *viewTagFunctions;	// 视图标签变量
ZEND_END_MODULE_GLOBALS(myclass_internal)
extern ZEND_DECLARE_MODULE_GLOBALS(myclass_internal);
#define AG(v) ZEND_MODULE_GLOBALS_ACCESSOR(myclass_internal, v)


zend_class_entry *children_ce;
PHP_FUNCTION(hello);
PHP_METHOD(children,__construct);
PHP_METHOD(children, set);
PHP_METHOD(children, test);
PHP_METHOD(children, init);
PHP_METHOD(children, run);

ZEND_BEGIN_ARG_INFO_EX(arginfo_children_learn, 0, 0, 1)
    ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()

/* Always refer to the globals in your function as MYCLASS_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

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