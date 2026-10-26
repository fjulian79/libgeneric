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

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "generic/ringbuffer.hpp"

/**
 * @brief Byte FiFo operating on a buffer provided by the user.
 *
 * Safe to be used by one producer and one consumer concurrently, e.g. an
 * interrupt and the main loop, see RingBuffer for details.
 *
 *  char rxData[64];
 *  Fifo rxFifo(rxData, sizeof(rxData));
 *
 *  void UART_IRQHandler(void)              // producer
 *  {
 *      char c = UART->DR;
 *      rxFifo.put(&c);                     // dropped if the fifo is full
 *  }
 *
 *  void loop()                             // consumer
 *  {
 *      char c;
 *      while (rxFifo.get(&c))
 *          process(c);
 *  }
 *
 * To transmit via DMA, getReadBlock() provides the oldest data without copying
 * it and free() releases it once it has been transferred:
 *
 *  void *data;
 *  size_t len = txFifo.getReadBlock(&data);
 *  startDma(data, len);
 *  ...
 *  txFifo.free(len);                       // in the DMA complete interrupt
 */
class Fifo : public RingBuffer
{
    public:

        Fifo();

        Fifo(char *buf, size_t siz);

        /**
         * Used to put just one byte to the fifo.
         *
         * @param c         The byte to write to the fifo.
         *
         * @return The number of bytes written (0/1).
         */
        size_t put(const void *c);

        /**
         * Used to a single byte from the fifo to the provided buffer.
         *
         * @param buf       The target buffer to write to.
         *
         * @return          The number of bytes read.
         */
        size_t get(void *buf);
};
