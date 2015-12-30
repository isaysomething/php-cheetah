#define CHEETAH_CLASS_CHEETAH()	\
	{	\
		zend_class_entry cheetah_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_ce_local, "Cheetah", cheetah_methods );	\
		cheetah_ce = zend_register_internal_class_ex ( &cheetah_ce_local, NULL );	\
		zend_declare_property_null ( cheetah_ce, "app",3, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_ce, "container",9, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_ce, "classMap",8, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_ce, "logger",6, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC TSRMLS_CC );	\
	}

PHP_METHOD( Cheetah , getVersion );
PHP_METHOD( Cheetah , configure );
PHP_METHOD( Cheetah , createObject );
PHP_METHOD( Cheetah , autoload );
PHP_METHOD( Cheetah , trace );
PHP_METHOD( Cheetah , error );
PHP_METHOD( Cheetah , warning );
PHP_METHOD( Cheetah , info );
static zend_function_entry cheetah_methods[] = {
	ZEND_ME ( Cheetah, getVersion, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, configure, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, createObject, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, autoload, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, trace, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, error, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, warning, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	ZEND_ME ( Cheetah, info, NULL,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah , getVersion ) {
	RETURN_STRING (PHP_CHEETAH_VERSION)
}
ZEND_METHOD( Cheetah , configure ) {
	zval* object;
	zval* properties;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "oa",&object, &properties ) == FAILURE ) {
		RETURN_FALSE
	}
	uint32_t properties_count;
	properties_count = zend_hash_num_elements( Z_ARRVAL_P( properties ) ); // The size of properties
	if ( properties_count == 0 ) {
		RETURN_TRUE
	}
	zval* property_value;
	zend_hash_internal_pointer_reset( Z_ARRVAL_P( properties ) );
	int i;
	for ( i = 0; i < properties_count ; i++ ) {
		zend_string* key;
		zend_ulong idx;
		property_value = zend_hash_get_current_data( Z_ARRVAL_P( properties ) );
		if ( zend_hash_get_current_key( Z_ARRVAL_P( properties ) , &key , &idx ) == HASH_KEY_IS_STRING ) {
			zend_update_property( Z_OBJCE_P( object ) , object , key->val , key->len , property_value );
		} else {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "The property key must be a String." );
			return;
			RETURN_NULL()
		}
		zend_hash_move_forward( Z_ARRVAL_P( properties ) );
	}
	RETURN_ZVAL( object , 1 , 0 )
}
ZEND_METHOD( Cheetah , createObject ) {
	zval* type;
	zval* params;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z|z",&type, &params ) == FAILURE ) {
		RETURN_FALSE
	}
	zval retval;
	zval container;
	cheetah_read_static_property( cheetah_ce , "container" , 0 , &container );
	if ( Z_TYPE_P( type ) == IS_STRING ) {
		zval get_params[2];
		ZVAL_ZVAL( &( get_params[0] ) , type , 0 , 1 );
		array_init( &( get_params[1] ) );
		cheetah_call_user_function( NULL , &container , "get" , &retval , 2 , get_params );
	} else if ( Z_TYPE_P( type ) == IS_ARRAY ) {
		// 判断是否包含class
		if ( zend_hash_str_exists( Z_ARRVAL_P( type ) , "class" , 5 ) != TRUE ) {
			// 不存在Class，抛出异常
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
					"Object configuration must be an array containing a \"class\" element." );
			return;
			RETURN_NULL()
		}
		zval z_class;
		ZVAL_ZVAL( &z_class , zend_hash_str_find( Z_ARRVAL_P( type ) , "class" , 5 ) , 0 , 1 );
		if ( Z_TYPE_P( &z_class ) != IS_STRING ) {
			RETURN_FALSE
		}
		zval get_params[2];
		ZVAL_STRING( &( get_params[0] ) , zval_get_string( &z_class )->val );
		// Delete "class" element from $type
		zend_hash_str_del( Z_ARRVAL_P( type ) , "class" , 5 );
		ZVAL_ZVAL( &( get_params[1] ) , type , 0 , 0 );
		if ( cheetah_call_user_function( NULL , &container , "get" , &retval , 2 , get_params ) == FAILURE ) {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
					"Failed to create object when invoke get of container." );
			return;
		}
	} else {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Unsupported configuration type." );
		return;
	}
	if ( Z_TYPE( retval ) == IS_OBJECT ) {
		RETURN_ZVAL( &retval , 0 , 1 )
	}
	zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Failed to create object." );
	return;
}
ZEND_METHOD( Cheetah , autoload ) {
	char *class;
	size_t class_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s",&class, &class_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval class_map;
	cheetah_read_static_property( cheetah_ce , "classMap" , 0 , &class_map );
	// Throw exception if classMap is not an array or it is empty.
	if ( ( Z_TYPE( class_map ) != IS_ARRAY ) || ( zend_hash_num_elements( Z_ARRVAL( class_map ) ) == 0 ) ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The Cheetah::$classMap is empty, cannot autoload the class named %s." , class );
		return;
	}
	size_t pos = strlen( rindex( class , '\\' ) );
	size_t namespace_len = class_len - pos;
	char namespace[201] = "";
	if ( namespace_len >= 200 ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The namespace length must be less than 200 characters." );
		return;
	}
	strncpy( namespace , class , namespace_len );
	char class_name[101] = "";
	size_t class_name_len = pos - 1;
	if ( class_name_len >= 100 ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The class name length must be less than 100 characters." );
		return;
	}
	strncpy( class_name , class + namespace_len + 1 , class_name_len );
	if ( zend_hash_str_exists( Z_ARRVAL( class_map ) , namespace , strlen( namespace ) ) != TRUE ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The class namespace has not yet been loaded or not found: %s\\%s." , namespace , class_name );
		return;
	}
	zval class_path;
	ZVAL_ZVAL( &class_path , zend_hash_str_find( Z_ARRVAL( class_map ) , namespace , strlen( namespace ) ) , 1 , 0 );
	size_t class_file_len = strlen( Z_STRVAL( class_path ) ) + 1 + strlen( class_name ) + 4;
	char class_file[class_file_len];
	strcpy( class_file , Z_STRVAL( class_path ) );
	strcat( class_file , DIRECTORY_SEPARATOR );
	strcat( class_file , class_name );
	strcat( class_file , ".php" );
// Check whether there exists the source file of class
	if ( access( class_file , 0 ) == -1 ) {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"The class named \"%s\" does not exist which should be declared in %s." , class , class_file );
		return;
	}

	if ( cheetah_include( class_file ) == FAILURE ) {
		RETURN_NULL()
	}

	RETURN_TRUE;
}
ZEND_METHOD( Cheetah , trace ) {
	zval *message;
	char *category;
	size_t category_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z|s",&message, &category,&category_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval logger;
	cheetah_read_static_property( cheetah_ce , "logger" , 0 , &logger );
	zval retval , params[3];
	if ( ZEND_NUM_ARGS() == 1 ) {
		category = "application";
	}
	ZVAL_STRING( &( params[0] ) , category );
	zval *level;
	level = cheetah_get_class_constant( "Cheetah\\Log\\Logger::LEVEL_TRACE" , NULL , 0 );
	ZVAL_ZVAL( &( params[0] ) , message , 1 , 0 );
	ZVAL_LONG( &( params[1] ) , zval_get_long( level ) );
	ZVAL_STRING( &( params[2] ) , category );
	cheetah_call_user_function( NULL , &logger , "log" , &retval , 3 , params );
}
ZEND_METHOD( Cheetah , error ) {
	zval *message;
	char *category;
	size_t category_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z|s",&message, &category,&category_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval logger;
	cheetah_read_static_property( cheetah_ce , "logger" , 0 , &logger );
	zval retval , params[3];
	if ( ZEND_NUM_ARGS() == 1 ) {
		category = "application";
	}
	ZVAL_STRING( &( params[0] ) , category );
	zval *level;
	level = cheetah_get_class_constant( "Cheetah\\Log\\Logger::LEVEL_ERROR" , NULL , 0 );
	ZVAL_ZVAL( &( params[0] ) , message , 1 , 0 );
	ZVAL_LONG( &( params[1] ) , zval_get_long( level ) );
	ZVAL_STRING( &( params[2] ) , category );
	cheetah_call_user_function( NULL , &logger , "log" , &retval , 3 , params );
}
ZEND_METHOD( Cheetah , warning ) {
	zval *message;
	char *category;
	size_t category_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z|s",&message, &category,&category_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval logger;
	cheetah_read_static_property( cheetah_ce , "logger" , 0 , &logger );
	zval retval , params[3];
	if ( ZEND_NUM_ARGS() == 1 ) {
		category = "application";
	}
	ZVAL_STRING( &( params[0] ) , category );
	zval *level;
	level = cheetah_get_class_constant( "Cheetah\\Log\\Logger::LEVEL_WARNING" , NULL , 0 );
	ZVAL_ZVAL( &( params[0] ) , message , 1 , 0 );
	ZVAL_LONG( &( params[1] ) , zval_get_long( level ) );
	ZVAL_STRING( &( params[2] ) , category );
	cheetah_call_user_function( NULL , &logger , "log" , &retval , 3 , params );
}
ZEND_METHOD( Cheetah , info ) {
	zval *message;
	char *category;
	size_t category_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z|s",&message, &category,&category_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval logger;
	cheetah_read_static_property( cheetah_ce , "logger" , 0 , &logger );
	zval retval , params[3];
	if ( ZEND_NUM_ARGS() == 1 ) {
		category = "application";
	}
	ZVAL_STRING( &( params[0] ) , category );
	zval *level;
	level = cheetah_get_class_constant( "Cheetah\\Log\\Logger::LEVEL_INFO" , NULL , 0 );
	ZVAL_ZVAL( &( params[0] ) , message , 1 , 0 );
	ZVAL_LONG( &( params[1] ) , zval_get_long( level ) );
	ZVAL_STRING( &( params[2] ) , category );
	cheetah_call_user_function( NULL , &logger , "log" , &retval , 3 , params );
}
