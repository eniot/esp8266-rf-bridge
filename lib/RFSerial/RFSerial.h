#ifndef lib_rfserial_h
#define lib_rfserial_h

#include <Arduino.h>

#define RF_CODE_START 0xAA
#define RF_CODE_ACK 0xA0
#define RF_CODE_LEARN 0xA1
#define RF_CODE_LEARN_KO 0xA2
#define RF_CODE_LEARN_OK 0xA3
#define RF_CODE_RFIN 0xA4
#define RF_CODE_RFOUT 0xA5
#define RF_CODE_SNIFFING_ON 0xA6
#define RF_CODE_SNIFFING_OFF 0xA7
#define RF_CODE_RFOUT_NEW 0xA8
#define RF_CODE_LEARN_NEW 0xA9
#define RF_CODE_LEARN_KO_NEW 0xAA
#define RF_CODE_LEARN_OK_NEW 0xAB
#define RF_CODE_RFOUT_BUCKET 0xB0
#define RF_CODE_STOP 0x55

struct rf_message
{
    uint8_t action = 0;
    uint16_t sync_time = 0;
    uint16_t low_time = 0;
    uint16_t high_time = 0;
    uint32_t received_id = 0;
    String stemp;
};

void rf_serial_setup(int rate);
bool rf_serial_available();
rf_message rf_serial_read();
void rf_serial_send(uint8_t code);

#endif