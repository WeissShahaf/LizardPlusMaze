#include "DriverSerial.h"

DriverSerial::DriverSerial() :
  m_flag(false),
  m_readBufferIndex(0)
{
  
}

DriverSerial::~DriverSerial()
{
  
}

void DriverSerial::arm(const int baudRate)
{
  Serial.begin(baudRate);
  Serial.flush();
}

void DriverSerial::poll()
{
  m_flag = false;

  if (Serial.available())
  {
    // read current byte from Serial buffer
    char newByte = (char)Serial.read();

    // check for read buffer overflow
    if (m_readBufferIndex >= SERIAL_BUFFER_SIZE)
      m_readBufferIndex = 0;

    // add new byte to read buffer
    m_readBuffer[m_readBufferIndex] = newByte;
    m_readBufferIndex++;

    // check if message arrived
    if (newByte == MESSAGE_END)
    {
      m_readBuffer[m_readBufferIndex - 1] = '\0';
      m_flag = true;
      m_readBufferIndex = 0;
    }
  }
}

boolean DriverSerial::received()
{
  poll();
  return m_flag;
}

char* DriverSerial::message()
{
  return m_readBuffer;
}

void DriverSerial::send(char *package)
{
  // send message
  if (package != NULL)
  {
    Serial.println(package);
  }
}

