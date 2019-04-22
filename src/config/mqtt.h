#ifndef app_config_mqtt_h
#define app_config_mqtt_h

#include <WString.h>

struct config_mqtt_t
{
    bool mqtt;
    String server;
    int16_t port;
    String mqtt_username;
    String mqtt_password;
    String topic;
};

config_mqtt_t config_mqtt_get();
void config_mqtt_get(config_mqtt_t *data);
config_mqtt_t config_mqtt_default();
void config_mqtt_set(config_mqtt_t data);
bool config_mqtt_enabled();

#endif