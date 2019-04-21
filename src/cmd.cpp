#include "cmd.h"
#include "mqtt.h"
#include "config.h"
#include "network.h"
#include <Logger.h>

const char const_not_found[] = "not found";
const char const_failed[] = "failed";
const char const_set[] = "set";
const char const_get[] = "get";

cmd_resp_t _cmd_execute_io(cmd_t cmd);
cmd_resp_t _cmd_execute_mqtt(cmd_t cmd);
cmd_resp_t _cmd_execute_network(cmd_t cmd);
cmd_resp_t _cmd_execute_system(cmd_t cmd);

cmd_resp_t _ok(String msg, uint8_t action = CMD_RESP_ACTION_NONE)
{
    cmd_resp_t resp;
    resp.success = true;
    resp.msg = msg;
    resp.action = action;
    return resp;
}

cmd_resp_t _err(String msg)
{
    cmd_resp_t resp;
    resp.success = false;
    resp.msg = msg;
    return resp;
}

cmd_resp_t _invalid_cmd()
{
    return _err("invalid command");
}

cmd_resp_t cmd_execute(cmd_t cmd)
{
    if (cmd.domain.equalsIgnoreCase("mqtt"))
        return _cmd_execute_mqtt(cmd);
    if (cmd.domain.equalsIgnoreCase("network"))
        return _cmd_execute_network(cmd);
    if (cmd.domain.equalsIgnoreCase("system"))
        return _cmd_execute_system(cmd);
    return _err("invalid domain");
}

cmd_resp_t _cmd_execute_mqtt(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_mqtt");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase(MQTT_ACK))
        return _ok(MQTT_ACK);
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase(const_get))
        return _ok(mqtt_status());

    return _invalid_cmd();
}

#ifndef OTA_ENABLED
cmd_resp_t _cmd_execute_ota_update(cmd_t cmd)
{
    return _err("ota disabled");
}
#else
#include <ESP8266httpUpdate.h>
cmd_resp_t _cmd_execute_ota_update(cmd_t cmd)
{
    WiFiClient client;
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, cmd.param, VERSION);
    const char ok[] = "updated rebooting";
    cmd_resp_t res = _err("unknown update");
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        char logmsg[100];
        sprintf(logmsg, "%s - %s", const_failed, ESPhttpUpdate.getLastErrorString().c_str());
        LOG_ERROR(logmsg);
        res = _err(logmsg);
        break;

    case HTTP_UPDATE_NO_UPDATES:
        LOG_WARN(const_not_found);
        res = _err(const_not_found);
        break;

    case HTTP_UPDATE_OK:
        LOG_INFO(ok);
        res = _ok(ok, CMD_RESP_ACTION_RESTART);
        break;
    }
    return res;
}
#endif

cmd_resp_t _cmd_execute_system(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_system");
    if (cmd.cmd.equalsIgnoreCase("ver"))
        return _ok(VERSION);
    if (cmd.cmd.equalsIgnoreCase("update"))
        return _cmd_execute_ota_update(cmd);
    return _invalid_cmd();
}

cmd_resp_t _cmd_execute_network(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_network");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase(const_get))
        return _ok(network_status());
    return _invalid_cmd();
}