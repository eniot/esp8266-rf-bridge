#ifndef app_controller_api_h
#define app_controller_api_h

#ifndef WEB_API_ENABLED
#define api_controller()
#else

#include "webserver.h"
#include "mqtt.h"
#include "network.h"

void _api_resp_true()
{
    _webserver.send(200, ContentJSON, "true");
}

void api_controller()
{
    _webserver.on("/api", HTTP_GET, [] {
        _webserver.send(200, ContentJSON, "eniot API");
    });

    // MQTT APIs
    _webserver.on("/api/mqtt", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentJSON, mqtt_status());
    });
    _webserver.on("/api/mqtt", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_mqtt_t data;
        _update_mqtt_from_web(&data);
        config_mqtt_set(data);
        _api_resp_true();
        mqtt_setup();
    });

    _webserver.addHandler(new StartUriRequestHandler("/api/device/", HTTP_DELETE, [](HTTPMethod method, String path) {
        if (!_check_auth())
            return _webserver.requestAuthentication();

        char devid[15];
        sscanf(path.c_str(), "/api/device/%s", devid);

        config_device_remove(devid);
        _api_resp_true();
    }));

    // Network APIs
    _webserver.on("/api/network", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentJSON, network_status());
    });
    _webserver.on("/api/network", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_network_t data;
        config_name_set(_webserver.arg("name"));
        _update_network_from_web(&data);
        config_network_set(data);
        _api_resp_true();
        network_setup();
    });

    // Access APIs
    _webserver.on("/api/access", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_access_t data;
        _update_access_from_web(&data);
        config_access_set(data);
        _api_resp_true();
    });

    // Setup APIs
    _webserver.on("/api/setup", HTTP_POST, [] {
        if (config_setup_complete() && !_check_auth())
            return _webserver.requestAuthentication();
        config_setup_t data;
        _update_setup_from_web(&data);
        config_setup(data);
        _api_resp_true();
        ESP.restart();
    });
}

#endif
#endif