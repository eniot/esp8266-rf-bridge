#include "config/mqtt.h"
#include "config/_constants.h"
#include "config/network.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <Logger.h>

const char *_mqtt_json = "mqtt.json";

bool config_mqtt_enabled()
{
    return config_mqtt_get().mqtt;
}

config_mqtt_t config_mqtt_get()
{
    config_mqtt_t data;
    config_mqtt_get(&data);
    return data;
}

void config_mqtt_get(config_mqtt_t *data)
{
    if (!SPIFFS.exists(_mqtt_json))
        return;

    File f = SPIFFS.open(_mqtt_json, "r");
    String content = f.readString();
    f.close();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, content);

    data->mqtt = doc["mqtt"].as<bool>();
    data->server = doc["server"].as<String>();
    data->port = doc["port"].as<int>();
    data->mqtt_username = doc["username"].as<String>();
    data->mqtt_password = doc["password"].as<String>();
    data->topic = doc["topic"].as<String>();

    doc.clear();
}

void config_mqtt_set(config_mqtt_t data)
{
    DynamicJsonDocument doc(512);
    doc["mqtt"] = data.mqtt;
    doc["server"] = data.server;
    doc["port"] = data.port;
    doc["username"] = data.mqtt_username;
    doc["password"] = data.mqtt_password;
    doc["topic"] = data.topic;

    if (SPIFFS.exists(_mqtt_json))
        SPIFFS.remove(_mqtt_json);

    File f = SPIFFS.open(_mqtt_json, "w");
    String content;
    serializeJson(doc, content);
    f.print(content);
    f.flush();
    f.close();
}

config_mqtt_t config_mqtt_default()
{
    config_mqtt_t mdata;
    mdata.mqtt = false;
    mdata.server = "mqtt.local";
    mdata.port = 1883;
    mdata.topic = "";
    mdata.mqtt_username = "";
    mdata.mqtt_password = "";
    return mdata;
}