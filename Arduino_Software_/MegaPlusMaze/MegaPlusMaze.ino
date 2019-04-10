/*
 *  MegaPlusMaze
 *
 *  June 2018
 *  Georgi Tushev, Friedrich Kretschmer
 *  Scientific Computing
 *  Max-Planck Institute For Brain Research
 *  scicopm@lists.brain.mpg.de
 */

#include "DriverSerial.h"
#include "EventHandler.h"
#include "HardwareLayout.h"

DriverSerial driver = DriverSerial();
HardwareLayout layout = HardwareLayout();

struct Respond
  {
    uint8_t event;
    uint8_t port;
    uint32_t counter;
    uint32_t timestamp;
  };

boolean flag = true;

void setup()
{
  // assign callbacks

  // arm serial driver and test connection
  driver.arm(9600);
  layout.push(HardwareLayout::EVENT_HANDSHAKE, 0);
}

void loop()
{
 
  // check if command is received
  if (driver.received())
  {
    layout.dispatch(driver.message());
  }

  // check if an event queue message
  // is waiting to be sent
  if (layout.poll())
  {
    driver.send(layout.pop());
  }

  layout.readSwitches();
}
