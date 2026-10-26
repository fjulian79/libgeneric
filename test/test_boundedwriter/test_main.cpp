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

#include "generic/boundedwriter.hpp"

#define GUARD           ((char) 0xA5)

/**
 * A buffer with guard bytes behind the part handed to the writer, to detect
 * writes behind its end.
 */
typedef struct
{
    char buf[8];
    char guard[8];

} Guarded_t;

static void initGuarded(Guarded_t *g)
{
    memset(g->buf, 'x', sizeof(g->buf));
    memset(g->guard, GUARD, sizeof(g->guard));
}

static void assertGuardIntact(const Guarded_t *g)
{
    for (size_t i = 0; i < sizeof(g->guard); i++)
        TEST_ASSERT_EQUAL_HEX8(GUARD, g->guard[i]);
}

void setUp(void) {}

void tearDown(void) {}

void test_initially_empty(void)
{
    char buf[8];
    BoundedWriter out(buf, sizeof(buf));

    TEST_ASSERT_EQUAL_STRING("", out.c_str());
    TEST_ASSERT_EQUAL_STRING("", buf);
    TEST_ASSERT_EQUAL(0, out.length());
    TEST_ASSERT_FALSE(out.overflowed());
}

void test_writes_into_the_buffer(void)
{
    char buf[16];
    BoundedWriter out(buf, sizeof(buf));

    out.print("abc");
    out.print('d');
    out.print(42ul);
    TEST_ASSERT_EQUAL_STRING("abcd42", buf);
    TEST_ASSERT_EQUAL_PTR(buf, out.c_str());
    TEST_ASSERT_EQUAL(6, out.length());
    TEST_ASSERT_FALSE(out.overflowed());
}

void test_exact_fit_does_not_overflow(void)
{
    char buf[6];
    BoundedWriter out(buf, sizeof(buf));

    TEST_ASSERT_EQUAL(5, out.print("hello"));
    TEST_ASSERT_EQUAL_STRING("hello", out.c_str());
    TEST_ASSERT_EQUAL(5, out.length());
    TEST_ASSERT_FALSE(out.overflowed());
}

void test_write_returns_zero_when_full(void)
{
    char buf[3];
    BoundedWriter out(buf, sizeof(buf));

    TEST_ASSERT_EQUAL(1, out.write('a'));
    TEST_ASSERT_EQUAL(1, out.write('b'));
    TEST_ASSERT_EQUAL(0, out.write('c'));
    TEST_ASSERT_EQUAL_STRING("ab", out.c_str());
    TEST_ASSERT_TRUE(out.overflowed());
}

void test_truncates_and_terminates(void)
{
    char buf[6];
    BoundedWriter out(buf, sizeof(buf));

    TEST_ASSERT_EQUAL(5, out.print("hello world"));
    TEST_ASSERT_EQUAL_STRING("hello", out.c_str());
    TEST_ASSERT_EQUAL(5, out.length());
    TEST_ASSERT_TRUE(out.overflowed());
}

void test_never_writes_behind_the_end(void)
{
    Guarded_t g;

    initGuarded(&g);
    BoundedWriter out(g.buf, sizeof(g.buf));

    for (int i = 0; i < 100; i++)
        out.print("0123456789");

    TEST_ASSERT_EQUAL_STRING("0123456", g.buf);
    TEST_ASSERT_EQUAL(sizeof(g.buf) - 1, out.length());
    assertGuardIntact(&g);
}

void test_size_one_holds_only_the_terminator(void)
{
    Guarded_t g;

    initGuarded(&g);
    BoundedWriter out(g.buf, 1);

    TEST_ASSERT_EQUAL(0, out.print("abc"));
    TEST_ASSERT_EQUAL_STRING("", out.c_str());
    TEST_ASSERT_EQUAL(0, out.length());
    TEST_ASSERT_TRUE(out.overflowed());
    TEST_ASSERT_EQUAL_HEX8('x', g.buf[1]);
    assertGuardIntact(&g);
}

void test_clear_allows_reuse(void)
{
    char buf[4];
    BoundedWriter out(buf, sizeof(buf));

    out.print("abcdef");
    TEST_ASSERT_TRUE(out.overflowed());

    out.clear();
    TEST_ASSERT_EQUAL_STRING("", out.c_str());
    TEST_ASSERT_EQUAL(0, out.length());
    TEST_ASSERT_FALSE(out.overflowed());

    out.print("xy");
    TEST_ASSERT_EQUAL_STRING("xy", out.c_str());
    TEST_ASSERT_FALSE(out.overflowed());
}

void test_usable_as_print(void)
{
    char buf[8];
    BoundedWriter out(buf, sizeof(buf));
    Print &p = out;

    p.print("12345678");
    TEST_ASSERT_EQUAL_STRING("1234567", buf);
    TEST_ASSERT_TRUE(out.overflowed());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initially_empty);
    RUN_TEST(test_writes_into_the_buffer);
    RUN_TEST(test_exact_fit_does_not_overflow);
    RUN_TEST(test_write_returns_zero_when_full);
    RUN_TEST(test_truncates_and_terminates);
    RUN_TEST(test_never_writes_behind_the_end);
    RUN_TEST(test_size_one_holds_only_the_terminator);
    RUN_TEST(test_clear_allows_reuse);
    RUN_TEST(test_usable_as_print);
    return UNITY_END();
}
