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

/*
 * Arduino cores define function like macros like constrain(), radians() or
 * sq() and constants like PI. The preprocessor does not know namespaces, so
 * generic.hpp must not use these names, not even within its namespace. Here
 * these macros are defined like Arduino.h does, after the standard headers,
 * before including generic.hpp. If this test does not build, generic.hpp
 * collides with Arduino.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <type_traits>
#include <unity.h>

#define PI                     3.1415926535897932384626433832795
#define HALF_PI                 1.5707963267948966192313216916398
#define TWO_PI                  6.283185307179586476925286766559
#define DEG_TO_RAD              0.017453292519943295769236907684886
#define RAD_TO_DEG              57.295779513082320876798154814105
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))
#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define abs(x)                  ((x)>0?(x):-(x))
#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
long map(long x, long in_min, long in_max, long out_min, long out_max);

#include "generic/generic.hpp"

void setUp(void) {}
void tearDown(void) {}

void test_generic_usable_with_arduino_macros(void)
{
    TEST_ASSERT_EQUAL(8, generic::clamp(10, 2, 8));
    TEST_ASSERT_EQUAL(50, generic::mapRange(5, 0, 10, 0, 100));
    TEST_ASSERT_EQUAL(9, generic::square(3));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, generic::halfPi, generic::toRadians(90.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, generic::pi, PI);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_generic_usable_with_arduino_macros);
    return UNITY_END();
}
