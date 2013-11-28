/**
 *
 */

#include "main/php_output.h"

#define VIEW_BUFFER_BLOCK_SIZE	4096
#define VIEW_BUFFER_SIZE_MASK 	4095

zend_class_entry *daily_view_simple_ce;

ZEND_BEGIN_ARG_INFO(daily_view_simple_construct_arginfo, 0)
	ZEND_ARG_INFO(0, template_dir)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_simple_get_arginfo, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_simple_isset_arginfo, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_simple_assign_by_ref_arginfo, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(1, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_simple_eval_arginfo, 0)
	ZEND_ARG_INFO(0, tpl_str)
	ZEND_ARG_INFO(0, vars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_simple_clear_arginfo, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

#if ((PHP_MAJOR_VERSION == 5)  && (PHP_MINOR_VERSION < 4>))

static int daily_view_simple_render_write(const char *str, uint str_length TSRMLS_DC)
{
	char *target;
	daily_view_simple_buffer *buffer = DAILY_G(buffer);

	if(!buffer->size) {
		buffer->size = (str_length | VIEW_BUFFER_SIZE_MASK)  + 1;
		buffer->len = str_length;
		buffer->buffer = emalloc(buffer->size);
		target = buffer->buffer;
	} else {
		size_t len = buffer->len + str_length;

		if(buffer->size < len + 1) {
			buffer->size = (len | VIEW_BUFFER_SIZE_MASK) + 1;
		}
	}

}

#endif

yaf_view_t * yaf_view_simple_instance(yaf_view_t *view, zval *tpl_dir, zval *options TSRMLS_DC)
{
	zval *instance, *tpl_vars;

	instance = view;
	if(!instance) {
		MAKE_STD_ZVAL(instance);
		object_init_ex(instance, yaf_view_simple_ce);
	}
	MAKE_STD_ZVAL(tpl_vars);
	array_init(tpl_vars);
	zend_update_property(yaf_view_simple_ce, instance, ZEND_STRL(""), tpl_vars TSRMLS_CC);
	zval_ptr_dtor(&tpl_vars);

	if(tpl_dir && Z_TYPE_P(tpl_dir) == IS_STRING) {

	}

}

int yaf_view_simple_render(yaf_view_t *view, zval *tpl, zval *vars, zval *ret TSRMLS_DC)
{
	zval *tpl_vars;
	char *script;

}




