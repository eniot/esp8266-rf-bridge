#include "config/device.h"
#include <FS.h>
#include <ArduinoJson.h>
#include "Logger.h"

const char *_devicedir = "/device";

String _config_device_filepath(String id)
{
    char path[50];
    sprintf(path, "%s/%s.json", _devicedir, id.c_str());
    return String(path);
}

void config_device_set(device_t device)
{
    device.name = device.name == "" ? device.id : device.name;
    StaticJsonDocument<300> doc;
    doc["id"] = device.id;
    doc["name"] = device.name;
    doc["type"] = device.type;
    doc["proto"] = device.proto;
    doc["bit"] = device.bit;
    doc["delay"] = device.delay;
    String content;
    serializeJson(doc, content);
    String path = _config_device_filepath(device.id);
    if (SPIFFS.exists(path))
        SPIFFS.remove(path);
    File f = SPIFFS.open(path, "w");
    f.print(content);
    f.flush();
    f.close();
}

device_t config_device_get(String id, bool tech_spec)
{
    String path = _config_device_filepath(id);
    if (!SPIFFS.exists(path))
    {
        return device_t{};
    }
    File f = SPIFFS.open(path, "r");
    String content = f.readString();
    StaticJsonDocument<300> doc;
    deserializeJson(doc, content);
    device_t dev;
    dev.id = doc["id"].as<String>();
    dev.name = doc["name"].as<String>();
    dev.type = doc["type"].as<String>();
    if (tech_spec)
    {
        dev.bit = doc["bit"].as<int>();
        dev.proto = doc["proto"].as<int>();
        dev.delay = doc["delay"].as<int>();
    }
    return dev;
}

int _config_device_count()
{
    int count = 0;
    Dir d = SPIFFS.openDir(_devicedir);
    while (d.next())
    {
        count++;
    }
    return count;
}

device_list_t config_device_list()
{
    size_t count = _config_device_count();
    String *ids = new String[count];
    Dir d = SPIFFS.openDir(_devicedir);
    for (size_t i = 0; i < count; i++)
    {
        d.next();
        ids[i] = d.fileName().substring(strlen(_devicedir) + 1, d.fileName().length() - 5);
    }
    device_list_t ls;
    ls.count = count;
    ls.ids = ids;
    return ls;
}

void config_device_remove(String id)
{
    String path = _config_device_filepath(id);
    if (SPIFFS.exists(path))
        SPIFFS.remove(path);
}