#ifndef app_views_setup_index_h
#define app_views_setup_index_h

#include <Arduino.h>
#include "html.h"
#include "config.h"
#include "mqtt.h"
#include "views/menu.h"

String view_setup_index_content(config_setup_t data, String actionTitle)
{
    String mqtt_state = mqtt_state_str();
    bool net_connected = WiFi.status() == WL_CONNECTED;

    String enable_values[2] = {"1", "0"};
    String enable_displays[2] = {"Yes", "No"};
    return "<form method=\"POST\" action=\"/api/setup\">" +
           html_field("text", "username", "Username", data.username) +
           html_field("text", "password", "Password", data.password) +
           "<br/>" +
           html_display("Status", net_connected ? "connected" : "disconnected", net_connected ? "green" : "orange") +
           (net_connected ? html_display("Local IP", WiFi.localIP().toString()) : "") +
           html_field("text", "name", "Device Name", config_name_get()) +
           html_field("text", "wifi_ssid", "WiFi SSID", data.wifi_ssid) +
           html_field("text", "wifi_password", "WiFi Password", data.wifi_password) +
           html_radios("dhcp", "DHCP Enable", enable_values, enable_displays, 2, data.dhcp ? "1" : "0") +
           "<div id=\"dhcp-section\">" +
           html_field("text", "ip", "IP Address", html_ip_str(data.ip), "dhcp-input", false) +
           html_field("text", "subnet", "Subnet Mask", html_ip_str(data.subnet), "dhcp-input", false) +
           html_field("text", "gateway", "Default Gateway", html_ip_str(data.gateway), "dhcp-input", false) +
           "</div>" +
           html_radios("dns", "Custom DNS", enable_values, enable_displays, 2, data.dns ? "1" : "0") +
           "<div id=\"dns-section\">" +
           html_field("text", "dns1", "Primary DNS", html_ip_str(data.dns1), "dns-input", false) +
           html_field("text", "dns2", "Secondary DNS", html_ip_str(data.dns2), "dns-input", false) +
           "</div><br/>" +
           html_radios("mqtt", "MQTT Enable", enable_values, enable_displays, 2, data.mqtt ? "1" : "0") +
           "<div id=\"mqtt-section\">" +
           html_display("Status", mqtt_state, mqtt_state == "connected" ? "green" : "orange") +
           html_field("text", "server", "Server (Broker)", data.server, "mqtt-input") +
           html_field("number", "port", "Port", String(data.port), "mqtt-input", false) +
           html_field("text", "topic", "Topic", data.topic, "mqtt-input", false) +
           html_field("text", "mqtt_username", "Username", data.mqtt_username, "mqtt-input", false) +
           html_field("text", "mqtt_password", "Password", data.mqtt_password, "mqtt-input", false) +
           "</div>" +
           html_button(actionTitle) +
           "</form>";
}

String view_setup_index(config_setup_t data)
{
    String menu = config_setup_complete() ? html_menu(menu_list, menu_size, "Setup") : "";
    return "<html lang=\"en\">" +
           html_head("IOT Portal - Setup") +
           "<body><div class='container'>" + menu +
           view_setup_index_content(data, "Activate") +
           "</div>" +
           html_script() +
           "<script>dnsChange('" + (data.dns ? "1" : "0") + "');dhcpChange('" + (data.dhcp ? "1" : "0") + "');mqttChange('" + (data.mqtt ? "1" : "0") + "');</script>" +
           "</body></html>";
}

#endif