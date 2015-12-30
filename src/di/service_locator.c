#define CHEETAH_CLASS_DI_SERVICE_LOCATOR()	\
{	\
	zend_class_entry cheetah_di_service_locator_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_di_service_locator_ce_local, "Cheetah\\Di\\ServiceLocator", cheetah_di_service_locator_methods );	\
	cheetah_di_service_locator_ce = zend_register_internal_class_ex ( &cheetah_di_service_locator_ce_local, cheetah_kernel_component_ce );	\
	zend_declare_property_null ( cheetah_di_service_locator_ce, "_components",11, ZEND_ACC_PRIVATE TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_di_service_locator_ce, "_definitions",12, ZEND_ACC_PRIVATE TSRMLS_CC );	\
}

PHP_METHOD( Cheetah_Di_ServiceLocator , __get );
PHP_METHOD( Cheetah_Di_ServiceLocator , init );
PHP_METHOD( Cheetah_Di_ServiceLocator , has );
PHP_METHOD( Cheetah_Di_ServiceLocator , get );
PHP_METHOD( Cheetah_Di_ServiceLocator , set );
PHP_METHOD( Cheetah_Di_ServiceLocator , clear );
PHP_METHOD( Cheetah_Di_ServiceLocator , getComponents );
PHP_METHOD( Cheetah_Di_ServiceLocator , setComponents );
static zend_function_entry cheetah_di_service_locator_methods[] = {
	ZEND_ME ( Cheetah_Di_ServiceLocator, init,NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, __get, arginfo_magic_func_get,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, get, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, has, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, set, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, clear, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, getComponents, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_ServiceLocator, setComponents, NULL,ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Di_ServiceLocator , init ) {
	zval _components , _definitions;
	array_init_size( &_components , 0 );
	array_init_size( &_definitions , 0 );
	zend_update_property( cheetah_di_service_locator_ce , getThis() , "_components" , 11 , &_components );
	zend_update_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 12 , &_definitions );
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , __get ) {
	char *id;
	size_t id_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &id,&id_len ) == FAILURE ) {
		RETURN_FALSE
	}
	// Invoke get function of $this
	zval retval , params[1];
	ZVAL_STRINGL( &( params[0] ) , id , id_len );
	cheetah_call_user_function( NULL , getThis() , "has" , &retval , 1 , params );
	if ( Z_TYPE( retval ) == IS_TRUE ) {
		cheetah_call_user_function( NULL , getThis() , "get" , &retval , 1 , params );
		RETURN_ZVAL( &retval , 0 , 1 );
	}
	cheetah_call_user_function( NULL , getThis() , "Cheetah\\Kernel\\Component::__get" , &retval , 1 , params );
	RETURN_ZVAL( &retval , 0 , 1 );
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , has ) {
	char *id;
	size_t id_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &id,&id_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval _components , _definitions;
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_components" , 0 , &_components );
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 0 , &_definitions );

	if ( zend_hash_str_exists( Z_ARRVAL( _components ) , id , id_len )
			|| zend_hash_str_exists( Z_ARRVAL( _definitions ) , id , id_len ) ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , get ) {
	char *id;
	size_t id_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &id,&id_len) == FAILURE ) {
		RETURN_FALSE
	}
	zval _components , _definitions;
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_components" , 0 , &_components );
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 0 , &_definitions );
	// Return the object if it was found in $_components
	if ( zend_hash_str_exists( Z_ARRVAL( _components ) , id , id_len ) ) {
		RETURN_ZVAL( zend_hash_str_find( Z_ARRVAL( _components ) , id , id_len ) , 1 , 0 );
	}
	if ( zend_hash_str_exists( Z_ARRVAL( _definitions ) , id , id_len ) ) {
		zval definition;
		ZVAL_ZVAL( &definition , zend_hash_str_find( Z_ARRVAL( _definitions ) , id , id_len ) , 0 , 1 );
		if ( ( Z_TYPE_P( &definition ) == IS_OBJECT )
				&& ( instanceof_function( Z_CE( definition ) , zend_ce_closure ) == FALSE ) ) {
			// Return the object if it found in $_definitions and it is not closure.
			zend_hash_str_add( Z_ARRVAL( _components ) , id , id_len , &definition );
			zend_update_property( cheetah_di_service_locator_ce , getThis() , "_components" , 11 , &_components );
			RETURN_ZVAL( &definition , 0 , 1 );
		} else {
			// Create object
			zval params[1];
			ZVAL_ZVAL( &( params[0] ) , &definition , 0 , 1 );
			zval retval;
			if ( cheetah_call_user_function( EG( function_table ) , NULL , "Cheetah::createObject" , &retval , 1 ,
					params ) == FAILURE ) {
				RETURN_FALSE
			}
			if ( Z_TYPE_P( &retval ) == IS_FALSE ) {
				RETURN_FALSE
			}
			zend_hash_str_add( Z_ARRVAL( _components ) , id , id_len , &retval );
			zend_update_property( cheetah_di_service_locator_ce , getThis() , "_components" , 11 , &_components );
			RETURN_ZVAL( &retval , 1 , 0 );
		}
	} else {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Unknown component ID: %s" , id );
		return;
	}

	RETURN_NULL()
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , set ) {
	zval* definition;
	char *id;
	size_t id_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "sz", &id,&id_len,&definition ) == FAILURE ) {
		RETURN_FALSE
	}

	// Read $_components and $_definitions
	zval _components , _definitions;
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_components" , 0 , &_components );
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 0 , &_definitions );

	// Delete the component if it is null.
	if ( ZVAL_IS_NULL( definition ) ) {
		if ( zend_hash_str_exists( Z_ARRVAL( _components ) , id , id_len ) ) {
			zend_hash_str_del_ind( Z_ARRVAL( _components ) , id , id_len );
		}
		if ( zend_hash_str_exists( Z_ARRVAL( _definitions ) , id , id_len ) ) {
			zend_hash_str_del_ind( Z_ARRVAL( _definitions ) , id , id_len );
		}
	} else if ( ( Z_TYPE_P( definition ) == IS_OBJECT ) || ( Z_TYPE_P( definition ) == IS_CALLABLE ) ) {
		zend_hash_str_add( Z_ARRVAL( _components ) , id , id_len , definition );
	} else if ( Z_TYPE_P( definition ) == IS_ARRAY ) {
		if ( zend_hash_str_exists( Z_ARRVAL_P( definition ) , "class" , 5 ) ) {
			zend_hash_str_add( Z_ARRVAL( _definitions ) , id , id_len , definition );
		} else {
			zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
					"The configuration for the %s component must contain a \"class\" element." , id );
			return;
		}
	} else {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
				"Unexpected configuration type for the \"%s\"  component.Component configuration only supports the following types: array,object or callable." ,
				id );
		return;
	}

	// Update $_components and $_definitions
	zend_update_property( cheetah_di_service_locator_ce , getThis() , "_components" , 11 , &_components );
	zend_update_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 12 , &_definitions );
	RETURN_TRUE;
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , clear ) {
	char *id;
	size_t id_len;

	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "s", &id,&id_len ) == FAILURE ) {
		RETURN_FALSE
	}

	zval _components , _definitions;
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_components" , 0 , &_components );
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , 0 , &_definitions );
	if ( zend_hash_str_exists( Z_ARRVAL( _components ) , id , id_len ) ) {
		if ( zend_hash_str_del( Z_ARRVAL( _components ) , id , id_len ) == FAILURE ) {
			RETURN_FALSE
		}
	}
	if ( zend_hash_str_exists( Z_ARRVAL( _definitions ) , id , id_len ) ) {
		if ( zend_hash_str_del( Z_ARRVAL( _definitions ) , id , id_len ) == FAILURE ) {
			RETURN_FALSE
		}
	}
	// Update $_components and $_definitions
	cheetah_update_property( cheetah_di_service_locator_ce , getThis() , "_components" , &_components );
	cheetah_update_property( cheetah_di_service_locator_ce , getThis() , "_definitions" , &_definitions );
	RETURN_TRUE
}
ZEND_METHOD( Cheetah_Di_ServiceLocator , getComponents ) {
	zval* returnDefinitions;

	zval retval;
	char property[13] = "";
	if ( ( ZEND_NUM_ARGS() ==1 ) && ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "z",&returnDefinitions ) == SUCCESS ) && (zval_is_true(returnDefinitions) == FALSE) ) {
		strcpy( property , "_components" );
	} else {
		strcpy( property , "_definitions" );
	}
	cheetah_read_property( cheetah_di_service_locator_ce , getThis() , property , 0 , &retval );
	RETURN_ZVAL( &retval , 0 , 1 )
}

ZEND_METHOD( Cheetah_Di_ServiceLocator , setComponents ) {
	zval* components;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "a",&components ) == FAILURE ) {
		RETURN_FALSE
	}

	size_t i , count;
	zval value;
	HashPosition* pos;
	count = zend_hash_num_elements( Z_ARRVAL_P( components ) );
	zend_hash_internal_pointer_reset( Z_ARRVAL_P( components ) );
	zval retval;
	zval params[2];
	for ( i = 0; i < count ; i++ ) {
		zend_string* key;
		zend_ulong idx;
		ZVAL_ZVAL( &value , zend_hash_get_current_data( Z_ARRVAL_P( components ) ) , 0 , 1 );
		if ( zend_hash_get_current_key( Z_ARRVAL_P( components ) , &key , &idx ) == HASH_KEY_IS_STRING ) {
			ZVAL_STRING( &( params[0] ) , key->val );
			ZVAL_ZVAL( &( params[1] ) , &value , 0 , 1 );
			cheetah_call_user_function( NULL , getThis() , "set" , &retval , 2 , params );
		}
		zend_hash_move_forward( Z_ARRVAL_P( components ) );
	}
	return;
}
