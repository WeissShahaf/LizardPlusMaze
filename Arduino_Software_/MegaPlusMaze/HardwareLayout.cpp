#include "HardwareLayout.h"

HardwareLayout::HardwareLayout()
{
  // link events to actions
  ActionTable[EVENT_SELECT_START_ARM] = &HardwareLayout::ActionSelectStartArm;
  ActionTable[EVENT_SELECT_TARGET_ARM] = &HardwareLayout::ActionSelectTargetArm;
  ActionTable[EVENT_OPEN_DOOR] = &HardwareLayout::ActionOpenDoor;
  ActionTable[EVENT_CLOSE_DOOR] = &HardwareLayout::ActionCloseDoor;
  ActionTable[EVENT_SELECT_SENSOR] = &HardwareLayout::ActionSelectSensor;
  ActionTable[EVENT_START_TRIAL] = &HardwareLayout::ActionStartTrial;
  ActionTable[EVENT_STOP_TRIAL] = &HardwareLayout::ActionStopTrial;
  ActionTable[EVENT_DISABLE_ARMS] = &HardwareLayout::ActionDisableArms;
  ActionTable[EVENT_DISABLE_SENSORS] = &HardwareLayout::ActionDisableSensors;
  ActionTable[EVENT_ERROR] = &HardwareLayout::ActionError;
  ActionTable[EVENT_TASK_STATE1_ON] = &HardwareLayout::ActionTaskState1_ON;
  ActionTable[EVENT_TASK_STATE2_ON] = &HardwareLayout::ActionTaskState2_ON;
  ActionTable[EVENT_TASK_STATE1_OFF] = &HardwareLayout::ActionTaskState1_OFF;
  ActionTable[EVENT_TASK_STATE2_OFF] = &HardwareLayout::ActionTaskState2_OFF;
  //ActionTable[EVENT_TASK_STATE2] = &HardwareLayout::ActionTaskState2;
  
  

  // set pins mode
  for(int i=0;i<NUM_ARMS;i++){
    pinMode(PINS_SELECT_START_ARM[i], OUTPUT);
    pinMode(PINS_SELECT_TARGET_ARM[i], OUTPUT);
    pinMode(PINS_OPEN_DOOR[i], OUTPUT);
    pinMode(PINS_CLOSE_DOOR[i], OUTPUT);
  }
  
  for(int i=0;i<NUM_SENSORS;i++){
    pinMode(PINS_SELECT_SENSOR[i], OUTPUT);
  }

  pinMode(PIN_START_TRIAL, OUTPUT);
  pinMode(PIN_STOP_TRIAL, OUTPUT);

  for(int i=0;i<NUM_ARMS;i++){
    pinMode(PINS_TASK_STATE1_ON[i], OUTPUT);//PINS_TASK_STATE1
    pinMode(PINS_TASK_STATE2_ON[i], OUTPUT);//PINS_TASK_STATE2
  }
}



HardwareLayout::~HardwareLayout()
{
  
}

void HardwareLayout::push(byte command, char *p_data)
{
  // check event validity
//  if (command >= EVENT_ERROR)
//    command = EVENT_ERROR;

  // push event to queue
  EventHandler::push(command, p_data);

  // evoke callback
  if (ActionTable[command] != NULL)
    (this->*ActionTable[command])(p_data);
}

void HardwareLayout::ActionSelectStartArm(char *p_data)
{
  Serial.print("HardwareLayout::ActionSelectStartArm ");
  if (p_data) {
    Serial.print(p_data);
  }
  Serial.println();
  
  for(int i=0 ; i < NUM_ARMS ; i++){
    if(i == atoi(p_data)-1){
      digitalWrite(PINS_SELECT_START_ARM[i], HIGH);
    }else{
      digitalWrite(PINS_SELECT_START_ARM[i], LOW);
    }
  }
  
}

void HardwareLayout::ActionSelectTargetArm(char *p_data)
{
  Serial.print("HardwareLayout::ActionSelectTargetArm ");
  if (p_data) {
    Serial.print(p_data);
  }
  Serial.println();
  
  for(int i=0 ; i < NUM_ARMS ; i++){
    if(i == atoi(p_data)-1){
      digitalWrite(PINS_SELECT_TARGET_ARM[i], HIGH);
    }else{
      digitalWrite(PINS_SELECT_TARGET_ARM[i], LOW);
    }
  }
}

void HardwareLayout::ActionSelectSensor(char *p_data)
{
  Serial.println("HardwareLayout::ActionSelectSensor ");
  if (p_data) {
    Serial.print(p_data);
  }
  Serial.println();

  for(int i=0 ; i < NUM_SENSORS ; i++){
    if(i==atoi(p_data)-1){
      digitalWrite(PINS_SELECT_SENSOR[i], HIGH);
    }else{
      digitalWrite(PINS_SELECT_SENSOR[i], LOW);
    }
  }
}

