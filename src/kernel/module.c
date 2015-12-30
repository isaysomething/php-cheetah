#define CHEETAH_CLASS_KERNEL_MODULE()	\
{	\
	zend_class_entry cheetah_kernel_module_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_kernel_module_ce_local, "Cheetah\\Kernel\\Module",cheetah_kernel_module_methods );	\
	cheetah_kernel_module_ce = zend_register_internal_class_ex (&cheetah_kernel_module_ce_local, cheetah_di_service_locator_ce );	\
	zend_declare_property_string ( cheetah_kernel_module_ce, "defaultRoute",strlen ( "defaultRoute" ),"default", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "params",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "id",2, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "module",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "namespaces",10, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "controllerNamespace",19, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "viewPath",8, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_module_ce, "layout",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
}

PHP_METHOD( Cheetah_Kernel_Module , init );
PHP_METHOD( Cheetah_Kernel_Module , beforeAction );
PHP_METHOD( Cheetah_Kernel_Module , runAction );
PHP_METHOD( Cheetah_Kernel_Module , afterAction );
PHP_METHOD( Cheetah_Kernel_Module , handle );
PHP_METHOD( Cheetah_Kernel_Module , registerNamespaces );
PHP_METHOD( Cheetah_Kernel_Module , getViewPath );
PHP_METHOD( Cheetah_Kernel_Module , setViewPath );
static zend_function_entry cheetah_kernel_module_methods[] = {
	ZEND_ME(Cheetah_Kernel_Module, init, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, beforeAction, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, runAction, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, afterAction, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, handle, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, registerNamespaces, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, getViewPath, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Kernel_Module, setViewPath, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Module , init ) {
	zval retval;
	cheetah_call_user_function_no_params( NULL , getThis() , "registerNamespaces" , &retval );
}
ZEND_METHOD( Cheetah_Kernel_Module , beforeAction ) {
}
ZEND_METHOD( Cheetah_Kernel_Module , runAction ) {
	zval *route;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &route ) == FAILURE ) {
		RETURN_FALSE
	}
	zval retval , params[1];
	ZVAL_ZVAL( &( params[0] ) , route , 1 , 0 );
	cheetah_call_user_function( NULL , getThis() , "handle" , &retval , 1 , params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Module , afterAction ) {
}
ZEND_METHOD( Cheetah_Kernel_Module , handle ) {
	zval *route;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &route ) == FAILURE ) {
		RETURN_FALSE
	}

	char *module_name = NULL; // module name
	char *controller_namespace = NULL; // controller namespace
	char *controller_name = NULL; // controller name
	char *action_name = NULL; // action name
	size_t action_params_count_max = 0; // action params max count
	zval module; // The instance of module
	if ( zend_array_count( Z_ARRVAL_P( route ) ) > 2 ) {
		action_params_count_max = zend_array_count( Z_ARRVAL_P( route ) ) - 2;
	}
	zval action_params[action_params_count_max];
	//array_init_size(action_params, action_params_count_max);

	size_t action_params_index = 0;
	zval z_controller_namespace , z_default_route;
	cheetah_read_property( Z_OBJCE_P( getThis() ) , getThis() , "controllerNamespace" , 0 , &z_controller_namespace );
	cheetah_read_property( Z_OBJCE_P( getThis() ) , getThis() , "defaultRoute" , 0 , &z_default_route );
	if ( zend_array_count( Z_ARRVAL_P( route ) ) == 0 ) {
		// Use the controller namespace and name if route is empty.
		controller_namespace = ( char * ) malloc( Z_STRLEN( z_controller_namespace ) + 1 );
		strcpy( controller_namespace , Z_STRVAL( z_controller_namespace ) );
		controller_name = ( char * ) malloc( Z_STRLEN( z_default_route ) + 1 );
		strcpy( controller_name , Z_STRVAL( z_default_route ) );
	} else {
		size_t i , route_count;
		BOOL get_module = TRUE , get_controller = TRUE , get_action = TRUE; // flag
		zval route_value;
		HashPosition* pos;
		route_count = zend_hash_num_elements( Z_ARRVAL_P( route ) );
		zend_hash_internal_pointer_reset( Z_ARRVAL_P( route ) );
		zval modules; // Get application loaded modules.
		cheetah_read_property( NULL , getThis() , "modules" , 0 , &modules );
		for ( i = 0; i < route_count ; i++ ) {
			ZVAL_ZVAL( &route_value , zend_hash_get_current_data( Z_ARRVAL_P( route ) ) , 0 , 1 );
			if ( get_module == TRUE ) { // Get Module
				// Check whether the specified module exist.
				if ( ( Z_TYPE( modules ) == IS_ARRAY )
						&& ( zend_hash_str_exists( Z_ARRVAL( modules ) , Z_STRVAL( route_value ) ,
								Z_STRLEN( route_value ) ) ) ) {
					// set module name
					module_name = ( char * ) malloc( Z_STRLEN( route_value ) + 1 );
					strcpy( module_name , Z_STRVAL( route_value ) );
					zval module_array;
					ZVAL_ZVAL( &module_array ,
							zend_hash_str_find( Z_ARRVAL( modules ) , module_name , strlen( module_name ) ) , 1 , 0 );
					// Create module object.
					zval module_params[1];
					// Throw exception if "class" element does not exist.
					if ( zend_hash_str_exists( Z_ARRVAL( module_array ) , "class" , 5 ) == IS_FALSE ) {
						zend_throw_exception_ex( cheetah_kernel_invalid_param_exception_ce , 0 ,
								"Module configure must be an array containing a \"class\" element." );
						return;
					}
					char *module_class = Z_STRVAL_P( zend_hash_str_find( Z_ARRVAL( module_array ) , "class" , 5 ) );
					ZVAL_STRING( &( module_params[0] ) , module_class );
					cheetah_call_user_function( NULL , NULL , "Cheetah::createObject" , &module , 1 , module_params );
					if ( Z_TYPE( module ) != IS_OBJECT ) {
						zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Failed to cretae module object." );
						return;
						RETURN_NULL()
					}

					if ( instanceof_class( Z_OBJCE( module ) , cheetah_kernel_module_ce ) != TRUE ) {
						zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
								"The module is not an instance of Cheetah\\Kernel\\Module or it's subclass." );
						return;
						RETURN_NULL()
					}

					cheetah_update_property_string( Z_OBJCE( module ) , &module , "id" , module_name ); // Set module id
					// set controller namespace
					controller_namespace = ( char * ) malloc( strlen( module_class ) + 13 );
					strcpy( controller_namespace , module_class );
					strcat( controller_namespace , "\\Controllers" );
				} else {
					controller_namespace = ( char * ) malloc( Z_STRLEN( z_controller_namespace ) + 1 );
					strcpy( controller_namespace , Z_STRVAL( z_controller_namespace ) );
					controller_name = ( char * ) malloc( Z_STRLEN( route_value ) + 1 );
					strcpy( controller_name , Z_STRVAL( route_value ) );
					get_controller = FALSE;
				}
				get_module = FALSE;
			} else if ( get_controller == TRUE ) { // Get controller
				controller_name = ( char * ) malloc( Z_STRLEN( route_value ) + 1 );
				strcpy( controller_name , Z_STRVAL( route_value ) );
				get_controller = FALSE;
			} else if ( get_action == TRUE ) { // Get action
				action_name = ( char * ) malloc( Z_STRLEN( route_value ) + 1 );
				strcpy( action_name , Z_STRVAL( route_value ) );
				get_action = FALSE;
			} else { // Get action params
				ZVAL_ZVAL( &( action_params[action_params_index] ) , &route_value , 1 , 0 );
				action_params_index++;
			}
			zend_hash_move_forward( Z_ARRVAL_P( route ) );
		}
		// Set controller name as default if it is empty
		if ( ( module_name != NULL ) && ( controller_name == NULL ) ) {
			zval default_controller;
			cheetah_read_property( Z_OBJCE( module ) , &module , "defaultRoute" , 0 , &default_controller );
			controller_name = ( char * ) malloc( Z_STRLEN( default_controller ) + 1 );
			strcpy( controller_name , Z_STRVAL( default_controller ) );
		}
	}

	char *controller_class = NULL; // The controller class name with namespace
	size_t controller_spacename_len = strlen( controller_namespace );
	controller_class = ( char * ) malloc( controller_spacename_len + strlen( controller_name ) + 12 );
	strcpy( controller_class , controller_namespace );
	strcat( controller_class , "\\" );
	strcat( controller_class , controller_name );
	strcat( controller_class , "Controller" );
	controller_class[controller_spacename_len + 1] = toupper( controller_class[controller_spacename_len + 1] );

	//php_printf( "namespace：%s、controller：%s、action：%s、class：%s\n" , controller_namespace , controller_name ,action_name , controller_class );

	// Create controller object
	zval controller;
	zval controller_params[1];
	ZVAL_STRING( &( controller_params[0] ) , controller_class );
	cheetah_call_user_function( NULL , NULL , "Cheetah::createObject" , &controller , 1 , controller_params );
	zend_update_property( NULL , getThis() , "controller" , 10 , &controller );
	if ( Z_TYPE( controller ) != IS_OBJECT ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Failed to create controller object." );
		return;
	}
	if ( instanceof_class( Z_OBJCE( controller ) , cheetah_kernel_controller_ce ) != TRUE ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The controller is not an instance of Cheetah\\Kernel\\Controller or it's subclass." );
		return;
	}
	cheetah_update_property( Z_OBJCE_P( getThis() ) , getThis() , "controller" , &controller ); // Update controller property of Application.
	cheetah_update_property_string( Z_OBJCE( controller ) , &controller , "id" , controller_name ); // Update id property of controller
	if ( module_name != NULL ) {
		cheetah_update_property( Z_OBJCE( controller ) , &controller , "module" , &module ); // Update module property of controller
	}

	// Run controller action
	zval retval;
	uint32_t action_params_count = ( action_params_index > 0 ) ? action_params_index : 0; // action params count
	if ( action_name == NULL ) {
		// Set action as default value if it is empty
		zval default_action;
		cheetah_read_property( Z_OBJCE( controller ) , &controller , "defaultAction" , 0 , &default_action );
		action_name = ( char * ) malloc( Z_STRLEN( default_action ) + 1 );
		strcpy( action_name , Z_STRVAL( default_action ) );
	}
	cheetah_update_property_string( Z_OBJCE( controller ) , &controller , "action" , action_name ); // update action property of controller

	// Run beforeAction()
	zval action_event_ret , action_event_params[1];
	ZVAL_STRING( &( action_event_params[0] ) , action_name );
	// Run beforeAction() of Module if it is not null.
	if ( module_name != NULL ) {
		cheetah_call_user_function( NULL , &module , "beforeAction" , &action_event_ret , 1 , action_event_params );
	}
	// Run beforeAction() of Controller
	cheetah_call_user_function( NULL , &controller , "beforeAction" , &action_event_ret , 1 , action_event_params );

	char *action = NULL;
	action = ( char * ) malloc( strlen( action_name ) + 1 + strlen( "action" ) );
	strcpy( action , "action" );
	action_name[0] = toupper( action_name[0] );
	strcat( action , action_name );
	if ( cheetah_call_user_function( NULL , &controller , action , &retval , action_params_count , action_params )
			== FAILURE && Z_TYPE( retval ) == IS_UNDEF ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"Failed to run action,because the action named \"%s\" of \"%s\" does not exist." , action ,
				controller_class );
		return;
	}
	// Run afterAction() of Module if it is not null.
	if ( module_name != NULL ) {
		cheetah_call_user_function( NULL , &module , "afterAction" , &action_event_ret , 1 , action_event_params );
	}
	// Run afterAction() of Controller
	cheetah_call_user_function( NULL , &controller , "afterAction" , &action_event_ret , 1 , action_event_params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Module , registerNamespaces ) {
	zval namespaces;
	cheetah_read_property( NULL , getThis() , "namespaces" , 0 , &namespaces );
	if ( Z_TYPE( namespaces ) != IS_ARRAY ) {
		RETURN_FALSE;
	}
	if ( zend_array_count( Z_ARRVAL( namespaces ) ) == 0 ) {
		RETURN_TRUE;
	}

	// 读取Cheetah::$classMap
	zval classmap;
	cheetah_read_static_property( cheetah_ce , "classMap" , 0 , &classmap );

	size_t i , count;
	zval namespace;
	HashPosition* pos;
	count = zend_hash_num_elements( Z_ARRVAL( namespaces ) );
	zend_hash_internal_pointer_reset( Z_ARRVAL( namespaces ) );
	for ( i = 0; i < count ; i++ ) {
		ZVAL_ZVAL( &namespace , zend_hash_get_current_data( Z_ARRVAL( namespaces ) ) , 0 , 1 );
		zend_string* key;
		zend_ulong idx;
		if ( zend_hash_get_current_key( Z_ARRVAL( namespaces ) , &key , &idx ) == HASH_KEY_IS_STRING ) {
			if ( zend_hash_str_exists( Z_ARRVAL( classmap ) , key->val , key->len ) ) {
				zend_hash_str_update( Z_ARRVAL( classmap ) , key->val , key->len , &namespace );
			} else {
				add_assoc_zval_ex( &classmap , key->val , key->len , &namespace );
			}
		} else {
			zend_throw_exception_ex( cheetah_kernel_invalid_param_exception_ce , 0 ,
					"The key of namespace must be a String." );
			return;
		}
		zend_hash_move_forward( Z_ARRVAL( namespaces ) );
	}
	// Update property Cheetah::$classMap
	zend_update_static_property( cheetah_ce , "classMap" , 8 , &classmap );
	RETURN_TRUE;
}
ZEND_METHOD( Cheetah_Kernel_Module , getViewPath ) {
	zval view_path;
	cheetah_read_property( NULL , getThis() , "_viewPath" , 0 , &view_path );
	if ( Z_TYPE( view_path ) == IS_NULL ) {
		zval base_path;
		cheetah_read_property( NULL , getThis() , "basePath" , 0 , &base_path );
		cheetah_update_property( NULL , getThis() , "_viewPath" , &view_path );
	}
	RETURN_ZVAL( &view_path , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Module , setViewPath ) {
	zval *view_path;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &view_path ) == FAILURE ) {
		RETURN_FALSE
	}
	cheetah_update_property( NULL , getThis() , "_viewPath" , view_path );
}
