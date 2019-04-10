
const int buttonPin1 = 2; // Door 1 open input
const int buttonPin2 = 3; // Door 1 close input
const int buttonPin3 = 4; // Door 2 open input
const int buttonPin4 = 5; // Door 2 close input
const int buttonPin5 = 6; // Door 3 open input
const int buttonPin6 = 7; // Door 3 close input
const int buttonPin7 = 8; // Door 4 open input
const int buttonPin8 = 9; // Door 4 close input

const int Door1open = 22;
const int Door1close = 23;
const int Door2open = 24;
const int Door2close = 25;
const int Door3open = 26;
const int Door3close = 27;
const int Door4open = 28;
const int Door4close = 29;




const int Freigabe_Analog_Sensor_1 = 39;
const int Freigabe_Analog_Sensor_2 = 40;
const int Freigabe_Analog_Sensor_3 = 41;
const int Signal_Arm_1 = 42;
const int Signal_Arm_2 = 43;
const int Signal_Arm_3 = 44;
const int Signal_Arm_4 = 45;
const int TRIAL_START = 46;
const int TRIAL_END = 47;



int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;
int buttonState8 = 0;






void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
  pinMode(buttonPin8, INPUT);

  pinMode(Door1open,OUTPUT);
  pinMode(Door1close,OUTPUT);
  pinMode(Door2open,OUTPUT);
  pinMode(Door2close,OUTPUT);
  pinMode(Door3open,OUTPUT);
  pinMode(Door3close,OUTPUT);
  pinMode(Door4open,OUTPUT);
  pinMode(Door4close,OUTPUT);


  pinMode(Freigabe_Analog_Sensor_1,OUTPUT);
  pinMode(Freigabe_Analog_Sensor_2,OUTPUT);
  pinMode(Freigabe_Analog_Sensor_3,OUTPUT);
  pinMode(Signal_Arm_1,OUTPUT);
  pinMode(Signal_Arm_2,OUTPUT);
  pinMode(Signal_Arm_3,OUTPUT);
  pinMode(Signal_Arm_4,OUTPUT);

  pinMode(TRIAL_START,OUTPUT);
  pinMode(TRIAL_END,OUTPUT);


  //digitalWrite(Signal_Arm_1,LOW);
  //digitalWrite(Signal_Arm_2,LOW);
  //digitalWrite(Signal_Arm_3,LOW);
  //digitalWrite(Signal_Arm_4,LOW);
  //digitalWrite(Freigabe_Analog_Sensor_1,LOW);
  //digitalWrite(Freigabe_Analog_Sensor_2,LOW);
  //digitalWrite(Freigabe_Analog_Sensor_3,LOW);



  //digitalWrite(TRIAL_START,LOW);
  //digitalWrite(TRIAL_END,LOW);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  buttonState6 = digitalRead(buttonPin6);
  buttonState7 = digitalRead(buttonPin7);
  buttonState8 = digitalRead(buttonPin8);

  if (buttonState1 == HIGH && buttonState2 == LOW){
    digitalWrite(Door1open,HIGH);
    digitalWrite(Door1close,LOW);
  }
   if (buttonState1 == LOW && buttonState2 == HIGH){
    digitalWrite(Door1open,LOW);
    digitalWrite(Door1close,HIGH);
  }
  if (buttonState3 == HIGH && buttonState4 == LOW){
    digitalWrite(Door2open,HIGH);
    digitalWrite(Door2close,LOW);
  }
   if (buttonState3 == LOW && buttonState4 == HIGH){
    digitalWrite(Door2open,LOW);
    digitalWrite(Door2close,HIGH);
  }
   if (buttonState5 == HIGH && buttonState6 == LOW){
    digitalWrite(Door3open,HIGH);
    digitalWrite(Door3close,LOW);
  }
   if (buttonState5 == LOW && buttonState6 == HIGH){
    digitalWrite(Door3open,LOW);
    digitalWrite(Door3close,HIGH);
  }
   if (buttonState7 == HIGH && buttonState8 == LOW){
    digitalWrite(Door4open,HIGH);
    digitalWrite(Door4close,LOW);
  }
   if (buttonState7 == LOW && buttonState8 == HIGH){
    digitalWrite(Door4open,LOW);
    digitalWrite(Door4close,HIGH);
  }
  
}
