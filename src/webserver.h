#ifndef app_webserver_h
#define app_webserver_h

#include <ESP8266WebServer.h>

static const char ContentJSON[] = "application/json";
static const char ContentHTML[] = "text/html";

void webserver_setup();
void webserver_execute();
void webserver_redirect(String path);

extern ESP8266WebServer _webserver;

typedef std::function<void(HTTPMethod, String)> THandlerFunction;

class StartUriRequestHandler : public RequestHandler
{
    String _startUri;
    HTTPMethod _method;
    THandlerFunction _handler;
    
    public:
    StartUriRequestHandler(String startUri, HTTPMethod method, THandlerFunction handler) {
        _startUri = startUri;
        _method = method;
        _handler = handler;
    }

    bool canHandle(HTTPMethod method, String uri);
    bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri);
};

#endif