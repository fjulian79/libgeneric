/*
 * libgeneric example: SysLog and EventLog
 *
 * Logs a message of a random severity every second. Messages up to info are
 * kept in the system log, errors and worse are printed to Serial as well.
 * Every message is also counted as event in an event log, which prints new
 * events live. On an ESP32 the system log is kept across a reset.
 *
 * Send 's' via the serial monitor to print the system log, 'e' to print the
 * event log.
 */

#include <Arduino.h>
#include <generic/syslog.hpp>
#include <generic/eventlog.hpp>
#include <generic/timestamp.hpp>
#include <generic/task.hpp>

#ifdef ESP32
/* Not initialized on startup, so the log is kept across a reset */
__NOINIT_ATTR static SysLogMemory<2048> logMemory;
#else
static SysLogMemory<2048> logMemory;
#endif

SysLog sysLog(logMemory);
char eventData[512];
EventLog eventLog(eventData, sizeof(eventData));
Task logTask(1000);
uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);

#ifdef ESP32
    sysLog.begin(esp_reset_reason() != ESP_RST_POWERON);
#else
    sysLog.begin(false);
#endif
    sysLog.setTimeStamp(timeStampSystem);
    sysLog.notice("--- Boot");

    eventLog.setTimeStamp(timeStampMillis);
    eventLog.setMonitor(&Serial);
}

void loop()
{
    if (logTask.isScheduled(millis()))
    {
        SysLog::Severity severity = (SysLog::Severity) random(SysLog::Crit, SysLog::Debug + 1);

        counter++;
        sysLog.log(severity, "Message %lu", (unsigned long) counter);
        eventLog.log("Logged message %lu as %s", (unsigned long) counter,
            SysLog::getSeverityName(severity));
    }

    if (Serial.available())
    {
        char c = Serial.read();

        if (c == 's')
            sysLog.printTo(Serial);
        else if (c == 'e')
            eventLog.printTo(Serial);
    }
}
