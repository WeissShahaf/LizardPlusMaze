

#include <PololuLedStrip.h> // load LED with controller library
#include <Stepper.h> //load stepper library




//constants
const int stepsPerRevolution = 2048; //360 degrees
const long interval=40000;
const int dirA = 12;//turn direction of motor
const int dirB = 13;// other direction of motor
const int SoundCorrect = 700;//correct sound
const int SoundError = 1400;//error sound

#define Sensor1Analog A0 //analog signal sensor 1
#define Sensor2Analog A1 //analog signal sensor 2
#define Sensor3Analog A2 //analog signal sensor 3





PololuLedStrip<6> ledStrip; //GREEN LED
Stepper myStepper(stepsPerRevolution, dirA, dirB); // defining the stepper function

#define LED_COUNT 1 //how many LEDs in the strip
rgb_color correct_LED[LED_COUNT];//
rgb_color LED_OFF[LED_COUNT];//
rgb_color error_LED[LED_COUNT];//
rgb_color check[LED_COUNT];//

const int pwmA = 3;// motor pwm direction A
const int pwmB = 11;//motor pwm direction B
const int ButtonSensor1 = 4; // analog sensor button 1
const int ButtonSensor2 = 5; // analog sensor button 2
const int ButtonSensor3 = 10; // analog sensor button 3
const int ButtonArm = 2; // arm switch- which is active
unsigned long currentTime = millis();// counter
unsigned long curr_time=millis();
unsigned long time;
// buttons
int Sensor1State = 0;// button of sensor 1
int Sensor2State = 0;// button of sensor 2
int Sensor3State = 0;// button of sensor 3
int ArmState = 0;// button of arm selection

int Motordrive = 8;// TTL out of motor 
int piezoPin = 7;// output for buzzer
int IRSensorTTL = 9;// IR sensor TTL out - any of the sensors in the arm


// function for one turn of motor
void moveMotor(){ 
  //turn ON stepper controller
  digitalWrite(pwmA,HIGH);
  digitalWrite(pwmB,HIGH);
  // turn stepper
  myStepper.step(+980); //default step size +980
  //turn OFF motor
  digitalWrite(pwmA,LOW);
  digitalWrite(pwmB,LOW);
}

void setup(){
  unsigned long curr_time=millis();
  Serial.begin(9600);//for serial print
  //sensor selection
  pinMode(ButtonSensor1, INPUT);
  pinMode(ButtonSensor2, INPUT);
  pinMode(ButtonSensor3, INPUT);
  //current arm selected
  pinMode(ButtonArm, INPUT);
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  myStepper.setSpeed(30);
  pinMode(IRSensorTTL,OUTPUT);
  pinMode(piezoPin,OUTPUT);
  pinMode(Motordrive,OUTPUT);
  pinMode(Sensor1Analog, INPUT);
  pinMode(Sensor2Analog, INPUT);
  pinMode(Sensor3Analog, INPUT); 
// this turns all the LED in the strip controller ON
for (uint16_t i= 0; i<LED_COUNT;i++)
{
//    byte x1 = 50;
 //   byte x2 = 0;
  //  byte x3 = 32;
   // byte x4 = 165;
    //byte x5 = 255;
    //byte x6 = 0;
    //rgb_color accepts uint8
    correct_LED[i] = rgb_color(50,0,0);//green
    LED_OFF[i] = rgb_color(0,0,0);//OFF
    error_LED[i]=rgb_color(100,165,0);// orange
    check[i] = rgb_color(0,50,0); //blue

}   


  // Write the colors to the LED strip.
  ledStrip.write(LED_OFF, LED_COUNT);
  ledStrip.write(LED_OFF, LED_COUNT);

//new section to replace strip
  //ledStrip.write(check, LED_COUNT);



  
}

