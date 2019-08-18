#include <Logger.h>

void logger_init()
{
#ifdef LOG_ENABLED
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    PRINTLN()
#endif
}