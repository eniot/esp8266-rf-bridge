#ifndef lib_rfserial_h
#define lib_rfserial_h

#include <Arduino.h>

bool rf_serial_available(int length);
String rf_serial_read();

#endif