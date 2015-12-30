#define Cheetah_Di_Container_H
zend_class_entry *cheetah_di_container_ce;
ZEND_BEGIN_ARG_INFO_EX ( arginfo_di_container_get, 0, 0, 3 )
ZEND_ARG_INFO ( 0, class)
ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 0)
ZEND_ARG_TYPE_INFO(0, config, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_di_container_build, 0, 0, 3 )
ZEND_ARG_INFO ( 0, class)
ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 0)
ZEND_ARG_TYPE_INFO(0, config, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_di_container_get_dependencies, 0, 0, 1 )
ZEND_ARG_TYPE_INFO(0, class, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_di_container_resolve_dependencies, 0, 0, 2 )
ZEND_ARG_TYPE_INFO(0, class, IS_STRING, 0)
ZEND_ARG_TYPE_INFO(0, class, IS_OBJECT, 1)
ZEND_END_ARG_INFO()
