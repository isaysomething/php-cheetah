#define Cheetah_Kernel_Object_H
zend_class_entry *cheetah_kernel_object_ce;

ZEND_BEGIN_ARG_INFO_EX ( arginfo_magic_func_get, 0, 0,1 )
ZEND_ARG_TYPE_INFO ( 0, name ,IS_STRING,0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_magic_func_set, 0, 0, 2 )
ZEND_ARG_TYPE_INFO ( 0, name ,IS_STRING,0)
ZEND_ARG_INFO ( 0, value )
ZEND_END_ARG_INFO()
