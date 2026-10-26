/*
 * libgeneric example: crc8
 *
 * Calculates the CRC of the check string "123456789" with the default
 * parameters, CRC-8/SMBUS, and with the parameter set of the Dallas/Maxim
 * 1-Wire CRC. The expected values are 0xF4 and 0xA1.
 */

#include <Arduino.h>
#include <string.h>
#include <generic/crc8.hpp>

void setup()
{
    char data[] = "123456789";
    crc8 smbus;
    crc8 maxim(CRC8_MAXIM);

    Serial.begin(115200);
    delay(1000);

    Serial.print("CRC-8/SMBUS: 0x");
    Serial.println(smbus.calc(data, strlen(data)), HEX);
    Serial.print("CRC-8/MAXIM: 0x");
    Serial.println(maxim.calc(data, strlen(data)), HEX);

    /* Resetting the value keeps the parameters */
    maxim = 0;
    Serial.print("CRC-8/MAXIM: 0x");
    Serial.println(maxim.calc(data, strlen(data)), HEX);
}

void loop()
{
}
