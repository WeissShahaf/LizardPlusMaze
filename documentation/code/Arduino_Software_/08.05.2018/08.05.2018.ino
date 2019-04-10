#include <PololuLedStrip.h>
#include <Stepper.h>




const int stepsPerRevolution = 2048;

const int dirA = 12;
const int dirB = 13;

#define pin1 A0
#define pin2 A1
#define pin3 A2





PololuLedStrip<6> ledStrip;
Stepper myStepper(stepsPerRevolution, dirA, dirB);

#define LED_COUNT 60
rgb_color colorsa[LED_COUNT];
rgb_color colorsb[LED_COUNT];


const int pwmA = 3;
const int pwmB = 11;
const int buttonPin1 = 4; 
const int buttonPin2 = 5;
const int buttonPin3 = 10;
const int buttonPin4 = 2;


int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int Motordrive = 8;
int piezoPin = 7;
int lichtschranken = 9;



void moveMotor(){
  digitalWrite(pwmA,HIGH);
  digitalWrite(pwmB,HIGH);
  myStepper.step(+980);
  digitalWrite(pwmA,LOW);
  digitalWrite(pwmB,LOW);
}

void setup(){
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  myStepper.setSpeed(30);
  pinMode(lichtschranken,OUTPUT);
  pinMode(piezoPin,OUTPUT);
  pinMode(Motordrive,OUTPUT);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT); 

 for (uint16_t i = 0; i < LED_COUNT; i++)
  {
    byte x1 = 50;
    byte x2 = 0;
    byte x3 = 0;
    byte x4 = 0;
    byte x5 = 0;
    byte x6 = 0;
    colorsa[i] = rgb_color(x1,x2,x3);
    colorsb[i] = rgb_color(x4,x5,x6);
  }

  // Write the colors to the LED strip.
  ledStrip.write(colorsb, LED_COUNT);
  ledStrip.write(colorsb, LED_COUNT);




  
}

void loop(){
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);





  if (buttonState4== HIGH && buttonState1 == HIGH && buttonState2 == LOW && buttonState3 == LOW){
  uint16_t value1 = analogRead (pin1);
    Serial.print("Value_Sensor_1:");
   Serial.println (value1);                 //Print the data to the arduino serial monitor
   Serial.println ();
   if (value1>150){
   tone(piezoPin, 700, 500);digitalWrite(Motordrive, HIGH); ledStrip.write(colorsa, LED_COUNT); moveMotor();
   delay (1000);
   noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(colorsb, LED_COUNT);
   delay (40000);     
   }
  }

 if (buttonState4== HIGH && buttonState1 == LOW && buttonState2 == HIGH && buttonState3 == LOW){
     uint16_t value2 = analogRead (pin2);
    Serial.print("Value_Sensor_2:");
    Serial.println (value2);                 //Print the data to the arduino serial monitor
    Serial.println ();
    if (value2>150){
    tone(piezoPin, 700, 500);digitalWrite(Motordrive, HIGH); ledStrip.write(colorsa, LED_COUNT); moveMotor();
    delay (1000);
    noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(colorsb, LED_COUNT);
    delay (40000);  
    }
  }

  if (buttonState4== HIGH && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == HIGH){
    uint16_t value3 = analogRead (pin3);
    Serial.print("Value_Sensor_3:");
    Serial.println (value3);                 //Print the data to the arduino serial monitor
    Serial.println ();
    if (value3>150){
    tone(piezoPin, 700, 500);digitalWrite(Motordrive, HIGH); ledStrip.write(colorsa, LED_COUNT); moveMotor();
    delay (1000);
    noTone(piezoPin);digitalWrite(Motordrive, LOW);ledStrip.write(colorsb, LED_COUNT);
    delay (40000);  
    }
  }
    
    if (buttonState4 == HIGH && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW){
    digitalWrite(pwmA,HIGH);digitalWrite(Motordrive, HIGH);
    digitalWrite(pwmB,HIGH);
    myStepper.step(+980);
    digitalWrite(pwmA,LOW);digitalWrite(Motordrive, LOW);
    digitalWrite(pwmB,LOW);
    delay(3000);  
    }


    uint16_t value1 = analogRead (pin1);
    uint16_t value2 = analogRead (pin2);
    uint16_t value3 = analogRead (pin3);
    if (value1>150 || value2>150 || value3>150){
    digitalWrite(lichtschranken, HIGH);
    }else{
    digitalWrite(lichtschranken, LOW);
    }
  
     
}

