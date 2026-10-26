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

#ifndef GENERIC_RINGBUFFER_HPP_
#define GENERIC_RINGBUFFER_HPP_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief A byte ring buffer operating on a buffer provided by the user.
 *
 * Base class for byte and line oriented ring buffers like Fifo and
 * LineRingBuffer. Positions are stored as indices, not as pointers, so the
 * state can be saved and restored, e.g. to keep the content across a reset.
 *
 * One byte of the buffer is always kept free to distinguish between a full
 * and an empty buffer. Thereby it is safe to use one producer calling write()
 * and one consumer calling read(), free() and getReadBlock() concurrently,
 * e.g. an interrupt and the main loop, as the producer only modifies Head and
 * the consumer only modifies Tail.
 */
class RingBuffer
{
    public:

        /**
         * @brief The state of the ring buffer, used to save and restore it.
         */
        typedef struct
        {
            size_t Head;
            size_t Tail;
        } State;

        RingBuffer();

        RingBuffer(char *buf, size_t siz);

        /**
         * Used to initialize the ring buffer, the buffer is empty afterwards.
         *
         * @param buf       The buffer to operate on.
         * @param siz       The size of the provided buffer.
         */
        void init(char *buf, size_t siz);

        /**
         * Used to remove all data from the ring buffer.
         */
        void clear(void);

        /**
         * To get the size of the ring buffer during runtime.
         *
         * @return The data buffer size in bytes, one byte less can be used.
         */
        size_t getSize(void) const;

        /**
         * To get the amount of used buffer space.
         *
         * @return The number of used bytes.
         */
        size_t getUsed(void) const;

        /**
         * To get the amount of free buffer space.
         *
         * @return the number of free bytes.
         */
        size_t getFree(void) const;

        /**
         * Used to write a given number of bytes to the ring buffer.
         *
         * @param buf       The provided data.
         * @param siz       The number of bytes to write.
         *
         * @return The number of written bytes, less than requested if there
         *         is not enough free space.
         */
        size_t write(const void *buf, size_t siz);

        /**
         * Used to copy data from the ring buffer to the provided buffer and to
         * remove it from the ring buffer.
         *
         * @param buf       The target buffer to write to.
         * @param siz       Number of bytes to read. Hence that less bytes than
         *                  requested will be read if less bytes are available.
         *
         * @return          The number of bytes read.
         */
        size_t read(void *buf, size_t siz);

        /**
         * Used to copy data from the ring buffer to the provided buffer
         * without removing it.
         *
         * @param buf       The target buffer to write to.
         * @param siz       Number of bytes to read.
         * @param offset    Number of bytes to skip, counted from the oldest
         *                  byte.
         *
         * @return          The number of bytes read.
         */
        size_t peek(void *buf, size_t siz, size_t offset = 0) const;

        /**
         * Used to get the oldest data in the ring buffer without copying it.
         *
         * @param buf       A pointer to a pointer to take the address of the
         *                  data.
         *
         * @return          The number of bytes which can be read from the
         *                  given address in a subsequent way. Hence that more
         *                  data might be available after calling free().
         */
        size_t getReadBlock(void **buf) const;

        /**
         * Used to free space in the ring buffer by removing the oldest data.
         * The function will free not more bytes than provided, but maybe less
         * if less bytes are used.
         *
         * @param siz       Number of bytes to free.
         */
        void free(size_t siz);

        /**
         * Used to get the current state, e.g. to restore it after a reset.
         *
         * @return The current state.
         */
        State getState(void) const;

        /**
         * Used to restore a state previously returned by getState(). The
         * buffer given to init() has to hold the data belonging to the state.
         *
         * @param state     The state to restore.
         *
         * @return true if the state is valid and has been restored, false
         *         otherwise. The ring buffer is not modified in this case.
         */
        bool setState(const State &state);

    protected:

        /**
         * Used to get the byte at the given offset, counted from the oldest
         * byte. The offset has to be less than getUsed().
         */
        inline char at(size_t offset) const
        {
            return pData[(Tail + offset) % Size];
        }

        /**
         * The data array.
         */
        char *pData;

        /**
         * Size of the data array.
         */
        size_t Size;

        /**
         * Write index, only modified by the producer.
         */
        volatile size_t Head;

        /**
         * Read index, only modified by the consumer.
         */
        volatile size_t Tail;
};

#endif /* GENERIC_RINGBUFFER_HPP_ */
