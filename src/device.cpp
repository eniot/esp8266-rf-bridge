#include "device.h"
#include "config.h"
#include <FS.h>
#include <Logger.h>
#include <ArduinoJson.h>

void device_setup()
{
    config_device_set("[[\"11111\",\"Backyard Door\",\"door\"],[\"2222222\",\"Front Door\",\"pir\"]]");
    device_config_t conf = config_device_get();
    for (size_t i = 0; i < conf.size; i++)
    {
        PRINTF("id: %s, name: %s, type: %s", conf.devices[i].id.c_str(), conf.devices[i].name.c_str(), conf.devices[i].type.c_str());
    }
}