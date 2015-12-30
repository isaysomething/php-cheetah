#define CHEETAH_CLASS_LOG_LOGGER()	\
{	\
	zend_class_entry cheetah_log_logger_ce_local;	\
	INIT_CLASS_ENTRY(cheetah_log_logger_ce_local,"Cheetah\\Log\\Logger", cheetah_log_logger_methods);	\
	cheetah_log_logger_ce = zend_register_internal_class_ex(&cheetah_log_logger_ce_local TSRMLS_CC, cheetah_kernel_component_ce);	\
	zend_declare_class_constant_long(cheetah_log_logger_ce,"LEVEL_TRACE",11,1);	\
	zend_declare_class_constant_long(cheetah_log_logger_ce,"LEVEL_ERROR",11,2);	\
	zend_declare_class_constant_long(cheetah_log_logger_ce,"LEVEL_WARNING",13,4);	\
	zend_declare_class_constant_long(cheetah_log_logger_ce,"LEVEL_INFO",10,8);	\
	zend_declare_property_null(cheetah_log_logger_ce,"messages",8,ZEND_ACC_PUBLIC);	\
}
PHP_METHOD( Cheetah_Log_Logger , init );
PHP_METHOD( Cheetah_Log_Logger , log );
PHP_METHOD( Cheetah_Log_Logger , flush );
static zend_function_entry cheetah_log_logger_methods[] = {
	ZEND_ME ( Cheetah_Log_Logger, init, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Log_Logger, log, NULL, ZEND_ACC_PUBLIC )
	ZEND_ME ( Cheetah_Log_Logger, flush, NULL, ZEND_ACC_PUBLIC )
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Log_Logger , init ) {
	// Invoke parent class init function
	zval retval;
	if ( cheetah_call_user_function_no_params( NULL , getThis() , "Cheetah\\Kernel\\Component::init" , &retval )
			== FAILURE ) {
	}

	// Initialize $messagses
	zval messages;
	array_init( &messages );
	zend_update_property( NULL , getThis() , "messages" , 8 , &messages );

	// register_shutdown_function
	zval shutdown_function , params[1];
	array_init( &shutdown_function );
	add_next_index_zval( &shutdown_function , getThis() );
	add_next_index_string( &shutdown_function , "flush" );
	ZVAL_ZVAL( &( params[0] ) , &shutdown_function , 1 , 0 );
	cheetah_call_user_function( NULL , NULL , "register_shutdown_function" , &retval , 1 , params );
}
ZEND_METHOD( Cheetah_Log_Logger , log ) {
	zval *message;
	long level;
	char *category;
	size_t category_len;
	if ( zend_parse_parameters ( ZEND_NUM_ARGS() TSRMLS_CC, "zl|s",&message,&level,&category,&category_len) == FAILURE ) {
		RETURN_FALSE
	}
	if ( ZEND_NUM_ARGS() == 2 ) {
		category = "application";
	}
	zval msg , time , time_params[1] , traces;
	ZVAL_BOOL( &( time_params[0] ) , IS_TRUE );
	array_init( &traces );
	cheetah_call_user_function( NULL , NULL , "microtime" , &time , 1 , time_params );
	array_init( &msg );
	add_next_index_zval( &msg , message );
	add_next_index_long( &msg , level );
	add_next_index_string( &msg , category );
	add_next_index_double( &msg , zval_get_double( &time ) );
	add_next_index_zval( &msg , &traces );
	zval messages;
	cheetah_read_property( NULL , getThis() , "messages" , 0 , &messages );
	add_next_index_zval( &messages , &msg );
	cheetah_update_property( NULL , getThis() , "messages" , &messages );
}
ZEND_METHOD( Cheetah_Log_Logger , flush ) {
	zval messages;
	cheetah_read_property( NULL , getThis() , "messages" , 0 , &messages );
	if ( EG( exception ) ) {
		php_var_dump( &messages , 1 );
	}
}
