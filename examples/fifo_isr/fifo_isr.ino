/*
 * libgeneric example: Fifo between an interrupt and the main loop
 *
 * Each press of a button connected between BUTTON_PIN and GND is recorded
 * within the interrupt and printed by the main loop. The fifo needs no
 * locking, as the interrupt only writes and the main loop only reads.
 */

#include <Arduino.h>
#include <generic/fifo.hpp>

#define BUTTON_PIN      0

char eventData[32];
Fifo events(eventData, sizeof(eventData));

void onButton()
{
    char event = 'P';

    /* Dropped if the fifo is full */
    events.put(&event);
}

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButton, FALLING);
}

void loop()
{
    char event;

    while (events.get(&event))
    {
        Serial.print("Event: ");
        Serial.print(event);
        Serial.print(", ");
        Serial.print(events.getUsed());
        Serial.println(" more pending");
    }
}
