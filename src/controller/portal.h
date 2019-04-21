#ifndef app_controller_portal_h
#define app_controller_portal_h

#ifndef WEB_PORTAL_ENABLED
#define portal_controller()
#else

#include <ESP8266WebServer.h>
#include "network.h"
#include "views/portal_index.h"
#include "views/portal_other.h"
#include "controller/setup.h"
#include "controller/_utils.h"
#include "webserver.h"
#include "mqtt.h"

void portal_controller()
{
    _webserver.on("/", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentHTML, view_portal_index());
    });

    _webserver.on("/setup", HTTP_GET, setup_get_handler);

    _webserver.on("/other", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentHTML, view_portal_other());
    });
}

#endif
#endif