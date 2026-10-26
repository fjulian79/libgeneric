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

#include "generic/eventlog.hpp"

static char data[256];

/**
 * Everything printTo() writes.
 */
static String printed(const EventLog &log)
{
    StringPrint out;

    log.printTo(out);
    return out.Output;
}

static String last(const EventLog &log)
{
    char buf[256];

    return log.readLast(buf, sizeof(buf)) ? String(buf) : String("<none>");
}

void setUp(void)
{
    memset(data, 0, sizeof(data));
}

void tearDown(void) {}

void test_initially_empty(void)
{
    EventLog log(data, sizeof(data));

    TEST_ASSERT_EQUAL(0, log.getLength());
    TEST_ASSERT_EQUAL_STRING("", printed(log).c_str());
    TEST_ASSERT_EQUAL_STRING("<none>", last(log).c_str());
    TEST_ASSERT_NULL(log.getMonitor());
}

void test_log_without_timestamp(void)
{
    EventLog log(data, sizeof(data));

    log.log("NEC 0x%04X", 0x1234);
    log.log("RC5 %d", 7);
    TEST_ASSERT_EQUAL_STRING("NEC 0x1234\nRC5 7\n", printed(log).c_str());
    TEST_ASSERT_EQUAL_STRING("RC5 7", last(log).c_str());
}

void test_length_matches_printed(void)
{
    EventLog log(data, sizeof(data));

    log.log("first");
    log.log("second event");
    TEST_ASSERT_EQUAL(printed(log).length(), log.getLength());
}

void test_timestamp_is_prefixed(void)
{
    EventLog log(data, sizeof(data));

    log.setTimeStamp([](Print &out) { out.print("TS"); });
    log.log("event");
    TEST_ASSERT_EQUAL_STRING("TS event", last(log).c_str());
}

void test_empty_timestamp_adds_no_space(void)
{
    EventLog log(data, sizeof(data));

    log.setTimeStamp([](Print &out) { (void) out; });
    log.log("event");
    TEST_ASSERT_EQUAL_STRING("event", last(log).c_str());
}

void test_long_timestamp_is_truncated(void)
{
    EventLog log(data, sizeof(data));
    String expected = String(EVENTLOG_TIMESTAMP_MAX, 'T') + " event";

    log.setTimeStamp([](Print &out)
    {
        for (int i = 0; i < 100; i++)
            out.print('T');
    });
    log.log("event");
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), last(log).c_str());
}

void test_long_event_is_truncated(void)
{
    static char big[512];
    EventLog log(big, sizeof(big));
    String text(300, 'e');

    log.log("%s", text.c_str());
    TEST_ASSERT_EQUAL(EVENTLOG_LINE_MAX - 1, last(log).length());
}

void test_empty_event_is_not_stored(void)
{
    EventLog log(data, sizeof(data));

    log.log("%s", "");
    TEST_ASSERT_EQUAL(0, log.getLength());
}

void test_oldest_events_are_removed(void)
{
    static char small[32];
    EventLog log(small, sizeof(small));

    log.log("event 1");
    log.log("event 2");
    log.log("event 3");
    log.log("event 4");
    /* 8 bytes each including the terminator, one byte is always kept free */
    TEST_ASSERT_EQUAL_STRING("event 2\nevent 3\nevent 4\n", printed(log).c_str());
    TEST_ASSERT_EQUAL(printed(log).length(), log.getLength());
}

void test_monitor_gets_new_events(void)
{
    EventLog log(data, sizeof(data));
    StringPrint monitor;

    log.log("before");
    log.setMonitor(&monitor);
    TEST_ASSERT_EQUAL_PTR(&monitor, log.getMonitor());
    log.log("during");
    log.setMonitor(nullptr);
    log.log("after");

    TEST_ASSERT_EQUAL_STRING("during\n", monitor.Output.c_str());
    TEST_ASSERT_EQUAL_STRING("before\nduring\nafter\n", printed(log).c_str());
}

void test_read_last_with_small_buffer(void)
{
    EventLog log(data, sizeof(data));
    char buf[4];

    log.log("event");
    TEST_ASSERT_EQUAL(0, log.readLast(buf, sizeof(buf)));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initially_empty);
    RUN_TEST(test_log_without_timestamp);
    RUN_TEST(test_length_matches_printed);
    RUN_TEST(test_timestamp_is_prefixed);
    RUN_TEST(test_empty_timestamp_adds_no_space);
    RUN_TEST(test_long_timestamp_is_truncated);
    RUN_TEST(test_long_event_is_truncated);
    RUN_TEST(test_empty_event_is_not_stored);
    RUN_TEST(test_oldest_events_are_removed);
    RUN_TEST(test_monitor_gets_new_events);
    RUN_TEST(test_read_last_with_small_buffer);
    return UNITY_END();
}
