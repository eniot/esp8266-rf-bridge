
#ifndef app_config_network_h
#define app_config_network_h

#include <WString.h>
#include <IPAddress.h>

struct config_network_t
{
    String wifi_ssid;
    String wifi_password;
    bool dhcp;
    IPAddress ip;
    IPAddress subnet;
    IPAddress gateway;
    bool dns;
    IPAddress dns1;
    IPAddress dns2;
};

config_network_t config_network_get();
void config_network_get(config_network_t *data);
config_network_t config_network_default();
void config_network_set(config_network_t data);
String config_name_get();
void config_name_set(String name);

#endif