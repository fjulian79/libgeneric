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

#include "generic/syslog.hpp"

static SysLogMemory<512> memory;
static StringPrint console;

/**
 * Everything printTo() writes.
 */
static String printed(const SysLog &log)
{
    StringPrint out;

    log.printTo(out);
    return out.Output;
}

void setUp(void)
{
    memset(&memory, 0, sizeof(memory));
    console.Output.clear();
    Serial.Output.clear();
}

void tearDown(void) {}

void test_default_levels(void)
{
    SysLog log(memory);

    TEST_ASSERT_EQUAL(SysLog::Info, log.getLogLevel());
    TEST_ASSERT_EQUAL(SysLog::Err, log.getConsoleLevel());
}

void test_severity_names(void)
{
    TEST_ASSERT_EQUAL_STRING("emerg", SysLog::getSeverityName(SysLog::Emerg));
    TEST_ASSERT_EQUAL_STRING("alert", SysLog::getSeverityName(SysLog::Alert));
    TEST_ASSERT_EQUAL_STRING("crit", SysLog::getSeverityName(SysLog::Crit));
    TEST_ASSERT_EQUAL_STRING("err", SysLog::getSeverityName(SysLog::Err));
    TEST_ASSERT_EQUAL_STRING("warning", SysLog::getSeverityName(SysLog::Warning));
    TEST_ASSERT_EQUAL_STRING("notice", SysLog::getSeverityName(SysLog::Notice));
    TEST_ASSERT_EQUAL_STRING("info", SysLog::getSeverityName(SysLog::Info));
    TEST_ASSERT_EQUAL_STRING("debug", SysLog::getSeverityName(SysLog::Debug));
    TEST_ASSERT_EQUAL_STRING("?", SysLog::getSeverityName((SysLog::Severity) 8));
}

void test_severity_functions(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.setLogLevel(SysLog::Debug);
    log.emerg("m");
    log.alert("m");
    log.crit("m");
    log.err("m");
    log.warning("m");
    log.notice("m");
    log.info("m");
    log.debug("m");
    log.log(SysLog::Info, "%s %d", "log", 1);
    TEST_ASSERT_EQUAL_STRING(
        "emerg   m\n"
        "alert   m\n"
        "crit    m\n"
        "err     m\n"
        "warning m\n"
        "notice  m\n"
        "info    m\n"
        "debug   m\n"
        "info    log 1\n", printed(log).c_str());
}

void test_timestamp_is_prefixed(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.setTimeStamp([](Print &out) { out.print("TS"); });
    log.info("message");
    TEST_ASSERT_EQUAL_STRING("TS info    message\n", printed(log).c_str());
}

void test_empty_timestamp_adds_no_space(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.setTimeStamp([](Print &out) { (void) out; });
    log.info("message");
    TEST_ASSERT_EQUAL_STRING("info    message\n", printed(log).c_str());
}

void test_long_timestamp_is_truncated(void)
{
    SysLog log(memory);
    String expected = String(SYSLOG_TIMESTAMP_MAX, 'T') + " info    message\n";

    log.begin(false, &console);
    log.setTimeStamp([](Print &out)
    {
        for (int i = 0; i < 100; i++)
            out.print('T');
    });
    log.info("message");
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), printed(log).c_str());
}

void test_long_message_is_truncated(void)
{
    SysLog log(memory);
    String text(300, 'm');
    String expected = "info    " + String(SYSLOG_LINE_MAX - 1, 'm') + "\n";

    log.begin(false, &console);
    log.info("%s", text.c_str());
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), printed(log).c_str());
}

void test_trailing_newlines_are_removed(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.info("one\n");
    log.info("two\r\n");
    TEST_ASSERT_EQUAL_STRING("info    one\ninfo    two\n", printed(log).c_str());
}

void test_levels_decide_storing_and_printing(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.err("stored and printed");
    log.info("stored only");
    log.debug("neither");
    TEST_ASSERT_EQUAL_STRING("err     stored and printed\ninfo    stored only\n", printed(log).c_str());
    TEST_ASSERT_EQUAL_STRING("err     stored and printed\n", console.Output.c_str());
}

void test_lower_log_level_lowers_console_level(void)
{
    SysLog log(memory);

    log.setConsoleLevel(SysLog::Warning);
    log.setLogLevel(SysLog::Err);
    TEST_ASSERT_EQUAL(SysLog::Err, log.getLogLevel());
    TEST_ASSERT_EQUAL(SysLog::Err, log.getConsoleLevel());
}

void test_higher_console_level_raises_log_level(void)
{
    SysLog log(memory);

    log.setConsoleLevel(SysLog::Debug);
    TEST_ASSERT_EQUAL(SysLog::Debug, log.getConsoleLevel());
    TEST_ASSERT_EQUAL(SysLog::Debug, log.getLogLevel());
}

void test_levels_are_limited_to_debug(void)
{
    SysLog log(memory);

    log.setLogLevel((SysLog::Severity) 20);
    TEST_ASSERT_EQUAL(SysLog::Debug, log.getLogLevel());
    log.setConsoleLevel((SysLog::Severity) 20);
    TEST_ASSERT_EQUAL(SysLog::Debug, log.getConsoleLevel());
}

void test_default_console_is_serial(void)
{
    SysLog log(memory);

    log.begin(false);
    log.err("to serial");
    TEST_ASSERT_EQUAL_STRING("err     to serial\n", Serial.Output.c_str());
}

