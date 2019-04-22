#ifndef app_controller_portal_h
#define app_controller_portal_h

#ifndef WEB_PORTAL_ENABLED
#define portal_controller()
#else

#include <ESP8266WebServer.h>
#include "network.h"
#include "views/portal_index.h"
#include "views/portal_other.h"
#include "views/portal_device.h"
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

    _webserver.on("/device", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentHTML, view_portal_device_list());
    });

    _webserver.on("/device/learn-toggle", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();

        device_learn_toggle();
        webserver_redirect("/device");
    });

    _webserver.addHandler(new StartUriRequestHandler("/device/", HTTP_GET, [](HTTPMethod method, String path) {
        if (!_check_auth())
            return _webserver.requestAuthentication();

        char devid[15];
        sscanf(path.c_str(), "/device/%s", devid);
        device_t dev = config_device_get(devid);
        if (dev.id == NULL)
        {
            webserver_redirect("/device");
            return;
        }
        _webserver.send(200, ContentHTML, view_portal_device(dev));
    }));

    _webserver.addHandler(new StartUriRequestHandler("/device/", HTTP_POST, [](HTTPMethod method, String path) {
        if (!_check_auth())
            return _webserver.requestAuthentication();

        char devid[15];
        sscanf(path.c_str(), "/device/%s", devid);

        device_t dev = config_device_get(devid);
        if (dev.id != NULL)
        {
            dev.name = _webserver.arg("name");
            dev.type = _webserver.arg("type");
            config_device_set(dev);
        }
        webserver_redirect("/device");
    }));

    _webserver.on("/setup", HTTP_GET, setup_get_handler);

    _webserver.on("/other", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentHTML, view_portal_other());
    });
}

#endif
#endif