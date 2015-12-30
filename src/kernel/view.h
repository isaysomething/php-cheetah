#define Cheetah_Kernel_View_H
zend_class_entry *cheetah_kernel_view_ce;

ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_view_render, 0, 0, 3 )
ZEND_ARG_TYPE_INFO ( 0, view,IS_STRING, 0 )
ZEND_ARG_ARRAY_INFO ( 0, params, 0 )
ZEND_ARG_INFO ( 0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_view_find_view_file, 0, 0, 2)
ZEND_ARG_TYPE_INFO ( 0, name,IS_STRING, 0 )
ZEND_ARG_INFO ( 0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_view_render_file, 0, 0, 2 )
ZEND_ARG_TYPE_INFO ( 0, filename,IS_STRING, 0 )
ZEND_ARG_ARRAY_INFO ( 0, params, 0 )
ZEND_END_ARG_INFO()
