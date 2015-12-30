typedef int BOOL;
#ifndef TRUE
#   define TRUE  1
#endif
#ifndef FALSE
#   define FALSE 0
#endif

#ifndef DIRECTORY_SEPARATOR
#define DIRECTORY_SEPARATOR  Z_STRVAL_P(zend_get_constant_str("DIRECTORY_SEPARATOR", 19))
#endif

#ifndef Cheetah_Kernel_Exception_H
#include "src/kernel/exception.h"
#endif

#ifndef Cheetah_Kernel_Invalid_Param_Exception_H
#include "src/kernel/invalid_param_exception.h"
#endif

#ifndef Cheetah_H
#include "src/cheetah.h"
#endif

#ifndef Cheetah_Kernel_Object_H
#include "src/kernel/object.h"
#endif

#ifndef Cheetah_Kernel_Component_H
#include "src/kernel/component.h"
#endif

#ifndef Cheetah_Di_Container_H
#include "src/di/container.h"
#endif

#ifndef Cheetah_Di_Service_Locator_H
#include "src/di/service_locator.h"
#endif

#ifndef Cheetah_Kernel_Module_H
#include "src/kernel/module.h"
#endif

#ifndef Cheetah_Kernel_Application_H
#include "src/kernel/application.h"
#endif

#ifndef Cheetah_Kernel_Controller_H
#include "src/kernel/controller.h"
#endif

#ifndef Cheetah_Kernel_View_H
#include "src/kernel/view.h"
#endif

#ifndef Cheetah_Kernel_Request_H
#include "src/kernel/request.h"
#endif

#ifndef Cheetah_Kernel_Response_H
#include "src/kernel/response.h"
#endif

#ifndef Cheetah_LOG_LOGGER_H
#include "src/log/logger.h"
#endif

#ifndef Cheetah_Web_Application_H
#include "src/web/application.h"
#endif

#ifndef Cheetah_Web_Controller_H
#include "src/web/controller.h"
#endif

#ifndef Cheetah_Web_View_H
#include "src/web/view.h"
#endif

#ifndef Cheetah_Web_Request_H
#include "src/web/request.h"
#endif

#ifndef Cheetah_Web_Response_H
#include "src/web/response.h"
#endif
