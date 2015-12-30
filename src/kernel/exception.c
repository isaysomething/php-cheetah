#define CHEETAH_CLASS_KERNEL_EXCEPTION()	\
{	\
	zend_class_entry cheetah_kernel_exception_ce_local;	\
	INIT_CLASS_ENTRY(cheetah_kernel_exception_ce_local,"Cheetah\\Kernel\\Exception", cheetah_kernel_exception_methods);	\
	cheetah_kernel_exception_ce = zend_register_internal_class_ex(&cheetah_kernel_exception_ce_local TSRMLS_CC, zend_ce_exception);	\
}
PHP_METHOD( Cheetah_Kernel_Exception , getName );
static zend_function_entry cheetah_kernel_exception_methods[] = {
	ZEND_ME ( Cheetah_Kernel_Exception, getName, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Exception , getName ) {
	RETURN_STRING( "Exception" )
}
