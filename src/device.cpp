#include "device.h"
#include "config.h"
#include <FS.h>
#include <Logger.h>
#include <ArduinoJson.h>
#include "mqtt.h"

#ifdef SONOFF_BRIDGE
#include "RFSerial.h"
#else
#include <RCSwitch.h>
RCSwitch _rc = RCSwitch();
#endif

bool _device_learn = false;
String _device_id_cache = "";
unsigned long _device_id_cache_time = 0;

void device_setup(int pin)
{
#ifndef SONOFF_BRIDGE
    _rc.enableReceive(pin);
#endif
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
#ifdef SONOFF_BRIDGE
    if (rf_serial_available(4))
#else
    if (_rc.available())
#endif
    {
        unsigned long current_time = millis();

#ifdef SONOFF_BRIDGE
        String id = rf_serial_read();
        unsigned long proto = 10;
        unsigned long bit = 40;
        unsigned long delay = -1;
#else
        String id = String(_rc.getReceivedValue());
        unsigned long proto = _rc.getReceivedProtocol();
        unsigned long bit = _rc.getReceivedBitlength();
        unsigned long delay = _rc.getReceivedDelay();
        _rc.resetAvailable();
#endif

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
            dev.proto = proto;
            dev.bit = bit;
            dev.delay = delay;
            config_device_set(dev);
        }
        if (dev.id == NULL)
            return;

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