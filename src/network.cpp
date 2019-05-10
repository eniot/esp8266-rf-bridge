#include "network.h"
#include "config.h"
#include <Logger.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

config_network_t _cfg_network;

#ifdef SONOFF_BRIDGE
#define NETWORK_LED 13
#endif

void _network_sta()
{
    _cfg_network = config_network_get();

    if (WiFi.status() == WL_CONNECTED)
        WiFi.disconnect();

    WiFi.mode(WIFI_STA);
    WiFi.hostname(config_name_get());
    WiFi.config(_cfg_network.ip, _cfg_network.gateway, _cfg_network.subnet, _cfg_network.dns1, _cfg_network.dns2);

    LOG_INFO("Connecting to network...");
    WiFi.begin(_cfg_network.wifi_ssid.c_str(), _cfg_network.wifi_password.c_str());
    unsigned long currTime = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.status() == WL_CONNECT_FAILED || currTime >= 10000)
        {
            LOG_WARN("Connection failed!");
            LOG_INFO("Restarting...");
            ESP.restart();
        }
#ifdef SONOFF_BRIDGE
        digitalWrite(NETWORK_LED, LOW);
        delay(250);
        digitalWrite(NETWORK_LED, HIGH);
        delay(250);
#else
        delay(500);
#endif
        currTime += 500;
    }
#ifdef SONOFF_BRIDGE
    digitalWrite(NETWORK_LED, LOW);
#endif
    LOG_INFO("Connected to Network");
    PRINTSTATUS("IP", WiFi.localIP().toString() + " netmask " + WiFi.subnetMask().toString());
    PRINTSTATUS("GW", WiFi.gatewayIP().toString());
    PRINTSTATUS("DNS", WiFi.dnsIP().toString());
}

void _network_ap()
{
    LOG_INFO("Listen on activation AP...");
    String name = config_name_get();
    IPAddress apIP(192, 168, 1, 1);
    String ssid = "(inactive) " + name;
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid.c_str());
    PRINTSTATUS("SSID", ssid);
    PRINTSTATUS("Web", "http://192.168.1.1/");
}

void network_setup()
{
#ifdef SONOFF_BRIDGE
    pinMode(NETWORK_LED, OUTPUT);
#endif
    return config_setup_complete() ? _network_sta() : _network_ap();
}

void network_execute()
{
    if (WiFi.getMode() == WIFI_STA && WiFi.status() != WL_CONNECTED)
        network_setup();
}

String network_status()
{
    StaticJsonDocument<400> net;
    net["mode"] = WiFi.getMode() == WIFI_STA ? "STA" : "AP";
    if (WiFi.getMode() == WIFI_STA)
    {
        net["connected"] = WiFi.status() == WL_CONNECTED;
        net["ssid"] = WiFi.SSID();
        net["ip"] = WiFi.localIP().toString();
        net["netmask"] = WiFi.subnetMask().toString();
        net["gateway"] = WiFi.gatewayIP().toString();
        net["dhcp"] = _cfg_network.dhcp;
        net["dns"] = _cfg_network.dns ? "custom" : "DHCP";
        net["dns1"] = WiFi.dnsIP().toString();
        net["dns2"] = _cfg_network.dns2.toString();
    }
    else
    {
        net["ssid"] = WiFi.softAPSSID();
        net["ip"] = WiFi.softAPIP().toString();
    }
    String status;
    serializeJson(net, status);
    return status;
}