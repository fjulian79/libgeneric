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

#ifndef GENERIC_HPP_
#define GENERIC_HPP_


#ifndef PI
/**
 * @brief Math constant PI
 */
#define PI                      3.1415926535897932384626433832795
#endif


#ifndef HALF_PI
/**
 * @brief Defines PI/2 as this is often used in caluclations
 */
#define HALF_PI                 1.5707963267948966192313216916398
#endif


#ifndef TWO_PI
/**
 * @brief Defines 2*PI as this is often used in caluclations
 */
#define TWO_PI                  6.283185307179586476925286766559
#endif


#ifndef DEG_TO_RAD
/**
 * @brief Used to convert degrees to radiant
 */
#define DEG_TO_RAD              0.017453292519943295769236907684886
#endif


#ifndef RAD_TO_DEG
/**
 * @brief Used to convert radiant to degrees
 */
#define RAD_TO_DEG              57.295779513082320876798154814105
#endif


#ifndef min
/**
 * @brief Returns the minimum of _a and _b.
 */
#define min(_a, _b)             ((_a) < (_b) ? (_a) : (_b))
#endif


#ifndef max
/**
 * @brief Returns the maximum of _a and _b.
 */
#define max(_a, _b)             (_a > _b ? _a : _b)
#endif


#ifndef abs
/**
 * @brief Returns the absolute value of _x
 */
#define abs(_x)                 ((_x)<0 ? -(_x) : (_x))
#endif


#ifndef wrapInc
/**
 * @brief Used to increment _x by _n and wrap around to zero at _s.
 */
#define wrapInc(_x, _n, _s)     (((_x) + (_n)) % (_s))
#endif


#ifndef constrain
/**
 * @brief Used to limit  _x to become not higher than _h AND not lower then _l.
 */
#define constrain(_x, _l, _h)                                           \
                                                                        \
        ((_x) < (_l) ? (_l) : ((_x) > (_h) ? (_h) : (_x)))
#endif


#ifndef map
/**
 * @brief Used to map a value from a given range into another.
 * 
 * WARNING: Use with care, might have to improved.
 */
#define map(_val, _in_min, _in_max, _out_min, _out_max)         \
                                                                \
        ((_val - _in_min) * (_out_max - _out_min)               \
         / (_in_max - _in_min) + _out_min)
#endif


#ifndef round
/**
 * @brief Used to round  _x next integer value.
 */
#define round(_x)                                                       \
                                                                        \
        ((_x) >=0 ? (long)((_x) + 0.5) : (long)((_x) - 0.5))
#endif


#ifndef radians
/**
 * @brief Used to convert degrees to radiants.
 */
#define radians(_d)             ((_d) * DEG_TO_RAD)
#endif


#ifndef degrees
/**
 * @brief Used to convert radiants to degrees.
 */
#define degrees(r)              ((_r) * RAD_TO_DEG)
#endif


#ifndef sq
/**
 * @brief Used to calculate the power of the given value.
 */
#define sq(_x)                  ((_x) * (_x))
#endif


#ifndef arraysize
/**
 * @brief Used to get the size of a array.
 */
#define arraysize(_a)           (sizeof(_a)/sizeof(_a[0]))
#endif


#ifndef offsetof
/**
 * @brief Used to get the offset of a member in a structure.
 */
#define offsetof(_st, _m)       ((size_t)&(((_st *)0)->_m))
#endif


#ifndef container_of
/**
 * @brief To get a pointer to the structure that contains _ptr.
 *
 * @param _ptr      Pointer to a member of a structure.
 * @param _type     Type of the desired structure pointer.
 * @param _member   Name of the member in _type to which _ptr is pointing to.
 */
#define container_of(_ptr, _type, _member)                              \
                                                                        \
        ({                                                              \
            const typeof( ((_type *)0)->_member ) *__mptr = (_ptr);     \
            (_type *)( (char *)__mptr - offsetof(_type, _member) );     \
        })
#endif


#ifndef unused
/**
 * @brief Used su supress warnings of type:
 * warning: unused parameter '_x' [-Wunused-parameter]
 */
#define unused(_x)              (void)(_x)
#endif


#ifndef breakIfDiverse
/**
 * @brief Provides a easy exit from loops if the privided values differe.
 */
#define breakIfDiverse(_a, _b)                  \
                                                \
        if(_a != _b) break;
#endif

#endif /* GENERIC_HPP_ */
