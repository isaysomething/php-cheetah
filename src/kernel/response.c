#define CHEETAH_CLASS_KERNEL_RESPONSE()	\
	{	\
		zend_class_entry cheetah_kernel_response_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_kernel_response_ce_local, "Cheetah\\Kernel\\Response",cheetah_kernel_response_methods );	\
		cheetah_kernel_response_ce = zend_register_internal_class_ex (&cheetah_kernel_response_ce_local, cheetah_kernel_component_ce );	\
		zend_declare_property_long ( cheetah_kernel_response_ce, "exitStatus",10, 0, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	}

PHP_METHOD( Cheetah_Kernel_Response , send );
static zend_function_entry cheetah_kernel_response_methods[] = {
	ZEND_ME(Cheetah_Kernel_Response, send, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Response , send ) {
}
