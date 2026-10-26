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

#include <string.h>

#include "generic/lineringbuffer.hpp"

LineRingBuffer::LineRingBuffer(void) :
    RingBuffer()
{

}

LineRingBuffer::LineRingBuffer(char *buf, size_t siz) :
    RingBuffer(buf, siz)
{

}

bool LineRingBuffer::append(const char *str, size_t len)
{
    const char term = '\0';

    /* One byte of the buffer is always kept free, one is needed for the
     * null terminator */
    if (str == 0 || len == 0 || Size < 2 || len > Size - 2)
        return false;

    while (getFree() < len + 1)
        dropOldest();

    RingBuffer::write(str, len);
    RingBuffer::write(&term, 1);
    return true;
}

bool LineRingBuffer::append(const char *str)
{
    return str != 0 && append(str, strlen(str));
}

void LineRingBuffer::dropOldest(void)
{
    size_t pos = first();

    if (pos != npos)
        RingBuffer::free(getLength(pos) + 1);
}

size_t LineRingBuffer::first(void) const
{
    return getUsed() != 0 ? 0 : npos;
}

size_t LineRingBuffer::last(void) const
{
    size_t used = getUsed();

    if (used == 0)
        return npos;

    /* used - 1 is the null terminator of the newest line, search the one of
     * the line before */
    for (size_t pos = used - 1; pos > 0; pos--)
    {
        if (at(pos - 1) == '\0')
            return pos;
    }

    return 0;
}

size_t LineRingBuffer::next(size_t pos) const
{
    size_t used = getUsed();

    if (pos >= used)
        return npos;

    pos += getLength(pos) + 1;
    return pos < used ? pos : npos;
}

size_t LineRingBuffer::prev(size_t pos) const
{
    if (pos == 0 || pos == npos || pos >= getUsed())
        return npos;

    /* pos - 1 is the null terminator of the previous line, search the one of
     * the line before */
    for (pos = pos - 1; pos > 0; pos--)
    {
        if (at(pos - 1) == '\0')
            return pos;
    }

    return 0;
}

size_t LineRingBuffer::getLength(size_t pos) const
{
    size_t used = getUsed();
    size_t len = 0;

    while (pos + len < used && at(pos + len) != '\0')
        len++;

    return len;
}

size_t LineRingBuffer::read(size_t pos, char *line, size_t len) const
{
    size_t linelen = 0;

    if (line == 0 || pos >= getUsed())
        return 0;

    linelen = getLength(pos);
    if (linelen + 1 > len)
        return 0;

    peek(line, linelen, pos);
    line[linelen] = '\0';
    return linelen;
}

bool LineRingBuffer::setState(const State &state)
{
    State old = getState();

    if (!RingBuffer::setState(state))
        return false;

    if (getUsed() != 0 && at(getUsed() - 1) != '\0')
    {
        RingBuffer::setState(old);
        return false;
    }

    return true;
}
