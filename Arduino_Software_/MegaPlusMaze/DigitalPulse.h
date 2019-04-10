#ifndef DIGITALPULSE_H
#define DIGITALPULSE_H

#include "Arduino.h"

class DigitalPulse
{
public:
    DigitalPulse(){};
    DigitalPulse(const byte pin);
    uint32_t duration();
    boolean isRunning();
    void start();
    void stop();
    void update();

    void setCycles(uint8_t);
    void setDelay(uint32_t);
    void setUptime(uint32_t);
    void setDowntime(uint32_t);
    
private:
    byte m_pin;
    byte m_dovalue;
    byte m_event_dovalue;
    byte m_active;

    const uint8_t STATE_DELAY = 0;
    const uint8_t STATE_UPTIME = 1;
    const uint8_t STATE_DOWNTIME = 2;
    const uint8_t STATE_IDLE = 3;

    uint8_t m_cycles;
    uint8_t m_event_cycle;
    uint8_t m_state;
    
    uint32_t m_delay;
    uint32_t m_uptime;
    uint32_t m_downtime;
    uint32_t m_event_timestamp;
    uint32_t m_interval[3];
    
};

#endif /* DIGITALPULSE_H */
