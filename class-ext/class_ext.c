/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
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

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_class_ext.h"


ZEND_BEGIN_ARG_INFO(arg_construct,2)  
ZEND_ARG_INFO(0, width)  
ZEND_ARG_INFO(0, height)  
ZEND_END_ARG_INFO()  
  
ZEND_BEGIN_ARG_INFO(arg_set_width,1)  
ZEND_ARG_INFO(0, width)  
ZEND_END_ARG_INFO()  
  
ZEND_BEGIN_ARG_INFO(arg_set_height,1)  
ZEND_ARG_INFO(0, height)  
ZEND_END_ARG_INFO()  


/* If you declare any globals in php_class_ext.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(class_ext)
*/

/* True global resources - no need for thread safety here */
static int le_class_ext;

/* {{{ class_ext_functions[]
 *
 * Every user visible function must have an entry in class_ext_functions[].
 */
const zend_function_entry class_ext_functions[] = {
	PHP_FE(confirm_class_ext_compiled,	NULL)
	PHP_ME(Rectangle, __construct, arg_construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
        PHP_ME(Rectangle, clone, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Rectangle, setWidth, arg_set_width, ZEND_ACC_PUBLIC)
        PHP_ME(Rectangle, setHeight, arg_set_height, ZEND_ACC_PUBLIC)
        PHP_ME(Rectangle, getWidth, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Rectangle, getHeight, NULL, ZEND_ACC_PUBLIC)  
        PHP_ME(Rectangle, getArea, NULL, ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}	/* Must be the last line in class_ext_functions[] */
};
/* }}} */

/* {{{ class_ext_module_entry
 */
zend_module_entry class_ext_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"class_ext",
	class_ext_functions,
	PHP_MINIT(class_ext),
	PHP_MSHUTDOWN(class_ext),
	PHP_RINIT(class_ext),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(class_ext),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(class_ext),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CLASS_EXT
ZEND_GET_MODULE(class_ext)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("class_ext.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_class_ext_globals, class_ext_globals)
    STD_PHP_INI_ENTRY("class_ext.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_class_ext_globals, class_ext_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_class_ext_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_class_ext_init_globals(zend_class_ext_globals *class_ext_globals)
{
	class_ext_globals->global_value = 0;
	class_ext_globals->global_string = NULL;
}
*/
/* }}} */

zend_class_entry *Rectangle_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(class_ext)
{
	
	zend_class_entry Rectangle;  
    	INIT_CLASS_ENTRY(Rectangle, "Rectangle", class_ext_functions); //第二个参数为类名，第三个参数为类的函数列表  
    	Rectangle_ce = zend_register_internal_class_ex(&Rectangle, NULL, NULL TSRMLS_CC); //注册类  
    	zend_declare_property_null(Rectangle_ce, ZEND_STRL("_width"), ZEND_ACC_PRIVATE TSRMLS_CC); //初始化类的属性_width  
    	zend_declare_property_null(Rectangle_ce, ZEND_STRL("_height"), ZEND_ACC_PRIVATE TSRMLS_CC);  //初始化类的属性_height 

	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(class_ext)
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
PHP_RINIT_FUNCTION(class_ext)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(class_ext)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(class_ext)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "class_ext support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_class_ext_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_class_ext_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "class_ext", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


PHP_METHOD(Rectangle, __construct)  
{  
    long width,height;  
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &width, &height) == FAILURE){ //获取构造函数的两个函数参数_width和_height  
        WRONG_PARAM_COUNT;  
    }  
    if( width <= 0 ) {  
        width = 1; //如果_width为0，则赋默认值1  
    }  
    if( height <= 0 ) {  
        height = 1; //如果_height为0，则赋默认值1  
    }  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), width TSRMLS_CC); //更新类成员变量_width的值  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), height TSRMLS_CC);  //更新类成员变量_height的值  
    RETURN_TRUE;  
}  
  
PHP_METHOD(Rectangle, clone)  
{  
    zval *clone_obj;  
    zval *width,*height;  
    MAKE_STD_ZVAL(clone_obj);  
    object_init_ex(clone_obj, Rectangle_ce); //初始化对象，对象所属的类为Rectangle_ce  
    width = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), 0 TSRMLS_CC); //获取类成员变量_width的值  
    height = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), 0 TSRMLS_CC); //获取类成员变量_height的值  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), width TSRMLS_CC); //更新Rectangle_ce类对象clone_obj的属性值_width  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), height TSRMLS_CC); //更新Rectangle_ce类对象clone_obj的属性值_height  
    RETURN_ZVAL(clone_obj, 1, 0);  //返回该对象  
}  
  
PHP_METHOD(Rectangle, setWidth)  
{  
    long width;  
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &width) == FAILURE){  
        WRONG_PARAM_COUNT;  
    }  
    if( width <= 0 ) {  
        width = 1;  
    }  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), width TSRMLS_CC); //更新类成员变量_width的值  
    RETURN_TRUE;  
}  
  
PHP_METHOD(Rectangle, setHeight)  
{  
    long height;  
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &height) == FAILURE){  
        WRONG_PARAM_COUNT;  
    }  
    if( height <= 0 ) {  
        height = 1;  
    }  
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), height TSRMLS_CC); //更新类成员变量_height的值  
    RETURN_TRUE;  
}  
  
PHP_METHOD(Rectangle, getWidth)  
{  
    zval *zWidth;  
    long width;  
    zWidth = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), 0 TSRMLS_CC); //获取类成员变量_width的值  
    width = Z_LVAL_P(zWidth);  
    RETURN_LONG(width);  
}  
  
PHP_METHOD(Rectangle, getHeight)  
{  
    zval *zHeight;  
    long height;  
    zHeight = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), 0 TSRMLS_CC);  
    height = Z_LVAL_P(zHeight);  
    RETURN_LONG(height);  
}  
  
PHP_METHOD(Rectangle, getArea)  
{  
    zval *zWidth,*zHeight;  
    long width,height,area;  
    zWidth = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_width"), 0 TSRMLS_CC);  
    zHeight = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_height"), 0 TSRMLS_CC);  
    width = Z_LVAL_P(zWidth);  
    height = Z_LVAL_P(zHeight);  
    area = width * height;  
    RETURN_LONG(area);  
}  
  

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
