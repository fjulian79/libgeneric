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
 * Only timeStampMillis() is tested, timeStampSystem() depends on the system
 * time of the ESP32 and the Raspberry Pi Pico and is not available on the
 * host.
 */

#include <unity.h>
#include <Arduino.h>

#include "generic/timestamp.hpp"

static String timeStamp(uint32_t ms)
{
    StringPrint out;

    setMillis(ms);
    timeStampMillis(out);
    return out.Output;
}

void setUp(void)
{
    setMillis(0);
}

void tearDown(void) {}

void test_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("00:00:00.000", timeStamp(0).c_str());
}

void test_fields(void)
{
    TEST_ASSERT_EQUAL_STRING("01:02:03.004", timeStamp(3723004).c_str());
    TEST_ASSERT_EQUAL_STRING("00:00:00.999", timeStamp(999).c_str());
    TEST_ASSERT_EQUAL_STRING("00:00:59.000", timeStamp(59000).c_str());
    TEST_ASSERT_EQUAL_STRING("00:59:00.000", timeStamp(3540000).c_str());
}

void test_hours_are_not_limited_to_a_day(void)
{
    TEST_ASSERT_EQUAL_STRING("25:00:00.000", timeStamp(25 * 3600000UL).c_str());
}

void test_three_digit_hours_from_100_hours_on(void)
{
    TEST_ASSERT_EQUAL_STRING("99:59:59.999", timeStamp(100 * 3600000UL - 1).c_str());
    TEST_ASSERT_EQUAL_STRING("100:00:00.000", timeStamp(100 * 3600000UL).c_str());
}

void test_max_millis(void)
{
    /* The last value before millis() wraps after 49.7 days */
    TEST_ASSERT_EQUAL_STRING("1193:02:47.295", timeStamp(0xFFFFFFFFUL).c_str());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_zero);
    RUN_TEST(test_fields);
    RUN_TEST(test_hours_are_not_limited_to_a_day);
    RUN_TEST(test_three_digit_hours_from_100_hours_on);
    RUN_TEST(test_max_millis);
    return UNITY_END();
}
