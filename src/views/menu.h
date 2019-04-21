#ifndef app_views_menu_h
#define app_views_menu_h

#include "html.h"

const size_t menu_size = 5;
html_menu_t menu_list[menu_size] = {
    html_menu_t{"Home", "/"},
    html_menu_t{"Setup", "/setup"},
    html_menu_t{"Devices", "/device"},
    html_menu_t{"Other", "/other"}};

#endif