void loop(){
  //reset counter
 //Serial.println(millis()-curr_time);
//  time=millis();
  //Serial.println(millis());
  //read buttons
  Sensor1State = digitalRead(ButtonSensor1);
  Sensor2State = digitalRead(ButtonSensor2);
  Sensor3State = digitalRead(ButtonSensor3);
  ArmState = digitalRead(ButtonArm);



// read IR sensors in arm
    uint16_t value1 = analogRead (Sensor1Analog);
    uint16_t value2 = analogRead (Sensor2Analog);
    uint16_t value3 = analogRead (Sensor3Analog);
    //TTL out when IR sensor is tripped
    if (value1>150 || value2>150 || value3>150){
    digitalWrite(IRSensorTTL, HIGH);
    }
    else{
    digitalWrite(IRSensorTTL, LOW);
    }
  

//sensor 1 is ON and 2,3 OFF & one of the arms is selected
  if (ArmState== HIGH && Sensor1State == HIGH && Sensor2State == LOW && Sensor3State == LOW){
  uint16_t value1 = analogRead (Sensor1Analog);
    
   if (value1>150){//IR1 tripped
    
    if ((millis()-curr_time) >= interval){
     //Serial.println("reward");
     tone(piezoPin, 700, 500);digitalWrite(Motordrive, HIGH); moveMotor(); ledStrip.write(correct_LED, LED_COUNT); 
     delay (1000);
     noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(LED_OFF, LED_COUNT);
     long curr_time=millis();
    // Serial.print("Value_Sensor_1:");
     //Serial.println (value1);        
     //Serial.print("interval");//Print the data to the arduino serial monitor
     //Serial.println (millis()-curr_time);
    }
     else{
      Serial.println((millis()-curr_time));
     }
    }
    //long time_from_counter=activation_time-currentTime;
      //      Serial.print("currentTime");Serial.println(currentTime);

        
   // if  (time_from_counter >= interval){
   //tone(piezoPin, 700, 500);digitalWrite(Motordrive, HIGH); moveMotor(); ledStrip.write(correct_LED, LED_COUNT); 
   //delay (1000);
   //noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(LED_OFF, LED_COUNT);
  // long currentTime = millis();// counter
  //delay(interval);
   }     

//sensor 2 is ON and 1,3 OFF & one of the arms is selected
 if (ArmState== HIGH && Sensor1State == LOW && Sensor2State == HIGH && Sensor3State == LOW){
     uint16_t value2 = analogRead (Sensor2Analog);
    //Serial.print("Value_Sensor_2:");
    //Serial.println (value2);                 //Print the data to the arduino serial monitor
    //Serial.println ();
    if (value2>150){
    tone(piezoPin, SoundCorrect, 500);digitalWrite(Motordrive, HIGH); ledStrip.write(correct_LED, LED_COUNT); moveMotor();
    delay (1000);
    noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(LED_OFF, LED_COUNT);
    delay (interval);  
    }
  }
//sensor 3 is ON and 1,3 OFF & one of the arms is selected
  if (ArmState== HIGH && Sensor1State == LOW && Sensor2State == LOW && Sensor3State == HIGH){
    uint16_t value3 = analogRead (Sensor3Analog);
    Serial.print("Value_Sensor_3:");
    Serial.println (value3);                 //Print the data to the arduino serial monitor
    Serial.println ();
    if (value3>150){
    tone(piezoPin, SoundCorrect, 500);digitalWrite(Motordrive, HIGH); ledStrip.write(correct_LED, LED_COUNT); moveMotor();
    delay (1000);
    noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(LED_OFF, LED_COUNT);
    delay (interval);  
    }
  }
    //NO sensor selected, arm is chosen- will repeat turn of selected motor
    if (ArmState == HIGH && Sensor1State == LOW && Sensor2State == LOW && Sensor3State == LOW){
    
    myStepper.step(+980);
    digitalWrite(pwmA,LOW);digitalWrite(Motordrive, LOW); digitalWrite(pwmB,LOW);    
    digitalWrite(pwmA,HIGH);  digitalWrite(pwmB,HIGH); //digitalWrite(Motordrive, HIGH);
    tone(piezoPin, SoundCorrect, 500); ledStrip.write(correct_LED, LED_COUNT); moveMotor();
    delay (1000);
    noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(LED_OFF, LED_COUNT);
    delay (1000);  
    
    }

//arm not selected and any sensor ON
  if (ArmState== LOW && Sensor1State == HIGH || Sensor2State == HIGH || Sensor3State == HIGH){
    uint16_t value1 = analogRead (Sensor1Analog);    uint16_t value2 = analogRead (Sensor2Analog);    uint16_t value3 = analogRead (Sensor3Analog);
    
    if (value1>150 || value2>150 || value3>150){
    Serial.print("Value_Sensor_1:");    Serial.println (value1);                 //Print the data to the arduino serial monitor
    Serial.print("Value_Sensor_2:");    Serial.println (value2);                 //Print the data to the arduino serial monitor
    Serial.print("Value_Sensor_3:");    Serial.println (value3);                 //Print the data to the arduino serial monitor
    Serial.println ();
   // tone(piezoPin, SoundError, 2000); ledStrip.write(error_LED, LED_COUNT);
    //delay (1000);
   // noTone(piezoPin);ledStrip.write(LED_OFF, LED_COUNT);
    //delay (interval);  

    }
  } 
}
//end
