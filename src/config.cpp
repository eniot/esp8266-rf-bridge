#include <Arduino.h>
#include <FS.h>
#include "config.h"
#include <Logger.h>

void config_init()
{
    while (!SPIFFS.begin())
    {
    };
    const char *formatfile = "formatted";
    if (!SPIFFS.exists(formatfile))
    {
        LOG_INFO("Formatting SPIFFS")
        if (!SPIFFS.format())
        {
            LOG_ERROR("Error formatting SPIFFS");
            return;
        }
        File f = SPIFFS.open(formatfile, "w");
        f.print(1);
        f.close();
    }
}

void config_reset()
{
    LOG_INFO("Reseting to default settings");
    //Initial execution - Setup default values
    config_network_set(config_network_default());
    config_access_set(config_access_default());
    config_mqtt_set(config_mqtt_default());
    config_setup_reset();
}