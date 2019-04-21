#ifndef app_cmd_h
#define app_cmd_h

#include <WString.h>

struct cmd_t
{
    String domain;
    String prop;
    String param;
    String cmd;
};

#define CMD_RESP_ACTION_NONE 0x00
#define CMD_RESP_ACTION_RESTART 0x01

struct cmd_resp_t
{
    bool success;
    uint8_t action;
    String msg;
};

cmd_resp_t cmd_execute(cmd_t cmd);

#endif