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

#include "generic/task.hpp"

static uint32_t calls = 0;
static uint32_t lastCall = 0;

static void taskFunction(uint32_t now)
{
    calls++;
    lastCall = now;
}

void setUp(void)
{
    calls = 0;
    lastCall = 0;
}

void tearDown(void) {}

void test_scheduled_after_interval(void)
{
    Task task(100);

    TEST_ASSERT_FALSE(task.isScheduled(0));
    TEST_ASSERT_FALSE(task.isScheduled(99));
    TEST_ASSERT_TRUE(task.isScheduled(100));
    TEST_ASSERT_FALSE(task.isScheduled(100));
    TEST_ASSERT_FALSE(task.isScheduled(199));
    TEST_ASSERT_TRUE(task.isScheduled(200));
}

void test_interval_counts_from_last_schedule(void)
{
    Task task(100);

    /* Scheduled late, the next interval starts at the time of scheduling */
    TEST_ASSERT_TRUE(task.isScheduled(150));
    TEST_ASSERT_EQUAL(150, task.getLastTick());
    TEST_ASSERT_FALSE(task.isScheduled(249));
    TEST_ASSERT_TRUE(task.isScheduled(250));
}

void test_disabled_task_is_not_scheduled(void)
{
    Task task(100, false);

    TEST_ASSERT_FALSE(task.isEnabled());
    TEST_ASSERT_FALSE(task.isScheduled(1000));
    task.enable();
    TEST_ASSERT_TRUE(task.isEnabled());
    TEST_ASSERT_TRUE(task.isScheduled(1000));
    task.enable(false);
    TEST_ASSERT_FALSE(task.isScheduled(2000));
}

/**
 * millis() overflows after 49.7 days, scheduling has to continue across it.
 */
void test_scheduling_across_tick_overflow(void)
{
    Task task(0x200);

    task.setLastTick(0xFFFFFF00);
    TEST_ASSERT_FALSE(task.isScheduled(0xFFFFFFFF));
    TEST_ASSERT_FALSE(task.isScheduled(0x000000FF));
    TEST_ASSERT_TRUE(task.isScheduled(0x00000100));
    TEST_ASSERT_EQUAL(0x00000100, task.getLastTick());
}

void test_set_tick(void)
{
    Task task(100);

    task.setTick(500);
    TEST_ASSERT_EQUAL(500, task.getTick());
    TEST_ASSERT_FALSE(task.isScheduled(499));
    TEST_ASSERT_TRUE(task.isScheduled(500));
}

void test_loop_calls_task_function(void)
{
    Task task(100, true, taskFunction);

    task.loop(50);
    TEST_ASSERT_EQUAL(0, calls);
    task.loop(100);
    TEST_ASSERT_EQUAL(1, calls);
    TEST_ASSERT_EQUAL(100, lastCall);
    task.loop(150);
    TEST_ASSERT_EQUAL(1, calls);
}

void test_loop_without_task_function(void)
{
    Task task(100);

    task.loop(100);
    TEST_ASSERT_EQUAL(100, task.getLastTick());
    task.setTaskFunction(taskFunction);
    task.loop(200);
    TEST_ASSERT_EQUAL(1, calls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_scheduled_after_interval);
    RUN_TEST(test_interval_counts_from_last_schedule);
    RUN_TEST(test_disabled_task_is_not_scheduled);
    RUN_TEST(test_scheduling_across_tick_overflow);
    RUN_TEST(test_set_tick);
    RUN_TEST(test_loop_calls_task_function);
    RUN_TEST(test_loop_without_task_function);
    return UNITY_END();
}