void HardwareLayout::ActionOpenDoor(char *p_data)
{
  Serial.println("HardwareLayout::ActionOpenDoor");
  if (p_data) {
    Serial.print(p_data);
  }
  Serial.println();
  
  for(int i=0 ; i < NUM_ARMS ; i++){
    if(i==atoi(p_data)-1){
      digitalWrite(PINS_CLOSE_DOOR[i], LOW);
      digitalWrite(PINS_OPEN_DOOR[i], HIGH);
    }else{
      //digitalWrite(PINS_OPEN_DOOR[i], LOW);
    }
  }
}

void HardwareLayout::ActionCloseDoor(char *p_data)
{
  Serial.println("HardwareLayout::ActionCloseDoor");
  if (p_data) {
    Serial.print(p_data);
  }
  Serial.println();
  
  for(int i=0 ; i < NUM_ARMS ; i++){
    if(i==atoi(p_data)-1){
      digitalWrite(PINS_OPEN_DOOR[i], LOW);
      digitalWrite(PINS_CLOSE_DOOR[i], HIGH);
    }else{
      //digitalWrite(PINS_CLOSE_DOOR[i], LOW);
    }
  }
}

void HardwareLayout::ActionStartTrial(char *p_data)
{
  Serial.println("HardwareLayout::ActionStartTrial");

  digitalWrite(PIN_START_TRIAL, HIGH);
  delay(1);  
  digitalWrite(PIN_START_TRIAL, LOW);
  
}


void HardwareLayout::ActionStopTrial(char *p_data)
{
  Serial.println("HardwareLayout::ActionStopTrial");
  
  digitalWrite(PIN_STOP_TRIAL, HIGH);
  delay(1);  
  digitalWrite(PIN_STOP_TRIAL, LOW);
}

void HardwareLayout::ActionDisableArms(char *p_data)
{
  Serial.println("HardwareLayout::ActionDisableArms");
  for(int i=0 ; i < NUM_ARMS ; i++){
    //digitalWrite(PINS_SELECT_START_ARM[i], HIGH);
    digitalWrite(PINS_SELECT_TARGET_ARM[i], LOW);    
  }
}

void HardwareLayout::ActionDisableSensors(char *p_data)
{
  Serial.println("HardwareLayout::ActionDisableSensors");
  for(int i=0 ; i < NUM_SENSORS ; i++){
    digitalWrite(PINS_SELECT_SENSOR[i], LOW);
  }
}

void HardwareLayout::ActionError(char *p_data)
{
  Serial.println("HardwareLayout::ActionError");
}

void HardwareLayout::ActionTaskState1_ON(char *p_data)
{
  Serial.println("HardwareLayout::ActionTaskState1ON");
  int arm = atoi(p_data)-1;
  digitalWrite(PINS_TASK_STATE1_ON[arm], HIGH);
}

void HardwareLayout::ActionTaskState2_ON(char *p_data)
{
  Serial.println("HardwareLayout::ActionTaskState2ON");
  int arm = atoi(p_data)-1;
  digitalWrite(PINS_TASK_STATE2_ON[arm], HIGH);
}

void HardwareLayout::ActionTaskState1_OFF(char *p_data)
{
  Serial.println("HardwareLayout::ActionTaskState1OFF");
  int arm = atoi(p_data)-1;
  digitalWrite(PINS_TASK_STATE1_OFF[arm], LOW);
}

void HardwareLayout::ActionTaskState2_OFF(char *p_data)
{
  Serial.println("HardwareLayout::ActionTaskState2OFF");
  int arm = atoi(p_data)-1;
  digitalWrite(PINS_TASK_STATE2_OFF[arm], LOW);
}

//void HardwareLayout::ActionTaskState2(char *p_data, char *ONOFF)
//{
//  Serial.println("HardwareLayout::ActionTaskState2");
//  int arm = atoi(p_data)-1;
//  if (ONOFF==1) 
//  digitalWrite(PINS_TASK_STATE2[arm], HIGH);
//}
//  else{
//    digitalWrite(PINS_TASK_STATE2[arm], LOW);
//  }

void HardwareLayout::readSwitches()
{
  for(int i = 0 ; i < NUM_ARMS ; i++){
     int openDoor = digitalRead(PINS_BUTTON_OPEN_DOOR[i]);
     int closeDoor = digitalRead(PINS_BUTTON_CLOSE_DOOR[i]);

    if(openDoor == HIGH & closeDoor == LOW){
      digitalWrite(PINS_OPEN_DOOR[i], HIGH);
      digitalWrite(PINS_CLOSE_DOOR[i], LOW);
    }else if(openDoor == LOW & closeDoor == HIGH){
      digitalWrite(PINS_OPEN_DOOR[i], LOW);
      digitalWrite(PINS_CLOSE_DOOR[i], HIGH);
    }
  }
}


