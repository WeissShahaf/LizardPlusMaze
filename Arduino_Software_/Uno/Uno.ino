//This is a script for running the lizard maze round feeder/LED/buzzer combination
//The script contains logic for activation of a single arm on an Arduino Uno board from a manual control box
//Written by Erik Papuschin and Shahaf Weiss May 2018

#include <PololuLedStrip.h> // load LED with controller library
#include <Stepper.h> //load stepper library

//constant variable
const int stepsPerRevolution = 2048; //360 degrees
const unsigned long INTERVAL = 30000; //minimum time between rewards. default 40,000 (40 seconds)
const int dirA = 12;//turn direction of motor
const int dirB = 13;// other direction of motor
const int SoundCorrect = 700;//correct sound
const int SoundError = 1000;//error sound
const int pwmA = 3;// motor pwm direction A
const int pwmB = 11;//motor pwm direction B
const int Buttonsensor1 = 4; // analog sensor button 1
const int Buttonsensor2 = 5; // analog sensor button 2
const int Buttonsensor3 = 10; // analog sensor button 3
const int ButtonArm = 2; // arm switch- target arm
const int DistanceThreshold = 100; //default 150******************************

#define sensor1Analog A0 //analog signal sensor 1
#define sensor2Analog A1 //analog signal sensor 2
#define sensor3Analog A2 //analog signal sensor 3
#define StartingArm A5 //Starting Arm pin
////////////////////////////////////////////////////////////////////////
#define TaskAnalog1 A3 // place + response 
#define TaskAnalog2 A4 // cue

//   ________|_______|_______|
//           |       |       |

// TaskAnalog1 LOW + TaskAnalog2 LOW ->place /response with a starting arm
// TaskAnalog1 HIGH + TaskAnalog2 LOW ->place /response without a starting arm
// TaskAnalog1 LOW + TaskAnalog2 HIGH ->visual cue with starting arm
// TaskAnalog1 HIGH + TaskAnalog2 HIGH ->visual cue without a starting arm


Stepper myStepper(stepsPerRevolution, dirA, dirB); // defining the stepper function
////////////////LED//////////////////////////
PololuLedStrip<6> ledStrip; //GREEN LED
#define LED_COUNT 1 //how many LEDs in the strip
rgb_color correct_LED[LED_COUNT];//
rgb_color LED_OFF[LED_COUNT];//
rgb_color error_LED[LED_COUNT];//
rgb_color cue_LED[LED_COUNT];//
rgb_color green_LED[LED_COUNT];//
rgb_color red_LED[LED_COUNT];//
rgb_color blue_LED[LED_COUNT];
rgb_color white_LED[LED_COUNT];
rgb_color orange_LED[LED_COUNT];
rgb_color yellow_LED[LED_COUNT];
rgb_color cyan_LED[LED_COUNT];
rgb_color magenta_LED[LED_COUNT];
rgb_color silver_LED[LED_COUNT];
////////////////time counters/////////////////
boolean isFirstRun = true;
unsigned long lastRewardTime = 0;
///////// buttons /////////////////
int sensor1State = 0;// button of sensor 1
int sensor2State = 0;// button of sensor 2
int sensor3State = 0;// button of sensor 3
int TargetArmState = 0;// button of taget arm selection
int StartingArmState = 0;// button of Starting arm  selection
int TaskAnalong1 = 0; // place /response
int TaskAnalong2 = 0; //Cue
int piezoPin = 7;// output for buzzer
int MotorTTL = 8;// TTL out of motor
int IRsensorTTL = 9;// IR sensor TTL out - any of the sensors in the arm
//sensor values
uint16_t value; uint16_t value1; uint16_t value2; uint16_t value3;
uint16_t valueplace; uint16_t vlauecue;
uint16_t StartArm;
uint16_t  TaskState1 = 0;
uint16_t TaskState2 = 0;

////////////////// FUNCTIONS //////////////////////////////////
void waitXmillisec(unsigned long x) {
  unsigned long counter = millis();
  while (millis() < counter + x) { //do nothing for x milliseconds
  }
}//END OF waitXmillisec

// function for one turn of motor
void moveMotor() {
  Serial.println("motor moved");
  //turn ON stepper controller
  digitalWrite(pwmA, LOW);  digitalWrite(pwmB, LOW); //start with power OFF
  digitalWrite(pwmA, HIGH);  digitalWrite(pwmB, HIGH); //power ON
  // turn stepper motor
  myStepper.step(+980); //default step size +980
  //send motor TTL
  digitalWrite(MotorTTL, HIGH);//send TTL
  waitXmillisec(1);
  digitalWrite(MotorTTL, LOW);//stop TTL
  //turn OFF motor
  digitalWrite(pwmA, LOW);  digitalWrite(pwmB, LOW); //power OFF
  waitXmillisec(1000);
}// end of moveMotor

