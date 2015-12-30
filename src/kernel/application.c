#define CHEETAH_CLASS_KERNEL_APPLICATION()	\
{	\
	zend_class_entry cheetah_kernel_application_ce_local;	\
	INIT_CLASS_ENTRY(cheetah_kernel_application_ce_local,"Cheetah\\Kernel\\Application", cheetah_kernel_application_methods);	\
	cheetah_kernel_application_ce = zend_register_internal_class_ex(&cheetah_kernel_application_ce_local, cheetah_kernel_module_ce);	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "controllerNamespace",19,"App\\Controllers", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "name",4,"Cheetah Application", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "version",7,PHP_CHEETAH_VERSION, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "charset",7,"UTF-8", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "language",8,"zh-CN", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "requestedRoute",14, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "requestedAction",15, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "requestedParams",15, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "modules",7, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "basePath",8, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_kernel_application_ce, "params",6, ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_string ( cheetah_kernel_application_ce, "layout",6,"layout", ZEND_ACC_PUBLIC TSRMLS_CC );	\
}

PHP_METHOD( Cheetah_Kernel_Application , __construct );
PHP_METHOD( Cheetah_Kernel_Application , preInit );
PHP_METHOD( Cheetah_Kernel_Application , coreComponents );
PHP_METHOD( Cheetah_Kernel_Application , getDb );
PHP_METHOD( Cheetah_Kernel_Application , getRequest );
PHP_METHOD( Cheetah_Kernel_Application , getResponse );
PHP_METHOD( Cheetah_Kernel_Application , getView );
PHP_METHOD( Cheetah_Kernel_Application , setClassMap );
static zend_function_entry cheetah_kernel_application_methods[] = {
	ZEND_ME ( Cheetah_Kernel_Application, __construct,NULL,ZEND_ACC_PUBLIC | ZEND_ACC_CTOR )
	ZEND_ME ( Cheetah_Kernel_Application, preInit,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, coreComponents,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, getDb,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, getRequest,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, getResponse,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, getView,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME ( Cheetah_Kernel_Application, setClassMap,NULL,ZEND_ACC_PUBLIC)
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Application , __construct ) {
	// Create Container instance
	zval container;
	object_init_ex( &container , cheetah_di_container_ce );
	zval retval;
	cheetah_call_user_function_no_params( NULL , &container , "init" , &retval );
	CHEETAH_G (container) = &container;
	// Update static property container of class Cheetah
	zend_update_static_property( cheetah_ce , "container" , 9 , &container );

	// Create Logger instance
	zval logger;
	object_init_ex( &logger , cheetah_log_logger_ce );
	cheetah_call_user_function_no_params( NULL , &logger , "init" , &retval );
	CHEETAH_G (logger) = &logger;
	zend_update_static_property( cheetah_ce , "logger" , 6 , &logger );

	CHEETAH_G (application) = getThis();
	// Update static property app of class Cheetah
	zend_update_static_property( cheetah_ce , "app" , 3 , getThis() );

	zval *config;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &config ) == FAILURE ) {
		RETURN_FALSE
	}

	zval params[1];
	ZVAL_ZVAL( &( params[0] ) , config , 1 , 0 );

	//  Do something before initialize
	zval pre_init_retval;
	cheetah_call_user_function( NULL , getThis() , "preInit" , &pre_init_retval , 1 , params );

	// Initialize the Service Locator private property: $_components and $_definitions as ean empty array.
	cheetah_call_user_function_no_params( NULL , getThis() , "Cheetah\\Di\\ServiceLocator::init" , &retval );

	// Set application properties
	cheetah_call_user_function( NULL , getThis() , "Cheetah\\Kernel\\Component::__construct" , &retval , 1 , params );

	// Set class autoload function
	zval loader , loader_params[3];
	array_init_size( &loader , 2 );
	add_next_index_string( &loader , "Cheetah" );
	add_next_index_string( &loader , "autoload" );
	ZVAL_ZVAL( &( loader_params[0] ) , &loader , 1 , 0 );
	ZVAL_BOOL( &( loader_params[1] ) , TRUE );
	ZVAL_BOOL( &( loader_params[2] ) , TRUE );
	cheetah_call_user_function( NULL , NULL , "spl_autoload_register" , &retval , 3 , loader_params );
}
ZEND_METHOD( Cheetah_Kernel_Application , preInit ) {
	zval *config;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &config ) == FAILURE ) {
		RETURN_FALSE
	}

	zval components;
	if ( zend_hash_str_exists( Z_ARRVAL_P( config ) , "components" , 10 ) ) {
		ZVAL_ZVAL( &components , zend_hash_str_find( Z_ARRVAL_P( config ) , "components" , 10 ) , 1 , 0 );
	}
	if ( Z_TYPE( components ) != IS_ARRAY ) {
		array_init( &components );
	}
	zval core_components , core_components_format;
	array_init( &core_components_format );
	cheetah_call_user_function_no_params( NULL , getThis() , "coreComponents" , &core_components );

	zval *value;
	size_t i , count;
	count = zend_hash_num_elements( Z_ARRVAL( core_components ) );
	zend_hash_internal_pointer_reset( Z_ARRVAL( core_components ) );
	for ( i = 0; i < count ; i++ ) {
		value = zend_hash_get_current_data( Z_ARRVAL( core_components ) );
		zend_string* key;
		zend_ulong idx;
		if ( ( zend_hash_get_current_key( Z_ARRVAL( core_components ) , &key , &idx ) == HASH_KEY_IS_STRING )
				&& ( zend_hash_str_exists( Z_ARRVAL( components ) , key->val , key->len ) != TRUE ) ) {
			zval component;
			array_init( &component );
			add_assoc_zval_ex( &component , "class" , 5 , value );
			add_assoc_zval_ex( &core_components_format , key->val , key->len , &component );
		}
		zend_hash_move_forward( Z_ARRVAL( core_components ) );
	}
	zval retval , params[2];
	ZVAL_ZVAL( &( params[0] ) , &core_components_format , 0 , 1 );
	ZVAL_ZVAL( &( params[1] ) , &components , 1 , 0 );
	cheetah_call_user_function( NULL , NULL , "array_merge" , &retval , 2 , params );
	zend_hash_str_update( Z_ARRVAL_P( config ) , "components" , 10 , &retval );
	RETURN_TRUE
}
ZEND_METHOD( Cheetah_Kernel_Application , coreComponents ) {
	zval components;
	array_init( &components );
	add_assoc_string_ex( &components , "view" , 4 , "Cheetah\\Web\\View" );
	RETURN_ZVAL( &components , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Application , getDb ) {
	zval retval;
	zval params[1];
	ZVAL_STRING( &( params[0] ) , "db" );
	if ( cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params ) == FAILURE ) {
		RETURN_FALSE zval
		retval;
		zval params[1];
		ZVAL_STRING( &( params[0] ) , "view" );
		if ( cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params ) == FAILURE ) {
			RETURN_FALSE
		}
		ZVAL_DESTRUCTOR( params );
		RETURN_ZVAL( &retval , 1 , 0 );
	}
	ZVAL_DESTRUCTOR( params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Application , getRequest ) {
	zval retval;
	zval params[1];
	ZVAL_STRING( &( params[0] ) , "request" );
	if ( cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params ) == FAILURE ) {
		RETURN_FALSE
	}
	ZVAL_DESTRUCTOR( params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Application , getResponse ) {
	zval retval;
	zval params[1];
	ZVAL_STRING( &( params[0] ) , "response" );
	if ( cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params ) == FAILURE ) {
		RETURN_FALSE
	}
	ZVAL_DESTRUCTOR( params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Application , getView ) {
	zval retval;
	zval params[1];
	ZVAL_STRING( &( params[0] ) , "view" );
	if ( cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params ) == FAILURE ) {
		RETURN_FALSE
	}
	ZVAL_DESTRUCTOR( params );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Application , setClassMap ) {
	zval *class_map;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &class_map ) == FAILURE ) {
		RETURN_FALSE
	}
	cheetah_update_static_property( cheetah_ce , "classMap" , class_map );
}
