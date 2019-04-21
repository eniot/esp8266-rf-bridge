
#ifndef app_views_html_h
#define app_views_html_h

#include <Arduino.h>

String html_meta(String content);
String html_head(String title);
String html_row(String content);
String html_row(String col1, String col2);
String html_row_with_label(String label, String content);
String html_field(String type, String name, String label, String value = "", String className = "", bool required = true);
String html_radios(String name, String label, String values[], String displays[], size_t valCount, String value = "");
String html_button(String text, String type = "submit");
String html_select(String name, String label, String values[], String displays[], size_t valCount, String value = "");
String html_select_option(String value, String display, bool selected = false);
String html_select(String name, String label, String options);
String html_script();

struct html_menu_t
{
    String label;
    String link;
};

String html_menu(html_menu_t items[], size_t size, String active);
String html_display(String name, String value, String color = "black");
#endif