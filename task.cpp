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

#include "generic/task.hpp"

Task_t::Task_t(uint32_t tick, bool state) :
      ms(tick)
    , last(0)
    , enabled(state)
{

}

bool Task_t::isScheduled(uint32_t now)
{
    if (!enabled || (now - last < ms))
    {
        return false;
    }

    last = now;
    return true;
}

uint32_t Task_t::getTick(void)
{
    return ms;
}

void Task_t::setTick(uint32_t tick)
{
    ms = tick;
}

uint32_t Task_t::getLastTick(void)
{
    return last;
}

bool Task_t::isEnabled()
{
    return enabled;
}

void Task_t::enable(bool val)
{
    enabled = val;
}