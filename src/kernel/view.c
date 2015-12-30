#define CHEETAH_CLASS_KERNEL_VIEW()	\
	{	\
		zend_class_entry cheetah_kernel_view_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_kernel_view_ce_local, "Cheetah\\Kernel\\View",cheetah_kernel_view_methods );	\
		cheetah_kernel_view_ce = zend_register_internal_class_ex (&cheetah_kernel_view_ce_local, cheetah_kernel_component_ce );	\
		zend_declare_property_null ( cheetah_kernel_view_ce, "_view",5, ZEND_ACC_PROTECTED TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_kernel_view_ce, "context",7, ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_string ( cheetah_kernel_view_ce, "defaultExtension",16,"php", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	}

PHP_METHOD( Cheetah_Kernel_View, render);
PHP_METHOD( Cheetah_Kernel_View, findViewFile);
PHP_METHOD( Cheetah_Kernel_View, renderFile);
static zend_function_entry cheetah_kernel_view_methods[] = {
	ZEND_ME(Cheetah_Kernel_View, render, arginfo_kernel_view_render, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_View, findViewFile, arginfo_kernel_view_find_view_file, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_View, renderFile, arginfo_kernel_view_render_file, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_View, render) {
	char *view_name;
	size_t view_name_len;
	zval *params;
	zval *context;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s|az", &view_name, &view_name_len, &params,&context ) == FAILURE) {
		RETURN_FALSE
	}
	zval view_file;
	zval find_params[2];
	ZVAL_STRINGL(&(find_params[0]), view_name, view_name_len);
	ZVAL_ZVAL(&(find_params[1]), context, 1, 0);
	cheetah_call_user_function(NULL, getThis(), "findViewFile", &view_file, 2, find_params);
	if (access(Z_STRVAL(view_file), 0) == -1) {
		zend_throw_exception_ex(cheetah_kernel_exception_ce, 0, "The view file does not exist: %s",
				Z_STRVAL(view_file));
		return;
	}
	zval retval, render_params[2];
	ZVAL_ZVAL(&(render_params[0]), &view_file, 1, 0);
	if (Z_TYPE_P(params) == IS_ARRAY) {
		ZVAL_ZVAL(&(render_params[1]), params, 1, 0);
	} else {
		zval empty_array;
		array_init(&empty_array);
		ZVAL_ZVAL(&(render_params[1]), &empty_array, 1, 0);
	}
	cheetah_call_user_function(NULL, getThis(), "renderFile", &retval, 2, render_params);
	RETURN_ZVAL(&retval, 1, 0)
}
ZEND_METHOD( Cheetah_Kernel_View, findViewFile) {
	char *name;
	size_t name_len;
	zval *context;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &name, &name_len,&context ) == FAILURE) {
		RETURN_FALSE
	}
	zval *app, base_path, module, module_id, controller, controller_id, default_extension;
	app = CHEETAH_G(application);
	char *view_file = NULL;
	size_t view_file_len = 0;
	//cheetah_read_static_property(cheetah_ce, "app", 0, &app);
	cheetah_read_property(NULL, app, "controller", 0, &controller);
	cheetah_read_property(NULL, app, "basePath", 0, &base_path);
	cheetah_read_property(NULL, &controller, "id", 0, &controller_id);
	cheetah_read_property(NULL, &controller, "module", 0, &module);
	if (Z_TYPE(module) == IS_OBJECT) {
		cheetah_read_property(NULL, &module, "id", 0, &module_id);
		view_file_len = view_file_len + Z_STRLEN(module_id) + 7;
	}
	cheetah_read_property(NULL, getThis(), "defaultExtension", 0, &default_extension);
	view_file_len = view_file_len + Z_STRLEN(base_path) + 1 + 5 + 1 + Z_STRLEN(controller_id) + 1 + name_len + 1
			+ Z_STRLEN(default_extension) + 1;
	view_file = (char *) malloc(view_file_len);
	strcpy(view_file, Z_STRVAL(base_path));
	strcat(view_file, DIRECTORY_SEPARATOR);
	if (Z_TYPE(module) == IS_OBJECT) {
		strcat(view_file, "modules");
		strcat(view_file, DIRECTORY_SEPARATOR);
		strcat(view_file, Z_STRVAL(module_id));
		strcat(view_file, DIRECTORY_SEPARATOR);
	}
	strcat(view_file, "views");
	strcat(view_file, DIRECTORY_SEPARATOR);
	strcat(view_file, Z_STRVAL(controller_id));
	strcat(view_file, DIRECTORY_SEPARATOR);
	strcat(view_file, name);
	strcat(view_file, ".");
	strcat(view_file, Z_STRVAL(default_extension));
	RETURN_STRING(view_file);
}
ZEND_METHOD( Cheetah_Kernel_View, renderFile) {
	char *filename;
	size_t filename_len;
	zval *params;
	zval *context;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "sa", &filename, &filename_len,&params ) == FAILURE) {
		RETURN_FALSE
	}
	zval retval, params1[1], params2[2];

	cheetah_call_user_function_no_params(NULL, NULL, "ob_start", &retval);

	ZVAL_BOOL(&(params1[0]), FALSE);
	cheetah_call_user_function(NULL, NULL, "ob_implicit_flush", &retval, 1, params1);
	ZVAL_NULL(&retval);

	ZVAL_ZVAL(&(params2[0]), params, 1, 0);
	ZVAL_LONG(&(params2[1]), 0);
	cheetah_call_user_function(NULL, NULL, "extract", &retval, 2, params2);
	ZVAL_NULL(&retval);

	if (cheetah_include(filename) == FAILURE) {
		RETURN_NULL()
	}

	cheetah_call_user_function_no_params(NULL, NULL, "ob_get_clean", &retval);
	RETURN_ZVAL(&retval, 1, 0);
}
