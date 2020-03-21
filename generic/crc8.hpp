/*
 * libgeneric, a collection of usefool macros and classes to be used in any 
 * kind of C/C++ project.
 *
 * Copyright (C) 2019 Julian Friedrich
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

#ifndef CRC8_HPP_
#define CRC8_HPP_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief The polynom used per default if nothing else is specified.
 */
#define CRC8_POLY_DEFAULT               0x07

/**
 * @brief The following polynoms are used for well known applications.
 */
#define CRC8_POLY_CDMA2000              0x9B
#define CRC8_POLY_DARC                  0x39
#define CRC8_POLY_DVB_S2                0xD5
#define CRC8_POLY_EBU                   0x1D
#define CRC8_POLY_I_CODE                0x1D
#define CRC8_POLY_ITU                   0x07
#define CRC8_POLY_MAXIM                 0x31
#define CRC8_POLY_ROHC                  0x07
#define CRC8_POLY_WCDMA                 0x9B

/**
 * @brief A Class to calculate crc8 values.
 * 
 * This class maintains its own internal crc value. It can be updated by calling 
 * the calc(...) member functions. To reset the crc value to a value of your 
 * choice just use the assignment operator, to print the current value cast the 
 * instace to uint8_t. See the following example:
 * 
 *  void test()
 *  {
 *     crc8 crc;
 *     char data[] = "Hello World!!";
 *     
 *     printf("Test CRC: 0x%02x\n", crc.calc(&data[0], strlen(data)));
 * 
 *     crc = 0x21;
 *     printf("Test CRC: 0x%02x\n", (uint8_t)crc);
 * 
 *     crc = 0;
 *     printf("Test CRC: 0x%02x\n", crc.calc(&data[0], strlen(data)));    
 *     printf("Test CRC: 0x%02x\n", (uint8_t)crc);
 *  };
 * 
 * This class has been tested against https://crccalc.com
 * ... at least roughly ;-)
 */
class crc8
{
    public:
 
        /**
         * @brief Construct a new crc8 object
         * 
         * @param val       Optional parameter used to initialize the crc value.
         *                  Defaults to zero.
         * @param poly      Optional parameter used to set the polynom.
         *                  Defaults to CRC8_POLY_DEFAULT
         */
        crc8(uint8_t val = 0, uint8_t poly = CRC8_POLY_DEFAULT);

        /**
         * @brief Destroy the crc8 object
         */
        ~crc8();

        /**
         * @brief Used to update the crc value by a single byte.
         * 
         *        If the class has been used before, the crc might has to be 
         *        reset before calling this function. See the example in the 
         *        class documentation.
         * 
         * @param data      The data byte.
         * @return uint8_t  The new crc value.
         */
        uint8_t calc(uint8_t data);
        
        /**
         * @brief Used to update the crc value by processing the provided data.
         *        
         *        If the class has been used before, the crc might has to be 
         *        reset before calling this function. See the example in the 
         *        class documentation.
         * 
         * @param pData     Pointer to the data to process.
         * @param siz       Number of bytes to process.
         * @return uint8_t  The new crc value.
         */
        uint8_t calc(void* pData, size_t siz);

        /**
         * @brief Cast to uint8_t operator used for reading the crc.
         * 
         * @return uint8_t  The current crc value.
         */
        operator uint8_t();

    private:

        /**
         * @brief The crc value stored in this calss.
         */
        uint8_t value; 
        
        /**
         * @brief The polynom used to calculate the crc value.
         */
        uint8_t polynom;
};

#endif /* CRC8_HPP_ */