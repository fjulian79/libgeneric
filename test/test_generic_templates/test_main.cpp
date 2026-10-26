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
#include <stdint.h>
#include <type_traits>

#include "generic/generic.hpp"

void setUp(void) {}
void tearDown(void) {}

/* The functions are constexpr, so they can be evaluated at compile time */
static_assert(generic::square(3) == 9, "square() not constexpr");
static_assert(generic::clamp(12, 0, 10) == 10, "clamp() not constexpr");
static_assert(generic::mapRange(5, 0, 10, 0, 100) == 50, "mapRange() not constexpr");
static_assert(generic::wrapInc(3, 1, 4) == 0, "wrapInc() not constexpr");

void test_clamp(void)
{
    TEST_ASSERT_EQUAL(2, generic::clamp(0, 2, 8));
    TEST_ASSERT_EQUAL(8, generic::clamp(10, 2, 8));
    TEST_ASSERT_EQUAL(5, generic::clamp(5, 2, 8));
    TEST_ASSERT_EQUAL(2, generic::clamp(2, 2, 8));
    TEST_ASSERT_EQUAL(8, generic::clamp(8, 2, 8));
    TEST_ASSERT_EQUAL(-5, generic::clamp(-10, -5, 5));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.5f, generic::clamp(0.5f, 0.0f, 1.0f));
}

/**
 * The type of the result is the type of the first argument, the limits are
 * converted, e.g. literals.
 */
void test_clamp_keeps_type(void)
{
    uint8_t u8 = 200;
    int16_t i16 = -300;

    static_assert(std::is_same<decltype(generic::clamp(u8, 0, 100)), uint8_t>::value, "");
    static_assert(std::is_same<decltype(generic::clamp(i16, -100, 100)), int16_t>::value, "");
    static_assert(std::is_same<decltype(generic::clamp(1.5f, 0, 1)), float>::value, "");
    TEST_ASSERT_EQUAL(100, generic::clamp(u8, 0, 100));
    TEST_ASSERT_EQUAL(-100, generic::clamp(i16, -100, 100));
}

void test_map_range(void)
{
    TEST_ASSERT_EQUAL(0, generic::mapRange(0, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, generic::mapRange(5, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(100, generic::mapRange(10, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, generic::mapRange(5, 0, 10, 100, 0));
    TEST_ASSERT_EQUAL(0, generic::mapRange(-10, -10, 10, 0, 100));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 0.25f, generic::mapRange(25.0f, 0, 100, 0, 1));
}

void test_map_range_with_expressions(void)
{
    int a = 2;
    int b = 2;
    int x = 5;

    TEST_ASSERT_EQUAL(50, generic::mapRange(x, a - b, 10, 0, 100));
    TEST_ASSERT_EQUAL(50, generic::mapRange(x + 1, 1, 11, 0, 100));
}

void test_map_range_keeps_type(void)
{
    uint16_t adc = 512;

    static_assert(std::is_same<decltype(generic::mapRange(adc, 0, 1023, 0, 100)), uint16_t>::value, "");
    static_assert(std::is_same<decltype(generic::mapRange(1.0f, 0, 2, 0, 10)), float>::value, "");
    TEST_ASSERT_EQUAL(50, generic::mapRange(adc, 0, 1024, 0, 100));
}

void test_square(void)
{
    int a = 1;

    TEST_ASSERT_EQUAL(9, generic::square(3));
    TEST_ASSERT_EQUAL(16, generic::square(-4));
    TEST_ASSERT_EQUAL(9, generic::square(a + 2));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 2.25f, generic::square(1.5f));
    static_assert(std::is_same<decltype(generic::square(1.5f)), float>::value, "");
}

/**
 * Each argument is evaluated exactly once, in contrast to the former macros.
 */
void test_arguments_evaluated_once(void)
{
    int i = 5;

    TEST_ASSERT_EQUAL(25, generic::square(i++));
    TEST_ASSERT_EQUAL(6, i);
    TEST_ASSERT_EQUAL(6, generic::clamp(i++, 0, 10));
    TEST_ASSERT_EQUAL(7, i);
}

void test_wrap_inc(void)
{
    int n = 3;

    TEST_ASSERT_EQUAL(3, generic::wrapInc(2, 1, 4));
    TEST_ASSERT_EQUAL(0, generic::wrapInc(3, 1, 4));
    TEST_ASSERT_EQUAL(1, generic::wrapInc(3, 2, 4));
    TEST_ASSERT_EQUAL(3, generic::wrapInc(n - 1, n - 2, n + 1));
    static_assert(std::is_same<decltype(generic::wrapInc((uint8_t) 3, 1, 4)), uint8_t>::value, "");
}

void test_constants(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1e-15, 3.14159265358979323846, generic::pi);
    TEST_ASSERT_DOUBLE_WITHIN(1e-15, generic::pi / 2, generic::halfPi);
    TEST_ASSERT_DOUBLE_WITHIN(1e-15, generic::pi * 2, generic::twoPi);
}

/**
 * Angles are converted in the type of the argument, a float is not promoted
 * to double.
 */
void test_angle_conversion(void)
{
    static_assert(std::is_same<decltype(generic::toRadians(1.0f)), float>::value, "");
    static_assert(std::is_same<decltype(generic::toDegrees(1.0f)), float>::value, "");
    static_assert(std::is_same<decltype(generic::toRadians(1.0)), double>::value, "");

    TEST_ASSERT_DOUBLE_WITHIN(1e-6, (float) generic::pi, generic::toRadians(180.0f));
    TEST_ASSERT_DOUBLE_WITHIN(1e-4, 180.0f, generic::toDegrees((float) generic::pi));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, generic::halfPi, generic::toRadians(90.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 90.0, generic::toDegrees(generic::halfPi));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_clamp);
    RUN_TEST(test_clamp_keeps_type);
    RUN_TEST(test_map_range);
    RUN_TEST(test_map_range_with_expressions);
    RUN_TEST(test_map_range_keeps_type);
    RUN_TEST(test_square);
    RUN_TEST(test_arguments_evaluated_once);
    RUN_TEST(test_wrap_inc);
    RUN_TEST(test_constants);
    RUN_TEST(test_angle_conversion);
    return UNITY_END();
}
