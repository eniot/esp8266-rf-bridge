#ifndef app_views_portal_device_h
#define app_views_portal_device_h

#include <Arduino.h>
#include "views/html.h"
#include "views/menu.h"
#include "device.h"

String view_portal_device_list()
{
    device_list_t list = config_device_list();

    String learn_form = "<form method='POST' action='/device/learn-toggle'>" +
                        html_button(device_learn() ? "Learning... | Stop" : "Learn Paused | Start") + "</form>";

    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal") +
                     "<body>" +
                     "<div class='container'>" +
                     html_menu(menu_list, menu_size, "Devices") +
                     html_row("", learn_form) +
                     "<div class='home-device'>";
    for (size_t i = 0; i < list.count; i++)
    {
        device_t dev = config_device_get(list.ids[i], false);
        char link[50];
        sprintf(link, "<a href='/device/%s'>%s</a>", dev.id.c_str(), dev.id.c_str());
        content += html_row(link, dev.name);
    }

    content += "</div></div></body></html>";
    return content;
}

String view_portal_device(device_t dev)
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal") +
                     "<body>" +
                     "<div class='container'>" +
                     html_menu(menu_list, menu_size, "Devices") +
                     "<div class='home-device-item'><form method='POST' action='/device/" + dev.id + "'>" +
                     html_display("Device ID", dev.id) +
                     html_field("text", "name", "Name", dev.name) +
                     html_field("text", "type", "Type", dev.type) +
                     html_display("Protocol", String(dev.proto)) +
                     html_display("Bit Length", String(dev.bit)) +
                     html_display("Delay", String(dev.delay)) +
                     html_button("Save") +
                     "</form>" +
                     html_row("", "<a onclick=\"{fetch('/api/device/" + dev.id + "', {method: 'DELETE'}).then(()=>{location='/device'})}\">Remove this device</a>") +
                     "</div></div></body></html>";
    return content;
}

#endif