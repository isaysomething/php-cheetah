#define CHEETAH_CLASS_WEB_APPLICATION()	\
{	\
	zend_class_entry cheetah_web_application_ce_local;	\
	INIT_CLASS_ENTRY ( cheetah_web_application_ce_local, "Cheetah\\Web\\Application",cheetah_web_application_methods );	\
	cheetah_web_application_ce = zend_register_internal_class_ex (&cheetah_web_application_ce_local, cheetah_kernel_application_ce );	\
	zend_declare_property_string ( cheetah_web_application_ce, "defaultRoute",12,"site", ZEND_ACC_PUBLIC TSRMLS_CC );	\
	zend_declare_property_null ( cheetah_web_application_ce, "controller",10, ZEND_ACC_PUBLIC TSRMLS_CC );	\
}
PHP_METHOD( Cheetah_Web_Application , run );
PHP_METHOD( Cheetah_Web_Application , handleRequest );
PHP_METHOD( Cheetah_Web_Application , getHomeUrl );
PHP_METHOD( Cheetah_Web_Application , coreComponents );
static zend_function_entry cheetah_web_application_methods[] = {
	ZEND_ME(Cheetah_Web_Application, run, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Web_Application, getHomeUrl, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Web_Application, handleRequest, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME(Cheetah_Web_Application, coreComponents, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Web_Application , run ) {
	// Handle request
	zval request;
	cheetah_call_user_function_no_params( NULL , getThis() , "getRequest" , &request );
	zval response , params[1];
	ZVAL_ZVAL( &( params[0] ) , &request , 1 , 0 );
	cheetah_call_user_function( NULL , getThis() , "handleRequest" , &response , 1 , params );

	// Send response to client
	zval retval;
	cheetah_call_user_function_no_params( NULL , &response , "send" , &retval );
	zval exit_status;
	cheetah_read_property( Z_OBJCE( response ) , &response , "exitStatus" , 0 , &exit_status );
	RETURN_LONG( zval_get_long( &exit_status ) );
}
ZEND_METHOD( Cheetah_Web_Application , getHomeUrl ) {
}
ZEND_METHOD( Cheetah_Web_Application , handleRequest ) {
	zval *request;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "o", &request ) == FAILURE ) {
		RETURN_FALSE
	}
	zval route;
	cheetah_call_user_function_no_params( NULL , request , "resolve" , &route );
	zval retval , params[1];
	ZVAL_ZVAL( &( params[0] ) , &route , 1 , 0 );
	cheetah_call_user_function( NULL , getThis() , "runAction" , &retval , 1 , params );

	// Return result of function runAction if it is instance of class Response
	if ( ( Z_TYPE( retval ) == IS_OBJECT ) && instanceof_class( Z_OBJCE( retval ) , cheetah_kernel_response_ce ) ) {
		RETURN_ZVAL( &retval , 1 , 0 );
	}
	zval response;
	object_init_ex( &response , cheetah_web_response_ce );
	cheetah_call_user_function_no_params( NULL , getThis() , "getResponse" , &response );
	zend_update_property( Z_OBJCE( response ) , &response , "content" , 7 , &retval );
	RETURN_ZVAL( &response , 1 , 0 );
}
ZEND_METHOD( Cheetah_Web_Application , coreComponents ) {
	zval components;
	cheetah_call_user_function_no_params( NULL , getThis() , "parent::coreComponents" , &components );
	if ( Z_TYPE( components ) != IS_ARRAY ) {
		array_init( &components );
	}
	add_assoc_string_ex( &components , "request" , 7 , "Cheetah\\Web\\Request" );
	add_assoc_string_ex( &components , "response" , 8 , "Cheetah\\Web\\Response" );
	add_assoc_string_ex( &components , "session" , 7 , "Cheetah\\Web\\Session" );
	add_assoc_string_ex( &components , "user" , 4 , "Cheetah\\Web\\User" );
	add_assoc_string_ex( &components , "errorHandler" , 12 , "Cheetah\\Web\\ErrorHandler" );
	RETURN_ZVAL( &components , 0 , 1 );
}
