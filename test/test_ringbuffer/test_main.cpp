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
#include <string.h>

#include "generic/ringbuffer.hpp"

void setUp(void) {}
void tearDown(void) {}

void test_capacity_keeps_one_byte_free(void)
{
    char buf[8];
    RingBuffer rb(buf, sizeof(buf));

    TEST_ASSERT_EQUAL(8, rb.getSize());
    TEST_ASSERT_EQUAL(7, rb.getFree());
    TEST_ASSERT_EQUAL(0, rb.getUsed());
    TEST_ASSERT_EQUAL(7, rb.write("abcdefghij", 10));
    TEST_ASSERT_EQUAL(0, rb.getFree());
    TEST_ASSERT_EQUAL(7, rb.getUsed());
}

void test_read_and_write_wrap_around(void)
{
    char buf[8];
    char out[16] = {0};
    RingBuffer rb(buf, sizeof(buf));

    rb.write("abcdefg", 7);
    TEST_ASSERT_EQUAL(5, rb.read(out, 5));
    TEST_ASSERT_EQUAL_MEMORY("abcde", out, 5);
    TEST_ASSERT_EQUAL(3, rb.write("XYZ", 3));
    memset(out, 0, sizeof(out));
    TEST_ASSERT_EQUAL(5, rb.read(out, sizeof(out)));
    TEST_ASSERT_EQUAL_MEMORY("fgXYZ", out, 5);
    TEST_ASSERT_EQUAL(0, rb.getUsed());
}

void test_peek_does_not_consume(void)
{
    char buf[8];
    char out[8] = {0};
    RingBuffer rb(buf, sizeof(buf));

    rb.write("abcde", 5);
    TEST_ASSERT_EQUAL(3, rb.peek(out, 3, 2));
    TEST_ASSERT_EQUAL_MEMORY("cde", out, 3);
    TEST_ASSERT_EQUAL(2, rb.peek(out, 10, 3));
    TEST_ASSERT_EQUAL(0, rb.peek(out, 1, 5));
    TEST_ASSERT_EQUAL(5, rb.getUsed());
}

void test_read_block_ends_at_buffer_end(void)
{
    char buf[8];
    char out[8];
    void *p = 0;
    RingBuffer rb(buf, sizeof(buf));

    rb.write("abcdefg", 7);
    rb.read(out, 5);
    rb.write("XYZ", 3);
    TEST_ASSERT_EQUAL(3, rb.getReadBlock(&p));
    TEST_ASSERT_EQUAL_MEMORY("fgX", p, 3);
    rb.free(3);
    TEST_ASSERT_EQUAL(2, rb.getReadBlock(&p));
    TEST_ASSERT_EQUAL_MEMORY("YZ", p, 2);
}

void test_state_restore_and_validation(void)
{
    char buf[8];
    char out[8] = {0};
    RingBuffer rb(buf, sizeof(buf));
    RingBuffer copy(buf, sizeof(buf));
    RingBuffer::State state;

    rb.write("abcdefg", 7);
    rb.read(out, 4);
    rb.write("hi", 2);
    state = rb.getState();

    TEST_ASSERT_TRUE(copy.setState(state));
    TEST_ASSERT_EQUAL(5, copy.read(out, sizeof(out)));
    TEST_ASSERT_EQUAL_MEMORY("efghi", out, 5);

    state.Head = sizeof(buf);
    TEST_ASSERT_FALSE(rb.setState(state));
    TEST_ASSERT_EQUAL(5, rb.getUsed());
}

void test_uninitialized_buffer(void)
{
    RingBuffer rb;
    char c = 0;
    void *p = 0;

    TEST_ASSERT_EQUAL(0, rb.getFree());
    TEST_ASSERT_EQUAL(0, rb.getUsed());
    TEST_ASSERT_EQUAL(0, rb.write("a", 1));
    TEST_ASSERT_EQUAL(0, rb.read(&c, 1));
    TEST_ASSERT_EQUAL(0, rb.getReadBlock(&p));
    TEST_ASSERT_FALSE(rb.setState(rb.getState()));
}

void test_clear(void)
{
    char buf[8];
    RingBuffer rb(buf, sizeof(buf));

    rb.write("abc", 3);
    rb.clear();
    TEST_ASSERT_EQUAL(0, rb.getUsed());
    TEST_ASSERT_EQUAL(7, rb.getFree());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_capacity_keeps_one_byte_free);
    RUN_TEST(test_read_and_write_wrap_around);
    RUN_TEST(test_peek_does_not_consume);
    RUN_TEST(test_read_block_ends_at_buffer_end);
    RUN_TEST(test_state_restore_and_validation);
    RUN_TEST(test_uninitialized_buffer);
    RUN_TEST(test_clear);
    return UNITY_END();
}
