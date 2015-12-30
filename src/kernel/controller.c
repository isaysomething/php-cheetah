#define CHEETAH_CLASS_KERNEL_CONTROLLER()	\
	{	\
		zend_class_entry cheetah_kernel_controller_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_kernel_controller_ce_local, "Cheetah\\Kernel\\Controller",cheetah_kernel_controller_methods );	\
		cheetah_kernel_controller_ce = zend_register_internal_class_ex (&cheetah_kernel_controller_ce_local, cheetah_kernel_component_ce );	\
		zend_declare_property_null ( cheetah_kernel_controller_ce, "id",2, ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_kernel_controller_ce, "module",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_string ( cheetah_kernel_controller_ce, "defaultAction",13,"index", ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_kernel_controller_ce, "action",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_kernel_controller_ce, "_view",5, ZEND_ACC_PROTECTED TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_kernel_controller_ce, "layout",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	}

PHP_METHOD( Cheetah_Kernel_Controller , beforeAction );
PHP_METHOD( Cheetah_Kernel_Controller , afterAction );
PHP_METHOD( Cheetah_Kernel_Controller , render );
PHP_METHOD( Cheetah_Kernel_Controller , renderContent );
PHP_METHOD( Cheetah_Kernel_Controller , findLayoutFile );
PHP_METHOD( Cheetah_Kernel_Controller , getView );
static zend_function_entry cheetah_kernel_controller_methods[] = {
	ZEND_ME(Cheetah_Kernel_Controller, beforeAction, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Controller, afterAction, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Controller, render, arginfo_kernel_controller_render, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Controller, renderContent, arginfo_kernel_controller_render_content, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Controller, findLayoutFile, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Controller, getView, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Controller , beforeAction ) {
}
ZEND_METHOD( Cheetah_Kernel_Controller , afterAction ) {
}
ZEND_METHOD( Cheetah_Kernel_Controller , render ) {
	char *view_name;
	size_t view_name_len;
	zval *params;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &view_name, &view_name_len, &params ) == FAILURE ) {
		RETURN_FALSE
	}
	// Get view object
	zval view;
	cheetah_call_user_function_no_params( NULL , getThis() , "getView" , &view );

	// Call function render of object view
	zval content;
	zval view_params[3];
	ZVAL_STRINGL( &( view_params[0] ) , view_name , view_name_len );
	if ( Z_TYPE_P( params ) != IS_ARRAY ) {
		zval _params;
		array_init( &_params );
		ZVAL_ZVAL( &( view_params[1] ) , &_params , 1 , 0 );
	} else {
		ZVAL_ZVAL( &( view_params[1] ) , params , 1 , 0 );
	}
	ZVAL_ZVAL( &( view_params[2] ) , getThis() , 1 , 0 );
	cheetah_call_user_function( NULL , &view , "render" , &content , 3 , view_params );

	// Load layout
	zval retval , layout_params[1];
	ZVAL_ZVAL( &( layout_params[0] ) , &content , 1 , 0 );
	cheetah_call_user_function( NULL , getThis() , "renderContent" , &retval , 1 , layout_params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Controller , renderContent ) {
	zval *content;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z", &content ) == FAILURE ) {
		RETURN_FALSE
	}

	// Get layout file
	zval layout_file;
	cheetah_call_user_function_no_params( NULL , getThis() , "findLayoutFile" , &layout_file );
	// Return content if layout file is not a string or empty.
	if ( Z_TYPE( layout_file ) != IS_STRING || ( Z_STRLEN( layout_file ) <= 0 ) ) {
		RETURN_ZVAL( content , 1 , 0 );
	}

	// Get view object
	zval view;
	cheetah_call_user_function_no_params( NULL , getThis() , "getView" , &view );
	// Call renderFile function
	zval retval , view_params[2] , params;
	array_init_size( &params , 1 );
	add_assoc_zval_ex( &params , "content" , 7 , content );
	ZVAL_STRINGL( &( view_params[0] ) , Z_STRVAL( layout_file ) , Z_STRLEN( layout_file ) );
	ZVAL_ZVAL( &( view_params[1] ) , &params , 1 , 0 );
	cheetah_call_user_function( NULL , &view , "renderFile" , &retval , 2 , view_params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Controller , findLayoutFile ) {
	// Get current controller layout
	zval layout;
	cheetah_read_property( NULL , getThis() , "layout" , 0 , &layout );
	// Return NULL if layout was disabled
	if ( Z_TYPE( layout ) == IS_FALSE ) {
		RETURN_NULL()
	}
	// Find layout file
	char *layout_file = NULL , *module_layout_file = NULL;
	size_t layout_file_len = 0 , module_layout_file_len = 0;
	zval *app , base_path , app_layout , module , module_id , module_layout , view , default_extension;
	app = CHEETAH_G( application );
	//cheetah_read_static_property(cheetah_ce, "app", 0, &app);

	cheetah_read_property( NULL , app , "basePath" , 0 , &base_path );
	cheetah_read_property( NULL , getThis() , "module" , 0 , &module );
	cheetah_read_property( NULL , app , "layout" , 0 , &app_layout );
	if ( Z_TYPE( module ) == IS_OBJECT ) {
		cheetah_read_property( NULL , &module , "layout" , 0 , &module_layout );
		// Return NULL if controller layout is null and module_layout is false
		if ( ( Z_TYPE( layout ) == IS_NULL ) && ( Z_TYPE( module_layout ) == IS_FALSE ) ) {
			RETURN_NULL()
		}
		// Return NULL if layout was disabled by module.
		cheetah_read_property( NULL , &module , "id" , 0 , &module_id );
	}
	cheetah_read_property( NULL , app , "layout" , 0 , &app_layout );
	// Set layout as default value if it is null
	if ( Z_TYPE( layout ) != IS_STRING ) {
		if ( Z_TYPE( module_layout ) == IS_STRING ) {
			ZVAL_STRING( &layout , Z_STRVAL( module_layout ) );
		} else if ( Z_TYPE( app_layout ) == IS_STRING ) {
			ZVAL_STRING( &layout , Z_STRVAL( app_layout ) );
		} else {
			RETURN_NULL();
		}
	}
	cheetah_call_user_function_no_params( NULL , getThis() , "getView" , &view );
	cheetah_read_property( NULL , &view , "defaultExtension" , 0 , &default_extension );
	layout_file_len = layout_file_len + Z_STRLEN( base_path ) + Z_STRLEN( layout ) + Z_STRLEN( default_extension ) + 8;
	layout_file = ( char * ) malloc( layout_file_len );
	strcpy( layout_file , Z_STRVAL( base_path ) );
	strcat( layout_file , DIRECTORY_SEPARATOR );
	if ( Z_TYPE( module ) == IS_OBJECT ) {
		module_layout_file_len = layout_file_len + Z_STRLEN( module_id ) + 7;
		module_layout_file = ( char * ) malloc( module_layout_file_len );
		strcpy( module_layout_file , layout_file );
		free( layout_file );
		if ( Z_TYPE( module_layout ) == IS_STRING ) {
			strcat( module_layout_file , "modules" );
			strcat( module_layout_file , DIRECTORY_SEPARATOR );
			strcat( module_layout_file , Z_STRVAL( module_id ) );
			strcat( module_layout_file , DIRECTORY_SEPARATOR );
		}
		strcat( module_layout_file , "views" );
		strcat( module_layout_file , DIRECTORY_SEPARATOR );
		strcat( module_layout_file , Z_STRVAL( layout ) );
		strcat( module_layout_file , "." );
		strcat( module_layout_file , Z_STRVAL( default_extension ) );
		cheetah_check_file_exist( module_layout_file );
		RETURN_STRING( module_layout_file );
	} else {
		free( module_layout_file );
		strcat( layout_file , "views" );
		strcat( layout_file , DIRECTORY_SEPARATOR );
		strcat( layout_file , Z_STRVAL( layout ) );
		strcat( layout_file , "." );
		strcat( layout_file , Z_STRVAL( default_extension ) );
		cheetah_check_file_exist( layout_file );
		RETURN_STRING( layout_file );
	}
}
ZEND_METHOD( Cheetah_Kernel_Controller , getView ) {
	zval view;
	zend_class_entry *ce;
	ce = Z_OBJCE_P( getThis() );
	cheetah_read_property( ce , getThis() , "_view" , 0 , &view );
	if ( Z_TYPE( view ) == IS_NULL ) {
		zval *app = CHEETAH_G( application );
		//cheetah_read_static_property(cheetah_ce, "app", 0, &app);
		cheetah_call_user_function_no_params( NULL , app , "getView" , &view );
		cheetah_update_property( ce , getThis() , "_view" , &view );
	}
	RETURN_ZVAL( &view , 1 , 0 );
}
