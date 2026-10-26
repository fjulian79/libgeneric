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
#include <deque>
#include <string>
#include <vector>

#include "generic/lineringbuffer.hpp"

void setUp(void) {}
void tearDown(void) {}

/**
 * Collects all lines from the oldest to the newest one.
 */
static std::vector<std::string> linesForward(const LineRingBuffer &l)
{
    std::vector<std::string> lines;
    char tmp[256];

    for (size_t pos = l.first(); pos != LineRingBuffer::npos; pos = l.next(pos))
    {
        size_t len = l.read(pos, tmp, sizeof(tmp));
        TEST_ASSERT_EQUAL(l.getLength(pos), len);
        lines.push_back(std::string(tmp, len));
    }

    return lines;
}

/**
 * Collects all lines from the newest to the oldest one, returned in the same
 * order as linesForward().
 */
static std::vector<std::string> linesBackward(const LineRingBuffer &l)
{
    std::vector<std::string> lines;
    char tmp[256];

    for (size_t pos = l.last(); pos != LineRingBuffer::npos; pos = l.prev(pos))
    {
        size_t len = l.read(pos, tmp, sizeof(tmp));
        lines.insert(lines.begin(), std::string(tmp, len));
    }

    return lines;
}

void test_append_and_iterate(void)
{
    char buf[32];
    LineRingBuffer l(buf, sizeof(buf));
    std::vector<std::string> expected = {"one", "two", "three"};

    TEST_ASSERT_TRUE(l.append("one"));
    TEST_ASSERT_TRUE(l.append("two"));
    TEST_ASSERT_TRUE(l.append("three"));
    TEST_ASSERT_TRUE(linesForward(l) == expected);
    TEST_ASSERT_TRUE(linesBackward(l) == expected);
}

void test_oldest_lines_are_removed(void)
{
    char buf[16];
    LineRingBuffer l(buf, sizeof(buf));
    std::vector<std::string> expected = {"ccccc", "ddddd"};

    l.append("aaaaa");
    l.append("bbbbb");
    l.append("ccccc");
    l.append("ddddd");
    TEST_ASSERT_TRUE(linesForward(l) == expected);
}

void test_invalid_lines_are_rejected(void)
{
    char buf[8];
    LineRingBuffer l(buf, sizeof(buf));

    TEST_ASSERT_FALSE(l.append(""));
    TEST_ASSERT_FALSE(l.append((const char *) 0));
    TEST_ASSERT_FALSE(l.append("1234567"));
    TEST_ASSERT_TRUE(l.append("123456"));
}

void test_read_into_too_small_buffer(void)
{
    char buf[16];
    char tmp[3];
    LineRingBuffer l(buf, sizeof(buf));

    l.append("hello");
    TEST_ASSERT_EQUAL(0, l.read(l.first(), tmp, sizeof(tmp)));
}

void test_drop_oldest(void)
{
    char buf[16];
    LineRingBuffer l(buf, sizeof(buf));
    std::vector<std::string> expected = {"b"};

    l.append("a");
    l.append("b");
    l.dropOldest();
    TEST_ASSERT_TRUE(linesForward(l) == expected);
    l.dropOldest();
    TEST_ASSERT_EQUAL(LineRingBuffer::npos, l.first());
    TEST_ASSERT_EQUAL(LineRingBuffer::npos, l.last());
}

void test_restore_after_reset(void)
{
    char buf[16];
    LineRingBuffer l(buf, sizeof(buf));

    l.append("aaaaa");
    l.append("bbbbb");
    l.append("ccc");

    /* A new object on the same buffer, as after a reset */
    LineRingBuffer restored(buf, sizeof(buf));
    TEST_ASSERT_TRUE(restored.setState(l.getState()));
    TEST_ASSERT_TRUE(linesForward(restored) == linesForward(l));
}

void test_restore_rejects_missing_terminator(void)
{
    char buf[16];
    LineRingBuffer l(buf, sizeof(buf));
    RingBuffer::State state;

    l.append("aaaaa");
    state = l.getState();
    state.Head = (state.Head + sizeof(buf) - 1) % sizeof(buf);

    LineRingBuffer restored(buf, sizeof(buf));
    TEST_ASSERT_FALSE(restored.setState(state));
    TEST_ASSERT_EQUAL(0, restored.getUsed());
}

/**
 * Random lines compared against a std::deque as model, removing the oldest
 * lines as long as the new one does not fit.
 */
void test_random_lines_against_model(void)
{
    char msg[128];

    for (unsigned seed = 1; seed <= 200; seed++)
    {
        srand(seed);
        size_t siz = 4 + rand() % 120;
        std::vector<char> buf(siz);
        LineRingBuffer l(buf.data(), siz);
        std::deque<std::string> model;
        size_t used = 0;

        for (int op = 0; op < 500; op++)
        {
            size_t len = rand() % (siz + 2);
            bool valid = len > 0 && len + 2 <= siz;
            std::string line;

            for (size_t i = 0; i < len; i++)
                line += (char) ('a' + rand() % 26);

            snprintf(msg, sizeof(msg), "seed %u, op %d, size %zu", seed, op, siz);
            TEST_ASSERT_EQUAL_MESSAGE(valid, l.append(line.c_str()), msg);

            if (valid)
            {
                while (used + len + 1 > siz - 1)
                {
                    used -= model.front().size() + 1;
                    model.pop_front();
                }
                model.push_back(line);
                used += len + 1;
            }

            std::vector<std::string> expected(model.begin(), model.end());
            TEST_ASSERT_TRUE_MESSAGE(linesForward(l) == expected, msg);
            TEST_ASSERT_TRUE_MESSAGE(linesBackward(l) == expected, msg);
            TEST_ASSERT_EQUAL_MESSAGE(used, l.getUsed(), msg);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_append_and_iterate);
    RUN_TEST(test_oldest_lines_are_removed);
    RUN_TEST(test_invalid_lines_are_rejected);
    RUN_TEST(test_read_into_too_small_buffer);
    RUN_TEST(test_drop_oldest);
    RUN_TEST(test_restore_after_reset);
    RUN_TEST(test_restore_rejects_missing_terminator);
    RUN_TEST(test_random_lines_against_model);
    return UNITY_END();
}
