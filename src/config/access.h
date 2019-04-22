#ifndef app_config_access_h
#define app_config_access_h

#include <WString.h>

struct config_access_t
{
    String username;
    String password;
};

config_access_t config_access_get();
void config_access_get(config_access_t *data);
config_access_t config_access_default();
void config_access_set(config_access_t data);

#endif
