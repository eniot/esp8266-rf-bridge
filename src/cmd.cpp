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

cmd_resp_t _cmd_execute_system(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_system");
    if (cmd.cmd.equalsIgnoreCase("ver"))
        return _ok(VERSION);
    return _invalid_cmd();
}

cmd_resp_t _cmd_execute_network(cmd_t cmd)
{
    LOG_TRACE("_cmd_execute_network");
    if (cmd.prop.equals("") && cmd.cmd.equalsIgnoreCase(const_get))
        return _ok(network_status());
    return _invalid_cmd();
}