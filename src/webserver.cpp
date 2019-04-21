#include "webserver.h"
#include "controller/portal.h"
#include "controller/setup.h"
#include "controller/api.h"
#include <Logger.h>
#include "config.h"
#include "mqtt.h"
#include "network.h"

ESP8266WebServer _webserver(80);

#ifndef OTA_ENABLED
#define _setup_ota_server() 
#else
#include <ESP8266HTTPUpdateServer.h>
ESP8266HTTPUpdateServer _httpUpdater;
void _setup_ota_server() 
{
    String password = config_access_get().access;
    password.trim();
    _httpUpdater.setup(&_webserver, "/ota", "admin", password);
}
#endif

void webserver_setup()
{
    if (config_setup_complete())
    {
        portal_controller();
        _setup_ota_server();
    }
    else
        setup_controller();    

    api_controller();

    _webserver.onNotFound([]() {
        webserver_redirect("/");
    });

    _webserver.begin();
}

void webserver_redirect(String path)
{
    _webserver.sendHeader("Location", path, true); //Redirect to our html web page
    _webserver.send(302, "text/plane", "");
}

void webserver_execute()
{
    _webserver.handleClient();
}

bool StartUriRequestHandler::canHandle(HTTPMethod method, String uri)
{
    return uri != NULL && uri.startsWith(_startUri) && method == _method;
}

bool StartUriRequestHandler::handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
{
    _handler(requestMethod, requestUri);
    return true;
}