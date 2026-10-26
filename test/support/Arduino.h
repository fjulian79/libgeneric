/*
 * libgeneric, a collection of usefool macros and classes to be used in any
 * kind of C/C++ project.
 *
 * Copyright (C) 2026 Julian Friedrich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can file issues at https://github.com/fjulian79/libgeneric
 */

/*
 * Minimal replacement of the Arduino API, only used to run the unit tests of
 * Arduino dependent classes on the host. Provides just what these classes
 * need, millis() is controlled by the test via setMillis() and the output of
 * Serial is collected in Serial.Output.
 */

#ifndef TEST_SUPPORT_ARDUINO_H_
#define TEST_SUPPORT_ARDUINO_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>

typedef std::string String;

/**
 * Like the Print class of Arduino, all output ends up in write(uint8_t).
 */
class Print
{
    public:

        virtual ~Print() {}

        virtual size_t write(uint8_t c) = 0;

        /* Stops at the first character not written, like Arduino */
        virtual size_t write(const uint8_t *buffer, size_t size)
        {
            size_t n = 0;

            while (size-- && write(*buffer++))
                n++;

            return n;
        }

        size_t write(const char *str)
        {
            return str == 0 ? 0 : write((const uint8_t *) str, strlen(str));
        }

        size_t print(const char *str)
        {
            return write(str);
        }

        size_t print(char c)
        {
            return write((uint8_t) c);
        }

        size_t print(const String &str)
        {
            return write((const uint8_t *) str.c_str(), str.length());
        }

        size_t print(unsigned long value)
        {
            char tmp[24];

            snprintf(tmp, sizeof(tmp), "%lu", value);
            return write(tmp);
        }
};

/**
 * A Print collecting everything written to it, e.g. as console.
 */
class StringPrint : public Print
{
    public:

        using Print::write;

        size_t write(uint8_t c) override
        {
            Output += (char) c;
            return 1;
        }

        String Output;
};

inline StringPrint &testSerial(void)
{
    static StringPrint serial;
    return serial;
}

#define Serial testSerial()

inline uint32_t &testMillisValue(void)
{
    static uint32_t value = 0;
    return value;
}

inline void setMillis(uint32_t ms)
{
    testMillisValue() = ms;
}

inline uint32_t millis(void)
{
    return testMillisValue();
}

#endif /* TEST_SUPPORT_ARDUINO_H_ */
