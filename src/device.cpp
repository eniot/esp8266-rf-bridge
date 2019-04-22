#include "device.h"
#include "config.h"
#include <FS.h>
#include <Logger.h>
#include <ArduinoJson.h>
#include <RCSwitch.h>
#include "mqtt.h"

bool _device_learn = false;
RCSwitch _rc = RCSwitch();
String _device_id_cache = "";
unsigned long _device_id_cache_time = 0;

void device_setup(int pin)
{
    _rc.enableReceive(pin);
}

void device_learn_toggle()
{
    _device_learn = !_device_learn;
}

bool device_learn()
{
    return _device_learn;
}

void device_execute()
{
    if (_rc.available())
    {
        unsigned long current_time = millis();
        String id = String(_rc.getReceivedValue());

        if ((current_time - _device_id_cache_time) < 3000 && id == _device_id_cache)
            return;

        _device_id_cache = id;
        _device_id_cache_time = current_time;

        device_t dev = config_device_get(id);
        if (dev.id == NULL && _device_learn)
        {
            LOGF_INFO("Adding device %s", id.c_str())
            dev.id = id;
            dev.name = id;
            dev.type = "generic";
            dev.proto = _rc.getReceivedProtocol();
            dev.bit = _rc.getReceivedBitlength();
            dev.delay = _rc.getReceivedDelay();
            config_device_set(dev);
        }

        StaticJsonDocument<300> doc;
        doc["id"] = dev.id;
        doc["name"] = dev.name;
        doc["type"] = dev.type;
        doc["trigger"] = true;
        String content;
        serializeJson(doc, content);
        LOG_TRACE(content.c_str())
        mqtt_send(content, "device/" + dev.name);
    }
}