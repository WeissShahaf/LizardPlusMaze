#include "EventHandler.h"

EventHandler::EventHandler() :
  mv_flag(false),
  mv_indexPush(0),
  mv_indexPop(0),
  mv_counter(0)
{
  // set queue memory to 0
  for (byte q = 0; q < EVENT_QUEUE_SIZE; q++)
  {
    m_queue[q].event = 0;
    m_queue[q].port = 0;
    m_queue[q].counter = 0;
    m_queue[q].timestamp = 0;
  }              
}

EventHandler::~EventHandler()
{
  
}

boolean EventHandler::poll()
{
  return mv_flag;
}

void EventHandler::dispatch(char *p_message)
{
  byte code;
  char *p_data;
  unpack(p_message, &code, &p_data);
  push(code, p_data);
}

void EventHandler::push(byte code, char *p_data)
{
  noInterrupts();
  
    // fill current queue element
    m_queue[mv_indexPush].event = code;
    m_queue[mv_indexPush].port = PIND;
    m_queue[mv_indexPush].counter = mv_counter;
    m_queue[mv_indexPush].timestamp = millis();

    // update event counter
    mv_counter++;

    // update push index
    mv_indexPush++;
    if (mv_indexPush == EVENT_QUEUE_SIZE)
      mv_indexPush = 0;
      
    // rise queue event
    mv_flag = true;  
    
  interrupts();
}

char* EventHandler::pop()
{
  char *p_package = NULL;
  if (mv_flag)
  {
    // pack respond
    EventHandler::Respond respond = m_queue[mv_indexPop];    
    char package[PACKAGE_BUFFER_SIZE];
    p_package = package;
    pack(&respond, p_package);

    // update pop index
    mv_indexPop++;
    if (mv_indexPop == EVENT_QUEUE_SIZE)
      mv_indexPop = 0;
  }

  if (mv_indexPop == mv_indexPush)
    mv_flag = false;

  return p_package; 
}

void EventHandler::unpack(char *p_message, byte *code, char **p_data)
{
  
  *code = (uint8_t)strtol(p_message, &p_message, 10);
  p_message++;
  if (*p_message != '\0')
  {
    *p_data = p_message;
  }
}

void EventHandler::pack(EventHandler::Respond *respond, char *p_package)
{
  sprintf(p_package, "%u,%u,%lu,%lu", 
          respond->event, 
          respond->port,
          respond->counter,
          respond->timestamp);               
}

