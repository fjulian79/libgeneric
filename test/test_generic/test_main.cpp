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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_arraysize);
    RUN_TEST(test_container_of);
    RUN_TEST(test_unused);
    return UNITY_END();
}
