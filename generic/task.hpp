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

#ifndef GENERIC_TASK_HPP_
#define GENERIC_TASK_HPP_

#include <stdint.h>
#include <stddef.h>

/**
 * Implents all whats needed for a typical Arduino task which lives in the mail loop.
 */
class Task
{
    public:
        /**
         * @brief Construct a new Task_t object.
         * 
         * @param tick The Tasks tick intervall in ms.
         * @param state If the taks shall be enabled or not. True by default.
         */
        Task(uint32_t tick, bool state = true);

        /**
         * @brief Checks if the task shall be scheduled or not.
         * 
         * If yes, the function updates also the LastTick value to the value
         * given by the "now" parameter. No need for any further house keeping.
         *          
         * @param now the current ticks in ms.
         * @return true     if it's time to schedule the task.
         * @return false    if it's NOT time to schedule the task.
         */
        bool isScheduled(uint32_t now);

        /**
         * @brief Get the configured tick interval.
         * 
         * @return uint32_t the tick interval in ms.
         */
        uint32_t getTick(void);

        /**
         * @brief Set the tick interval.
         * 
         * @param tick new tick interval in ms.
         */
        void setTick(uint32_t tick);

        /**
         * @brief Get the tick value of the last scheduling occurence.
         * 
         * @return uint32_t the last timestamp the task has been scheduled.
         */
        uint32_t getLastTick(void);

        /**
         * @brief Set the Last Tick value.
         * 
         * Comes in handy if a task has to be delayed or synced to some asyncronous events.
         * 
         * @param ms the tick value in ms to set as last tick time.
         */
        void setLastTick(uint32_t ms);

        /**
         * @brief If the task is enabled or not.
         * 
         * @return true     If the task is enabled.
         * @return false    If the task is not enabled.
         */
        bool isEnabled();
        
        /**
         * @brief Sets the enabled state of the task.
         * 
         * @param val   the new enabled state, true by default.
         */
        void enable(bool val = true);

    private:

        /**
         * @brief The tick intervall of the task.
         */
        uint32_t TickInterval;
        
        /**
         * @brief The last tick of the task.
         */
        uint32_t LastTick;
        
        /**
         * Stores if the task is enabled or not.
         */
        bool Enabled;
};

#endif /* GENERIC_TASK_HPP_ */