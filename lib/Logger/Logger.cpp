#include <Logger.h>

void logger_init()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    Serial.println();
}