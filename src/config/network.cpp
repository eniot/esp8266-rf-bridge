#include "config/network.h"
#include "config/_constants.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <Logger.h>

const char *_network_json = "network.json";
const char *_hostfile = "hostfile";

config_network_t config_network_get()
{
    config_network_t data;
    config_network_get(&data);
    return data;
}

void config_network_get(config_network_t *data)
{
    if (!SPIFFS.exists(_network_json))
        return;

    File f = SPIFFS.open(_network_json, "r");
    String content = f.readString();
    f.close();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, content);

    data->wifi_ssid = doc["ssid"].as<String>();
    data->wifi_password = doc["password"].as<String>();
    data->dhcp = doc["dhcp"].as<bool>();
    data->ip.fromString(doc["ip"].as<String>());
    data->subnet.fromString(doc["subnet"].as<String>());
    data->gateway.fromString(doc["gateway"].as<String>());
    data->dns = doc["dns"].as<bool>();
    data->dns1.fromString(doc["dns1"].as<String>());
    data->dns1.fromString(doc["dns2"].as<String>());
}

String _config_ip_str(IPAddress ip)
{
    return ip.isSet() ? ip.toString() : "";
}

void config_network_set(config_network_t data)
{
    if (data.dhcp)
        data.ip = data.gateway = data.subnet = 0U;
    if (!data.dns)
        data.dns1 = data.dns2 = 0U;

    DynamicJsonDocument doc(512);
    doc["ssid"] = data.wifi_ssid;
    doc["password"] = data.wifi_password;
    doc["dhcp"] = data.dhcp;
    doc["ip"] = _config_ip_str(data.ip);
    doc["subnet"] = _config_ip_str(data.subnet);
    doc["gateway"] = _config_ip_str(data.gateway);
    doc["dns"] = data.dns;
    doc["dns1"] = _config_ip_str(data.dns1);
    doc["dns2"] = _config_ip_str(data.dns2);

    File f = SPIFFS.open(_network_json, "w");
    String content;
    serializeJson(doc, content);
    f.print(content);
    f.flush();
    f.close();
}

String config_name_get()
{
    if (SPIFFS.exists(_hostfile))
    {
        File f = SPIFFS.open(_hostfile, "r");
        String name = f.readString();
        f.close();
        return name;
    }
    else
    {
        String name;
        for (size_t i = 0; i < 4; i++)
        {
            name += char('a' + random(26));
        }
        name += "-iot";
        config_name_set(name);
        return name;
    }
}

void config_name_set(String name)
{
    if (SPIFFS.exists(_hostfile))
        SPIFFS.remove(_hostfile);
    File f = SPIFFS.open(_hostfile, "w");
    f.print(name);
    f.flush();
    f.close();
}

config_network_t config_network_default()
{
    config_network_t ndata;
    ndata.dhcp = true;
    ndata.dns = false;
    ndata.wifi_ssid = "";
    ndata.wifi_password = "";
    return ndata;
}