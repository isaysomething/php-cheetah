#define CHEETAH_CLASS_KERNEL_INVALID_PARAM_EXCEPTION()	\
	{	\
		zend_class_entry cheetah_kernel_invalid_param_exception_ce_local;	\
		INIT_CLASS_ENTRY(cheetah_kernel_invalid_param_exception_ce_local,"Cheetah\\Kernel\\InvalidParamException", cheetah_kernel_invalid_param_exception_methods);	\
		cheetah_kernel_invalid_param_exception_ce = zend_register_internal_class_ex(&cheetah_kernel_invalid_param_exception_ce_local TSRMLS_CC, cheetah_kernel_exception_ce);	\
	}
PHP_METHOD( Cheetah_Kernel_Invalid_Param_Exception , getName );
static zend_function_entry cheetah_kernel_invalid_param_exception_methods[] = {
	ZEND_ME ( Cheetah_Kernel_Invalid_Param_Exception, getName, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Invalid_Param_Exception , getName ) {
	RETURN_STRING( "Invalid Parameter" )
}
