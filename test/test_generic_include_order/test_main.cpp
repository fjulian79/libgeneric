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
 * generic.hpp has to work regardless of the include order. Here the standard
 * headers declaring functions with the names of the former macros, e.g.
 * round(), abs() or std::min(), are included after it. If this test does not
 * build, generic.hpp breaks these headers.
 */

#include "generic/generic.hpp"

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_standard_functions_usable(void)
{
    TEST_ASSERT_EQUAL(3, std::abs(-3));
    TEST_ASSERT_EQUAL(3, std::lround(2.5));
    TEST_ASSERT_EQUAL(2, std::min(2, 3));
    TEST_ASSERT_EQUAL(3, std::max(2, 3));
}

/**
 * Macros colliding with the standard library or Arduino must not be defined.
 */
void test_colliding_macros_not_defined(void)
{
#if defined(abs) || defined(round) || defined(min) || defined(max)
    TEST_FAIL_MESSAGE("abs, round, min or max defined as macro");
#endif
#if defined(constrain) || defined(map) || defined(sq) || defined(radians) || defined(degrees)
    TEST_FAIL_MESSAGE("constrain, map, sq, radians or degrees defined as macro");
#endif
#if defined(PI) || defined(HALF_PI) || defined(TWO_PI) || defined(DEG_TO_RAD) || defined(RAD_TO_DEG)
    TEST_FAIL_MESSAGE("PI or related constants defined as macro");
#endif
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_standard_functions_usable);
    RUN_TEST(test_colliding_macros_not_defined);
    return UNITY_END();
}
