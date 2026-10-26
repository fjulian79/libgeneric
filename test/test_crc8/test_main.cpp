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

#include "generic/crc8.hpp"

void setUp(void) {}
void tearDown(void) {}

/**
 * The check value of a CRC is the CRC of the ASCII string "123456789", see
 * the catalogue of parametrised CRC algorithms:
 * https://reveng.sourceforge.io/crc-catalogue/1-15.htm#crc.cat-bits.8
 */
static const char checkData[] = "123456789";

static uint8_t checkValue(uint8_t init, uint8_t poly)
{
    crc8 crc(init, poly);

    return crc.calc((void *) checkData, strlen(checkData));
}

void test_default_is_smbus(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xF4, checkValue(0x00, CRC8_POLY_DEFAULT));
}

void test_cdma2000(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xDA, checkValue(0xFF, CRC8_POLY_CDMA2000));
}

void test_darc(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x15, checkValue(0x00, CRC8_POLY_DARC));
}

void test_dvb_s2(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xBC, checkValue(0x00, CRC8_POLY_DVB_S2));
}

void test_ebu(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x97, checkValue(0xFF, CRC8_POLY_EBU));
}

void test_i_code(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x7E, checkValue(0xFD, CRC8_POLY_I_CODE));
}

void test_itu(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xA1, checkValue(0x00, CRC8_POLY_ITU));
}

void test_maxim(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xA1, checkValue(0x00, CRC8_POLY_MAXIM));
}

void test_rohc(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xD0, checkValue(0xFF, CRC8_POLY_ROHC));
}

void test_wcdma(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x25, checkValue(0x00, CRC8_POLY_WCDMA));
}

void test_bytewise_equals_blockwise(void)
{
    crc8 block;
    crc8 bytewise;

    block.calc((void *) checkData, strlen(checkData));
    for (size_t i = 0; i < strlen(checkData); i++)
        bytewise.calc((uint8_t) checkData[i]);

    TEST_ASSERT_EQUAL_HEX8((uint8_t) block, (uint8_t) bytewise);
}

void test_reset_by_assignment(void)
{
    crc8 crc;

    crc.calc((void *) checkData, strlen(checkData));
    crc = 0;
    TEST_ASSERT_EQUAL_HEX8(0xF4, crc.calc((void *) checkData, strlen(checkData)));
}

void test_empty_data_keeps_value(void)
{
    crc8 crc(0x5A);

    TEST_ASSERT_EQUAL_HEX8(0x5A, crc.calc((void *) checkData, 0));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_is_smbus);
    RUN_TEST(test_cdma2000);
    RUN_TEST(test_darc);
    RUN_TEST(test_dvb_s2);
    RUN_TEST(test_ebu);
    RUN_TEST(test_i_code);
    RUN_TEST(test_itu);
    RUN_TEST(test_maxim);
    RUN_TEST(test_rohc);
    RUN_TEST(test_wcdma);
    RUN_TEST(test_bytewise_equals_blockwise);
    RUN_TEST(test_reset_by_assignment);
    RUN_TEST(test_empty_data_keeps_value);
    return UNITY_END();
}
