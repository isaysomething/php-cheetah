#define Cheetah_H
zend_class_entry *cheetah_ce;
ZEND_BEGIN_ARG_INFO_EX ( arginfo_cheetah_configure, 0, 0, 2 )
ZEND_ARG_OBJ_INFO(0, object, NULL, 0)
ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_cheetah_create_object, 0, 0, 2 )
ZEND_ARG_INFO(0, type)
ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_cheetah_autoload, 0, 0, 1 )
ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_cheetah_set_logger, 0, 0, 1 )
ZEND_ARG_TYPE_INFO(0, logger, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX ( arginfo_cheetah_log_message, 0, 0, 2 )
ZEND_ARG_INFO(0, message)
ZEND_ARG_TYPE_INFO(0, category, IS_STRING, 0)
ZEND_END_ARG_INFO()

