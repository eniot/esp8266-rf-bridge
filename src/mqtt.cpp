#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "config.h"
#include <Logger.h>
#include "cmd.h"

bool _mqtt_enabled = false;
config_mqtt_t _mqtt_cfg;
char _mqtt_topic_out[50];
char _mqtt_topic_in_boardcast[10];
char _mqtt_topic_err[50];
char _mqtt_topic_in[50];
String _mqtt_clientid;

WiFiClient _wificlient;
PubSubClient _mqttclient(_wificlient);

void _callback(char *topic, byte *payload, unsigned int length);

void mqtt_setup()
{
    if (!config_setup_complete() || !config_mqtt_enabled())
        return;

    if (_mqttclient.connected())
        _mqttclient.disconnect();

    String net_name = config_name_get();

    _mqtt_enabled = true;
    _mqtt_cfg = config_mqtt_get();
    _mqtt_clientid = net_name;
    String topic = _mqtt_cfg.topic == "" ? net_name : _mqtt_cfg.topic;
    sprintf(_mqtt_topic_in, "cmd/%s/#", topic.c_str());
    sprintf(_mqtt_topic_out, "res/%s", topic.c_str());
    sprintf(_mqtt_topic_err, "err/%s", topic.c_str());
    strcpy(_mqtt_topic_in_boardcast, "cmd/*/#\0");
    PRINTSTATUS("MQTT", _mqtt_cfg.server + " port " + _mqtt_cfg.port);
    _mqttclient.setServer(_mqtt_cfg.server.c_str(), _mqtt_cfg.port);
    _mqttclient.setCallback(_callback);
}

bool mqtt_send(String payload, String topicsuffix)
{
    char topic[50];
    sprintf(topic, "%s/%s", _mqtt_topic_out, topicsuffix.c_str());
    return _mqttclient.publish(topic, payload.c_str());
}

bool mqtt_err(String payload, String topicsuffix)
{
    char topic[50];
    sprintf(topic, "%s/%s", _mqtt_topic_err, topicsuffix.c_str());
    return _mqttclient.publish(topic, payload.c_str());
}

bool _tryconnect()
{
    if (_mqttclient.connected())
        return true;

    LOG_INFO("Attempting MQTT connection...");
    if (!_mqttclient.connect(_mqtt_clientid.c_str(), _mqtt_cfg.mqtt_username.c_str(), _mqtt_cfg.mqtt_password.c_str()))
    {
        LOG_ERROR(String("MQTT connection Failed, rc: ") + _mqttclient.state());
        return false;
    }
    LOG_INFO("MQTT Connected");
    PRINTSTATUS("Topic IN", _mqtt_topic_in);
    PRINTSTATUS("Topic OUT", _mqtt_topic_out);
    PRINTSTATUS("Topic ERR", _mqtt_topic_err);
    mqtt_send(MQTT_ACK_INIT, "mqtt");
    _mqttclient.subscribe(_mqtt_topic_in);
    _mqttclient.subscribe(_mqtt_topic_in_boardcast);
    return true;
}

void mqtt_execute()
{
    if (!_mqtt_enabled || !_tryconnect())
        return;

    _mqttclient.loop();
}

cmd_t _mqtt_parse_cmd(const char *topic, byte *payload)
{
    cmd_t cmd;
    char tmp[4][32];
    size_t count = sscanf(topic, "%[^'/']/%[^'/']/%[^'/']/%s", tmp[0], tmp[1], tmp[2], tmp[3]);
    if (count >= 4)
        cmd.prop = tmp[3];
    if (count >= 3)
        cmd.domain = tmp[2];
    count = sscanf((char *)payload, "%[^':']:%s", tmp[0], tmp[1]);
    if (count >= 2)
        cmd.param = tmp[1];
    if (count >= 1)
        cmd.cmd = tmp[0];
    return cmd;
}

void _callback(char *topic, byte *payload, size_t length)
{
    LOG_TRACE("MQTT Message arrived");
    payload[length] = '\0';
    cmd_t cmd = _mqtt_parse_cmd(topic, payload);
    PRINTSTATUS("Domain", cmd.domain);
    PRINTSTATUS("Property", cmd.prop);
    PRINTSTATUS("Command", cmd.cmd);
    PRINTSTATUS("Parameter", cmd.param);
    cmd_resp_t resp = cmd_execute(cmd);
    String topicsuffix = cmd.domain;
    if (cmd.prop != "")
        topicsuffix += String("/") + cmd.prop;

    bool succeed = resp.success ? mqtt_send(resp.msg, topicsuffix) : mqtt_err(resp.msg, topicsuffix);
    if (!succeed)
    {
        LOG_ERROR("MQTT failed to send message");
    }
    switch (resp.action)
    {
    case CMD_RESP_ACTION_RESTART:
        ESP.restart();
        break;
    }
}

String mqtt_status()
{
    StaticJsonDocument<400> root;
    root["enabled"] = _mqtt_enabled;
    root["connected"] = _mqttclient.connected() == 1;
    root["state"] = mqtt_state_str();
    root["client_id"] = _mqtt_clientid;
    root["server"] = _mqtt_cfg.server;
    root["port"] = _mqtt_cfg.port;
    root["username"] = _mqtt_cfg.mqtt_username;
    root["topic_in"] = _mqtt_topic_in;
    root["topic_out"] = _mqtt_topic_out;
    root["topic_err"] = _mqtt_topic_err;
    String status;
    serializeJson(root, status);
    return status;
}

String mqtt_state_str()
{
    switch (_mqttclient.state())
    {
    case MQTT_CONNECTION_TIMEOUT:
        return "connection_timeout";
    case MQTT_CONNECTION_LOST:
        return "connection_lost";
    case MQTT_CONNECT_FAILED:
        return "connection_failed";
    case MQTT_DISCONNECTED:
        return "disconnected";
    case MQTT_CONNECTED:
        return "connected";
    case MQTT_CONNECT_BAD_PROTOCOL:
        return "bad_protocol";
    case MQTT_CONNECT_BAD_CLIENT_ID:
        return "bad_client_id";
    case MQTT_CONNECT_UNAVAILABLE:
        return "unavailable";
    case MQTT_CONNECT_BAD_CREDENTIALS:
        return "bad_credentials";
    case MQTT_CONNECT_UNAUTHORIZED:
        return "unauthorized";
    default:
        return "unknown_state";
    };
}