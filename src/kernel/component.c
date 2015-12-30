#define CHEETAH_CLASS_KERNEL_COMPONENT()	\
	{	\
		zend_class_entry cheetah_kernel_component_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_kernel_component_ce_local, "Cheetah\\Kernel\\Component",cheetah_kernel_component_methods ); 	\
		cheetah_kernel_component_ce = zend_register_internal_class_ex (&cheetah_kernel_component_ce_local, cheetah_kernel_object_ce );	\
	}
static zend_function_entry cheetah_kernel_component_methods[] = { PHP_FE_END };
