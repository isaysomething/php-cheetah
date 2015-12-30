#define Cheetah_Kernel_Module_H
zend_class_entry *cheetah_kernel_module_ce;
ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_module_run_action, 0, 0, 1 )
ZEND_ARG_ARRAY_INFO ( 0, resolve, 0 )
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX ( arginfo_kernel_module_create_controller, 0, 0, 1 )
ZEND_ARG_ARRAY_INFO ( 0, route, 0 )
ZEND_END_ARG_INFO()
