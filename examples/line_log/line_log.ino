/*
 * libgeneric example: LineRingBuffer as log
 *
 * Keeps the most recent log lines in a fixed buffer, the oldest lines are
 * removed as needed. Send any character via the serial monitor to print the
 * log.
 */

#include <Arduino.h>
#include <generic/lineringbuffer.hpp>
#include <generic/task.hpp>

char logData[256];
LineRingBuffer logBuffer(logData, sizeof(logData));
Task logTask(1000);
uint32_t counter = 0;

void printLog()
{
    char line[64];

    Serial.print("--- log, ");
    Serial.print(logBuffer.getUsed());
    Serial.print(" of ");
    Serial.print(logBuffer.getSize());
    Serial.println(" bytes used");
    for (size_t pos = logBuffer.first(); pos != LineRingBuffer::npos; pos = logBuffer.next(pos))
    {
        if (logBuffer.read(pos, line, sizeof(line)))
        {
            Serial.println(line);
        }
    }
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    char line[64];

    if (logTask.isScheduled(millis()))
    {
        snprintf(line, sizeof(line), "%lu ms: event %lu", millis(), (unsigned long) ++counter);
        logBuffer.append(line);
    }

    if (Serial.available())
    {
        while (Serial.available())
            Serial.read();
        printLog();
    }
}
