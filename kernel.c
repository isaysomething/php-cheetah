char *strupr( char *str ) {
	char *orign = str;

	for ( ; *str != '\0 ' ; str++ )
		*str = toupper( *str );

	return orign;
}

char *strlwr( char *str ) {
	char *orign = str;

	for ( ; *str != '\0 ' ; str++ )
		*str = tolower( *str );

	return orign;
}

int cheetah_call_user_function( HashTable *function_table , zval *object , char* function_name , zval *retval_ptr ,
		uint32_t param_count , zval params[] ) {
	zval function;
	ZVAL_STRING( &function , function_name );
	int result = call_user_function( function_table , object , &function , retval_ptr , param_count , params );
	ZVAL_DESTRUCTOR( &function );
	return result;
}

int cheetah_call_user_function_no_params( HashTable *function_table , zval *object , char* function_name ,
		zval *retval_ptr ) {
	zval function;
	ZVAL_STRING( &function , function_name );
	zval params[0];
	int result = call_user_function( function_table , object , &function , retval_ptr , 0 , params );
	ZVAL_DESTRUCTOR( &function );
	return result;
}

BOOL cheetah_method_exists( zval *object , char *function ) {
	zval ret , params[2];
	ZVAL_ZVAL( &( params[0] ) , object , 1 , 0 );
	ZVAL_STRING( &( params[1] ) , function );
	if ( cheetah_call_user_function( EG( function_table ) , NULL , "method_exists" , &ret , 2 , params ) == SUCCESS ) {
		if ( Z_TYPE( ret ) == IS_TRUE ) {
			return TRUE;
		}
	}
	return FALSE;
}

char *cheetah_get_object_class_name( zval *object ) {
	return Z_OBJCE_P( object )->name->val;
}

void cheetah_read_property( zend_class_entry *scope , zval *object , const char *name , zend_bool silent , zval *rv ) {
	ZVAL_ZVAL( rv , zend_read_property( scope , object , name , strlen( name ) , silent , rv ) , 1 , 0 );
}

void cheetah_read_static_property( zend_class_entry *scope , const char *name , zend_bool silent , zval *rv ) {
	ZVAL_ZVAL( rv , zend_read_static_property( scope , name , strlen( name ) , silent ) , 1 , 0 );
}

void cheetah_update_property( zend_class_entry *scope , zval *object , const char *name , zval *value ) {
	zend_update_property( scope , object , name , strlen( name ) , value );
}
void cheetah_update_property_long( zend_class_entry *scope , zval *object , const char *name , zend_long value ) {
	zend_update_property_long( scope , object , name , strlen( name ) , value );
}

void cheetah_update_property_string( zend_class_entry *scope , zval *object , const char *name , const char *value ) {
	zend_update_property_string( scope , object , name , strlen( name ) , value );
}

char *cheetah_get_constant_str( const char *name ) {
	return Z_STRVAL_P( zend_get_constant_str( name , strlen( name ) ) );
}

int cheetah_update_static_property( zend_class_entry *scope , const char *name , zval *value ) {
	return zend_update_static_property( scope , name , strlen( name ) , value );
}

zval *cheetah_get_class_constant( char *name , zend_class_entry *scope , zend_ulong flags ) {
	zend_string *cname;
	cname = zend_string_init( name , strlen( name ) , 0 );
	return zend_get_constant_ex( cname , NULL , flags );
}
BOOL cheetah_check_file_exist( char *filename ) {
	if ( access( filename , 0 ) == 0 ) {
		return TRUE;
	}
	zend_throw_exception_ex( cheetah_kernel_exception_ce , 0 , "The file does not exist: %s." , filename );
	return FALSE;
}
int cheetah_include( char *filename ) {
	zend_file_handle file_handle;
	zend_op_array *op_array;
	char realpath[MAXPATHLEN];

	if ( !VCWD_REALPATH( filename , realpath ) ) {
		return 0;
	}

	file_handle.filename = filename;
	file_handle.free_filename = 0;
	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.handle.fp = NULL;

	op_array = zend_compile_file( &file_handle , ZEND_INCLUDE );

	if ( op_array && file_handle.handle.stream.handle ) {
		zval dummy;
		ZVAL_NULL( &dummy );

		if ( !file_handle.opened_path ) {
			file_handle.opened_path = zend_string_init( filename , strlen( filename ) , 0 );
		}
		zend_hash_add( &EG( included_files ) , file_handle.opened_path , &dummy );
	}
	zend_destroy_file_handle( &file_handle );

	if ( op_array ) {
		zval result;
		ZVAL_UNDEF( &result );
		zend_execute( op_array , &result );
		destroy_op_array( op_array );
		efree( op_array );
		if ( !EG( exception ) ) {
			zval_ptr_dtor( &result );
		}
		return 1;
	}
	return 0;
}
zval *cheetah_request_query_ex( uint type , zend_bool fetch_type , void *name , size_t len ) {
	zval *carrier = NULL , *ret;
	zend_bool jit_initialization = PG( auto_globals_jit );

	switch ( type ) {
	case TRACK_VARS_POST :
	case TRACK_VARS_GET :
	case TRACK_VARS_FILES :
	case TRACK_VARS_COOKIE :
		carrier = &PG( http_globals )[type];
		break;
	case TRACK_VARS_ENV :
		if ( jit_initialization ) {
			zend_string *env_str = zend_string_init( "_ENV" , sizeof( "_ENV" ) - 1 , 0 );
			zend_is_auto_global( env_str );
			zend_string_release( env_str );
		}
		carrier = &PG( http_globals )[type];
		break;
	case TRACK_VARS_SERVER :
		if ( jit_initialization ) {
			zend_string *server_str = zend_string_init( "_SERVER" , sizeof( "_SERVER" ) - 1 , 0 );
			zend_is_auto_global( server_str );
			zend_string_release( server_str );
		}
		carrier = &PG( http_globals )[type];
		break;
	case TRACK_VARS_REQUEST :
		if ( jit_initialization ) {
			zend_string *request_str = zend_string_init( "_REQUEST" , sizeof( "_REQUEST" ) - 1 , 0 );
			zend_is_auto_global( request_str );
			zend_string_release( request_str );
		}
		carrier = zend_hash_str_find( &EG( symbol_table ) , ZEND_STRL( "_REQUEST" ) );
		break;
	default :
		break;
	}

	if ( !carrier ) {
		return NULL;
	}

	if ( !name ) {
		Z_ADDREF_P( carrier );
		return carrier;
	}

	if ( EXPECTED( fetch_type ) ) {
		if ( ( ret = zend_hash_find( Z_ARRVAL_P( carrier ) , ( zend_string * ) name ) ) == NULL ) {
			return NULL;
		}
	} else {
		if ( ( ret = zend_hash_str_find( Z_ARRVAL_P( carrier ) , ( char * ) name , len ) ) == NULL ) {
			return NULL;
		}
	}
	Z_TRY_ADDREF_P( ret );
	return ret;
}

void cheetah_logger_trace_str( char *message ) {
	zval retval , params[2];
	ZVAL_STRING( &( params[0] ) , message );
	cheetah_call_user_function( NULL , NULL , "Cheetah::trace" , &retval , 1 , params );
}
