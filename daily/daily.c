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


#include "php_daily.h"
#include "basedb.h"

/* If you declare any globals in php_daily.h uncomment this:*/
//全局变量daily-ext_globals
ZEND_DECLARE_MODULE_GLOBALS(daily);


/* True global resources - no need for thread safety here */
static int le_daily;

/* {{{ daily_functions[]
 *
 * Every user visible function must have an entry in daily_functions[].
 */
const zend_function_entry daily_functions[] = {
	PHP_FE(confirm_daily_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in daily_functions[] */
};
/* }}} */

/* {{{ daily_module_entry
 */
zend_module_entry daily_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"daily",
	daily_functions,
	PHP_MINIT(daily),
	PHP_MSHUTDOWN(daily),
	PHP_RINIT(daily),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(daily),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(daily),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DAILY
ZEND_GET_MODULE(daily)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("daily.default_db_host",      "localhost", PHP_INI_ALL, OnUpdateString, default_db_host, zend_daily_globals, daily_globals)
    STD_PHP_INI_ENTRY("daily.default_db_user",      "root", PHP_INI_ALL, OnUpdateString, default_db_user, zend_daily_globals, daily_globals)
    STD_PHP_INI_ENTRY("daily.default_db_pwd",      "root", PHP_INI_ALL, OnUpdateString, default_db_pwd, zend_daily_globals, daily_globals)
    STD_PHP_INI_ENTRY("daily.default_db_name",      "test", PHP_INI_ALL, OnUpdateString, default_db_name, zend_daily_globals, daily_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_daily_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_daily_init_globals(zend_daily_globals *daily_globals)
{
	daily_globals->global_value = 0;
	daily_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(daily)
{
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();

	//注册basedb类
	DAILY_STARTUP(basedb);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(daily)
{
	/* uncomment this line if you have INI entries */
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(daily)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(daily)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(daily)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "daily support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini */
	DISPLAY_INI_ENTRIES();

}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_daily_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_daily_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "daily", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
