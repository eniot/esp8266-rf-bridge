#ifndef app_views_portal_other_h
#define app_views_portal_other_h

#include <Arduino.h>
#include "views/html.h"
#include "views/menu.h"

String view_portal_other()
{
    String content = "<html lang=\"en\">" +
                     html_head("IOT Portal") +
                     "<body>" +
                     "<div class='container'>" +
                     html_menu(menu_list, menu_size, "Other") +
                     "<div class='home-other'><form method='POST' action='/ota' enctype='multipart/form-data'>" +
                     html_field("file", "update", "Select Firmware (bin file)", "") +
                     html_button("Update") +
                     "</form></div></div></body></html>";
    return content;
}

#endif