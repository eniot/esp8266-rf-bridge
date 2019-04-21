#ifndef lib_logger_h
#define lib_logger_h

#include <Arduino.h>

void logger_init();

#ifdef LOG_ENABLED
#define PRINT(message) Serial.print(message);
#define PRINTLN(message) Serial.println(message);
#define PRINTF(message, args...) Serial.printf(message, args);
#else
#define PRINT(message)
#define PRINTLN(message)
#define PRINTF(message, args...)
#endif

#define PRINTSTATUS(name, status) \
    PRINT("- ");                  \
    PRINT(name);                  \
    PRINT("\t: ");                \
    PRINTLN(status);
#define PRINTSTATUS2(name, status) \
    PRINT("- ");                   \
    PRINT(name);                   \
    PRINT("\t\t: ");               \
    PRINTLN(status);

#define LOG(type, message) \
    PRINT("[");            \
    PRINT(type);           \
    PRINT("] ");           \
    PRINT(millis());       \
    PRINT("\t");           \
    PRINTLN(message);
#define LOGF(type, message, args...) \
    PRINT("[");                      \
    PRINT(type);                     \
    PRINT("] ");                     \
    PRINT(millis());                 \
    PRINT("\t");                     \
    PRINTF(message, args);           \
    PRINTLN("");

#ifdef LOG_LEVEL_TRACE
#define LOG_ERROR(message) LOG("ERROR", message)
#define LOG_INFO(message) LOG("INFO ", message)
#define LOG_WARN(message) LOG("WARN ", message)
#define LOG_TRACE(message) LOG("TRACE", message)
#define LOGF_ERROR(message, args...) LOGF("ERROR", message, args)
#define LOGF_WARN(message, args...) LOGF("WARN", message, args)
#define LOGF_INFO(message, args...) LOGF("INFO", message, args)
#define LOGF_TRACE(message, args...) LOGF("TRACE", message, args)
#elif LOG_LEVEL_INFO
#define LOG_ERROR(message) LOG("ERROR", message)
#define LOG_WARN(message) LOG("WARN ", message)
#define LOG_INFO(message) LOG("INFO ", message)
#define LOG_TRACE(message)
#define LOGF_ERROR(message, args...) LOGF("ERROR", message, args)
#define LOGF_WARN(message, args...) LOGF("WARN", message, args)
#define LOGF_INFO(message, args...) LOGF("INFO", message, args)
#define LOGF_TRACE(message, args...)
#elif LOG_LEVEL_WARN
#define LOG_ERROR(message) LOG("ERROR", message)
#define LOG_WARN(message) LOG("WARN ", message)
#define LOG_INFO(message)
#define LOG_TRACE(message)
#define LOGF_ERROR(message, args...) LOGF("ERROR", message, args)
#define LOGF_WARN(message, args...) LOGF("WARN", message, args)
#define LOGF_INFO(message, args...)
#define LOGF_TRACE(message, args...)
#elif LOG_LEVEL_ERROR
#define LOG_ERROR(message) LOG("ERROR", message)
#define LOG_WARN(message)
#define LOG_INFO(message)
#define LOG_TRACE(message)
#define LOGF_ERROR(message, args...) LOGF("ERROR", message, args)
#define LOGF_WARN(message, args...)
#define LOGF_INFO(message, args...)
#define LOGF_TRACE(message, args...)
#else
#define LOG_ERROR(message)
#define LOG_WARN(message)
#define LOG_INFO(message)
#define LOG_TRACE(message)
#define LOGF_ERROR(message, args...)
#define LOGF_WARN(message, args...)
#define LOGF_INFO(message, args...)
#define LOGF_TRACE(message, args...)
#endif

#endif