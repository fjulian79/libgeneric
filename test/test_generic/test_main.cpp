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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_min_max_basic);
    RUN_TEST(test_min_max_with_expressions);
    RUN_TEST(test_angle_conversion);
    return UNITY_END();
}
