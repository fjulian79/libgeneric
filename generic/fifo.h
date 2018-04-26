/*
 * fifo.h
 *
 *  Created on: May 28, 2017
 *      Author: julian
 */

#ifndef GENERIC_FIFO_H_
#define GENERIC_FIFO_H_

#include <stdint.h>
#include <stddef.h>

/**
 * FiFo Data structure with all data elements needed.
 */
class Fifo
{
    public:

        Fifo();

        Fifo(char *buf, size_t siz);

        /**
         * Used to initialize the fifo.
         *
         * @param buf       The fifo buffer to operate on.
         * @param siz       The size of the provided buffer.
         */
        void init(char *buf, size_t siz);

        /**
         * To get the size of the fifo during runtime.
         *
         * @return The data buffer size in bytes.
         */
        size_t getSize(void);

        /**
         * To get the amount of used fifo data buffer space.
         *
         * @return The number of used bytes.
         */
        size_t getUsed(void);

        /**
         * To get the amount of free fifo data buffer space.
         *
         * @return the number of free bytes.
         */
        size_t getFree(void);

        /**
         * Used to write a given number of bytes from to the fifo.
         *
         * @param buf       The provided data.
         * @param siz       The number of bytes to write.
         *
         * @return The number of written bytes.
         */
        size_t write(const void *buf, size_t siz);

        /**
         * Used to put just one byte to the fifo.
         *
         * @param c         The byte to write to the fifo.
         *
         * @return The number of bytes written (0/1).
         */
        size_t put(const void *c);

        /**
         * Used to copy data from the fifo to the provided buffer.
         *
         * @param buf       The target buffer to write to.
         * @param siz       Number of bytes to read. Hence that less bytes then
         *                  requested will be read from the fifo if less bytes are
         *                  available.
         *
         * @return          The number of bytes read.
         */
        size_t read(void *buf, size_t siz);

        /**
         *
         * @param buf       A pointer to a pointer to take the address of the fifo data
         *
         * @return          The number of bytes which can be read from the fifo in a
         *                  subsequent way.
         */
        size_t getReadBlock(void **buf);

        /**
         * Used to free space in the fifo. The function will free not more bytes then
         * provided, but maybe less if less bytes are used.
         *
         * @param f         The fifo to use.
         * @param siz       Number of bytes to free.
         */
        void free(size_t siz);

    private:

        /**
         * The data array.
         */
        char *pData;

        /**
         * Size of the data array.
         */
        size_t Size;

        /**
         * Write index.
         */
        volatile size_t Head;

        /**
         * Read index.
         */
        volatile size_t Tail;
};

#endif /* GENERIC_FIFO_H_ */
