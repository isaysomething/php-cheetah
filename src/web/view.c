#define CHEETAH_CLASS_WEB_VIEW()	\
	{	\
		zend_class_entry cheetah_web_view_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_web_view_ce_local, "Cheetah\\Web\\View",cheetah_web_view_methods );	\
		cheetah_web_view_ce = zend_register_internal_class_ex (&cheetah_web_view_ce_local, cheetah_kernel_view_ce );	\
	}

static zend_function_entry cheetah_web_view_methods[] = { PHP_FE_END };
