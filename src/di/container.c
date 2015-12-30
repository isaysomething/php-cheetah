#define CHEETAH_CLASS_DI_CONTAINER()	\
{	\
	zend_class_entry cheetah_di_container_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_di_container_ce_local, "Cheetah\\Di\\Container", cheetah_di_container_methods );	\
	cheetah_di_container_ce = zend_register_internal_class_ex ( &cheetah_di_container_ce_local, cheetah_kernel_component_ce );	\
	zend_declare_property_null ( cheetah_di_container_ce, "_singletons",11, ZEND_ACC_PRIVATE TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_di_container_ce, "_definitions",12, ZEND_ACC_PRIVATE TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_di_container_ce, "_params",7, ZEND_ACC_PRIVATE TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_di_container_ce, "_reflections",12, ZEND_ACC_PRIVATE TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_di_container_ce, "_dependencies",13, ZEND_ACC_PRIVATE TSRMLS_CC );	\
}

PHP_METHOD( Cheetah_Di_Container , init );
PHP_METHOD( Cheetah_Di_Container , get );
PHP_METHOD( Cheetah_Di_Container , set );
PHP_METHOD( Cheetah_Di_Container , setSingleton );
PHP_METHOD( Cheetah_Di_Container , has );
PHP_METHOD( Cheetah_Di_Container , hasSingleton );
PHP_METHOD( Cheetah_Di_Container , clear );
PHP_METHOD( Cheetah_Di_Container , normalizeDefinition );
PHP_METHOD( Cheetah_Di_Container , getDefinitions );
PHP_METHOD( Cheetah_Di_Container , build );
PHP_METHOD( Cheetah_Di_Container , mergeParams );
PHP_METHOD( Cheetah_Di_Container , getDependencies );
PHP_METHOD( Cheetah_Di_Container , resolveDependencies );
static zend_function_entry cheetah_di_container_methods[] = {
	ZEND_ME ( Cheetah_Di_Container, init, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, get, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, set, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, setSingleton, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, has, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, hasSingleton, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, clear, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, normalizeDefinition, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, getDefinitions, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, build, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Di_Container, mergeParams, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Di_Container, getDependencies, NULL,
			ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Di_Container, resolveDependencies,
			NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END};
ZEND_METHOD( Cheetah_Di_Container , init ) {
	//  Initialize
	zval _singletons , _definitions , _params , _reflections , _dependencies;
	array_init( &_singletons );
	array_init( &_definitions );
	array_init( &_params );
	array_init( &_reflections );
	array_init( &_dependencies );
	zend_update_property( cheetah_di_container_ce , getThis() , "_singletons" , 11 , &_singletons );
	zend_update_property( cheetah_di_container_ce , getThis() , "_definitions" , 12 , &_definitions );
	zend_update_property( cheetah_di_container_ce , getThis() , "_params" , 7 , &_params );
	zend_update_property( cheetah_di_container_ce , getThis() , "_reflections" , 12 , &_reflections );
	zend_update_property( cheetah_di_container_ce , getThis() , "_dependencies" , 13 , &_dependencies );
}
ZEND_METHOD( Cheetah_Di_Container , get ) {
	zval* params , *config;
	char *class;
	size_t class_len;
	uint args = ZEND_NUM_ARGS();
	if ( zend_parse_parameters(args TSRMLS_CC , "sa|a" , &class , &class_len , &params , &config )
	== FAILURE ) {
		RETURN_FALSE
	}
	zval _singletons;
	cheetah_read_property( cheetah_di_container_ce , getThis() , "_singletons" , 0 , &_singletons );
	// Return singleton if it exists.
	if ( zend_hash_str_exists( Z_ARRVAL( _singletons ) , class , class_len ) ) {
		RETURN_ZVAL( zend_hash_str_find( Z_ARRVAL( _singletons ) , class , class_len ) , 1 , 0 );
	}
	zval _definitions;
	cheetah_read_property( cheetah_di_container_ce , getThis() , "_definitions" , 0 , &_definitions );
	// Invoke build function when the class was not found in $_definitions.
	if ( !zend_hash_str_exists( Z_ARRVAL( _definitions ) , class , class_len ) ) {
		zval retval;
		zval build_params[3];
		ZVAL_STRING( &( build_params[0] ) , class );
		ZVAL_ZVAL( &( build_params[1] ) , params , 0 , 1 );
		if ( args == 3 ) {
			ZVAL_ZVAL( &( build_params[2] ) , config , 0 , 1 );
		} else {
			array_init_size( &( build_params[2] ) , 0 );
		}
		cheetah_call_user_function( NULL , getThis() , "build" , &retval , 3 , build_params );
		RETURN_ZVAL( &retval , 1 , 0 )
	}
	zval object;
	zval definition;
	ZVAL_ZVAL( &definition , zend_hash_str_find( Z_ARRVAL( _definitions ) , class , class_len ) , 1 , 0 );
	if ( Z_TYPE( definition ) == IS_OBJECT ) {
		// Save it into $_singletons
		zend_hash_str_add( Z_ARRVAL( _singletons ) , class , class_len , &definition );
		zend_update_property( cheetah_di_container_ce , getThis() , "_singletons" , 11 , &_singletons );
		ZVAL_ZVAL( &object , &definition , 1 , 0 );
	} else {
		zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "Unexpected object definition type: %s" ,
				zend_get_type_by_const( Z_TYPE( definition ) ) );
		return;
	}
	RETURN_ZVAL( &object , 0 , 1 );
}
ZEND_METHOD( Cheetah_Di_Container , set ) {
}
ZEND_METHOD( Cheetah_Di_Container , setSingleton ) {
}
ZEND_METHOD( Cheetah_Di_Container , has ) {
}
ZEND_METHOD( Cheetah_Di_Container , hasSingleton ) {
}
ZEND_METHOD( Cheetah_Di_Container , clear ) {
}
ZEND_METHOD( Cheetah_Di_Container , normalizeDefinition ) {
}
ZEND_METHOD( Cheetah_Di_Container , getDefinitions ) {
}
ZEND_METHOD( Cheetah_Di_Container , build ) {
	zval* params , *config;
	char *class;
	size_t class_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS()TSRMLS_CC , "saa" , &class , &class_len , &params , &config )
	== FAILURE ) {
		RETURN_FALSE
	}
	// Get dependencies
	zval retval , _params[1] , reflection , dependencies;
	ZVAL_STRING( &( _params[0] ) , class );
	if ( cheetah_call_user_function( NULL , getThis() , "getDependencies" , &retval , 1 , _params ) == FAILURE ) {

	}
	ZVAL_ZVAL( &reflection , zend_hash_index_find( Z_ARRVAL( retval ) , 0 ) , 1 , 0 );
	ZVAL_ZVAL( &dependencies , zend_hash_index_find( Z_ARRVAL( retval ) , 1 ) , 1 , 0 );

	// Save params into $_dependencies
	uint params_count;
	zval param_value;
	params_count = zend_hash_num_elements( Z_ARRVAL_P( params ) );
	zend_hash_internal_pointer_reset( Z_ARRVAL_P( params ) );
	int i;
	for ( i = 0; i < params_count ; i++ ) {
		zend_string* key;
		zend_ulong idx;
		ZVAL_ZVAL( &param_value , zend_hash_get_current_data( Z_ARRVAL_P( params ) ) , 0 , 1 );

		if ( zend_hash_get_current_key( Z_ARRVAL_P( params ) , &key , &idx ) == HASH_KEY_IS_STRING ) {
			zend_hash_str_update( Z_ARRVAL( dependencies ) , key->val , key->len , &param_value );
		} else {
			zend_hash_index_update_ptr( Z_ARRVAL( dependencies ) , idx , &param_value );
		}
		zend_hash_move_forward( Z_ARRVAL_P( params ) );
	}

	// Invoke resolveDependencies function
	zval resolve_params[2];
	ZVAL_ZVAL( &( resolve_params[0] ) , &dependencies , 1 , 0 );
	ZVAL_ZVAL( &( resolve_params[1] ) , &reflection , 1 , 0 );
	cheetah_call_user_function( NULL , getThis() , "resolveDependencies" , &dependencies , 2 , resolve_params );
	zval instance_retval , new_instance_params[1];
	zval new_instance_params_two;
	array_init( &new_instance_params_two );
	add_next_index_zval( &new_instance_params_two , &dependencies );
	ZVAL_ZVAL( &( new_instance_params[0] ) , &new_instance_params_two , 1 , 0 );
	// Invoke newInstanceArgs function
	cheetah_call_user_function( NULL , &reflection , "newInstanceArgs" , &instance_retval , 1 , new_instance_params );
	RETURN_ZVAL( &instance_retval , 0 , 1 );
}
ZEND_METHOD( Cheetah_Di_Container , mergeParams ) {
}
ZEND_METHOD( Cheetah_Di_Container , getDependencies ) {
	char *class;
	size_t class_len;
	if ( zend_parse_parameters( ZEND_NUM_ARGS()TSRMLS_CC , "s" , &class , &class_len ) == FAILURE ) {
		RETURN_FALSE
	}
	zval _reflections;
	cheetah_read_property( cheetah_di_container_ce , getThis() , "_reflections" , 0 , &_reflections );
	if ( zend_hash_str_exists( Z_ARRVAL( _reflections ) , class , class_len ) ) {
		// Return reflection if it already exist
		RETURN_ZVAL( zend_hash_str_find( Z_ARRVAL( _reflections ) , class , class_len ) , 1 , 0 );
	}
	zval dependencies , reflection , reflection_retval , params[1];
	array_init( &dependencies );
	object_init_ex( &reflection , reflection_class_ptr );
	ZVAL_STRING( &( params[0] ) , class );

	cheetah_call_user_function( NULL , &reflection , "__construct" , &reflection_retval , 1 , params );

	zval constructor;
	cheetah_call_user_function_no_params( NULL , &reflection , "getConstructor" , &constructor );

	if ( Z_TYPE( constructor ) != IS_NULL ) {
		// Get params of __construct function
		zval parameters;
		cheetah_call_user_function_no_params( NULL , &constructor , "getParameters" , &parameters );
		uint parameters_count;
		zval* parameter_value;
		HashPosition* parameter_pos;
		parameters_count = zend_hash_num_elements( Z_ARRVAL( parameters ) );
		zend_hash_internal_pointer_reset( Z_ARRVAL( parameters ) );
		zval parameter_default_value_available , parameter_default_value , parameter_value_key , parameter_class ,
				parameter_class_name;
		int i;
		for ( i = 0; i < parameters_count ; i++ ) {
			zend_string* key;
			zend_ulong idx;
			parameter_value = zend_hash_get_current_data( Z_ARRVAL( parameters ) );
			cheetah_read_property( Z_OBJCE_P( parameter_value ) , parameter_value , "name" , 0 , &parameter_value_key );
			cheetah_call_user_function_no_params( NULL , parameter_value , "isDefaultValueAvailable" ,
					&parameter_default_value_available );
			if ( Z_TYPE( parameter_default_value_available ) == IS_TRUE ) {
				// Save the default value of parameter into $_dependencies
				cheetah_call_user_function_no_params( NULL , parameter_value , "getDefaultValue" ,
						&parameter_default_value );
				add_assoc_zval_ex( &dependencies , zval_get_string( &parameter_value_key )->val ,
						zval_get_string( &parameter_value_key )->len , &parameter_default_value );
			} else {
				// Get dependent Class
				cheetah_call_user_function_no_params( NULL , parameter_value , "getClass" , &parameter_class );
				if ( instanceof_function( Z_OBJCE( parameter_class ) , NULL ) ) {
					add_assoc_null_ex( &dependencies , zval_get_string( &parameter_value_key )->val ,
							zval_get_string( &parameter_value_key )->len );
				} else {
					cheetah_call_user_function_no_params( NULL , &parameter_class , "getName" , &parameter_class_name );
					add_assoc_zval_ex( &dependencies , zval_get_string( &parameter_value_key )->val ,
							zval_get_string( &parameter_value_key )->len , &parameter_class_name );
				}
			}
			zend_hash_move_forward( Z_ARRVAL( parameters ) );
		}
	}
	// Update $_dependencies and $_reflections
	zval _dependencies;
	cheetah_read_property( cheetah_di_container_ce , getThis() , "_dependencies" , 0 , &_dependencies );
	add_assoc_zval_ex( &_reflections , class , class_len , &reflection );
	add_assoc_zval_ex( &_dependencies , class , class_len , &dependencies );
	zend_update_property( cheetah_di_container_ce , getThis() , "_reflections" , 12 , &_reflections );
	zend_update_property( cheetah_di_container_ce , getThis() , "_dependencies" , 13 , &_dependencies );
	zval retval;
	array_init( &retval );
	zend_hash_index_add( Z_ARRVAL( retval ) , 0 , &reflection );
	zend_hash_index_add( Z_ARRVAL( retval ) , 1 , &dependencies );
	RETURN_ZVAL( &retval , 1 , 0 );
}
ZEND_METHOD( Cheetah_Di_Container , resolveDependencies ) {
	zval* dependencies , *reflection;
	if ( zend_parse_parameters( ZEND_NUM_ARGS()TSRMLS_CC , "az" , &dependencies , &reflection ) == FAILURE ) {
		RETURN_FALSE
	}
	size_t i , array_count;
	zval* dependency;
	HashPosition* pos;
	array_count = zend_hash_num_elements( Z_ARRVAL_P( dependencies ) );
	zend_hash_internal_pointer_reset( Z_ARRVAL_P( dependencies ) );
	zval _service , _service_params[1] , class_name;
	for ( i = 0; i < array_count ; i++ ) {
		zend_string* key;
		zend_ulong idx;
		dependency = zend_hash_get_current_data( Z_ARRVAL_P( dependencies ) );
		if ( Z_TYPE_P( dependency ) == IS_OBJECT ) {
			zval dependency_id;
			cheetah_read_property( Z_OBJCE_P( dependency ) , dependency , "id" , 0 , dependency );
			if ( Z_TYPE( dependency_id ) != IS_NULL ) {
				cheetah_call_user_function( NULL , getThis() , "get" , &_service , 1 , _service_params );
				if ( zend_hash_get_current_key( Z_ARRVAL_P( dependencies ) , &key , &idx ) == HASH_KEY_IS_STRING ) {
					zend_hash_update_ptr( Z_ARRVAL_P( dependencies ) , key , &_service );
				} else {
					zend_hash_index_update_ptr( Z_ARRVAL_P( dependencies ) , idx , &_service );
				}
			} else {
				cheetah_call_user_function_no_params( NULL , reflection , "getName" , &class_name );
				zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 ,
						"Missing required some parameters when create a instance of %s" , Z_STRVAL( class_name ) );
				return;
			}
		}
		zend_hash_move_forward( Z_ARRVAL_P( dependencies ) );
	}
	RETURN_ZVAL( dependencies , 0 , 1 );
}
