#define CHEETAH_CLASS_KERNEL_REQUEST()	\
{	\
	zend_class_entry cheetah_kernel_request_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_kernel_request_ce_local, "Cheetah\\Kernel\\Request",cheetah_kernel_request_methods );	\
	cheetah_kernel_request_ce = zend_register_internal_class_ex (&cheetah_kernel_request_ce_local, cheetah_kernel_component_ce );	\
}
PHP_METHOD( Cheetah_Kernel_Request , resolve );
static zend_function_entry cheetah_kernel_request_methods[] = {
	ZEND_ME ( Cheetah_Kernel_Request, resolve, NULL,ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Request , resolve ) {
}
