/*
 * libgeneric example: Task and UpTime
 *
 * Blinks the built in LED and prints the uptime every 5 seconds, without
 * blocking the main loop by delay().
 */

#include <Arduino.h>
#include <generic/task.hpp>
#include <generic/uptime.hpp>

UpTime upTime;

void printUptime(uint32_t now)
{
    Serial.print("Uptime: ");
    Serial.println(upTime.toString());
}

Task ledTask(500);
Task printTask(5000, true, printUptime);

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    upTime.begin();
}

void loop()
{
    uint32_t now = millis();

    upTime.loop();

    if (ledTask.isScheduled(now))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

    /* Calls printUptime() when scheduled */
    printTask.loop(now);
}
