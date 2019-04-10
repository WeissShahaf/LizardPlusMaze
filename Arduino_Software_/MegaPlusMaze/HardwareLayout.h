#ifndef HARDWARELAYOUT_H
#define HARDWARELAYOUT_H

#include "EventHandler.h"
#include "DigitalPulse.h"

class HardwareLayout : public EventHandler
{
public:
  HardwareLayout();
  ~HardwareLayout();

  virtual void push(byte command, char *p_data) override;
//  void update();/
  
  // define Events
  enum EventType : int
  {
    EVENT_HANDSHAKE = 0,
    EVENT_SELECT_START_ARM = 1,
    EVENT_SELECT_TARGET_ARM = 2,
    EVENT_OPEN_DOOR = 3,
    EVENT_CLOSE_DOOR = 4,
    EVENT_SELECT_SENSOR = 5,
    EVENT_START_TRIAL = 6,
    EVENT_STOP_TRIAL = 7,
    EVENT_DISABLE_ARMS = 8,
    EVENT_DISABLE_SENSORS = 9,
    EVENT_ERROR = 10,
    EVENT_TASK_STATE1_ON = 12,
    EVENT_TASK_STATE2_ON = 13,
    EVENT_TASK_STATE1_OFF = 14,
    EVENT_TASK_STATE2_OFF = 15,
    //EVENT_TASK_STATE2 = 16,
    MAX_EVENTS = 16
    
  };


  int PIN_START_TRIAL = 46;
  int PIN_STOP_TRIAL = 47;
  int PINS_SELECT_START_ARM[4] = {10,11,12,48};
  int PINS_SELECT_TARGET_ARM[4] = {42,43,44,45};
  int PINS_OPEN_DOOR[4] = {22,24,26,28};
  int PINS_CLOSE_DOOR[4] = {23,25,27,29};
  int PINS_SELECT_SENSOR[3] = {39,40,41};
  int PINS_BUTTON_OPEN_DOOR[4] = {2,4,6,8};
  int PINS_BUTTON_CLOSE_DOOR[4] = {3,5,7,9};
  int PINS_TASK_STATE1_ON[4] = {14,16,18,38};
  int PINS_TASK_STATE2_ON[4] = {15,17,19,49};
  int PINS_TASK_STATE1_OFF[4] = {14,16,18,38};
  int PINS_TASK_STATE2_OFF[4] = {15,17,19,49};
 

//  int PIN_START_TRIAL = 13;
//  int PIN_STOP_TRIAL = 13;
//  int PINS_SELECT_START_ARM[4] = {2,3,4,5};
//  int PINS_SELECT_TARGET_ARM[4] = {2,3,4,5};
//  int PINS_OPEN_DOOR[4] = {2,3,4,5};
//  int PINS_CLOSE_DOOR[4] = {2,3,4,5};
//  int PINS_SELECT_SENSOR[3] = {10,11,12};
//  int PINS_BUTTON_OPEN_DOOR[4] = {6,7,8,9};
//  int PINS_BUTTON_CLOSE_DOOR[4] = {6,7,8,9};
  
  int NUM_ARMS = 4;
  int NUM_SENSORS = 3;

//  byte m_hwTimer_port;
//  uint32_t m_hwTimer_ISRcount;

  void readSwitches();
  
private:
//  double m_hwTimer_frequency;
//  void hwTimerArm();
//  void hwTimerEnable();
//  void hwTimerDisable();

//  DigitalPulse PulseTable[PIN_COUNT];
  
  typedef void (HardwareLayout::*ActionType)(char *p_data);
  ActionType ActionTable[MAX_EVENTS];
  
  void ActionSelectStartArm(char *p_data);
  void ActionSelectTargetArm(char *p_data);
  void ActionSelectSensor(char *p_data);
  void ActionOpenDoor(char *p_data);
  void ActionCloseDoor(char *p_data);
  void ActionStartTrial(char *p_data);
  void ActionStopTrial(char *p_data);
  void ActionDisableArms(char *p_data);
  void ActionDisableSensors(char *p_data);
  void ActionError(char *p_data);
  void ActionTaskState1_ON(char *p_data);
  void ActionTaskState2_ON(char *p_data);
  void ActionTaskState1_OFF(char *p_data);
  void ActionTaskState2_OFF(char *p_data);

};

#endif /* HARDWARELAYOUT_H */
