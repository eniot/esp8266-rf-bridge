#ifndef app_views_portal_index_h
#define app_views_portal_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "menu.h"

String view_portal_index()
{
    device_list_t devs = config_device_list();
    config_mqtt_t mqtt = config_mqtt_get();

    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal") +
                     "<body>" +
                     "<div class='container'>" +
                     html_menu(menu_list, menu_size, "Home") +
                     "<div class='home-io'>" +
                     html_row(String("<h3>eniot rf bridge <small>") + VERSION + "</small></h3>") +
                     html_row_with_label("Connected devices", String(devs.count)) +
                     html_row_with_label("Network", WiFi.SSID() + "(" + WiFi.localIP().toString() + ")") +
                     html_row_with_label("MQTT", mqtt.server) +
                     html_row_with_label("Contact", "team@enbiso.com") +
                     "</div></div></body></html>";
    return content;
}

#endif