/*
 * fifo.cpp
 *
 *  Created on: May 28, 2017
 *      Author: julian
 */

#include "generic/generic.h"
#include "generic/fifo.h"

#include <string.h>

Fifo::Fifo(void) :
      pData(0)
    , Size(0)
    , Head(0)
    , Tail(0)
{

}

Fifo::Fifo(char * buf, size_t size) :
      pData(buf)
    , Size(size)
    , Head(0)
    , Tail(0)
{

}

void Fifo::init(char * buf, size_t size)
{
    pData = buf;
    Size = size;
    Head = 0;
    Tail = 0;
}

size_t Fifo::getSize(void)
{
    return Size;
}

size_t Fifo::getUsed(void)
{
    size_t used = 0;

    if (Head >= Tail)
        used = Head - Tail;
    else
        used = Head + (Size - Tail);

    return used;
}

size_t Fifo::getFree(void)
{
    return Size - getUsed() - 1;
}

size_t Fifo::write(const void *buf, size_t siz)
{
    size_t tmp = 0;

    siz = min(siz, getFree());

    if (siz == 0)
        goto out;

    if (Head + siz > Size)
    {
        tmp = Size - Head;
        memcpy(&pData[Head], buf, tmp);
        siz -= tmp;
        buf = ((char *)buf) + tmp;
        Head = 0;
    }

    memcpy(&pData[Head], buf, siz);
    Head += siz;

    out:
    return siz + tmp;
}

size_t Fifo::put(const void *c)
{
    size_t tmp = 0;

    if (getFree() == 0)
        goto out;

    if (Head + 1 > Size)
    {
        Head = 0;
    }

    pData[Head++] = *((char*)c);
    tmp = 1;

    out:
    return tmp;
}

size_t Fifo::read(void *buf, size_t siz)
{
    size_t tmp = 0;

    siz = min(siz, getUsed());

    if (siz == 0)
        goto out;

    if (Tail + siz > Size)
    {
        tmp = Size - Tail;
        memcpy(buf, (const void*) &pData[Tail], tmp);
        siz -= tmp;
        buf = ((char *)buf) + tmp;
        Tail = 0;
    }

    memcpy(buf, (const void*) &pData[Tail], siz);
    Tail += siz;

    if (Tail == Head)
    {
        Head = 0;
        Tail = 0;
    }

    out:
    return siz + tmp;
}

size_t Fifo::getReadBlock(void **buf)
{
    size_t used = getUsed();

    if (used == 0)
        goto out;

    *buf = &pData[Tail];

    if (Tail + used > Size)
    {
        used = Size - Tail;
    }

    out:
    return used;
}

void Fifo::free(size_t siz)
{
    size_t used = min(siz, getUsed());

    if (used == 0)
        return;

    if ((Tail + used) >= Size)
    {
        used -= Size - Tail;
        Tail = 0;
    }

    Tail += used;

    if (Tail == Head)
    {
        Head = 0;
        Tail = 0;
    }
}
