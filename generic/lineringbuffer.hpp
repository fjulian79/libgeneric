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

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "generic/ringbuffer.hpp"

/**
 * @brief A ring buffer storing lines of text, e.g. for logs or a history.
 *
 * Lines are stored with their null terminator only, without any additional
 * metadata, and may wrap around the end of the buffer. Appending a line
 * removes as many of the oldest lines as needed to store it.
 *
 * Lines are addressed by positions, which are offsets counted from the start
 * of the oldest line. Positions are only valid until the next call to
 * append(), as it might remove old lines.
 *
 * Not safe for concurrent use, as append() modifies both ends of the buffer.
 *
 *  char logData[1024];
 *  LineRingBuffer log(logData, sizeof(logData));
 *  char line[128];
 *
 *  log.append("WiFi: Connected");
 *  log.append("NTP: Time synchronized");
 *
 *  for (size_t pos = log.first(); pos != LineRingBuffer::npos; pos = log.next(pos))
 *  {
 *      if (log.read(pos, line, sizeof(line)))
 *          Serial.println(line);
 *  }
 *
 * To keep the lines across a reset, place the buffer and the state in memory
 * which is not initialized at startup, e.g. RTC_NOINIT_ATTR on an ESP32, and
 * restore the state at startup. setState() only does a basic plausibility
 * check, so use an additional magic value to detect a power on:
 *
 *  RTC_NOINIT_ATTR char logData[1024];
 *  RTC_NOINIT_ATTR RingBuffer::State logState;
 *  RTC_NOINIT_ATTR uint32_t logMagic;
 *
 *  if (logMagic != LOG_MAGIC || !log.setState(logState))
 *  {
 *      log.clear();
 *      logMagic = LOG_MAGIC;
 *  }
 *  ...
 *  log.append("...");
 *  logState = log.getState();              // after each modification
 */
class LineRingBuffer : public RingBuffer
{
    public:

        /**
         * @brief Returned as position if there is no such line.
         */
        static const size_t npos = (size_t) -1;

        LineRingBuffer();

        LineRingBuffer(char *buf, size_t siz);

        /**
         * Used to store a line, removing the oldest lines as needed.
         *
         * @param str       The line to store, must not contain a null
         *                  terminator within len.
         * @param len       The length of the line excluding the null
         *                  terminator.
         *
         * @return true in case of success, false if the line is empty or
         *         larger than the buffer.
         */
        bool append(const char *str, size_t len);

        /**
         * Used to store a null terminated line, see append(str, len).
         */
        bool append(const char *str);

        /**
         * Used to remove the oldest line.
         */
        void dropOldest(void);

        /**
         * @return The position of the oldest line or npos if empty.
         */
        size_t first(void) const;

        /**
         * @return The position of the newest line or npos if empty.
         */
        size_t last(void) const;

        /**
         * @param pos       The position of a line.
         *
         * @return The position of the next (newer) line or npos.
         */
        size_t next(size_t pos) const;

        /**
         * @param pos       The position of a line.
         *
         * @return The position of the previous (older) line or npos.
         */
        size_t prev(size_t pos) const;

        /**
         * @param pos       The position of a line.
         *
         * @return The length of the line excluding the null terminator.
         */
        size_t getLength(size_t pos) const;

        /**
         * Used to copy a line including the null terminator to the provided
         * buffer.
         *
         * @param pos       The position of the line.
         * @param line      The buffer to copy the line to.
         * @param len       The size of the provided buffer.
         *
         * @return The number of bytes copied excluding the null terminator,
         *         0 if there is no such line or the buffer is too small.
         */
        size_t read(size_t pos, char *line, size_t len) const;

        /**
         * Used to restore a state previously returned by getState(), see
         * RingBuffer::setState(). In addition the buffer has to end with a
         * null terminator.
         *
         * @return true if the state is valid and has been restored.
         */
        bool setState(const State &state);

    private:

        /**
         * Hide the byte oriented functions of the base class, they would
         * break the line structure.
         */
        using RingBuffer::write;
        using RingBuffer::read;
        using RingBuffer::free;
        using RingBuffer::getReadBlock;
};
