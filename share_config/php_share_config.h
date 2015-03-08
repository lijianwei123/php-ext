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

#ifndef PHP_SHARE_CONFIG_H
#define PHP_SHARE_CONFIG_H

extern zend_module_entry share_config_module_entry;
#define phpext_share_config_ptr &share_config_module_entry

#ifdef PHP_WIN32
#	define PHP_SHARE_CONFIG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SHARE_CONFIG_API __attribute__ ((visibility("default")))
#else
#	define PHP_SHARE_CONFIG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(share_config);
PHP_MSHUTDOWN_FUNCTION(share_config);
PHP_RINIT_FUNCTION(share_config);
PHP_RSHUTDOWN_FUNCTION(share_config);
PHP_MINFO_FUNCTION(share_config);

PHP_FUNCTION(confirm_share_config_compiled);	/* For testing, remove later. */
//新增两个函数
PHP_FUNCTION(get_share_config);
PHP_FUNCTION(del_share_config);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/

ZEND_BEGIN_MODULE_GLOBALS(share_config)
	char *share_config_api_url;
	long config_item_num;
ZEND_END_MODULE_GLOBALS(share_config)


/* In every utility function you add that needs to use variables 
   in php_share_config_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as SHARE_CONFIG_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define SHARE_CONFIG_G(v) TSRMG(share_config_globals_id, zend_share_config_globals *, v)
#else
#define SHARE_CONFIG_G(v) (share_config_globals.v)
#endif



#define SHARE_CONFIG_SHM_KEY  ftok("/etc/rc.local", 1)
typedef struct {
	int ver_num;
} config_item_ver_t;

#endif	/* PHP_SHARE_CONFIG_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
