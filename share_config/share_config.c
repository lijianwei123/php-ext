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
#include "php_share_config.h"

#include <sys/types.h>
#include <sys/ipc.h>

/* If you declare any globals in php_share_config.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(share_config)

static HashTable *share_config_ht = NULL;
static void *item_ver_ptr = NULL;

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
    STD_PHP_INI_ENTRY("share_config.config_item_num", 10240, PHP_INI_ALL, OnUpdateLong, config_item_num, zend_share_config_globals, share_config_globals)
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

	share_config_ht = (HashTable *)pecalloc(1, sizeof(HashTable), 1);
	zend_hash_init(share_config_ht, 50, NULL, ZVAL_PTR_DTOR, 1);


	key_t ipcKey = SHARE_CONFIG_SHM_KEY;
	int sc_shm_id = shmget(ipcKey, SHARE_CONFIG_G(config_item_num) * sizeof(config_item_ver_t) + 9, IPC_CREATE | 0666);
	if (sc_shm_id < 0) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "shmget sys mem error Error: %s [%d]", strerror(errno), errno);
	}

	if ((item_ver_ptr = (void *)shmat(sc_shm_id, NULL, 0)) < 0) {
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

	if (share_config_ht) {
		zend_hash_destroy(share_config_ht);
		pefree(share_config_ht, 1);
	}

	if (item_ver_ptr) {
		shmdt(item_ver_ptr);
	}

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
	char *share_config_api_url = NULL, *request_url = NULL;

	char *item = NULL;
	int  item_len = 0, len = 0, ret = 0;

	zval **value = NULL, *ret_str = NULL;
	zval *call_params[2];
	zval *url = NULL;

	int item_ver = 0, share_item_ver = 0, item_index = 0;
	zval **ver = NULL, **index = NULL, **data = NULL;

	//获取参数
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s!", &item, &item_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!item) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "section or item empty!");
		RETURN_FALSE;
	}

	//比对版本
	if ((zend_hash_find(share_config_ht, item, item_len + 1, (void **)&value) == SUCCESS) && (Z_TYPE_PP(value) == IS_ARRAY)) {
		if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("ver"), (void **)&ver) == SUCCESS) {
			 convert_to_long(*ver);
			 item_ver = (int)Z_LVAL_PP(ver);
		}
		if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("index"), (void **)&index) == SUCCESS) {
			convert_to_long(*index);
			item_index = (int)Z_LVAL_PP(index);
		}
		if (item_ver > 0 && item_index > 0 && item_ver_ptr != NULL && !strncmp(item_ver_ptr, "lijianwei", 9)) {
			config_item_ver_t *ptr = (config_item_ver_t *)(item_ver_ptr + 9);
			share_item_ver = ptr[item_index - 1].ver_num;

			//版本相同
			if (share_item_ver > 0 && item_ver == share_item_ver){
				if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("data"), (void **)&data) == SUCCESS) {
					RETURN_ZVAL(*data, 1, 0);
				}
			}
		}
	}

	//如果没有找到
	share_config_api_url = SHARE_CONFIG_G(share_config_api_url);
	if (!share_config_api_url) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "php.ini share_config_api_url must be not empty!");
		RETURN_FALSE;
	}
	len = spprintf(&request_url, 0, "%s?action=%s&item=%s", share_config_api_url, "get_share_config", item);

	zval func;
	ZVAL_STRING(&func, "file_get_contents", 0);

	MAKE_STD_ZVAL(url);
	ZVAL_STRING(url, request_url, 1);
	call_params[0] = url;

	if (call_user_function(EG(function_table), NULL, &func, ret_str, 1, call_params TSRMLS_CC) == FAILURE) {
		if (ret_str)
			zval_dtor(ret_str);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "file_get_contents failure");
		goto failure;
	}
	if (ret_str) {
		ZVAL_STRING(&func, "json_decode", 0);
		zval assoc;
		ZVAL_BOOL(assoc, 1);

		call_params[0] = ret_str;
		call_params[1] = &assoc;
		if (call_user_function(EG(function_table), NULL, &func, *value, 2, call_params TSRMLS_CC) == FAILURE) {
			if (*value)
				zval_dtor(*value);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "json_decode failure");
			goto failure;
		}
		zval_dtor(ret_str);

		if (*value && Z_TYPE_P(*value) == IS_ARRAY) {
			if (zend_hash_find(Z_ARRVAL_P(*value), ZEND_STRS("ver"), (void **)&ver) == SUCCESS) {
				convert_to_long(*ver);
				item_ver = (int)Z_LVAL_PP(ver);

				if (item_ver > 0) {
					//缓存数据
					if (zend_hash_update(share_config_ht, item, item_len + 1, value, sizeof(zval *), NULL) == FAILURE) {
						php_error_docref(NULL TSRMLS_CC, E_WARNING, "update cache failure");
						goto failure;
					}
					Z_ADDREF_P(*value);
					//设置版本
					if (!strncmp(item_ver_ptr, "lijianwei", 9)) {
						if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("index"), (void **)&index) == SUCCESS) {
							convert_to_long(*index);
							item_index = (int)Z_LVAL_PP(index);
						} else {
							php_error_docref(NULL TSRMLS_CC, E_WARNING, "config item don't have index");
							goto failure;
						}
						config_item_ver_t *ptr = (config_item_ver_t *)(item_ver_ptr + 9);
						ptr[item_index - 1].ver_num = item_ver;

						RETURN_ZVAL(*value, 1, 0);
					}
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "config item don't have ver num");
				goto failure;
			}
		}
	}
	//失败
	failure:
		if (request_url)
			efree(request_url);
		if (url)
			zval_ptr_dtor(&url);
		if (ret_str)
			zval_dtor(ret_str);
		if (*value)
			zval_dtor(*value);

		RETURN_FALSE;
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
