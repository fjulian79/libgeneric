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

static uint8_t checkValue(const crc8_param_t &param)
{
    crc8 crc(param);

    return crc.calc((void *) checkData, strlen(checkData));
}

void test_smbus(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xF4, checkValue(CRC8_SMBUS));
}

void test_cdma2000(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xDA, checkValue(CRC8_CDMA2000));
}

void test_darc(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x15, checkValue(CRC8_DARC));
}

void test_dvb_s2(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xBC, checkValue(CRC8_DVB_S2));
}

void test_ebu(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x97, checkValue(CRC8_EBU));
}

void test_i_code(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x7E, checkValue(CRC8_I_CODE));
}

void test_itu(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xA1, checkValue(CRC8_ITU));
}

void test_maxim(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xA1, checkValue(CRC8_MAXIM));
}

void test_rohc(void)
{
    TEST_ASSERT_EQUAL_HEX8(0xD0, checkValue(CRC8_ROHC));
}

void test_wcdma(void)
{
    TEST_ASSERT_EQUAL_HEX8(0x25, checkValue(CRC8_WCDMA));
}

/**
 * The constructor without parameter set, as used before parameter sets have
 * been added, e.g. by libparam.
 */
void test_default_constructor_is_smbus(void)
{
    crc8 crc;

    TEST_ASSERT_EQUAL_HEX8(0xF4, crc.calc((void *) checkData, strlen(checkData)));
}

void test_constructor_with_all_parameters(void)
{
    crc8 crc(0x00, CRC8_POLY_MAXIM, true, 0x00);

    TEST_ASSERT_EQUAL_HEX8(0xA1, crc.calc((void *) checkData, strlen(checkData)));
}

/**
 * Regression: The assignment used to construct a new object with default 
 * parameters, e.g. "crc = 0" silently switched a MAXIM crc to SMBUS.
 */
void test_assignment_keeps_parameters(void)
{
    crc8 crc(CRC8_MAXIM);

    crc.calc((void *) checkData, strlen(checkData));
    crc = 0;
    TEST_ASSERT_EQUAL_HEX8(0xA1, crc.calc((void *) checkData, strlen(checkData)));
}

/**
 * The crc of an empty message is the reflected init value xored with xorout.
 */
void test_empty_message(void)
{
    crc8 itu(CRC8_ITU);
    crc8 rohc(CRC8_ROHC);
    crc8 icode(CRC8_I_CODE);

    TEST_ASSERT_EQUAL_HEX8(0x55, (uint8_t) itu);
    TEST_ASSERT_EQUAL_HEX8(0xFF, (uint8_t) rohc);
    TEST_ASSERT_EQUAL_HEX8(0xFD, (uint8_t) icode);
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
    RUN_TEST(test_smbus);
    RUN_TEST(test_cdma2000);
    RUN_TEST(test_darc);
    RUN_TEST(test_dvb_s2);
    RUN_TEST(test_ebu);
    RUN_TEST(test_i_code);
    RUN_TEST(test_itu);
    RUN_TEST(test_maxim);
    RUN_TEST(test_rohc);
    RUN_TEST(test_wcdma);
    RUN_TEST(test_default_constructor_is_smbus);
    RUN_TEST(test_constructor_with_all_parameters);
    RUN_TEST(test_assignment_keeps_parameters);
    RUN_TEST(test_empty_message);
    RUN_TEST(test_bytewise_equals_blockwise);
    RUN_TEST(test_reset_by_assignment);
    RUN_TEST(test_empty_data_keeps_value);
    return UNITY_END();
}
