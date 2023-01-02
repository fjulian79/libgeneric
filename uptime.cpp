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