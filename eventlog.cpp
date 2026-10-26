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

#include <stdarg.h>
#include <stdio.h>

#include "generic/eventlog.hpp"
#include "generic/boundedwriter.hpp"

/**
 * @brief Size of a stored event, the timestamp, the separator and the event.
 */
#define EVENTLOG_ENTRY_SIZE             (EVENTLOG_TIMESTAMP_MAX + 1 + EVENTLOG_LINE_MAX)

EventLog::EventLog(char *data, size_t siz) :
      Lines(data, siz)
    , TimeStampFmtCb(nullptr)
    , Monitor(nullptr)
{

}

void EventLog::setTimeStamp(TimeStampFunc timeStamp)
{
    TimeStampFmtCb = timeStamp;
}

void EventLog::setMonitor(Print *monitor)
{
    Monitor = monitor;
}

Print *EventLog::getMonitor(void) const
{
    return Monitor;
}

void EventLog::log(const char *format, ...)
{
    char entry[EVENTLOG_ENTRY_SIZE];
    size_t len = 0;
    va_list args;
    int ret;

    if (TimeStampFmtCb != nullptr)
    {
        /* The callback can not write behind the space of the timestamp */
        BoundedWriter out(entry, EVENTLOG_TIMESTAMP_MAX + 1);
        TimeStampFmtCb(out);
        len = out.length();
        if (len > 0)
            entry[len++] = ' ';
    }

    va_start(args, format);
    ret = vsnprintf(entry + len, EVENTLOG_LINE_MAX, format, args);
    va_end(args);
    if (ret > 0)
        len += (size_t) ret < EVENTLOG_LINE_MAX ? (size_t) ret : EVENTLOG_LINE_MAX - 1;

    /* An empty line can not be stored */
    if (len == 0)
        return;

    Lines.append(entry, len);

    if (Monitor != nullptr)
    {
        /* Only a newline like the rest of the output, println() sends \r\n */
        Monitor->print(entry);
        Monitor->print('\n');
    }
}

size_t EventLog::readLast(char *buf, size_t siz) const
{
    size_t pos = Lines.last();

    if (pos == LineRingBuffer::npos)
        return 0;

    return Lines.read(pos, buf, siz);
}

void EventLog::printTo(Print &out) const
{
    char entry[EVENTLOG_ENTRY_SIZE];
    size_t len;

    for (size_t pos = Lines.first(); pos != LineRingBuffer::npos; pos = Lines.next(pos))
    {
        len = Lines.read(pos, entry, sizeof(entry));
        entry[len] = '\n';
        out.write((const uint8_t *) entry, len + 1);
    }
}

size_t EventLog::getLength(void) const
{
    /* Each event is stored with a null terminator, printed as newline */
    return Lines.getUsed();
}