void test_no_console(void)
{
    SysLog log(memory);

    log.begin(false, nullptr);
    log.err("stored only");
    TEST_ASSERT_EQUAL_STRING("", Serial.Output.c_str());
    TEST_ASSERT_EQUAL_STRING("err     stored only\n", printed(log).c_str());

    log.setConsole(&console);
    log.err("printed");
    TEST_ASSERT_EQUAL_STRING("err     printed\n", console.Output.c_str());
}

void test_without_memory_only_prints(void)
{
    SysLog log;

    log.begin(true, &console);
    log.err("printed");
    TEST_ASSERT_EQUAL_STRING("err     printed\n", console.Output.c_str());
    TEST_ASSERT_EQUAL(0, log.getLength());
    TEST_ASSERT_EQUAL_STRING("", printed(log).c_str());
}

void test_length_matches_printed(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    log.info("first");
    log.warning("second message");
    TEST_ASSERT_EQUAL(printed(log).length(), log.getLength());
}

void test_oldest_lines_are_removed(void)
{
    SysLog log(memory);

    log.begin(false, &console);
    for (int i = 0; i < 100; i++)
        log.info("line %03d", i);

    /* Each line takes 17 bytes, 30 of them fit into 511 usable bytes */
    String all = printed(log);
    TEST_ASSERT_EQUAL(30 * 17, all.length());
    TEST_ASSERT_EQUAL(0, all.find("info    line 070\n"));
    TEST_ASSERT_EQUAL(all.length() - 17, all.find("info    line 099\n"));
}

void test_restore_keeps_the_lines(void)
{
    {
        SysLog log(memory);
        log.begin(false, &console);
        log.info("before reset");
    }

    SysLog log(memory);
    log.begin(true, &console);
    log.info("after reset");
    TEST_ASSERT_EQUAL_STRING("info    before reset\ninfo    after reset\n", printed(log).c_str());
}

void test_no_restore_clears_the_lines(void)
{
    {
        SysLog log(memory);
        log.begin(false, &console);
        log.info("before reset");
    }

    SysLog log(memory);
    log.begin(false, &console);
    TEST_ASSERT_EQUAL(0, log.getLength());
}

void test_random_memory_is_not_restored(void)
{
    memset(&memory, 0x5A, sizeof(memory));

    SysLog log(memory);
    log.begin(true, &console);
    TEST_ASSERT_EQUAL(0, log.getLength());
    log.info("works");
    TEST_ASSERT_EQUAL_STRING("info    works\n", printed(log).c_str());
}

void test_other_size_is_not_restored(void)
{
    {
        SysLog log(memory);
        log.begin(false, &console);
        log.info("before reset");
    }

    /* E.g. a new firmware using a smaller log at the same place */
    SysLog log(&memory.header, memory.data, sizeof(memory.data) / 2);
    log.begin(true, &console);
    TEST_ASSERT_EQUAL(0, log.getLength());
}

void test_broken_line_is_not_restored(void)
{
    {
        SysLog log(memory);
        log.begin(false, &console);
        log.info("first");
        log.info("second");
    }

    /* E.g. a reset while appending, the terminator of the last line is
     * missing */
    size_t end = memory.header.state.Head;
    memory.data[(end + sizeof(memory.data) - 1) % sizeof(memory.data)] = 'x';

    SysLog log(memory);
    log.begin(true, &console);
    TEST_ASSERT_EQUAL(0, log.getLength());
}

void test_too_long_line_is_not_restored(void)
{
    {
        SysLog log(memory);
        log.begin(false, &console);
        log.info("short");
    }

    /* A line longer than any line SysLog writes, e.g. random content which
     * happens to match the header */
    memset(memory.data, 'x', 300);
    memory.data[300] = 0;
    memory.header.state.Tail = 0;
    memory.header.state.Head = 301;

    SysLog log(memory);
    log.begin(true, &console);
    TEST_ASSERT_EQUAL(0, log.getLength());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_levels);
    RUN_TEST(test_severity_names);
    RUN_TEST(test_severity_functions);
    RUN_TEST(test_timestamp_is_prefixed);
    RUN_TEST(test_empty_timestamp_adds_no_space);
    RUN_TEST(test_long_timestamp_is_truncated);
    RUN_TEST(test_long_message_is_truncated);
    RUN_TEST(test_trailing_newlines_are_removed);
    RUN_TEST(test_levels_decide_storing_and_printing);
    RUN_TEST(test_lower_log_level_lowers_console_level);
    RUN_TEST(test_higher_console_level_raises_log_level);
    RUN_TEST(test_levels_are_limited_to_debug);
    RUN_TEST(test_default_console_is_serial);
    RUN_TEST(test_no_console);
    RUN_TEST(test_without_memory_only_prints);
    RUN_TEST(test_length_matches_printed);
    RUN_TEST(test_oldest_lines_are_removed);
    RUN_TEST(test_restore_keeps_the_lines);
    RUN_TEST(test_no_restore_clears_the_lines);
    RUN_TEST(test_random_memory_is_not_restored);
    RUN_TEST(test_other_size_is_not_restored);
    RUN_TEST(test_broken_line_is_not_restored);
    RUN_TEST(test_too_long_line_is_not_restored);
    return UNITY_END();
}
