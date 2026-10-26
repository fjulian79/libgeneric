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

#include <stddef.h>
#include <type_traits>

/*
 * Hence that the names used here must neither collide with the standard
 * library nor with the macros defined by Arduino cores, like constrain(),
 * radians() or PI. The preprocessor does not know namespaces, so this applies
 * to the content of the namespace as well.
 */


#ifndef arraysize
/**
 * @brief Used to get the number of elements of an array.
 *
 * Only works for arrays, not for pointers.
 */
#define arraysize(_a)           (sizeof(_a)/sizeof((_a)[0]))
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
            const __typeof__( ((_type *)0)->_member ) *__mptr = (_ptr); \
            (_type *)( (char *)__mptr - offsetof(_type, _member) );     \
        })
#endif


#ifndef unused
/**
 * @brief Used to suppress warnings of type:
 * warning: unused parameter '_x' [-Wunused-parameter]
 */
#define unused(_x)              (void)(_x)
#endif


namespace generic
{

/**
 * @brief Math constant pi.
 *
 * Use e.g. static_cast<float>(generic::pi) to get it as float, the conversion
 * is done at compile time.
 */
constexpr double pi = 3.14159265358979323846;

/**
 * @brief pi/2
 */
constexpr double halfPi = pi / 2;

/**
 * @brief 2*pi
 */
constexpr double twoPi = pi * 2;

/**
 * @brief Used to deduce the type of a template only from the first argument,
 * further arguments, e.g. literals, are converted to it.
 */
template <typename T> struct Identity
{
    typedef T type;
};

/**
 * @brief Used to limit x to the range [lo, hi].
 *
 * @return x if within the range, lo if below, hi if above. The type of the
 *         result is the type of x.
 */
template <typename T>
constexpr T clamp(T x, typename Identity<T>::type lo, typename Identity<T>::type hi)
{
    return x < lo ? lo : (hi < x ? hi : x);
}

/**
 * @brief Used to map x from the range [inMin, inMax] to [outMin, outMax].
 *
 * The calculation is done in the type of x, so for integers the result is
 * truncated and the intermediate product (x - inMin) * (outMax - outMin)
 * has to fit into this type, respectively into int.
 *
 * @return The mapped value, the type of the result is the type of x.
 */
template <typename T>
constexpr T mapRange(T x, typename Identity<T>::type inMin, typename Identity<T>::type inMax,
    typename Identity<T>::type outMin, typename Identity<T>::type outMax)
{
    return static_cast<T>((x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin);
}

/**
 * @brief Used to calculate the square of x.
 *
 * @return x * x, the type of the result is the type of x.
 */
template <typename T>
constexpr T square(T x)
{
    return static_cast<T>(x * x);
}

/**
 * @brief Used to increment x by n and to wrap around to zero at s, e.g. for
 * indices of a ring buffer.
 *
 * @return (x + n) % s, the type of the result is the type of x.
 */
template <typename T>
constexpr T wrapInc(T x, typename Identity<T>::type n, typename Identity<T>::type s)
{
    return static_cast<T>((x + n) % s);
}

/**
 * @brief Used to convert degrees to radians.
 *
 * Only for floating point types, the calculation is done in the type of deg,
 * e.g. a float is not promoted to double.
 */
template <typename T>
constexpr T toRadians(T deg)
{
    static_assert(std::is_floating_point<T>::value, "toRadians() needs a floating point type");
    return deg * static_cast<T>(pi / 180);
}

/**
 * @brief Used to convert radians to degrees.
 *
 * Only for floating point types, the calculation is done in the type of rad,
 * e.g. a float is not promoted to double.
 */
template <typename T>
constexpr T toDegrees(T rad)
{
    static_assert(std::is_floating_point<T>::value, "toDegrees() needs a floating point type");
    return rad * static_cast<T>(180 / pi);
}

} /* namespace generic */

#endif /* GENERIC_HPP_ */
