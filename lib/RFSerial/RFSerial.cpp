#include "RFSerial.h"

String _rf_content = "";
int _rf_byte_count = 0;

bool rf_serial_available(int length)
{
    int rflen = _rf_content.length();
    if (_rf_byte_count == 0 && rflen > 0)
    {
        _rf_content = "";
    }
    while (Serial.available())
    {
        char hex[2];
        sprintf(hex, "%.2X", Serial.read());
        _rf_content += hex;
        _rf_byte_count++;
    }
    return _rf_byte_count > length;
}

String rf_serial_read()
{
    _rf_byte_count = 0;
    return _rf_content;
}