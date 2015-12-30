/*
 +----------------------------------------------------------------------+
 | PHP Version 7                                                        |
 +----------------------------------------------------------------------+
 | Copyright (c) 1997-2015 The PHP Group                                |
 +----------------------------------------------------------------------+
 | This source file is subject to version 3.01 of the PHP license,      |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.php.net/license/3_01.txt                                  |
 | If you did not receive a copy of the PHP license and are unable to   |
 | obtain it through the world-wide-web, please send a note to          |
 | license@php.net so we can mail you a copy immediately.               |
 +----------------------------------------------------------------------+
 | Author:                                                              |
 +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "string.h"
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "zend_closures.h"
#include "ext/reflection/php_reflection.h"
#include "php_cheetah.h"
#include "cheetah_class.h"

#include "Zend/zend_operators.c"
#include "kernel.c"
#include "cheetah_class.c"

ZEND_DECLARE_MODULE_GLOBALS( cheetah )

PHP_INI_BEGIN() STD_PHP_INI_ENTRY("cheetah.controller_suffix", "Controller", PHP_INI_ALL, OnUpdateString, controller_suffix, zend_cheetah_globals, cheetah_globals)
STD_PHP_INI_ENTRY("cheetah.default_controller", "Site", PHP_INI_ALL, OnUpdateString, default_controller, zend_cheetah_globals, cheetah_globals)
STD_PHP_INI_ENTRY("cheetah.action_suffix", "Action", PHP_INI_ALL, OnUpdateString, action_suffix, zend_cheetah_globals, cheetah_globals)
STD_PHP_INI_ENTRY("cheetah.default_action", "index", PHP_INI_ALL, OnUpdateString, default_action, zend_cheetah_globals, cheetah_globals)
PHP_INI_END()

PHP_FUNCTION(cheetah_version) {
	RETURN_STRING( PHP_CHEETAH_VERSION )
}

static void php_cheetah_init_globals( zend_cheetah_globals *cheetah_globals ) {
}

PHP_MINIT_FUNCTION(cheetah) {
	ZEND_BEGIN_MODULE_GLOBALS(cheetah)
	ZEND_END_MODULE_GLOBALS(cheetah)
	ZEND_INIT_MODULE_GLOBALS( cheetah , php_cheetah_init_globals , NULL );
	REGISTER_INI_ENTRIES();
	CHEETAH_REGISTER_CLASS()
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(cheetah) {
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(cheetah) {
#if defined(COMPILE_DL_CHEETAH) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(cheetah) {
	return SUCCESS;
}

PHP_MINFO_FUNCTION(cheetah) {
	php_info_print_table_start();
	php_info_print_table_header( 2 , "cheetah support" , "enabled" );
	php_info_print_table_row( 2 , "version" , PHP_CHEETAH_VERSION );
	php_info_print_table_row( 2 , "author" , PHP_CHEETAH_AUTHOR );
	php_info_print_table_row( 2 , "email" , PHP_CHEETAH_AUTHOR_EAMIL );

	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

const zend_function_entry cheetah_functions[] = {
PHP_FE(cheetah_version, NULL)
PHP_FE_END };

zend_module_entry cheetah_module_entry = {
STANDARD_MODULE_HEADER , "cheetah" , cheetah_functions ,
PHP_MINIT(cheetah) ,
PHP_MSHUTDOWN(cheetah) ,
PHP_RINIT(cheetah) ,
PHP_RSHUTDOWN(cheetah) ,
PHP_MINFO(cheetah) ,
PHP_CHEETAH_VERSION ,
STANDARD_MODULE_PROPERTIES };

#ifdef COMPILE_DL_CHEETAH
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(cheetah)
#endif
