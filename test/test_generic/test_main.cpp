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

#include "generic/generic.hpp"

void setUp(void) {}
void tearDown(void) {}

void test_min_max_basic(void)
{
    TEST_ASSERT_EQUAL(1, min(1, 2));
    TEST_ASSERT_EQUAL(1, min(2, 1));
    TEST_ASSERT_EQUAL(2, max(1, 2));
    TEST_ASSERT_EQUAL(2, max(2, 1));
    TEST_ASSERT_EQUAL(-3, min(-3, 0));
    TEST_ASSERT_EQUAL(0, max(-3, 0));
}

/**
 * Arguments containing operators with a lower precedence than the comparison
 * must be evaluated as a whole, e.g. max(a | b, c).
 */
void test_min_max_with_expressions(void)
{
    int a = 1;
    int b = 0;
    int c = 5;

    TEST_ASSERT_EQUAL(1, min(a | b, c));
    TEST_ASSERT_EQUAL(5, max(a | b, c));
    TEST_ASSERT_EQUAL(5, max(c, a | b));
}

void test_angle_conversion(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, PI, 180 * DEG_TO_RAD);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 180.0, PI * RAD_TO_DEG);
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, TWO_PI, 2 * PI);
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, HALF_PI, PI / 2);
}

void test_abs(void)
{
    int a = 1;
    int b = 3;

    TEST_ASSERT_EQUAL(5, abs(-5));
    TEST_ASSERT_EQUAL(5, abs(5));
    TEST_ASSERT_EQUAL(0, abs(0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 2.5, abs(-2.5));
    TEST_ASSERT_EQUAL(2, abs(a - b));
}

void test_wrap_inc(void)
{
    int n = 3;

    TEST_ASSERT_EQUAL(3, wrapInc(2, 1, 4));
    TEST_ASSERT_EQUAL(0, wrapInc(3, 1, 4));
    TEST_ASSERT_EQUAL(1, wrapInc(3, 2, 4));
    TEST_ASSERT_EQUAL(0, wrapInc(3, 1, n + 1));
    TEST_ASSERT_EQUAL(3, wrapInc(n - 1, n - 2, n + 1));
}

void test_constrain(void)
{
    int l = 2;
    int h = 8;

    TEST_ASSERT_EQUAL(2, constrain(0, 2, 8));
    TEST_ASSERT_EQUAL(8, constrain(10, 2, 8));
    TEST_ASSERT_EQUAL(5, constrain(5, 2, 8));
    TEST_ASSERT_EQUAL(2, constrain(2, 2, 8));
    TEST_ASSERT_EQUAL(8, constrain(8, 2, 8));
    TEST_ASSERT_EQUAL(3, constrain(l | 1, l, h));
    TEST_ASSERT_EQUAL(8, constrain(h | 1, l - 1, h));
}

void test_map(void)
{
    TEST_ASSERT_EQUAL(0, map(0, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, map(5, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(100, map(10, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, map(5, 0, 10, 100, 0));
    TEST_ASSERT_EQUAL(0, map(-10, -10, 10, 0, 100));
}

/**
 * Arguments containing operators must be evaluated as a whole, e.g.
 * map(x, a - b, ...).
 */
void test_map_with_expressions(void)
{
    int a = 2;
    int b = 2;
    int x = 5;

    TEST_ASSERT_EQUAL(50, map(x, a - b, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, map(x, 0, 10, a - b, 100));
    TEST_ASSERT_EQUAL(50, map(x + 1, 1, 11, 0, 100));
}

void test_round(void)
{
    double a = 1.25;
    double b = 1.25;

    TEST_ASSERT_EQUAL(2, round(2.4));
    TEST_ASSERT_EQUAL(3, round(2.5));
    TEST_ASSERT_EQUAL(-2, round(-2.4));
    TEST_ASSERT_EQUAL(-3, round(-2.5));
    TEST_ASSERT_EQUAL(0, round(0.0));
    TEST_ASSERT_EQUAL(3, round(a + b));
}

void test_radians_and_degrees(void)
{
    double a = 90;
    double b = HALF_PI;

    TEST_ASSERT_DOUBLE_WITHIN(1e-12, PI, radians(180));
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 180.0, degrees(PI));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, PI, radians(a + a));
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 180.0, degrees(b + b));
}

void test_sq(void)
{
    int a = 1;

    TEST_ASSERT_EQUAL(9, sq(3));
    TEST_ASSERT_EQUAL(16, sq(-4));
    TEST_ASSERT_EQUAL(9, sq(a + 2));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 2.25, sq(1.5));
}

void test_arraysize(void)
{
    int ints[7];
    struct { char c; double d; } structs[3];
    char text[] = "hello";

    TEST_ASSERT_EQUAL(7, arraysize(ints));
    TEST_ASSERT_EQUAL(3, arraysize(structs));
    TEST_ASSERT_EQUAL(6, arraysize(text));
}

typedef struct
{
    char a;
    int b;
    double c;
} Container_t;

void test_offsetof(void)
{
    Container_t s;

    TEST_ASSERT_EQUAL(0, offsetof(Container_t, a));
    TEST_ASSERT_EQUAL((size_t) ((char *) &s.b - (char *) &s), offsetof(Container_t, b));
    TEST_ASSERT_EQUAL((size_t) ((char *) &s.c - (char *) &s), offsetof(Container_t, c));
}

void test_container_of(void)
{
    Container_t s;
    int *pb = &s.b;
    double *pc = &s.c;

    TEST_ASSERT_EQUAL_PTR(&s, container_of(pb, Container_t, b));
    TEST_ASSERT_EQUAL_PTR(&s, container_of(pc, Container_t, c));
    TEST_ASSERT_EQUAL_PTR(&s, container_of(&s.a, Container_t, a));
}

static int unusedParameter(int x)
{
    unused(x);
    return 1;
}

void test_unused(void)
{
    TEST_ASSERT_EQUAL(1, unusedParameter(0));
}

void test_break_if_diverse(void)
{
    const int a[] = {1, 2, 3, 4, 5};
    const int b[] = {1, 2, 9, 4, 5};
    size_t i = 0;

    for (i = 0; i < arraysize(a); i++)
    {
        breakIfDiverse(a[i], b[i]);
    }
    TEST_ASSERT_EQUAL(2, i);

    for (i = 0; i < arraysize(a); i++)
    {
        breakIfDiverse(a[i], a[i]);
    }
    TEST_ASSERT_EQUAL(arraysize(a), i);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_min_max_basic);
    RUN_TEST(test_min_max_with_expressions);
    RUN_TEST(test_angle_conversion);
    RUN_TEST(test_abs);
    RUN_TEST(test_wrap_inc);
    RUN_TEST(test_constrain);
    RUN_TEST(test_map);
    RUN_TEST(test_map_with_expressions);
    RUN_TEST(test_round);
    RUN_TEST(test_radians_and_degrees);
    RUN_TEST(test_sq);
    RUN_TEST(test_arraysize);
    RUN_TEST(test_offsetof);
    RUN_TEST(test_container_of);
    RUN_TEST(test_unused);
    RUN_TEST(test_break_if_diverse);
    return UNITY_END();
}
