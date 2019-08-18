#include "RFSerial.h"

#define RF_MESSAGE_SIZE 9
#define RF_MAX_MESSAGE_SIZE (112 + 4)

unsigned char _rf_msg_buff[RF_MESSAGE_SIZE + 3] = {0};
unsigned char _rf_byte_count = 0;

void rf_serial_setup(int rate)
{
    Serial.begin(rate);
    while (!Serial)
    {
        ;
    }
}

bool rf_serial_available()
{
    static bool receiving = false;
    while (Serial.available())
    {
        yield();
        byte c = Serial.read();

        if (receiving)
        {
            if (c == RF_CODE_STOP && (_rf_byte_count == 1 || _rf_byte_count == RF_MESSAGE_SIZE + 1))
            {
                receiving = false;
                return true;
            }
            else if (_rf_byte_count <= RF_MESSAGE_SIZE)
            {
                _rf_msg_buff[_rf_byte_count++] = c;
            }
            else
            {
                receiving = false;
                return true;
            }
        }
        else if (c == RF_CODE_START)
        {
            _rf_byte_count = 0;
            receiving = true;
        }
    }
    return false;
}

rf_message rf_serial_read()
{
    rf_message msg;
    msg.action = _rf_msg_buff[0];
    msg.sync_time = _rf_msg_buff[1] << 8 | _rf_msg_buff[2]; // Sync time in uSec
    msg.low_time = _rf_msg_buff[3] << 8 | _rf_msg_buff[4];  // Low time in uSec
    msg.high_time = _rf_msg_buff[5] << 8 | _rf_msg_buff[6]; // High time in uSec
    msg.received_id = _rf_msg_buff[7] << 16 | _rf_msg_buff[8] << 8 | _rf_msg_buff[9];

    char stemp[16];
    snprintf_P(stemp, sizeof(stemp), PSTR("%06X"), msg.received_id);
    msg.stemp = String(stemp);

    _rf_byte_count = 0;
    return msg;
}

void rf_serial_send(uint8_t code)
{
    Serial.write(RF_CODE_START);
    Serial.write(code);
    Serial.write(RF_CODE_STOP);
}