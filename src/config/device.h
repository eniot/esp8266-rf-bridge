#ifndef app_config_device_h
#define app_config_device_h

#include <WString.h>

struct device_t
{
    String id;
    String name;
    String type;
    unsigned int proto;
    unsigned int delay;
    unsigned int bit;
};

struct device_list_t
{
    String *ids;
    size_t count;
};

void config_device_set(device_t device);
device_t config_device_get(String id, bool tech_spec = true);
void config_device_remove(String id);
device_list_t config_device_list();

#endif