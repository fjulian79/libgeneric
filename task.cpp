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

#include "generic/task.hpp"

Task::Task(uint32_t tick, bool state) :
      TickInterval(tick)
    , LastTick(0)
    , Enabled(state)
{

}

bool Task::isScheduled(uint32_t now)
{
    if (!Enabled || (now - LastTick < TickInterval))
    {
        return false;
    }

    setLastTick(now);
    return true;
}

uint32_t Task::getTick(void)
{
    return TickInterval;
}

void Task::setTick(uint32_t tick)
{
    TickInterval = tick;
}

uint32_t Task::getLastTick(void)
{
    return LastTick;
}

void Task::setLastTick(uint32_t ms)
{
    LastTick = ms;
}

bool Task::isEnabled()
{
    return Enabled;
}

void Task::enable(bool val)
{
    Enabled = val;
}