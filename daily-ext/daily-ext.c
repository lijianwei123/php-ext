/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
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

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_daily-ext.h"

/* If you declare any globals in php_daily-ext.h uncomment this:*/
ZEND_DECLARE_MODULE_GLOBALS(daily-ext)


//daily-ext args
ZEND_BEGIN_ARG_INFO(arg_construct, 4)
	ZEND_ARG_INFO(0, db_host) //0 表示按值传递  1表示引用传递
	ZEND_ARG_INFO(0, db_user)
	ZEND_ARG_INFO(0, db_pwd)
	ZEND_ARG_INFO(0, db_name)
ZEND_END_ARG_INFO()

//select
ZEND_BEGIN_ARG_INFO(arg_select, 1)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

//insert
ZEND_BEGIN_ARG_INFO(arg_insert, 2)
	ZEND_ARG_INFO(0, tablename)
	ZEND_ARG_ARRAY_INFO(0, data, 1) //ZEND_ARG_ARRAY_INFO(by_ref, name, allow_null)
ZEND_END_ARG_INFO()


//update
ZEND_BEGIN_ARG_INFO(arg_update, 2)
	ZEND_ARG_INFO(0, tablename)
	ZEND_ARG_ARRAY_INFO(0, data, 1) //ZEND_ARG_ARRAY_INFO(by_ref, name, allow_null)
ZEND_END_ARG_INFO()

//del
ZEND_BEGIN_ARG_INFO(arg_del, 2)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()




/* True global resources - no need for thread safety here */
static int le_daily-ext;

/* {{{ daily-ext_functions[]
 *
 * Every user visible function must have an entry in daily-ext_functions[].
 */
const zend_function_entry daily-ext_functions[] = {
	PHP_FE(confirm_daily-ext_compiled,	NULL)		/* For testing, remove later. */
	PHP_ME(Basedb, __construct, arg_construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, select, arg_select, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, insert, arg_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, update, arg_update, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, del, arg_del, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}	/* Must be the last line in daily-ext_functions[] */
};
/* }}} */

/* {{{ daily-ext_module_entry
 */
zend_module_entry daily-ext_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"daily-ext",
	daily-ext_functions,
	PHP_MINIT(daily-ext),
	PHP_MSHUTDOWN(daily-ext),
	PHP_RINIT(daily-ext),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(daily-ext),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(daily-ext),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DAILY-EXT
ZEND_GET_MODULE(daily-ext)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini*/

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("daily-ext.default_db_host",      "localhost", PHP_INI_ALL, OnUpdateString, default_db_host, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_user",      "root", PHP_INI_ALL, OnUpdateString, default_db_user, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_pwd",      "root", PHP_INI_ALL, OnUpdateString, default_db_pwd, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_name",      "test", PHP_INI_ALL, OnUpdateString, default_db_name, zend_daily-ext_globals, daily-ext_globals)
PHP_INI_END()

/* }}} */

/* {{{ php_daily-ext_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_daily-ext_init_globals(zend_daily-ext_globals *daily-ext_globals)
{
	daily-ext_globals->global_value = 0;
	daily-ext_globals->global_string = NULL;
}
*/
/* }}} */

zend_class_entry *Basedb_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(daily-ext)
{
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();

	zend_class_entry Basedb;
	INIT_CLASS_ENTRY(Basedb, "Basedb", daily-ext_functions);
	Basedb_ce = zend_register_internal_class_ex(&Basedb, NULL, NULL TSRMLS_CC); //注册类  第二个参数  父类  父类名称
	//声明类属性
	zend_declare_property_null(Basedb_ce, ZEND_STRL("_conn"), ZEND_ACC_STATIC|ZEND_ACC_PRIVATE TSRMLS_CC);
	//声明db_host db_user db_pwd db_name
	zend_declare_property_string(Basedb_ce, ZEND_STRL("_db_host"), DAILY-EXT_G('default_db_host'), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(Basedb_ce, ZEND_STRL("_db_user"), DAILY-EXT_G('default_db_user'), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(Basedb_ce, ZEND_STRL("_db_pwd"), DAILY-EXT_G('default_db_pwd'), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(Basedb_ce, ZEND_STRL("_db_name"), DAILY-EXT_G('default_db_name'), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(daily-ext)
{
	/* uncomment this line if you have INI entries*/
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(daily-ext)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(daily-ext)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(daily-ext)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "daily-ext support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini*/
	DISPLAY_INI_ENTRIES();

}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_daily-ext_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_daily-ext_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "daily-ext", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */



/* {{{ PHP_MINFO_FUNCTION
 */
PHP_METHOD(Basedb, __construct)
{
	//获取值
	char *db_host = '', *db_user = '', *db_pwd = '', *db_name = '';
	int host_len, user_len, pwd_len, name_len;

	//zend_parse_parameters 这个参数之前的参数被认为是必须的，之后的便认为是非必须的了,如果没有传递，则不会去修改载体
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssss", &db_host, &host_len, &db_user, &user_len, &db_pwd, &pwd_len, &db_name, &name_len) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "param error!");
		return;
	}

	zend_update_property_string(Z_OBJCE(getThis()), getThis(), ZEND_STRL("_db_host"), db_host TSRMLS_CC);
	zend_update_property_string(Z_OBJCE(getThis()), getThis(), ZEND_STRL("_db_user"), db_user TSRMLS_CC);
	zend_update_property_string(Z_OBJCE(getThis()), getThis(), ZEND_STRL("_db_pwd"), db_pwd TSRMLS_CC);
	zend_update_property_string(Z_OBJCE(getThis()), getThis(), ZEND_STRL("_db_name"), db_name TSRMLS_CC);

	//RETURN_ZVAL 第一个参数  待复制的对象    第二个参数  是否复制    第三个参数 是否释放复制的对象
	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_METHOD(Basedb, connect)
{
	zval *db_host, *db_user, *db_pwd, *db_name;

	zend_class_entry *ce;
	ce = Z_OBJCE(getThis());


/*	zend_read_property 第五个参数的含义

    0: 如果属性不存在，则抛出一个notice错误。
    1: 如果属性不存在，不报错。*/

	db_host = zend_read_property(ce, getThis(), ZEND_STRL("db_host"), 1 TSRMLS_CC);
	db_user = zend_read_property(ce, getThis(), ZEND_STRL("db_user"), 1 TSRMLS_CC);
	db_pwd = zend_read_property(ce, getThis(), ZEND_STRL("db_pwd"), 1 TSRMLS_CC);
	db_name = zend_read_property(ce, getThis(), ZEND_STRL("db_name"), 1 TSRMLS_CC);

}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
