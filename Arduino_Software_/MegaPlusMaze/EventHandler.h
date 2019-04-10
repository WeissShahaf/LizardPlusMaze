#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Arduino.h"

class EventHandler
{
public:
  EventHandler();
  ~EventHandler();

  boolean poll();
  void dispatch(char *p_message);
  virtual void push(byte command, char *p_data);
  char* pop();
  
private:
  static const byte EVENT_QUEUE_SIZE = 8;
  static const byte PACKAGE_BUFFER_SIZE = 16;
  volatile boolean mv_flag;
  volatile byte mv_indexPush;
  volatile byte mv_indexPop;
  volatile uint32_t mv_counter;

  struct Respond
  {
    byte event;
    byte port;
    uint32_t counter;
    uint32_t timestamp;
  };

  EventHandler::Respond m_queue[EVENT_QUEUE_SIZE];
  
  void unpack(char *p_message, byte *code, char **p_data);
  void pack(EventHandler::Respond *respond, char *p_package);
  
};

#endif /* EVENTHANDLER_H */
