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

#include <unity.h>
#include <Arduino.h>

#include "generic/uptime.hpp"

#define MS_PER_DAY      86400000ULL
#define MS_OVERFLOW     0x100000000ULL

void setUp(void)
{
    setMillis(0);
}

void tearDown(void) {}

/**
 * The expected string for the given uptime, computed independent of UpTime.
 */
static String expected(uint64_t ms)
{
    char tmp[48];

    snprintf(tmp, sizeof(tmp), "%llu days, %02u:%02u:%02u.%03u",
        (unsigned long long) (ms / MS_PER_DAY),
        (unsigned) ((ms / 3600000) % 24),
        (unsigned) ((ms / 60000) % 60),
        (unsigned) ((ms / 1000) % 60),
        (unsigned) (ms % 1000));

    return String(tmp);
}

void test_format(void)
{
    UpTime up;

    TEST_ASSERT_EQUAL_STRING("0 days, 00:00:00.000", up.toString().c_str());
    up.set(MS_PER_DAY + 3600000 + 60000 + 1000 + 1);
    TEST_ASSERT_EQUAL_STRING("1 days, 01:01:01.001", up.toString().c_str());
    up.set(MS_PER_DAY - 1);
    TEST_ASSERT_EQUAL_STRING("0 days, 23:59:59.999", up.toString().c_str());
    up.set(100000 * MS_PER_DAY);
    TEST_ASSERT_EQUAL_STRING("100000 days, 00:00:00.000", up.toString().c_str());
}

void test_begin_starts_at_current_millis(void)
{
    UpTime up;

    setMillis(5000);
    up.begin();
    TEST_ASSERT_EQUAL_STRING(expected(5000).c_str(), up.toString().c_str());
}

void test_loop_accumulates(void)
{
    UpTime up;

    up.begin();
    setMillis(1500);
    up.loop();
    setMillis(2750);
    up.loop();
    TEST_ASSERT_EQUAL_STRING(expected(2750).c_str(), up.toString().c_str());
}

void test_set_continues_from_value(void)
{
    UpTime up;

    up.begin();
    setMillis(500);
    up.set(1000);
    setMillis(1500);
    up.loop();
    TEST_ASSERT_EQUAL_STRING(expected(2000).c_str(), up.toString().c_str());
}

void test_reset(void)
{
    UpTime up;

    setMillis(10000);
    up.begin();
    up.reset();
    setMillis(10500);
    up.loop();
    TEST_ASSERT_EQUAL_STRING(expected(500).c_str(), up.toString().c_str());
}

/**
 * millis() overflows after 49.7 days, the uptime has to continue across it.
 */
void test_millis_overflow(void)
{
    UpTime up;

    setMillis(0xFFFFF000);
    up.begin();
    setMillis(0x00001000);
    up.loop();
    TEST_ASSERT_EQUAL_STRING(expected(0xFFFFF000ULL + 0x2000).c_str(),
        up.toString().c_str());
}

/**
 * Uptimes beyond several millis() overflows, here about 149 days, as long as
 * loop() is called at least once per overflow period.
 */
void test_multiple_millis_overflows(void)
{
    UpTime up;
    uint64_t total = 0;

    up.begin();
    for (int i = 0; i < 6; i++)
    {
        total += 0x80000000ULL;
        setMillis((uint32_t) total);
        up.loop();
    }

    TEST_ASSERT_EQUAL_STRING(expected(3 * MS_OVERFLOW).c_str(), up.toString().c_str());
}

/**
 * Documents the limitation: If loop() is not called within one millis()
 * overflow period, the full periods in between are lost.
 */
void test_loop_has_to_run_within_overflow_period(void)
{
    UpTime up;

    setMillis(1000);
    up.begin();
    /* Exactly one overflow period later millis() shows the same value */
    setMillis(1000);
    up.loop();
    TEST_ASSERT_EQUAL_STRING(expected(1000).c_str(), up.toString().c_str());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_format);
    RUN_TEST(test_begin_starts_at_current_millis);
    RUN_TEST(test_loop_accumulates);
    RUN_TEST(test_set_continues_from_value);
    RUN_TEST(test_reset);
    RUN_TEST(test_millis_overflow);
    RUN_TEST(test_multiple_millis_overflows);
    RUN_TEST(test_loop_has_to_run_within_overflow_period);
    return UNITY_END();
}
