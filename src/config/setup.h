#ifndef app_config_setup_h
#define app_config_setup_h

#include "config/access.h"
#include "config/network.h"
#include "config/mqtt.h"

struct config_setup_t : config_network_t, config_access_t, config_mqtt_t
{
};

bool config_setup_complete();
void config_setup(config_setup_t data);
void config_setup_reset();
config_setup_t config_setup_get();
config_setup_t config_setup_log();

#endif