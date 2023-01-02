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

#ifndef GENERIC_UPTIME_HPP_
#define GENERIC_UPTIME_HPP_

#include <Arduino.h>

/**
 * Implements simple up time management without rollover within a typical
 * humans livetime.
 */
class UpTime
{
    public:
    
        /**
         * @brief Constructor.
        */
        UpTime(void);

        /**
         * @brief Syncs this class to millis(), shall be called in setup()!
         */
        void begin(void);

        /**
         * @brief Used to manipulate the uptime.
         * 
         * @param ms the new uptime value in milliseconds.
        */
        void set(uint64_t ms);

        /**
         * Used to reset everything to the intial state, uptime = 0ms.
         */
        void reset(void);

        /**
         * @brief The main loop function to be places in loop()
         * 
         * This function maintains the uptime. 
         */
        void loop(void);

        /**
         * Used to get a Linux style uptime string.
         * 
         * @return The current uptime as printable text.
         */
        String toString(void);

    private:

        /**
         * The millisecond counter which is the foundation 
         */
        uint64_t Milliseconds;

        /**
         * The last millis() value seen in the loop functions. 
         */
        uint32_t LastTick;
};

#endif /* GENERIC_UPTIME_HPP_ */