// get value from distance sensor and activate if enough time elapsed since last activation
void reward_fun(uint16_t value) {
  if ((value > DistanceThreshold) && ( ((millis() - lastRewardTime) >  INTERVAL) || (value > DistanceThreshold) && isFirstRun)) { //IR tripped and enough time elapsed
    lastRewardTime = millis();
    isFirstRun = false;
  //  if (TaskState2 > 500) {
      tone(piezoPin, SoundCorrect, 500);//play correct sound
      ledStrip.write( correct_LED, LED_COUNT); //LED correct color ON
      delay(500);
      noTone(piezoPin);//stop buzzer
      ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF
      delay(500);
      moveMotor();//give reward
      delay(1000);     
      // blue_LED[1] = error_LED[1];
 //   }
//    if (TaskState2 < 500) {
//      tone(piezoPin, SoundCorrect, 500);//play correct sound
//      ledStrip.write( red_LED, LED_COUNT); //LED correct color ON
//      delay(500);
//      noTone(piezoPin);//stop buzzer
//      ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF
//      delay(500);
//      moveMotor();//give reward
//      delay(1000);
//    }

  }//if
  
}//reward_fun


//excecute an error trial response (buzzer and LED) if wrong arm is reached
void error_choice(long unsigned value1, long unsigned value2, long unsigned value3, uint16_t TaskState1, uint16_t TaskState2) {
  Serial.println("error choice");
  //check all sensors
  if  ( ((millis() - lastRewardTime) >  INTERVAL || isFirstRun) && (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold)) {
    //    Serial.print("ERROR, ");
    lastRewardTime = millis();
    isFirstRun = false;
    //play error tone
    if (TaskState2 < 500) {
      ////error signal disabled by commenting the following lines
      //ledStrip.write(error_LED, LED_COUNT); tone(piezoPin, SoundError, 300);
      //waitXmillisec(300); ledStrip.write(LED_OFF, LED_COUNT); waitXmillisec(200);
      //ledStrip.write(error_LED, LED_COUNT); tone(piezoPin, SoundError, 300);
      //waitXmillisec(300); ledStrip.write(LED_OFF, LED_COUNT); waitXmillisec(200);
      //ledStrip.write(error_LED, LED_COUNT); tone(piezoPin, SoundError, 300);
      //waitXmillisec(300); noTone(piezoPin); ledStrip.write(LED_OFF, LED_COUNT);
    }
    else {

    }
    //waitXmillisec(1000);
  }// if (value1>DistanceThreshold || value2>DistanceThreshold || value3>DistanceThreshold){
}//error_choice

////////////// END FUNCTIONS/////////////////////////////



////////////////start setup//////////////////////////////
void setup() { //run once
  //TaskAnalog1=1000; TaskAnalog2=1000;
  Serial.begin(9600);//bitrate for serial print

  //IR sensor-selctor
  pinMode(Buttonsensor1, INPUT);  pinMode(Buttonsensor2, INPUT);  pinMode(Buttonsensor3, INPUT);
  //IR sensor input
  pinMode(sensor1Analog, INPUT);  pinMode(sensor2Analog, INPUT);  pinMode(sensor3Analog, INPUT);
  pinMode(IRsensorTTL, OUTPUT);  pinMode(StartingArm, INPUT);
  pinMode(TaskAnalong1, INPUT);  pinMode(TaskAnalong2, INPUT);

  //current arm selected
  pinMode(ButtonArm, INPUT);
  //Motor
  pinMode(pwmA, OUTPUT);  pinMode(pwmB, OUTPUT); pinMode(MotorTTL, OUTPUT);
  myStepper.setSpeed(30);//motor speed
  //buzzer
  pinMode(piezoPin, OUTPUT);

  //LED color library
  //rgb_color accepts uint8
  green_LED[0] = rgb_color(255, 0, 0); //green
  red_LED[0] = rgb_color(0, 255, 0); //red
  blue_LED[0] = rgb_color(0, 0, 255); //blue
  orange_LED[0] = rgb_color(100, 165, 0); // orange
  white_LED[0] =  rgb_color(255, 255, 255); // white-yellowish
  yellow_LED[0] = rgb_color(255, 255, 0); //yellow
  cyan_LED[0] = rgb_color(255, 0, 255); //cyan
  magenta_LED[0] = rgb_color(0, 255, 255);//magenta
  silver_LED[0] = rgb_color(192, 192, 192); //whiter white
  //task related colors
  LED_OFF[0] = rgb_color(0, 0, 0); //OFF
  correct_LED[0] = magenta_LED[0];
  error_LED[0] = orange_LED[0];
  cue_LED[0] = magenta_LED[0];

  // Write the colors to the LED strip.
  ledStrip.write(LED_OFF, LED_COUNT);//start with resetting to OFF

}//setup
///////////////////////end of setup////////////



///////////////// MAIN LOOP///////////////////////////

