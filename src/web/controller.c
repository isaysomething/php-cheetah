#define CHEETAH_CLASS_WEB_CONTROLLER()	\
	{	\
		zend_class_entry cheetah_web_controller_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_web_controller_ce_local, "Cheetah\\Web\\Controller",cheetah_web_controller_methods );	\
		cheetah_web_controller_ce = zend_register_internal_class_ex (&cheetah_web_controller_ce_local, cheetah_kernel_controller_ce );	\
	}

static zend_function_entry cheetah_web_controller_methods[] = { PHP_FE_END };
