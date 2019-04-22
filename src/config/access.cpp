#include "config/access.h"
#include <FS.h>
#include <ArduinoJson.h>

config_access_t *_access_cache = NULL;
const char *_accessfile = ".access";

config_access_t config_access_get()
{
    config_access_t data;
    config_access_get(&data);
    return data;
}

void config_access_get(config_access_t *data)
{
    if (_access_cache != NULL)
        *data = *_access_cache;
    else
    {
        if (!SPIFFS.exists(_accessfile))
            return;
        File f = SPIFFS.open(_accessfile, "r");
        String content = f.readString();
        StaticJsonDocument<200> doc;
        deserializeJson(doc, content);
        data->username = doc["username"].as<String>();
        data->password = doc["password"].as<String>();
        f.close();
        _access_cache = new config_access_t();
        *_access_cache = *data;
    }
}

void config_access_set(config_access_t data)
{
    StaticJsonDocument<200> doc;
    doc["username"] = data.username;
    doc["password"] = data.password;
    String content;
    serializeJson(doc, content);
    File f = SPIFFS.open(_accessfile, "w");
    f.print(content);
    f.flush();
    f.close();
    delete _access_cache;
}

config_access_t config_access_default()
{
    config_access_t adata;
    adata.username = "admin";
    adata.password = "";
    return adata;
}