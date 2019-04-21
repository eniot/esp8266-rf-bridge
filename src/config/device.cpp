#include "config/device.h"
#include <FS.h>
#include <ArduinoJson.h>
#include "Logger.h"

const char *_device_json = "device.json";

void config_device_set(String json)
{
    if (SPIFFS.exists(_device_json))
    {
        SPIFFS.remove(_device_json);
    }
    File file = SPIFFS.open(_device_json, "w");
    file.print(json);
    file.flush();
    file.close();
}

device_config_t config_device_get()
{
    if (!SPIFFS.exists(_device_json))
        return device_config_t{};

    File file = SPIFFS.open(_device_json, "r");
    String content = file.readString();
    file.close();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, content);
    JsonArray array = doc.as<JsonArray>();
    device_config_t conf;
    conf.size = array.size();
    conf.devices = new device_t[conf.size];
    for (size_t i = 0; i < conf.size; i++)
    {
        JsonArray obj = array[i].as<JsonArray>();
        device_t dev;
        dev.id = obj[0].as<String>();
        dev.name = obj[1].as<String>();
        dev.type = obj[2].as<String>();
        conf.devices[i] = dev;
    }
    doc.clear();
    return conf;
}