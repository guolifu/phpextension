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


#include "Zend/zend_exceptions.h"	// for zend_throw_exception
#include "Zend/zend_interfaces.h"	// for zend_call_method_with_*
#include "Zend/zend_smart_str.h"	// for smart_str
#include "ext/standard/url.h"	// for php_url_*
#include "ext/standard/php_var.h"	// for php_var_dump
#include "ext/standard/php_string.h"	// for php_trim
#include "ext/standard/php_filestat.h"	// for php_stat
#include "ext/session/php_session.h"	// for php_session_start
#include "ext/json/php_json.h"	// for php_json_encode
#include "main/SAPI.h"	// for sapi_header_op
/* If you declare any globals in php_myclass.h uncomment this:*/
ZEND_DECLARE_MODULE_GLOBALS(myclass)


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

const zend_function_entry myclass_functions[] = {
	PHP_ME(children, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(children, set, arginfo_children_learn, ZEND_ACC_PUBLIC)
	PHP_ME(children, test, NULL, ZEND_ACC_PUBLIC)
      PHP_ME(children, init, NULL, ZEND_ACC_PUBLIC)
      PHP_ME(children, run, NULL, ZEND_ACC_PUBLIC)
      
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
      

      
      // if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &app_dir) == FAILURE ){
      //       RETURN_NULL();
      // }
      // zend_update_static_property(children_ce, ZEND_STRL("app_dir"), app_dir TSRMLS_CC);
}
PHP_METHOD(children, init)
{
      zval *app_dir = NULL;
      zval *server, *field;
      zend_string *environ = NULL, *iniName, *tstr, *docRoot = NULL, *baseUri = NULL, *uri = NULL, *appRoot = NULL;
      if (PG(auto_globals_jit)) {
		tstr = zend_string_init(ZEND_STRL("_SERVER"), 0);
		zend_is_auto_global(tstr);
		zend_string_release(tstr);
	}
      server = &PG(http_globals)[TRACK_VARS_SERVER];
      
      do {
      // 还是先找 PHP_INFO
      if ((field = zend_hash_str_find(Z_ARRVAL_P(server), ZEND_STRL("PATH_INFO"))) && Z_TYPE_P(field) == IS_STRING) {
            uri = zend_string_copy(Z_STR_P(field));
            break;
      }
      // 接着找 REQUEST_URI
      if ((field = zend_hash_str_find(Z_ARRVAL_P(server), ZEND_STRL("REQUEST_URI"))) && Z_TYPE_P(field) == IS_STRING) {
            if (strncasecmp(Z_STRVAL_P(field), ZEND_STRL("http://")) &&
                  strncasecmp(Z_STRVAL_P(field), ZEND_STRL("https://"))) {
                  // not http url
                  char *pos = strstr(Z_STRVAL_P(field), "?");
                  if (pos) {
                  // found query
                  uri = zend_string_init(Z_STRVAL_P(field), pos - Z_STRVAL_P(field), 0);
                  } else {
                  uri = zend_string_copy(Z_STR_P(field));
                  }
            } else {
                  php_url *urlInfo = php_url_parse(Z_STRVAL_P(field));
                  if (urlInfo && urlInfo->path) {
                  uri = zend_string_init(urlInfo->path, strlen(urlInfo->path), 0);
                  }
                  php_url_free(urlInfo);
            }
           
            break;
      }
      // 最后找 ORIG_PATH_INFO
      if ((field = zend_hash_str_find(Z_ARRVAL_P(server), ZEND_STRL("ORIG_PATH_INFO"))) &&
                  Z_TYPE_P(field) == IS_STRING) {
            uri = zend_string_copy(Z_STR_P(field));
            break;
      }
     
      
      } while (0);



      if (uri) {
		zend_string *t = uri;
		uri = php_trim(uri, ZEND_STRL("/"), 3);
		zend_string_release(t);
	} else {
		uri = ZSTR_EMPTY_ALLOC();
	}
	MYCLASS_G(uri) = uri;

      //char *temp = ZSTR_VAL(uri);
      //php_printf("%s\n",temp);

}
PHP_METHOD(children, run){
      zend_string *uri;
      uri = MYCLASS_G(uri);
      
      char *temp = ZSTR_VAL(uri);
      php_printf("%s\n",temp);
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
            controller_name = zval_get_string(c_result);
        
      }else{
            zend_error_noreturn(E_CORE_ERROR,  "Couldn't find controller param in url.");

      }
      //释放key的变量
      zend_string_release(c_key);


      //获取action_name
      zend_string *a_key= zend_string_init("action", sizeof("action")-1, 0);

      if ((a_result = zend_hash_find(ht, a_key)) != NULL) {
            action_name = zval_get_string(a_result);
            // php_printf("%s\n", Z_STRVAL_P(a_result));
      }else{
            zend_error_noreturn(E_CORE_ERROR,"Couldn't find action param in url.");

      }

      //释放key的变量
      zend_string_release(a_key);


      //拼装controller文件路径
      char *path = Z_STRVAL_P(app_dir);

      //php_printf("%s\n", Z_STRVAL_P(app_dir));

      char *c_2 = "controllers/";
      strcat(path,c_2);

      //zend_string->char *
      char *c_3 = ZSTR_VAL(controller_name);
      strcat(path,c_3);

      char *c_4 = ".php";
      strcat(path,c_4);
      char *c_1 = path;

      // php_printf("%s\n", path);
      // php_printf("%s\n", controller_name);
      // php_printf("%s\n", action_name);
      PHPWRITE(Z_STRVAL_P(app_dir), Z_STRLEN_P(app_dir));


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
      MYCLASS_G(uri) = NULL;
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
      /*if (MYCLASS_G(uri)) {
		zend_string_release(MYCLASS_G(uri));
	}*/
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
