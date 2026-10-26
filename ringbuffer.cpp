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

#include "generic/generic.hpp"
#include "generic/ringbuffer.hpp"

RingBuffer::RingBuffer(void) :
      pData(0)
    , Size(0)
    , Head(0)
    , Tail(0)
{

}

RingBuffer::RingBuffer(char *buf, size_t siz) :
      pData(buf)
    , Size(siz)
    , Head(0)
    , Tail(0)
{

}

void RingBuffer::init(char *buf, size_t siz)
{
    pData = buf;
    Size = siz;
    Head = 0;
    Tail = 0;
}

void RingBuffer::clear(void)
{
    Head = 0;
    Tail = 0;
}

size_t RingBuffer::getSize(void) const
{
    return Size;
}

size_t RingBuffer::getUsed(void) const
{
    size_t head = Head;
    size_t tail = Tail;

    return head >= tail ? head - tail : Size - tail + head;
}

size_t RingBuffer::getFree(void) const
{
    if (Size == 0)
        return 0;

    return Size - getUsed() - 1;
}

size_t RingBuffer::write(const void *buf, size_t siz)
{
    size_t head = Head;
    size_t part = 0;

    siz = min(siz, getFree());
    if (siz == 0)
        return 0;

    part = min(siz, Size - head);
    memcpy(&pData[head], buf, part);
    memcpy(pData, ((const char *)buf) + part, siz - part);

    /* Update the index after the data has been written, see class
     * description */
    Head = (head + siz) % Size;
    return siz;
}

size_t RingBuffer::read(void *buf, size_t siz)
{
    siz = peek(buf, siz);
    free(siz);
    return siz;
}

size_t RingBuffer::peek(void *buf, size_t siz, size_t offset) const
{
    size_t used = getUsed();
    size_t pos = 0;
    size_t part = 0;

    if (offset >= used)
        return 0;

    siz = min(siz, used - offset);
    pos = (Tail + offset) % Size;
    part = min(siz, Size - pos);
    memcpy(buf, &pData[pos], part);
    memcpy(((char *)buf) + part, pData, siz - part);

    return siz;
}

size_t RingBuffer::getReadBlock(void **buf) const
{
    size_t used = getUsed();
    size_t tail = Tail;

    if (used == 0)
        return 0;

    *buf = &pData[tail];
    return min(used, Size - tail);
}

void RingBuffer::free(size_t siz)
{
    siz = min(siz, getUsed());
    if (siz == 0)
        return;

    Tail = (Tail + siz) % Size;
}

RingBuffer::State RingBuffer::getState(void) const
{
    State state;

    state.Head = Head;
    state.Tail = Tail;
    return state;
}

bool RingBuffer::setState(const State &state)
{
    if (Size == 0 || state.Head >= Size || state.Tail >= Size)
        return false;

    Head = state.Head;
    Tail = state.Tail;
    return true;
}
