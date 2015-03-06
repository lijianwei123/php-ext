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
123
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_share_config.h"

#include <sys/types.h>
#include <sys/ipc.h>

/* If you declare any globals in php_share_config.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(share_config)

static HashTable *share_config_ht = NULL;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(get_share_config_arg_info, 0, 0, 1)
	ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(set_share_config_arg_info, 0, 0, 2)
	ZEND_ARG_INFO(0, item)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* }}} */


/* {{{ share_config_functions[]
 *
 * Every user visible function must have an entry in share_config_functions[].
 */
const zend_function_entry share_config_functions[] = {
	PHP_FE(get_share_config, get_share_config_arg_info)
	PHP_FE(set_share_config, set_share_config_arg_info)
	PHP_FE(start_share_config, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in share_config_functions[] */
};
/* }}} */



/* {{{ share_config_module_entry
 */
zend_module_entry share_config_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER,
#endif
	"share_config",
	share_config_functions,
	PHP_MINIT(share_config),
	PHP_MSHUTDOWN(share_config),
	PHP_RINIT(share_config),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(share_config),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(share_config),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SHARE_CONFIG
ZEND_GET_MODULE(share_config)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini*/
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("share_config.share_config_api_url", "", PHP_INI_ALL, OnUpdateString, share_config_api_url, zend_share_config_globals, share_config_globals)
    STD_PHP_INI_ENTRY("share_config.config_item_num", 4096, PHP_INI_ALL, OnUpdateLong, config_item_num, zend_share_config_globals, share_config_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_share_config_init_globals
 */
/* Uncomment this function if you have INI entries*/
static void php_share_config_init_globals(zend_share_config_globals *share_config_globals)
{
	share_config_globals->share_config_api_url = "";
	share_config_globals->config_item_num = 0l;
}
/* }}} */



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(share_config)
{
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();

	key_t ipcKey = SHARE_CONFIG_SHM_KEY;
	int sc_shm_id = shmget(ipcKey, SHARE_CONFIG_SIZE, IPC_CREATE | 0666);
	if (sc_shm_id < 0) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "shmget sys mem error Error: %s [%d]", strerror(errno), errno);
	}

	if ((share_config_ht = (HashTable *)shmat(sc_shm_id, NULL, 0)) < 0) {
		 php_error_docref(NULL TSRMLS_CC, E_ERROR, "attach sys mem error Error: %s [%d]", strerror(errno), errno);
		 RETURN_FALSE;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(share_config)
{
	/* uncomment this line if you have INI entries*/
	UNREGISTER_INI_ENTRIES();

	if (share_config_ht != NULL)
		shmdt(share_config_ht);

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(share_config)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(share_config)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(share_config)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "share_config support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini*/
	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* {{{ get_share_config  获取共享配置
 */
PHP_FUNCTION(get_share_config)
{
	char *item = NULL;
	int item_len = 0;
	zval **value = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s!" &item, &item_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!item) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR,  "item is empty!");
		RETURN_FALSE;
	}

	if (zend_hash_find(share_config_ht, item, item_len + 1, (void **)&value) == FAILURE) {
		RETURN_FALSE;
	} else {
		RETURN_ZVAL(*value, 1, 0);
	}
}
/* }}} */


/* {{{ set_share_config  设置共享配置
 */
PHP_FUNCTION(set_share_config)
{
	char *item = NULL;
	int item_len = 0;
	zval *value = NULL;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s!z" &item, &item_len, &value) == FAILURE) {
		RETURN_FALSE;
	}

	if (!item || !value) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "item or value is empty!");
		RETURN_FALSE;
	}

	if (share_config_ht == NULL) {

	}

	if (zend_hash_update(share_config_ht, &item, &item_len, &value, sizeof(zval *), NULL) == SUCCESS) {
		Z_ADDREF_P(value);
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */


/* {{{ start_share_config  启动共享配置
 */
PHP_FUNCTION(start_share_config)
{
    if (strcasecmp("cli", sapi_module.name) != 0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "share_config must run at php_cli environment.");
        RETURN_FALSE;
    }
    share_config_ht =
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
