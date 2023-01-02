/*
 * libgeneric, a collection of usefool macros and classes to be used in any 
 * kind of C/C++ project.
 *
 * Copyright (C) 2023 Julian Friedrich
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

#include "generic/uptime.hpp"

UpTime::UpTime()
{
    reset();
}

void UpTime::begin(void)
{
    Milliseconds = millis();
    LastTick = Milliseconds;
}

void UpTime::set(uint64_t ms)
{
    Milliseconds = ms;
    LastTick = millis();
}

void UpTime::reset(void)
{
    set(0);
}

void UpTime::loop(void)
{
    uint32_t now = millis();
    uint32_t elapsed = now - LastTick;
    
    LastTick = now;
    Milliseconds += elapsed > 0 ? elapsed : 0;
}

String UpTime::toString(void)
{
    uint16_t ms   = (uint16_t)(Milliseconds % 1000);
    uint8_t sec   = (uint8_t)((Milliseconds /     1000) % 60);
    uint8_t min   = (uint8_t)((Milliseconds /    60000) % 60);
    uint8_t hrs   = (uint8_t)((Milliseconds /  3600000) % 24);
    uint32_t days = (uint32_t)(Milliseconds / 86400000);
    char tmp[32];

    snprintf(tmp, sizeof(tmp), "%lu days, %02u:%02u:%02u.%04u", days, hrs, min, sec, ms);

    return (String)(tmp);
}