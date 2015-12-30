#define CHEETAH_CLASS_KERNEL_OBJECT()	\
{	\
	zend_class_entry cheetah_kernel_object_ce_local;	\
	INIT_CLASS_ENTRY(cheetah_kernel_object_ce_local, "Cheetah\\Kernel\\Object",cheetah_kernel_object_methods);	\
	cheetah_kernel_object_ce = zend_register_internal_class_ex(&cheetah_kernel_object_ce_local TSRMLS_CC, NULL);	\
}

PHP_METHOD( Cheetah_Kernel_Object , className );
PHP_METHOD( Cheetah_Kernel_Object , __construct );
PHP_METHOD( Cheetah_Kernel_Object , init );
PHP_METHOD( Cheetah_Kernel_Object , __get );
PHP_METHOD( Cheetah_Kernel_Object , __set );
static zend_function_entry cheetah_kernel_object_methods[] = {
	ZEND_ME ( Cheetah_Kernel_Object, className, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah_Kernel_Object, __construct,NULL,ZEND_ACC_PUBLIC | ZEND_ACC_CTOR )
	ZEND_ME ( Cheetah_Kernel_Object, init, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Kernel_Object, __get, arginfo_magic_func_get,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Kernel_Object, __set, arginfo_magic_func_set,ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Kernel_Object , className ) {
	zval retval;
	if ( cheetah_call_user_function_no_params( EG( function_table ) , NULL , "get_called_class" , &retval )
			== SUCCESS ) {
		RETURN_ZVAL( &retval , 0 , 1 );
	} else {
		RETURN_FALSE
	}
}
ZEND_METHOD( Cheetah_Kernel_Object , __construct ) {
	// Invoke init function
	zval retval;
	if ( cheetah_call_user_function_no_params( NULL , getThis() , "init" , &retval ) == FAILURE ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Failed to invoke function init()." );
		return;
	}

	zval* config;
	if ( ZEND_NUM_ARGS() == 1 ) {
		if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a", &config ) == FAILURE ) {
			RETURN_FALSE
		}

		// Configure $this object's properties if $config is not empty.
		if ( zend_hash_num_elements( Z_ARRVAL_P( config ) ) > 0 ) {
			zval config_retval;
			zval config_params[2];
			ZVAL_ZVAL( &( config_params[0] ) , getThis() , 0 , 1 );
			ZVAL_ZVAL( &( config_params[1] ) , config , 0 , 1 );
			if ( cheetah_call_user_function( NULL , NULL , "Cheetah::configure" , &config_retval , 2 , config_params )
					== FAILURE ) {
				zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
						"Failed to configure the object's properties." );
				return;
			}
		}
	} else if ( ZEND_NUM_ARGS() > 1 ) {
		WRONG_PARAM_COUNT
	}

	RETURN_ZVAL( getThis() , 1 , 0 );
}
ZEND_METHOD( Cheetah_Kernel_Object , init ) {
	RETURN_TRUE
}
ZEND_METHOD( Cheetah_Kernel_Object , __get ) {
	char *name;
	size_t name_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE ) {
		WRONG_PARAM_COUNT
	}

	// Invoke getter function
	char *getter = NULL;
	getter = ( char * ) malloc( name_len + 4 );
	strcpy( getter , "get" );
	strcat( getter , name );
	getter[3] = toupper( getter[3] );

	zval retval;
	if ( cheetah_call_user_function_no_params( NULL , getThis() , getter , &retval ) == SUCCESS ) {
		RETURN_ZVAL( &retval , 1 , 0 )
	} else {
		//  Check whether the setter fucntion exists
		char *setter = NULL;
		setter = ( char * ) malloc( name_len + 4 );
		strcpy( setter , "set" );
		strcat( setter , name );
		setter[3] = toupper( setter[3] );

		if ( cheetah_method_exists( getThis() , setter ) ) {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Getting write-only property: %s::$%s" ,
					cheetah_get_object_class_name( getThis() ) , name );
			return;
		} else {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Getting unknown property: %s::$%s" ,
					cheetah_get_object_class_name( getThis() ) , name );
			return;
		}
	}
}
ZEND_METHOD( Cheetah_Kernel_Object , __set ) {
	zval* value;
	char* name;
	size_t name_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &name_len,
			&value ) == FAILURE ) {
		WRONG_PARAM_COUNT
	}

	// Invoke setter function
	char *setter = NULL;
	setter = ( char * ) malloc( name_len + 4 );
	strcpy( setter , "set" );
	strcat( setter , name );
	setter[3] = toupper( setter[3] );

	zval retval , setter_params[1];
	ZVAL_ZVAL( &( setter_params[0] ) , value , 1 , 0 );
	if ( cheetah_call_user_function( NULL , getThis() , setter , &retval , 1 , setter_params ) == SUCCESS ) {
		RETURN_ZVAL( &retval , 1 , 0 )
	} else {
		//  Check whether the getter fucntion exists
		char *getter = NULL;
		getter = ( char * ) malloc( name_len + 4 );
		strcpy( getter , "get" );
		strcat( getter , name );
		getter[3] = toupper( getter[3] );

		if ( cheetah_method_exists( getThis() , getter ) ) {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Setting read-only property: %s::$%s" ,
					cheetah_get_object_class_name( getThis() ) , name );
			return;
		} else {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Setting unknown property: %s::$%s" ,
					cheetah_get_object_class_name( getThis() ) , name );
			return;
		}
	}
}
