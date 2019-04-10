#include "DigitalPulse.h"

DigitalPulse::DigitalPulse(const byte pin) :
    m_dovalue(LOW),
    m_active(false),
    m_cycles(0),
    m_delay(0),
    m_uptime(1000),
    m_downtime(1000),
    m_state(STATE_IDLE)
{
    // set pin to digital output
    m_pin = pin;
    pinMode(m_pin, OUTPUT);

    // fill interval array
    m_interval[STATE_DELAY] = m_delay;
    m_interval[STATE_UPTIME] = m_uptime;
    m_interval[STATE_DOWNTIME] = m_downtime;
}

uint32_t DigitalPulse::duration()
{
    if (m_cycles == 0)
        return m_delay + m_uptime + m_downtime;
    
    return m_delay + m_cycles * (m_uptime + m_downtime);
}

boolean DigitalPulse::isRunning()
{
    return m_active;
}

void DigitalPulse::start()
{
    if (m_active)
      return;

    m_active = true;  
    m_state = STATE_DELAY;
    m_event_dovalue = m_dovalue;
    m_event_cycle = m_cycles;
    m_event_timestamp = millis();
    digitalWrite(m_pin, m_event_dovalue);
}

void DigitalPulse::stop()
{
    m_state = STATE_IDLE;
    digitalWrite(m_pin, m_dovalue);
}

void DigitalPulse::update()
{
    // skip if timer is not running
    if (m_state == STATE_IDLE)
        return;
    
    // check current interval
    if (millis() - m_event_timestamp >= m_interval[m_state])
    {
        // update state
        m_state++;
        
        // flip mode
        m_event_dovalue = (m_event_dovalue == HIGH) ? LOW : HIGH;
        
        // close cycle
        if (m_state == STATE_IDLE)
        {
            if (m_event_cycle != 1)
            {
                if (m_event_cycle == 0)
                    m_event_cycle++;
                m_event_cycle--;
                
                m_state = STATE_UPTIME;
            }
            else
            {
              m_event_dovalue = m_dovalue;
              m_active = false;
            }
        }
        
        // update event timestamp and line mode
        m_event_timestamp = millis();
        digitalWrite(m_pin, m_event_dovalue);
    }
}

void DigitalPulse::setCycles(uint8_t value)
{
  m_cycles = value;
}

void DigitalPulse::setDelay(uint32_t value)
{
  m_delay = value;
}

void DigitalPulse::setUptime(uint32_t value)
{
  m_uptime = value;
}

void DigitalPulse::setDowntime(uint32_t value)
{
  m_downtime = value;
}

