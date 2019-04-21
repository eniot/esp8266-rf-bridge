#include "config/access.h"
#include <FS.h>

config_access_t *_access_cache = NULL;
const char *_access_json = "access.json";

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
        if (!SPIFFS.exists(_access_json))
            return;
        File f = SPIFFS.open(_access_json, "r");
        data->access = f.readString();
        f.close();
        _access_cache = new config_access_t();
        *_access_cache = *data;
    }
}

void config_access_set(config_access_t data)
{
    File f = SPIFFS.open(_access_json, "w");
    f.print(data.access);
    f.flush();
    f.close();
    delete _access_cache;
}

config_access_t config_access_default()
{
    config_access_t adata;
    adata.access = "";
    return adata;
}