#define CHEETAH_CLASS_WEB_RESPONSE()	\
	{	\
		zend_class_entry cheetah_web_response_ce_local;	\
		INIT_CLASS_ENTRY ( cheetah_web_response_ce_local, "Cheetah\\Web\\Response",cheetah_web_response_methods );	\
		cheetah_web_response_ce = zend_register_internal_class_ex (&cheetah_web_response_ce_local, cheetah_kernel_response_ce );	\
		zend_declare_property_bool ( cheetah_web_response_ce, "isSent",6, FALSE,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "acceptParams",12,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "formatters",10,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "data",4,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "content",7,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "stream",6,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "charset",7,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_string( cheetah_web_response_ce, "statusText ",10,"OK",ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "version",7,ZEND_ACC_PUBLIC TSRMLS_CC );	\
		zend_declare_property_null ( cheetah_web_response_ce, "httpStatuses",12, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC );	\
		zend_declare_property_long( cheetah_web_response_ce, "_statusCode",11, 200,ZEND_ACC_PRIVATE TSRMLS_CC );	\
		zend_declare_property_null( cheetah_web_response_ce, "_headers",8,ZEND_ACC_PRIVATE TSRMLS_CC );	\
	}

PHP_METHOD( Cheetah_Web_Response, init);
PHP_METHOD( Cheetah_Web_Response, send);
PHP_METHOD( Cheetah_Web_Response, prepare);
PHP_METHOD( Cheetah_Web_Response, sendHeaders);
PHP_METHOD( Cheetah_Web_Response, sendContent);
PHP_METHOD( Cheetah_Web_Response, getStatusCode);
PHP_METHOD( Cheetah_Web_Response, setStatusCode);
PHP_METHOD( Cheetah_Web_Response, getHttpStatus);
static zend_function_entry cheetah_web_response_methods[] = {
	ZEND_ME(Cheetah_Web_Response, init, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, send, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, prepare, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, sendHeaders, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, sendContent, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, getStatusCode, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, setStatusCode, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Cheetah_Web_Response, getHttpStatus, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
ZEND_METHOD( Cheetah_Web_Response, init) {
	zend_class_entry *ce = Z_OBJCE_P(getThis());
	zval charset;
	cheetah_read_property(ce, getThis(), "charset", 0, &charset);
	if (Z_TYPE(charset) == IS_NULL) {
		zval *app = CHEETAH_G(application);
		cheetah_read_property(Z_OBJCE_P(app), app, "charset", 0, &charset);
		cheetah_update_property(ce, getThis(), "charset", &charset);
	}
}
ZEND_METHOD( Cheetah_Web_Response, send) {
	zval retval;
	cheetah_call_user_function_no_params(NULL, getThis(), "prepare", &retval);
	cheetah_call_user_function_no_params(NULL, getThis(), "sendHeaders", &retval);
	cheetah_call_user_function_no_params(NULL, getThis(), "sendContent", &retval);
}
ZEND_METHOD( Cheetah_Web_Response, prepare) {
	zend_class_entry *ce = Z_OBJCE_P(getThis());
	zval content;
	cheetah_read_property(ce, getThis(), "content", 0, &content);
	switch (Z_TYPE(content)) {
	case IS_STRING:
		break;
	case IS_ARRAY:
		zend_throw_exception_ex(cheetah_kernel_invalid_param_exception_ce, 0, "Response content must not be an array.");
		return;
		break;
	case IS_OBJECT:
		if (cheetah_call_user_function_no_params(NULL, &content, "__toString", &content) == SUCCESS) {
			cheetah_update_property(NULL, getThis(), "content", &content);
			break;
		}
	default:
		zend_throw_exception_ex(cheetah_kernel_invalid_param_exception_ce, 0,
				"Response content must be a string or an object implementing __toString().");
		return;
		break;
	}
}
ZEND_METHOD( Cheetah_Web_Response, sendHeaders) {
	zval headers_sent;
	cheetah_call_user_function_no_params(EG(function_table), NULL, "headers_sent", &headers_sent);
	if (Z_TYPE(headers_sent) == IS_TRUE) {
		RETURN_NULL();
	}
	zend_class_entry *ce = Z_OBJCE_P(getThis());
	zval status_code, version, status_text;
	cheetah_call_user_function_no_params(NULL, getThis(), "getStatusCode", &status_code);
	cheetah_read_property(ce, getThis(), "version", 0, &version);

	if (Z_TYPE(version) == IS_NULL) {
		zval *app = CHEETAH_G(application), request;
		cheetah_call_user_function_no_params(NULL, app, "getRequest", &request);
		cheetah_call_user_function_no_params(NULL, &request, "getProtocol", &version);
	}

	cheetah_read_property(ce, getThis(), "statusText", 0, &status_text);
	char *header = NULL;
	header = (char *) malloc(Z_STRLEN(status_code) + 3 + Z_STRLEN(status_text) + 8);
	strcpy(header, "HTTP/");
	strcat(header, " ");
	strcat(header, Z_STRVAL(status_code));
	strcat(header, " ");
	strcat(header, Z_STRVAL(status_text));
	zval retval, params[1];
	ZVAL_STRING(&(params[0]), header);
	cheetah_call_user_function(EG(function_table), NULL, "header", &retval, 1, params);
	ZVAL_STRING(&(params[0]), "yrssoft");
	cheetah_call_user_function(EG(function_table), NULL, "setcookie", &retval, 1, params);
}
ZEND_METHOD( Cheetah_Web_Response, sendContent) {
	zval content;
	cheetah_read_property(NULL, getThis(), "content", 0, &content);
	php_printf("%s", Z_STRVAL(content));
}
ZEND_METHOD( Cheetah_Web_Response, getStatusCode) {
	zval status_code;
	cheetah_read_property(Z_OBJCE_P(getThis()), getThis(), "_statusCode", 0, &status_code);
	char status[5];
	sprintf(status, "%d", zval_get_long(&status_code));
	RETURN_STRING (status)
}
ZEND_METHOD( Cheetah_Web_Response, setStatusCode) {
}
ZEND_METHOD( Cheetah_Web_Response, getHttpStatus) {
	zval http_statuses;
	cheetah_read_static_property(cheetah_web_response_ce, "httpStatuses", 0, &http_statuses);
	if (Z_TYPE(http_statuses) == IS_NULL) {
		array_init_size(&http_statuses, 65);
		add_index_string(&http_statuses, 100, "Continue");
		add_index_string(&http_statuses, 101, "Switching Protocols");
		add_index_string(&http_statuses, 102, "Processing");
		add_index_string(&http_statuses, 118, "Connection timed out");
		add_index_string(&http_statuses, 200, "OK");
		add_index_string(&http_statuses, 201, "Created");
		add_index_string(&http_statuses, 202, "Accepted");
		add_index_string(&http_statuses, 203, "Non-Authoritative");
		add_index_string(&http_statuses, 204, "No Content");
		add_index_string(&http_statuses, 205, "Reset Content");
		add_index_string(&http_statuses, 206, "Partial Content");
		add_index_string(&http_statuses, 207, "Multi-Status");
		add_index_string(&http_statuses, 208, "Already Reported");
		add_index_string(&http_statuses, 210, "Content Different");
		add_index_string(&http_statuses, 226, "IM Used");
		add_index_string(&http_statuses, 300, "Multiple Choices");
		add_index_string(&http_statuses, 301, "Moved Permanently");
		add_index_string(&http_statuses, 302, "Found");
		add_index_string(&http_statuses, 303, "See Other");
		add_index_string(&http_statuses, 304, "Not Modified");
		add_index_string(&http_statuses, 305, "Use Proxy");
		add_index_string(&http_statuses, 306, "Reserved");
		add_index_string(&http_statuses, 307, "Temporary Redirect");
		add_index_string(&http_statuses, 308, "Permanent Redirect");
		add_index_string(&http_statuses, 310, "Too many Redirect");
		add_index_string(&http_statuses, 400, "Bad Request");
		add_index_string(&http_statuses, 401, "Unauthorized");
		add_index_string(&http_statuses, 402, "Payment Required");
		add_index_string(&http_statuses, 403, "Forbidden");
		add_index_string(&http_statuses, 404, "Not Found");
		add_index_string(&http_statuses, 405, "Method Not Allowed");
		add_index_string(&http_statuses, 406, "Not Acceptable");
		add_index_string(&http_statuses, 407, "Proxy Authentication Required");
		add_index_string(&http_statuses, 408, "Request Time-out");
		add_index_string(&http_statuses, 409, "Conflict");
		add_index_string(&http_statuses, 410, "Gone");
		add_index_string(&http_statuses, 411, "Length Required");
		add_index_string(&http_statuses, 412, "Precondition Failed");
		add_index_string(&http_statuses, 413, "Request Entity Too Large");
		add_index_string(&http_statuses, 414, "Request-URI Too Long");
		add_index_string(&http_statuses, 415, "Unsupported Media Type");
		add_index_string(&http_statuses, 416, "Requested range unsatisfiable");
		add_index_string(&http_statuses, 417, "Expectation failed");
		add_index_string(&http_statuses, 418, "I\'m a teapot");
		add_index_string(&http_statuses, 422, "Unprocessable entity");
		add_index_string(&http_statuses, 423, "Locked");
		add_index_string(&http_statuses, 424, "Method failure");
		add_index_string(&http_statuses, 425, "Unordered Collection");
		add_index_string(&http_statuses, 426, "Upgrade Required");
		add_index_string(&http_statuses, 428, "Precondition Required");
		add_index_string(&http_statuses, 429, "Too Many Requests");
		add_index_string(&http_statuses, 431, "Request Header Fields Too Large");
		add_index_string(&http_statuses, 449, "Retry With");
		add_index_string(&http_statuses, 450, "Blocked by Windows Parental Controls");
		add_index_string(&http_statuses, 500, "Internal Server Error");
		add_index_string(&http_statuses, 501, "Not Implemented");
		add_index_string(&http_statuses, 502, "Bad Gateway or Proxy Error");
		add_index_string(&http_statuses, 503, "Service Unavailable");
		add_index_string(&http_statuses, 504, "Gateway Time-out");
		add_index_string(&http_statuses, 505, "HTTP Version not supported");
		add_index_string(&http_statuses, 507, "Insufficient storage");
		add_index_string(&http_statuses, 508, "Loop Detected");
		add_index_string(&http_statuses, 509, "Bandwidth Limit Exceeded");
		add_index_string(&http_statuses, 510, "Not Extended");
		add_index_string(&http_statuses, 511, "Network Authentication Required");
		cheetah_update_static_property(cheetah_web_response_ce, "httpStatuses", &http_statuses);
	}
	RETURN_ZVAL(&http_statuses, 1, 0)
}
