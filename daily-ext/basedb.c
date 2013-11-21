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
#include "main/SAPI.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_var.h"
#include "ext/standard/basic_functions.h"

#include "php_daily-ext.h"
#include "basedb.h"


zend_class_entry *Basedb_ce;

//basedb args
ZEND_BEGIN_ARG_INFO(basedb_arg_construct, 4)
	ZEND_ARG_INFO(0, db_host) //0 表示按值传递  1表示引用传递
	ZEND_ARG_INFO(0, db_user)
	ZEND_ARG_INFO(0, db_pwd)
	ZEND_ARG_INFO(0, db_name)
ZEND_END_ARG_INFO()

//select
ZEND_BEGIN_ARG_INFO(basedb_arg_select, 1)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

//insert
ZEND_BEGIN_ARG_INFO(basedb_arg_insert, 2)
	ZEND_ARG_INFO(0, tablename)
	ZEND_ARG_ARRAY_INFO(0, data, 1) //ZEND_ARG_ARRAY_INFO(by_ref, name, allow_null)
ZEND_END_ARG_INFO()


//update
ZEND_BEGIN_ARG_INFO(basedb_arg_update, 2)
	ZEND_ARG_INFO(0, tablename)
	ZEND_ARG_ARRAY_INFO(0, data, 1) //ZEND_ARG_ARRAY_INFO(by_ref, name, allow_null)
ZEND_END_ARG_INFO()

//del
ZEND_BEGIN_ARG_INFO(basedb_arg_del, 2)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()



/* {{{ basedb_methods[]
*/
const zend_function_entry basedb_methods[] = {
	PHP_ME(Basedb, __construct, basedb_arg_construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, select, basedb_arg_select, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, insert, basedb_arg_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, update, basedb_arg_update, ZEND_ACC_PUBLIC)
	PHP_ME(Basedb, del, basedb_arg_del, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}	/* Must be the last line in daily-ext_functions[] */
};
/* }}} */



/* {{{ Basedb::__construct($db_host = '', $db_user = '', $db_pwd = '', $db_name = '')
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


/* {{{ Basedb::connect()
 */
PHP_METHOD(Basedb, connect)
{
	zend_class_entry *ce;
	ce = Z_OBJCE(getThis());


	zval *conn;
	conn = zend_read_static_property(ce, ZEDN_STRL("_conn"), 1 TSRMLS_CC);
	if(IS_OBJECT == Z_TYPE_P(conn))
		return;


	zval func = {{0}, 0};
	zval *retval_ptr = NULL;
	zval *params[3] = {0};

	zval *db_host, *db_user, *db_pwd, *db_name;

/*	zend_read_property 第五个参数的含义

    0: 如果属性不存在，则抛出一个notice错误。
    1: 如果属性不存在，不报错。*/
	db_host = zend_read_property(ce, getThis(), ZEND_STRL("db_host"), 1 TSRMLS_CC);
	db_user = zend_read_property(ce, getThis(), ZEND_STRL("db_user"), 1 TSRMLS_CC);
	db_pwd = zend_read_property(ce, getThis(), ZEND_STRL("db_pwd"), 1 TSRMLS_CC);
	db_name = zend_read_property(ce, getThis(), ZEND_STRL("db_name"), 1 TSRMLS_CC);

	params[0] = db_host;
	params[1] = db_user;
	params[2] = db_pwd;

	//调用mysql_connect
	ZVAL_STRING(&function, "mysql_connect", 0);
	if(call_user_function(EG(function_table), NULL, &func, return_value, 3, $params TSRMLS_CC) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "mysql connect error");
		return;
	}

	//更新self::_conn
	zend_udpate_static_property(ce, ZEND_STRL("_conn"), return_value TSRMLS_CC);

	//选择数据库

}

DAILY_STARTUP_FUNCTION(basedb)
{
	zend_class_entry Basedb;
	INIT_CLASS_ENTRY(Basedb, "Basedb", basedb_methods);
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


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
