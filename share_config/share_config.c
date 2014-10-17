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

/* If you declare any globals in php_share_config.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(share_config)

HashTable *share_config_ht;

long shm_size = 0l;

/* True global resources - no need for thread safety here */
static int le_share_config;


/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(get_share_config_arg_info, 0, 0, 2)
	ZEND_ARG_INFO(0, section)
	ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(del_share_config_arg_info, 0, 0, 2)
	ZEND_ARG_INFO(0, section)
	ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()
/* }}} */


/* {{{ share_config_functions[]
 *
 * Every user visible function must have an entry in share_config_functions[].
 */
const zend_function_entry share_config_functions[] = {
	PHP_FE(confirm_share_config_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(get_share_config, get_share_config_arg_info)
	PHP_FE(del_share_config, del_share_config_arg_info)
	{NULL, NULL, NULL}	/* Must be the last line in share_config_functions[] */
};
/* }}} */

/** {{{ module depends
 *  依赖shmop 存储是否有更新
 */


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


int time_of_minit = 0;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(share_config)
{
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();

	//@see http://www.walu.cc/phpbook/3.1.md   pemalloc
	share_config_ht = (HashTable *)pemalloc(sizeof(HashTable), 1);

	if (!share_config_ht) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "pemalloc share_config_ht error");
	}

	//初始化hash @see http://www.walu.cc/phpbook/8.2.md
	zend_hash_init(share_config_ht, 8, NULL, NULL, 1);


	shm_size = strlen("2014-10-14 18:46:55") * SHARE_CONFIG_G(config_item_num);



	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(share_config)
{
	/* uncomment this line if you have INI entries*/
	UNREGISTER_INI_ENTRIES();

	//清除hashtable
	if (share_config_ht) {
		zend_hash_destroy(share_config_ht);
		pefree(share_config_ht, 1);
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


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_share_config_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_share_config_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "share_config", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */

/* {{{ get_share_config  获取共享配置
 */
PHP_FUNCTION(get_share_config)
{
	char *share_config_api_url = NULL;
	char *request_url = NULL;
	const char *action = "get_share_config";

	char *section = NULL;
	char *item = NULL;

	char *hash_key = NULL;

	int section_len = 0, item_len = 0, len = 0;

	char *ret_str = NULL;


	share_config_api_url = SHARE_CONFIG_G(share_config_api_url);
	if (!share_config_api_url) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "php.ini item share_config_api_url must be not empty!");
		RETURN_FALSE;
	}

	//获取参数
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &section, &section_len, &item, &item_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!section || !item) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "section or item empty!");
		RETURN_FALSE;
	}

	len = spprintf(&hash_key, 0, "%s %s", section, item);

#if ZEND_DEBUG
	php_printf("share_config_ht element nums: %d<br>", zend_hash_num_elements(share_config_ht));
	php_printf("module time: %d<br>", time_of_minit);
#endif

	if (zend_hash_find(share_config_ht, hash_key, len + 1, (void **)&ret_str) == SUCCESS) {
		efree(hash_key);
		RETURN_STRING(ret_str, 1);
	} else {
		//如果没有找到
		len = spprintf(&request_url, 0, "%s?action=%s&section=%s&item=%s", share_config_api_url, action, section, item);

		zval func = {{0}, 0};
		ZVAL_STRING(&func, "file_get_contents", 0);

		zval *call_params[1] = {0};
		MAKE_STD_ZVAL(call_params[0]);
		ZVAL_STRING(call_params[0], request_url, 1);

		zval *retval_ptr = NULL;
		MAKE_STD_ZVAL(retval_ptr);

		if (call_user_function(EG(function_table), NULL, &func, retval_ptr, 1, call_params TSRMLS_CC) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "file_get_contents error");
			ret_str = "";
			goto free_memory;
		}

		ret_str = pestrndup(Z_STRVAL_P(retval_ptr), Z_STRLEN_P(retval_ptr), 1);

#if ZEND_DEBUG
	php_printf("ret_str: %s<br>", ret_str);
#endif

		//缓存
		if (zend_hash_update(share_config_ht, hash_key, strlen(hash_key) + 1, (void *)ret_str, strlen(ret_str) + 1, NULL) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "save cache failure");
		}
#if ZEND_DEBUG
	char *hash_value = NULL;
	if (zend_hash_find(share_config_ht, hash_key, strlen(hash_key) + 1, (void **)&hash_value) == SUCCESS) {
		php_printf("hash_value: %s<br>", hash_value);
	} else {
		php_printf("get hash_value failure <br>");
	}
#endif

		free_memory:
			efree(request_url);
			efree(hash_key);

			zval_ptr_dtor(&retval_ptr);
			zval_ptr_dtor(&call_params[0]);


		RETURN_STRING(ret_str, 1);
	}
}
/* }}} */


/* {{{ get_share_config   删除共享配置
 */
PHP_FUNCTION(del_share_config)
{
	char *section = NULL, *item = NULL;
	int section_len = 0, item_len = 0, len = 0;
	char *hash_key = NULL;

	//获取参数
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &section, &section_len, &item, &item_len) == FAILURE) {
		RETURN_FALSE;
	}

	len = spprintf(&hash_key, 0, "%s %s", section, item);
	if (zend_hash_del(share_config_ht, hash_key, len + 1) == SUCCESS) {
		efree(hash_key);
		RETURN_TRUE;
	}
	efree(hash_key);
	RETURN_FALSE;
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