void loop() { //run in loop
  //start by reading all buttons and sensors
  //read buttons
  int sensor1State = digitalRead(Buttonsensor1);  int sensor2State = digitalRead(Buttonsensor2);  int sensor3State = digitalRead(Buttonsensor3);
  //task related buttons
  int TargetArmState = digitalRead(ButtonArm); // int StartingArmState = analogRead(StartingArm);
  uint16_t TaskState1 = analogRead(TaskAnalog1);  uint16_t TaskState2 = analogRead(TaskAnalog2);

  // read IR sensors in arm
  uint16_t value1 = analogRead (sensor1Analog); uint16_t value2 = analogRead (sensor2Analog); uint16_t value3 = analogRead (sensor3Analog);
  //read arm state
  uint16_t StartArm = analogRead (StartingArm); //read value of starting arm selector
  
  //read task states
  TaskState1 = analogRead (TaskAnalog1); //read value of starting arm selector
  TaskState2 = analogRead (TaskAnalog2); //read value of starting arm selector

  Serial.print("Start: ");Serial.print(StartArm); Serial.print(',');Serial.print("Target: ");Serial.print(TargetArmState);Serial.print(','); 
  uint16_t mini=min(min(value1,value2),value3);
  uint16_t maxi=max(max(value1,value2),value3);
  Serial.print("sensors min: ");Serial.print(mini); Serial.print(',');
  Serial.print("sensors max: ");Serial.print(maxi); Serial.print(',');
  
  Serial.print("TaskState1: "); Serial.print(TaskState1); Serial.print(','); Serial.print("TaskState2: "); Serial.println(TaskState2);
  ////////////////   cue task      //////////////////////////
  //in case Cue task is not selected selected
  if (TaskState1 > 500) {
    ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF//
  }
  //in case Cue task is selected
  if ((TaskState2 > 500) && (TargetArmState == HIGH)) { //goal arm
    ledStrip.write(cue_LED, LED_COUNT);//light cue ON
  }
  else {
    if ((TaskState2 > 500) && (TargetArmState == LOW) ) { //not goal arm
      //      ledStrip.write(error_LED, LED_COUNT);//LED correct color ERROR//
      ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF//
    }
  }

  if ((TaskState2 > 500) && (TargetArmState == LOW) ) { //not goal arm
    ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF//
  }
  ////////////////   cue task      //////////////////////////

  //TTL out when IR sensor is tripped
  if (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold) {
    digitalWrite(IRsensorTTL, HIGH);
    waitXmillisec(1);
    digitalWrite(IRsensorTTL, LOW);
  }
 if (StartArm < 180) {
  //sensor 1 is ON and 2,3 OFF & arm is selected as target
  if ((TargetArmState == HIGH && sensor1State == HIGH) && (sensor2State == LOW && sensor3State == LOW)) {
    uint16_t value = analogRead (sensor1Analog);
    reward_fun(value);
  }

  //sensor 2 is ON and 1,3 OFF & arm is selected as target
  if ((TargetArmState == HIGH && sensor1State == LOW) && (sensor2State == HIGH && sensor3State == LOW)) {
    uint16_t value = analogRead (sensor2Analog);
    reward_fun(value);
  }

  //sensor 3 is ON and 1,2 OFF & arm is selected as target
  if ((TargetArmState == HIGH && sensor1State == LOW) && (sensor2State == LOW && sensor3State == HIGH)) {
    uint16_t value = analogRead (sensor3Analog);
    reward_fun(value);
  }

  //manual rewarding
  //NO sensor selected, arm is chosen- will repeat turn of selected motor
  if ((TargetArmState == HIGH) && (sensor1State == LOW && sensor2State == LOW && sensor3State == LOW)) {
    // Serial.println("manual activation");
    tone(piezoPin, SoundCorrect, 500); ledStrip.write(correct_LED, LED_COUNT); moveMotor();
    waitXmillisec(1000);
    noTone(piezoPin);
    ledStrip.write(LED_OFF, LED_COUNT);
    //waitXmillisec(1000);
  }

  //entering wrong arm - error choice for any sensor
  if ((TargetArmState == LOW) && (sensor1State == HIGH || sensor2State == HIGH || sensor3State == HIGH)) {//arm not selected as target and any sensor ON
    if (StartArm < 180) { // arm is NOT selected as starting arm
      //read sensors
      uint16_t value1 = analogRead (sensor1Analog);   uint16_t  value2 = analogRead (sensor2Analog);   uint16_t  value3 = analogRead (sensor3Analog);
      if (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold) {
        Serial.println("error_choice");
        error_choice(value1, value2, value3, TaskState1, TaskState2); //initiate error signal
      }//if sensors triped
    }//if TargetArmState==low
  }//if arm not selected as target and any sensor ON
 }//if (StartArm < 180) {
}//void loop

///////////////////////////////////////////////////////////////////
//end of file

