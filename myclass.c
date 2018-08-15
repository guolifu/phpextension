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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_myclass.h"

/* If you declare any globals in php_myclass.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(myclass)
*/

/* True global resources - no need for thread safety here */
static int le_myclass;
int zend_execute_scripts_ext(char *filepath){

    zval retval;

    zend_file_handle zfd;
    zfd.type = ZEND_HANDLE_FILENAME;
    zfd.filename = filepath;
    zfd.free_filename = 0;
    zfd.opened_path = NULL;

    //zend_execute_scripts(int type, zval *retval, int file_count, ...);
    //FAILURE OR SUCCESS
    return  zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC,&retval,1,&zfd); 
    


}
int call_user_class_method(zval *retval, zend_class_entry *obj_ce, 
                           zval *obj, zval func,  uint32_t params_count, zval params[]){ 


    HashTable *function_table; 

    if(obj) { 
                function_table = &Z_OBJCE_P(obj)->function_table;
        }else{
                function_table = (CG(function_table));
    }

    zend_fcall_info fci;  
    fci.size = sizeof(fci);  
    fci.object =  obj ? Z_OBJ_P(obj) : NULL;;
    fci.function_name = func;   
    fci.retval = retval;  
    fci.param_count = params_count;  
    fci.params = params;  
    fci.no_separation = 1;  
 


  
    //FAILURE OR SUCCESS
    return  zend_call_function(&fci, NULL TSRMLS_CC);         //函数调用结束。  

}
/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("myclass.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_myclass_globals, myclass_globals)
    STD_PHP_INI_ENTRY("myclass.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_myclass_globals, myclass_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */


/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_myclass_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_myclass_init_globals(zend_myclass_globals *myclass_globals)
{
	myclass_globals->global_value = 0;
	myclass_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
/* {{{ myclass_functions[]
 *
 * Every user visible function must have an entry in myclass_functions[].
 */
const zend_function_entry myclass_functions[] = {
	PHP_ME(children, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(children, set, arginfo_children_learn, ZEND_ACC_PUBLIC)
	PHP_ME(children, test, NULL, ZEND_ACC_PUBLIC)
	PHP_FE(hello,	NULL)
	PHP_FE_END	/* Must be the last line in myclass_functions[] */
};
/* }}} */

PHP_MINIT_FUNCTION(myclass)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
 	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ExtensionClass", myclass_functions);

	children_ce = zend_register_internal_class(&ce);
	zend_declare_property_null(children_ce, ZEND_STRL("attr1"), ZEND_ACC_PUBLIC);
      //声明一个静态数据成员app_dir
      zend_declare_property_string(children_ce, "app_dir", strlen("app_dir"), "",ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_DC);
	return SUCCESS;
}

PHP_METHOD(children, set)
{
	char *love = NULL;
	size_t love_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &love, &love_len) == FAILURE) {
		return;
	}

	zend_update_property_string(children_ce,  getThis(), "attr1", sizeof("attr1") - 1, love);
}
PHP_METHOD(children, __construct)
{
	//php_printf("__construct\n");
      zval *app_dir = NULL;

      // if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &app_dir) == FAILURE ){
      //       RETURN_NULL();
      // }
      // zend_update_static_property(children_ce, ZEND_STRL("app_dir"), app_dir TSRMLS_CC);
}
PHP_METHOD(children, test)
{
	
      zend_string* controller_name = zend_string_init("Index",strlen("Index"),0);
      zend_string* action_name     = zend_string_init("Index",strlen("Index"),0);

      zval *c_result;
      zval *a_result;
      int flag;



      //设置站点目录
      zval *app_dir = zend_read_static_property(Z_OBJCE_P(getThis()), "app_dir", sizeof("app_dir")-1, 0 TSRMLS_DC);


      //获取GET请求参数hashtable
      zval *get_arr = &PG(http_globals)[TRACK_VARS_GET]; 
      HashTable *ht= HASH_OF(get_arr);
      //int array_count = zend_hash_num_elements(Z_ARRVAL_P(get_arr));
    

      //获取controller_name
      zend_string *c_key= zend_string_init("controller", sizeof("controller")-1, 0);

      if ((c_result = zend_hash_find(ht, c_key)) != NULL) {
      php_printf("%s\n","有c");
            controller_name = zval_get_string(c_result);
        
      }else{
            php_printf("%s\n","没有c");
            //zend_error_noreturn(E_CORE_ERROR,  "Couldn't find controller param in url.");

      }
      //释放key的变量
      zend_string_release(c_key);


      //获取action_name
      zend_string *a_key= zend_string_init("action", sizeof("action")-1, 0);

      if ((a_result = zend_hash_find(ht, a_key)) != NULL) {
php_printf("%s\n","有a");
            action_name = zval_get_string(a_result);
            //php_printf("%s\n", Z_STRVAL_P(a_result));
            //php_printf("%s\n", zval_get_string(a_result));
      }else{
php_printf("%s\n","没有a");
            zend_error_noreturn(E_CORE_ERROR,"Couldn't find action param in url.");

      }

      //释放key的变量
      zend_string_release(a_key);


      //拼装controller文件路径
      char *path = Z_STRVAL_P(app_dir);


      char *c_2 = "controllers/";
      strcat(path,c_2);

      //zend_string->char *
      char *c_3 = ZSTR_VAL(controller_name);
      strcat(path,c_3);

      char *c_4 = ".php";
      strcat(path,c_4);
      char *c_1 = path;

      // php_printf("%s\n", c_1);
      // php_printf("%s\n", controller_name);
      // php_printf("%s\n", action_name);
      // PHPWRITE(Z_STRVAL_P(app_dir), Z_STRLEN_P(app_dir));


      //加载执行controller文件
      flag = zend_execute_scripts_ext(c_1);

      if(flag == FAILURE){

            zend_error_noreturn(E_CORE_ERROR,"Couldn't find file: %s.",c_1);

      }


      //查找controller对应的
      //zend_class_entry *zend_lookup_class(zend_string *name);
      zend_class_entry *controller_ce = zend_lookup_class(controller_name);

      if(controller_ce == NULL){

            zend_error_noreturn(E_CORE_ERROR,"Couldn't find file: %s.",c_1);
      }


      zval obj;
      object_init_ex(&obj, controller_ce);

      
      zval function_name;
      ZVAL_STRING(&function_name,ZSTR_VAL(action_name));

      
      flag = call_user_class_method(return_value, controller_ce, &obj, function_name, 0, NULL);

      if(flag == FAILURE){


            zend_error_noreturn(E_CORE_ERROR, 
                                "Couldn't find implementation for method %s%s%s", 
                                controller_ce ? ZSTR_VAL(controller_ce->name) : "", 
                                controller_ce ? "::" : "", 
                                function_name);
        
      }

      // RETURN_ZVAL(get_arr, 1, 0);
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(myclass)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(myclass)
{
#if defined(COMPILE_DL_MYCLASS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(myclass)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(myclass)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "myclass support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


PHP_FUNCTION(hello){
	php_printf("Hello World!\n");
	RETURN_TRUE;
}
/* {{{ myclass_module_entry
 */
zend_module_entry myclass_module_entry = {
	STANDARD_MODULE_HEADER,
	"myclass",
	myclass_functions,
	PHP_MINIT(myclass),
	PHP_MSHUTDOWN(myclass),
	PHP_RINIT(myclass),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(myclass),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(myclass),
	PHP_MYCLASS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYCLASS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(myclass)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
