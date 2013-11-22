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

#ifndef PHP_DAILY_H
#define PHP_DAILY_H

extern zend_module_entry daily_module_entry;
#define phpext_daily_ptr &daily_module_entry

#ifdef PHP_WIN32
#	define PHP_DAILY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DAILY_API __attribute__ ((visibility("default")))
#else
#	define PHP_DAILY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define DAILY_STARTUP(module)	 		  	ZEND_MODULE_STARTUP_N(module)(INIT_FUNC_ARGS_PASSTHRU)
#define DAILY_STARTUP_FUNCTION(module)   	ZEND_MINIT_FUNCTION(module)

PHP_MINIT_FUNCTION(daily);
PHP_MSHUTDOWN_FUNCTION(daily);
PHP_RINIT_FUNCTION(daily);
PHP_RSHUTDOWN_FUNCTION(daily);
PHP_MINFO_FUNCTION(daily);

PHP_FUNCTION(confirm_daily_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(daily)
	char *default_db_host;
	char *default_db_user;
	char *default_db_pwd;
	char *default_db_name;
ZEND_END_MODULE_GLOBALS(daily)


/* In every utility function you add that needs to use variables 
   in php_daily_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as DAILY_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define DAILY_G(v) TSRMG(daily_globals_id, zend_daily_globals *, v)
#else
#define DAILY_G(v) (daily_globals.v)
#endif

extern ZEND_DECLARE_MODULE_GLOBALS(daily);

#endif	/* PHP_DAILY_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
