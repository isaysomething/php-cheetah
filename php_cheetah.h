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

#ifndef PHP_CHEETAH_H
#define PHP_CHEETAH_H

extern zend_module_entry cheetah_module_entry;
#define phpext_cheetah_ptr &cheetah_module_entry

#define PHP_CHEETAH_VERSION "1.0.0"
#define PHP_CHEETAH_AUTHOR "yrssoft"
#define PHP_CHEETAH_AUTHOR_EAMIL "yrsemail@163.com"

#ifdef PHP_WIN32
#	define PHP_CHEETAH_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CHEETAH_API __attribute__ ((visibility("default")))
#else
#	define PHP_CHEETAH_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS( cheetah )
zval *application;
zval *container;
zval *logger;
char *controller_suffix;
char *default_controller;
char *action_suffix;
char *default_action;
ZEND_END_MODULE_GLOBALS( cheetah )

#define CHEETAH_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(cheetah, v)

#if defined(ZTS) && defined(COMPILE_DL_CHEETAH)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

PHP_MINIT_FUNCTION( cheetah );
PHP_MSHUTDOWN_FUNCTION( cheetah );
PHP_RINIT_FUNCTION( cheetah );
PHP_RSHUTDOWN_FUNCTION( cheetah );
PHP_MINFO_FUNCTION( cheetah );

extern ZEND_DECLARE_MODULE_GLOBALS( cheetah );

#endif
