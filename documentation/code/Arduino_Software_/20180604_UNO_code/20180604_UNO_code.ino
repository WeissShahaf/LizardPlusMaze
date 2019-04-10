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
const int SoundError = 1400;//error sound
const int pwmA = 3;// motor pwm direction A
const int pwmB = 11;//motor pwm direction B
const int Buttonsensor1 = 4; // analog sensor button 1
const int Buttonsensor2 = 5; // analog sensor button 2
const int Buttonsensor3 = 10; // analog sensor button 3
const int ButtonArm = 2; // arm switch- which is active
const int DistanceThreshold = 100; //default 150******************************

#define sensor1Analog A0 //analog signal sensor 1
#define sensor2Analog A1 //analog signal sensor 2
#define sensor3Analog A2 //analog signal sensor 3
#define StartingArm A5 //Starting Arm
Stepper myStepper(stepsPerRevolution, dirA, dirB); // defining the stepper function
////////////////LED//////////////////////////
PololuLedStrip<6> ledStrip; //GREEN LED
#define LED_COUNT 1 //how many LEDs in the strip
rgb_color correct_LED[LED_COUNT];//
rgb_color LED_OFF[LED_COUNT];//
rgb_color error_LED[LED_COUNT];//
rgb_color check[LED_COUNT];//

////////////////time counters/////////////////
boolean isFirstRun = true; 
unsigned long lastRewardTime = 0;
///////// buttons /////////////////
int sensor1State = 0;// button of sensor 1
int sensor2State = 0;// button of sensor 2
int sensor3State = 0;// button of sensor 3
int ArmState = 0;// button of arm selection
int StartingArmState = 0;// button of Starting arm  selection
int piezoPin = 7;// output for buzzer
int MotorTTL = 8;// TTL out of motor
int IRsensorTTL = 9;// IR sensor TTL out - any of the sensors in the arm
//sensor values
uint16_t value; uint16_t value1; uint16_t value2; uint16_t value3;
uint16_t start_arm;

////////////////// FUNCTIONS //////////////////////////////////
//
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
  //Serial.println("enter sensor read");
  //if ((value > DistanceThreshold) && ((unsigned long) (millis() -(unsigned long) next_time) >= 0)) { //IR tripped and enough time elapsed
//  if ((value > DistanceThreshold) && ( (millis() >  next_time) )) { //IR tripped and enough time elapsed
  if ((value > DistanceThreshold) && ( ((millis() - lastRewardTime) >  INTERVAL) || isFirstRun)) { //IR tripped and enough time elapsed
    //  if ((long) (millis()-next_time)>=0){//check activation time reached
    lastRewardTime=millis();
    isFirstRun= false;
//    next_time += INTERVAL; //next activation time
    Serial.print("current: "); Serial.println( millis()/1000);
    Serial.print("next: "); Serial.println(lastRewardTime+INTERVAL);
//    Serial.print("diff: ");Serial.println((unsigned long) (millis() - (unsigned long) next_time) /1000);
    
    
    //Serial.println("reward");
    tone(piezoPin, SoundCorrect, 500);//play correct sound
    ledStrip.write(correct_LED, LED_COUNT); //LED correct color ON
    moveMotor();//give reward
    waitXmillisec(1000);
    noTone(piezoPin);//stop buzzer
    ledStrip.write(LED_OFF, LED_COUNT);//LED correct color OFF
    waitXmillisec(1000);
    
  }//if
}//reward_fun


//excecute an error trial response (buzzer and LED) if wrong arm is reached
void error_choice(long unsigned value1, long unsigned value2, long unsigned value3) {
  Serial.println("error choice");
  //check all sensors
  if (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold) {
    tone(piezoPin, SoundError, 2000);
    ledStrip.write(error_LED, LED_COUNT);
    waitXmillisec(1000);
    noTone(piezoPin); ledStrip.write(LED_OFF, LED_COUNT);

  }// if (value1>DistanceThreshold || value2>DistanceThreshold || value3>DistanceThreshold){
}//error_choice

////////////// END FUNCTIONS/////////////////////////////



