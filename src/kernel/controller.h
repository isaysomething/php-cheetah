#define Cheetah_Kernel_Controller_H
zend_class_entry *cheetah_kernel_controller_ce;

ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_controller_render, 0, 0, 2 )
ZEND_ARG_TYPE_INFO ( 0, view,IS_STRING, 0 )
ZEND_ARG_ARRAY_INFO ( 0, params, 0 )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_controller_render_content, 0, 0, 1 )
ZEND_ARG_INFO ( 0, content )
ZEND_END_ARG_INFO()
