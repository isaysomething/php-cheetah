#define CHEETAH_CLASS_WEB_REQUEST()	\
{	\
	zend_class_entry cheetah_web_request_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_web_request_ce_local, "Cheetah\\Web\\Request",cheetah_web_request_methods );	\
	cheetah_web_request_ce = zend_register_internal_class_ex (&cheetah_web_request_ce_local, cheetah_kernel_request_ce );	\
}

PHP_METHOD( Cheetah_Web_Request , resolve );
PHP_METHOD( Cheetah_Web_Request , getMethod );
PHP_METHOD( Cheetah_Web_Request , getProtocol );
PHP_METHOD( Cheetah_Web_Request , get );
PHP_METHOD( Cheetah_Web_Request , post );
PHP_METHOD( Cheetah_Web_Request , getIsGet );
PHP_METHOD( Cheetah_Web_Request , getIsPost );
PHP_METHOD( Cheetah_Web_Request , getIsOptions );
PHP_METHOD( Cheetah_Web_Request , getIsHead );
PHP_METHOD( Cheetah_Web_Request , getIsDelete );
PHP_METHOD( Cheetah_Web_Request , getIsPut );
PHP_METHOD( Cheetah_Web_Request , getIsPatch );
PHP_METHOD( Cheetah_Web_Request , getIsAjax );
static zend_function_entry cheetah_web_request_methods[] = {
	ZEND_ME(Cheetah_Web_Request, resolve, NULL,ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Web_Request, getMethod, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getProtocol, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, get, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, post, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsGet, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsPost, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsOptions, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsHead, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsDelete, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsPut, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsPatch, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Request, getIsAjax, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Web_Request , resolve ) {
	zval route , *ret;
	array_init( &route );
	ret = cheetah_request_query_ex( TRACK_VARS_SERVER , 0 , "PATH_INFO" , sizeof( "PATH_INFO" ) - 1 );
	if ( Z_TYPE_P( ret ) == IS_STRING ) {
		char *route_str = Z_STRVAL_P( ret );
		char *_route;
		// Get route
		_route = strtok( route_str , "/" );
		while ( _route ) {
			add_next_index_string( &route , _route );
			_route = strtok( NULL , "/" );
		}
	}
	RETURN_ZVAL( &route , 1 , 0 );
}
ZEND_METHOD( Cheetah_Web_Request , getMethod ) {
	zval *ret;
	ret = cheetah_request_query_ex( TRACK_VARS_SERVER , 0 , "HTTP_X_HTTP_METHOD_OVERRIDE" ,
			sizeof( "HTTP_X_HTTP_METHOD_OVERRIDE" ) - 1 );
	if ( Z_TYPE_P( ret ) == IS_STRING ) {
		RETURN_STRING( strupr( Z_STRVAL_P( ret ) ) );
	}
	ret = cheetah_request_query_ex( TRACK_VARS_SERVER , 0 , "REQUEST_METHOD" , sizeof( "REQUEST_METHOD" ) - 1 );
	if ( Z_TYPE_P( ret ) == IS_STRING ) {
		RETURN_STRING( strupr( Z_STRVAL_P( ret ) ) );
	}
	RETURN_STRING( "GET" );
}
ZEND_METHOD( Cheetah_Web_Request , getProtocol ) {
	zval *ret;
	ret = cheetah_request_query_ex( TRACK_VARS_SERVER , 0 , "SERVER_PROTOCOL" , sizeof( "SERVER_PROTOCOL" ) - 1 );
	if ( Z_TYPE_P( ret ) == IS_STRING && strcmp( "HTTP/1.0" , Z_STRVAL_P( ret ) ) ) {
		RETURN_STRING( "1.0" );
	}
	RETURN_STRING( "1.1" );
}
ZEND_METHOD( Cheetah_Web_Request , get ) {
	zval* get_params;
	get_params = &PG( http_globals )[TRACK_VARS_GET];
	if ( ZEND_NUM_ARGS() == 0 ) {
		RETURN_ZVAL( get_params , 1 , 0 )
	} else {
		zval* default_value;
		ZVAL_NULL( default_value );
		char* name;
		size_t name_len;
		if ( zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s|z", &name, &name_len, &default_value) == FAILURE ) {
			return;
		}
		if ( zend_hash_str_exist( HASH_OF( get_params ) , name , name_len ) ) {
			RETURN_ZVAL( zend_hash_str_find( HASH_OF( get_params ) , name , name_len ) , 1 , 0 )
		}
		RETURN_ZVAL( default_value , 1 , 0 )
	}
}
ZEND_METHOD( Cheetah_Web_Request , post ) {
	zval* post_params;
	post_params = &PG( http_globals )[TRACK_VARS_POST];
	if ( ZEND_NUM_ARGS() == 0 ) {
		RETURN_ZVAL( post_params , 1 , 0 )
	} else {
		zval* default_value;
		ZVAL_NULL( default_value );
		char* name;
		size_t name_len;
		if ( zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s|z", &name, &name_len, &default_value) == FAILURE ) {
			return;
		}
		if ( zend_hash_str_exists( HASH_OF( post_params ) , name , name_len ) ) {
			RETURN_ZVAL( zend_hash_str_find( HASH_OF( post_params ) , name , name_len ) , 1 , 0 )
		}
		RETURN_ZVAL( default_value , 1 , 0 )
	}
}
ZEND_METHOD( Cheetah_Web_Request , getIsGet ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "GET" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsPost ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "POST" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsOptions ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "OPTIONS" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsHead ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "HEAD" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsDelete ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "DELETE" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsPut ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "PUT" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsPatch ) {
	zval method;
	cheetah_call_user_function_no_params( NULL , getThis() , "getMethod" , &method );
	if ( Z_STRVAL( method ) == "PATCH" ) {
		RETURN_TRUE
	}
	RETURN_FALSE
}
ZEND_METHOD( Cheetah_Web_Request , getIsAjax ) {
	zval *value;
	zend_is_auto_global_str(ZEND_STRL("_SERVER") TSRMLS_CC);

	if ( zend_hash_str_exists( HASH_OF( &PG( http_globals )[TRACK_VARS_SERVER] ) , "HTTP_X_REQUESTED_WITH" , 21 ) ) {
		value = zend_hash_str_find( HASH_OF( &PG( http_globals )[TRACK_VARS_SERVER] ) , "HTTP_X_REQUESTED_WITH" , 21 );
		if ( 0 == strcmp( "XMLHttpRequest" , Z_STRVAL_P( value ) ) ) {
			RETURN_TRUE
		}
	}
	RETURN_FALSE;
}
