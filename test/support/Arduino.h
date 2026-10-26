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
 * need, millis() is controlled by the test via setMillis().
 */

#ifndef TEST_SUPPORT_ARDUINO_H_
#define TEST_SUPPORT_ARDUINO_H_

#include <stdint.h>
#include <stdio.h>
#include <string>

typedef std::string String;

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
