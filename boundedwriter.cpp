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

#include "generic/boundedwriter.hpp"

BoundedWriter::BoundedWriter(char *buf, size_t siz) :
      Buf(buf)
    , Size(siz)
    , Len(0)
    , Overflow(false)
{
    Buf[0] = 0;
}

size_t BoundedWriter::write(uint8_t c)
{
    if (Len + 1 >= Size)
    {
        Overflow = true;
        return 0;
    }

    Buf[Len++] = c;
    Buf[Len] = 0;
    return 1;
}

void BoundedWriter::clear(void)
{
    Len = 0;
    Overflow = false;
    Buf[0] = 0;
}

const char *BoundedWriter::c_str(void) const
{
    return Buf;
}

size_t BoundedWriter::length(void) const
{
    return Len;
}

bool BoundedWriter::overflowed(void) const
{
    return Overflow;
}
