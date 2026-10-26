/*
 * libgeneric, a collection of usefool macros and classes to be used in any 
 * kind of C/C++ project.
 *
 * Copyright (C) 2022 Julian Friedrich
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
 * @brief The polynoms of well known CRC8 algorithms. 
 * 
 * Hence that some of these algorithms need additional parameters to match the
 * standard, e.g. reflection or a final xor value. Use the parameter sets
 * defined below to get the standard CRC, e.g. crc8 crc(CRC8_MAXIM).
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
 * @brief The parameters of a CRC8 algorithm, see the catalogue of 
 * parametrised CRC algorithms:
 * https://reveng.sourceforge.io/crc-catalogue/1-15.htm#crc.cat-bits.8
 */
typedef struct
{
    /**
     * @brief The initial value of the crc register.
     */
    uint8_t init;

    /**
     * @brief The polynom, not reflected.
     */
    uint8_t poly;

    /**
     * @brief If the input bytes and the result are reflected (refin and 
     * refout in the catalogue).
     */
    bool reflected;

    /**
     * @brief The value the result is xored with.
     */
    uint8_t xorout;

} crc8_param_t;

/**
 * @brief Parameter sets of well known CRC8 algorithms.
 */
static const crc8_param_t CRC8_SMBUS        = {0x00, CRC8_POLY_DEFAULT,  false, 0x00};
static const crc8_param_t CRC8_CDMA2000     = {0xFF, CRC8_POLY_CDMA2000, false, 0x00};
static const crc8_param_t CRC8_DARC         = {0x00, CRC8_POLY_DARC,     true,  0x00};
static const crc8_param_t CRC8_DVB_S2       = {0x00, CRC8_POLY_DVB_S2,   false, 0x00};
static const crc8_param_t CRC8_EBU          = {0xFF, CRC8_POLY_EBU,      true,  0x00};
static const crc8_param_t CRC8_I_CODE       = {0xFD, CRC8_POLY_I_CODE,   false, 0x00};
static const crc8_param_t CRC8_ITU          = {0x00, CRC8_POLY_ITU,      false, 0x55};
static const crc8_param_t CRC8_MAXIM        = {0x00, CRC8_POLY_MAXIM,    true,  0x00};
static const crc8_param_t CRC8_ROHC         = {0xFF, CRC8_POLY_ROHC,     true,  0x00};
static const crc8_param_t CRC8_WCDMA        = {0x00, CRC8_POLY_WCDMA,    true,  0x00};

/**
 * @brief A Class to calculate crc8 values.
 * 
 * This class maintains its own internal crc value. It can be updated by calling 
 * the calc(...) member functions. To reset the crc value to a value of your 
 * choice just use the assignment operator, the parameters of the algorithm are
 * kept. To print the current value cast the instace to uint8_t. See the 
 * following example:
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
 * This class is tested against the check values of the CRC catalogue, see the
 * unit tests.
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
         * @param reflected Optional, if input bytes and result are reflected.
         *                  Defaults to false.
         * @param xorout    Optional, the value the result is xored with.
         *                  Defaults to zero.
         */
        crc8(uint8_t val = 0, uint8_t poly = CRC8_POLY_DEFAULT, 
            bool reflected = false, uint8_t xorout = 0);

        /**
         * @brief Construct a new crc8 object for a well known algorithm.
         * 
         * @param param     The parameters of the algorithm, e.g. CRC8_MAXIM.
         */
        crc8(const crc8_param_t &param);

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
        operator uint8_t() const;

        /**
         * @brief Used to reset the crc value, the parameters of the algorithm
         * are kept.
         * 
         * @param val       The new crc value, like the init value of the 
         *                  algorithm.
         * @return crc8&    This object.
         */
        crc8 &operator=(uint8_t val);

    private:

        /**
         * @brief The crc value stored in this calss.
         */
        uint8_t value; 
        
        /**
         * @brief The polynom used to calculate the crc value, already 
         * reflected if needed.
         */
        uint8_t polynom;

        /**
         * @brief If input bytes and result are reflected.
         */
        bool reflected;

        /**
         * @brief The value the result is xored with.
         */
        uint8_t xorout;
};

#endif /* CRC8_HPP_ */