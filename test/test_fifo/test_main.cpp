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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <vector>

#include "generic/fifo.hpp"

void setUp(void) {}
void tearDown(void) {}

/**
 * Regression: write() ending exactly at the end of the buffer left Head at
 * Size. After freeing the data getUsed() reported a full buffer and getFree()
 * underflowed, which broke e.g. a DMA based transmit path.
 */
void test_write_up_to_buffer_end(void)
{
    char buf[8];
    char tmp[8];
    void *p = 0;
    Fifo f(buf, sizeof(buf));

    f.write("abcdef", 6);
    f.read(tmp, 2);
    f.write("gh", 2);
    f.free(f.getReadBlock(&p));
    f.free(f.getReadBlock(&p));
    TEST_ASSERT_EQUAL(0, f.getUsed());
    TEST_ASSERT_EQUAL(7, f.getFree());
}

/**
 * Regression: read() ending exactly at the end of the buffer left Tail at
 * Size, the following get() read behind the buffer.
 */
void test_get_after_read_up_to_buffer_end(void)
{
    char buf[9];
    char tmp[8];
    char c = 0;
    Fifo f(buf, sizeof(buf) - 1);

    buf[8] = 'X';
    f.write("abcdef", 6);
    f.read(tmp, 4);
    f.write("ghi", 3);
    f.read(tmp, 4);
    TEST_ASSERT_EQUAL(1, f.get(&c));
    TEST_ASSERT_EQUAL('i', c);
}

/**
 * Regression: The consumer reset Head when the fifo became empty, which could
 * drop data written concurrently by the producer, e.g. from an interrupt. The
 * consumer must never modify Head.
 */
void test_consumer_does_not_modify_head(void)
{
    char buf[8];
    char tmp[8];
    char c = 0;
    void *p = 0;
    Fifo f(buf, sizeof(buf));
    size_t head = 0;

    f.write("abc", 3);
    head = f.getState().Head;
    f.get(&c);
    f.read(tmp, 1);
    f.free(f.getReadBlock(&p));
    TEST_ASSERT_EQUAL(0, f.getUsed());
    TEST_ASSERT_EQUAL(head, f.getState().Head);
}

void test_put_and_get(void)
{
    char buf[4];
    char c = 'a';
    Fifo f(buf, sizeof(buf));

    TEST_ASSERT_EQUAL(1, f.put(&c));
    c = 'b';
    TEST_ASSERT_EQUAL(1, f.put(&c));
    c = 'c';
    TEST_ASSERT_EQUAL(1, f.put(&c));
    TEST_ASSERT_EQUAL(0, f.put(&c));
    TEST_ASSERT_EQUAL(1, f.get(&c));
    TEST_ASSERT_EQUAL('a', c);
    TEST_ASSERT_EQUAL(1, f.get(&c));
    TEST_ASSERT_EQUAL(1, f.get(&c));
    TEST_ASSERT_EQUAL('c', c);
    TEST_ASSERT_EQUAL(0, f.get(&c));
}

/**
 * Random operations of all kinds compared against a std::deque as model,
 * including a guard area behind the buffer which must never be written.
 */
void test_random_operations_against_model(void)
{
    const size_t guard = 16;
    char msg[128];

    for (unsigned seed = 1; seed <= 300; seed++)
    {
        srand(seed);
        size_t siz = 2 + rand() % 40;
        std::vector<char> buf(siz + guard, 0x55);
        std::deque<char> model;
        Fifo f(buf.data(), siz);

        for (int op = 0; op < 2000; op++)
        {
            char data[64];
            char out[64];
            size_t len = rand() % 50;
            size_t space = siz - 1 - model.size();
            size_t n = 0;
            void *p = 0;

            for (size_t i = 0; i < len; i++)
                data[i] = (char) rand();

            snprintf(msg, sizeof(msg), "seed %u, op %d, size %zu", seed, op, siz);

            switch (rand() % 5)
            {
                case 0:
                    n = len < space ? len : space;
                    TEST_ASSERT_EQUAL_MESSAGE(n, f.write(data, len), msg);
                    model.insert(model.end(), data, data + n);
                    break;

                case 1:
                    TEST_ASSERT_EQUAL_MESSAGE(space ? 1 : 0, f.put(data), msg);
                    if (space)
                        model.push_back(data[0]);
                    break;

                case 2:
                    n = len < model.size() ? len : model.size();
                    TEST_ASSERT_EQUAL_MESSAGE(n, f.read(out, len), msg);
                    for (size_t i = 0; i < n; i++, model.pop_front())
                        TEST_ASSERT_EQUAL_MESSAGE(model.front(), out[i], msg);
                    break;

                case 3:
                    TEST_ASSERT_EQUAL_MESSAGE(model.empty() ? 0 : 1, f.get(out), msg);
                    if (!model.empty())
                    {
                        TEST_ASSERT_EQUAL_MESSAGE(model.front(), out[0], msg);
                        model.pop_front();
                    }
                    break;

                case 4:
                    n = f.getReadBlock(&p);
                    n = n < len ? n : len;
                    for (size_t i = 0; i < n; i++, model.pop_front())
                        TEST_ASSERT_EQUAL_MESSAGE(model.front(), ((char *) p)[i], msg);
                    f.free(n);
                    break;
            }

            TEST_ASSERT_EQUAL_MESSAGE(model.size(), f.getUsed(), msg);
            TEST_ASSERT_EQUAL_MESSAGE(siz - 1 - model.size(), f.getFree(), msg);
            for (size_t i = siz; i < siz + guard; i++)
                TEST_ASSERT_EQUAL_MESSAGE(0x55, buf[i], msg);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_write_up_to_buffer_end);
    RUN_TEST(test_get_after_read_up_to_buffer_end);
    RUN_TEST(test_consumer_does_not_modify_head);
    RUN_TEST(test_put_and_get);
    RUN_TEST(test_random_operations_against_model);
    return UNITY_END();
}
