/**
 * view interface
 */

zend_class_entry *daily_view_interface_ce;


ZEND_BEGIN_ARG_INFO(daily_view_assign_arginfo, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_display_arginfo, 0)
	ZEND_ARG_INFO(0, tpl)
	ZEND_ARG_INFO(0, tpl_vars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_render_arginfo, 0)
	ZEND_ARG_INFO(0, tpl)
	ZEND_ARG_INFO(0, tpl_vars)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_setpath_arginfo, 0)
	ZEND_ARG_INFO(0, template_dir)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(daily_view_getpath_arginfo, 0)
ZEND_END_ARG_INFO()

/** {{{ daily_view_interface_methods
 */
zend_function_entry daily_view_interface_methods[] = {
		ZEND_ABSTRACT_ME(Daily_View_Interface, assign, daily_view_assign_arginfo)
		ZEND_ABSTRACT_ME(Daily_View_Interface, display, daily_view_display_arginfo)
		ZEND_ABSTRACT_ME(Daily_View_Interface, render, daily_view_render_arginfo)
		ZEND_ABSTRACT_ME(Daily_View_Interface, setScriptPath, daily_view_setpath_arginfo)
		ZEND_ABSTRACT_ME(Daily_View_Interface, getScriptPath, daily_view_getpath_arginfo)
		{NULL, NULL, NULL}
};
/* }}} */

/** {{{
 */
DAILY_STARTUP_FUNCTION(view_interface)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Daily_View_Interface", daily_view_interface_methods);
	daily_view_interface_ce = zend_register_interface_interface(&ce TSRMLS_CC)
}
/* }}} */

