/*
 * libgeneric, a collection of usefool macros and classes to be used in any 
 * kind of C/C++ project.
 *
 * Copyright (C) 2023 Julian Friedrich
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

#include "generic/crc8.hpp"

crc8::crc8(uint8_t val, uint8_t poly) :
      value(val)
    , polynom(poly)

{

}

crc8::~crc8(void)
{

}

uint8_t crc8::calc(uint8_t data)
{
    value ^= data;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (value & 0x80)
        {
            value = (value << 1) ^ polynom;
        }
        else
        {
            value <<= 1;
        }
    }

    return value;
}
        
uint8_t crc8::calc(void* pData, size_t siz)
{
    uint8_t* tmp = (uint8_t*)pData;

    while (siz > 0)
    {
        calc(*tmp++);
        siz--;
    }

    return value;
}

crc8::operator uint8_t() 
{
    return value;
}