////////////////start setup//////////////////////////////
void setup() { //run once

  Serial.begin(9600);//for serial print
//   next_time = millis() + INTERVAL; //time to next activation

  //sensors
  pinMode(Buttonsensor1, INPUT);  pinMode(Buttonsensor2, INPUT);  pinMode(Buttonsensor3, INPUT);
  pinMode(sensor1Analog, INPUT);  pinMode(sensor2Analog, INPUT);  pinMode(sensor3Analog, INPUT);
  pinMode(IRsensorTTL, OUTPUT); pinMode(StartingArm,INPUT);

  //current arm selected
  pinMode(ButtonArm, INPUT);
  //Motor
  pinMode(pwmA, OUTPUT);  pinMode(pwmB, OUTPUT); pinMode(MotorTTL, OUTPUT);
  myStepper.setSpeed(30);//motor speed
  //buzzer
  pinMode(piezoPin, OUTPUT);

  //LED
  // this turns all the LED in the strip controller ON
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    //rgb_color accepts uint8
    correct_LED[i] = rgb_color(50, 0, 0); //green
    LED_OFF[i] = rgb_color(0, 0, 0); //OFF
    error_LED[i] = rgb_color(100, 165, 0); // orange
    check[i] = rgb_color(0, 50, 0); //blue
  }
  // Write the colors to the LED strip.
  ledStrip.write(LED_OFF, LED_COUNT);
  ledStrip.write(LED_OFF, LED_COUNT);

}//setup
///////////////////////end of setup////////////



/////////////////LOOP///////////////////////////

void loop() { //run in loop
  //waitXmillisec(1000);
  //read buttons
  int sensor1State = digitalRead(Buttonsensor1);
  int sensor2State = digitalRead(Buttonsensor2);
  int sensor3State = digitalRead(Buttonsensor3);
  int ArmState = digitalRead(ButtonArm);
  

  // read IR sensors in arm
  uint16_t value1 = analogRead (sensor1Analog); uint16_t value2 = analogRead (sensor2Analog); uint16_t value3 = analogRead (sensor3Analog);
  start_arm= analogRead (StartingArm);
// Serial.print("armstate "); Serial.println(ArmState);
 Serial.print("start arm "); Serial.println(start_arm);
  //TTL out when IR sensor is tripped
  if (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold) {
    digitalWrite(IRsensorTTL, HIGH);
    waitXmillisec(1);
    digitalWrite(IRsensorTTL, LOW);
  }
  else {
    //digitalWrite(IRsensorTTL, LOW);
  }

  //if  ((unsigned long) (millis() - next_time) > 0) {
    
   // Serial.print("next in: ");
   // Serial.println(( next_time)-((millis())));
    
    //sensor 1 is ON and 2,3 OFF & this arm is selected
    if ((ArmState == HIGH && sensor1State == HIGH) && (sensor2State == LOW && sensor3State == LOW)) {
      uint16_t value = analogRead (sensor1Analog);
      reward_fun(value);
    }


    //sensor 2 is ON and 1,3 OFF & one of the arms is selected
    if ((ArmState == HIGH && sensor1State == LOW) && (sensor2State == HIGH && sensor3State == LOW)) {
      uint16_t value = analogRead (sensor2Analog);
      reward_fun(value);
    }


    //sensor 3 is ON and 1,3 OFF & one of the arms is selected
    if ((ArmState == HIGH && sensor1State == LOW) && (sensor2State == LOW && sensor3State == HIGH)) {
      uint16_t value = analogRead (sensor3Analog);
      reward_fun(value);
    }
 
  //}//if  ((long) (millis() - next_time) >= 0) {

  //NO sensor selected, arm is chosen- will repeat turn of selected motor
  if ((ArmState == HIGH) && (sensor1State == LOW && sensor2State == LOW && sensor3State == LOW)) {
    Serial.println("manual activation");
    tone(piezoPin, SoundCorrect, 500); ledStrip.write(correct_LED, LED_COUNT); moveMotor();
    waitXmillisec(1000);
    noTone(piezoPin);
    ledStrip.write(LED_OFF, LED_COUNT);
    //waitXmillisec(1000);
  }

  //arm not selected and any sensor ON
  if ((ArmState == LOW) && (sensor1State == HIGH || sensor2State == HIGH || sensor3State == HIGH)) {
    uint16_t value1 = analogRead (sensor1Analog);   uint16_t  value2 = analogRead (sensor2Analog);   uint16_t  value3 = analogRead (sensor3Analog);
    if (value1 > DistanceThreshold || value2 > DistanceThreshold || value3 > DistanceThreshold) {
     // Serial.println("error_choice");
      
    
      // error_choice(value1,value2,value3);
    }
  }


}//void loop

///////////////////////////////////////////////////////////////////end

