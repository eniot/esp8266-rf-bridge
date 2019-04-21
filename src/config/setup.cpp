#include "config/setup.h"
#include "config/_constants.h"
#include <Logger.h>
#include <FS.h>

void config_setup(config_setup_t data)
{
    LOG_TRACE("config_setup");
    config_network_set(data);
    config_access_set(data);
    config_mqtt_set(data);

    File f = SPIFFS.open(".activated", "w");
    f.close();

    LOG_INFO("Device Activated.");
}

bool config_setup_complete()
{
    return SPIFFS.exists(".activated");
}

void config_setup_reset()
{
    LOG_TRACE("config_setup_reset");
    if (config_setup_complete())
        SPIFFS.remove(".activated");
}

config_setup_t config_setup_get()
{
    LOG_TRACE("config_setup_get");
    config_setup_t data;
    config_access_get(&data);
    config_network_get(&data);
    config_mqtt_get(&data);
    return data;
}

config_setup_t config_setup_log()
{
    config_setup_t data = config_setup_t();
    PRINTSTATUS("Name", config_name_get())
    PRINTSTATUS("WiFi", data.wifi_ssid)

    PRINTSTATUS("DHCP", data.dhcp ? "Enabled" : "Disabled")
    if (!data.dhcp)
    {
        PRINTSTATUS("-IP", data.ip.toString() + " netmask " + data.subnet.toString())
        PRINTSTATUS("-GW", data.gateway.toString())
    }
    PRINTSTATUS("DNS", data.dns ? "Custom" : "DHCP")
    if (data.dns)
    {
        PRINTSTATUS("-DNS1", data.dns1.toString())
        PRINTSTATUS("-DNS2", data.dns1.toString())
    }
    PRINTSTATUS("MQTT", data.mqtt ? "Enabled" : "Disabled");
    if (data.mqtt)
    {
        PRINTSTATUS("-Server", data.server + ":" + String(data.port))
        PRINTSTATUS("-Topic", data.topic)
    }
    return data;
}
