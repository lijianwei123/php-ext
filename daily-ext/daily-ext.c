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

#include "basedb.h"


/* {{{ daily-ext_functions[]
 */
const zend_function_entry daily-ext_functions[] = {
	PHP_FE(confirm_daily-ext_compiled,	NULL)
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
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("daily-ext.default_db_host",      "localhost", PHP_INI_ALL, OnUpdateString, default_db_host, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_user",      "root", PHP_INI_ALL, OnUpdateString, default_db_user, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_pwd",      "root", PHP_INI_ALL, OnUpdateString, default_db_pwd, zend_daily-ext_globals, daily-ext_globals)
    STD_PHP_INI_ENTRY("daily-ext.default_db_name",      "test", PHP_INI_ALL, OnUpdateString, default_db_name, zend_daily-ext_globals, daily-ext_globals)
PHP_INI_END()

/* }}} */



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(daily-ext)
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



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
