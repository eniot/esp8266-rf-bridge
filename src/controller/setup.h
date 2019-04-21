#ifndef app_controller_setup_h
#define app_controller_setup_h

#ifndef WEB_SETUP_ENABLED
#define setup_controller()
#define setup_get_handler()
#else

#include <ESP8266WebServer.h>
#include "config.h"
#include "views/setup_index.h"
#include "controller/_utils.h"
#include "webserver.h"

void setup_get_handler()
{
    if (!_check_auth())
        return _webserver.requestAuthentication();
    _webserver.send(200, ContentHTML, view_setup_index(config_setup_get()));
}

void setup_controller()
{
    _webserver.on("/", HTTP_GET, setup_get_handler);    
}

#endif
#endif