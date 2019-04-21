#ifndef app_config_device_h
#define app_config_device_h

#include <WString.h>

struct device_t
{
    String id;
    String name;
    String type;
};

struct device_config_t
{
    device_t *devices;
    size_t size;
};

void config_device_set(String json);
device_config_t config_device_get();

#endif