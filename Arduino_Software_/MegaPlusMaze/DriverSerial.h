#ifndef DRIVERSERIAL_H
#define DRIVERSERIAL_H

#include "Arduino.h"

class DriverSerial
{
public:
  DriverSerial();
  ~DriverSerial();

  void arm(const int baudRate);
  void poll();
  boolean received();
  char* message();
  void send(char *package);

private:
  const byte MESSAGE_END = 0x0A;
  static const byte SERIAL_BUFFER_SIZE = 16;
  boolean m_flag;
  int m_readBufferIndex;
  char m_readBuffer[SERIAL_BUFFER_SIZE];
};

#endif /* DRIVERSERIAL_H */
