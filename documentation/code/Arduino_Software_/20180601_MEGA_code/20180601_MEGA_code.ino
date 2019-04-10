//inputs
const int ButtonDoor1Open = 2; // Door 1 open input
const int ButtonDoor1Closed = 3; // Door 1 close input
const int ButtonDoor2Open = 4; // Door 2 open input
const int ButtonDoor2Closed = 5; // Door 2 close input
const int ButtonDoor3Open = 6; // Door 3 open input
const int ButtonDoor3Closed = 7; // Door 3 close input
const int ButtonDoor4Open = 8; // Door 4 open input
const int ButtonDoor4Closed = 9; // Door 4 close input
//outputs
const int Door1open = 22; 
const int Door1close = 23;
const int Door2open = 24;
const int Door2close = 25;
const int Door3open = 26;
const int Door3close = 27;
const int Door4open = 28;
const int Door4close = 29;




const int AutoIR_Sensor_1 = 39;
const int AutoIR_Sensor_2 = 40;
const int AutoIR_Sensor_3 = 41;
const int Select_Arm_1 = 42;
const int Select_Arm_2 = 43;
const int Select_Arm_3 = 44;
const int Select_Arm_4 = 45;
const int TRIAL_START = 46;
const int TRIAL_END = 47;


// initializing door buttons
int Door1StateOpen = 0;
int Door1StateClosed = 0;
int Door2StateOpen = 0;
int Door2StateClosed = 0;
int Door3StateOpen = 0;
int Door3StateClosed = 0;
int Door4StateOpen = 0;
int Door4StateClosed = 0;






void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonDoor1Open, INPUT);
  pinMode(ButtonDoor1Closed, INPUT);
  pinMode(ButtonDoor2Open, INPUT);
  pinMode(ButtonDoor2Closed, INPUT);
  pinMode(ButtonDoor3Open, INPUT);
  pinMode(ButtonDoor3Closed, INPUT);
  pinMode(ButtonDoor4Open, INPUT);
  pinMode(ButtonDoor4Closed, INPUT);

  pinMode(Door1open,OUTPUT);
  pinMode(Door1close,OUTPUT);
  pinMode(Door2open,OUTPUT);
  pinMode(Door2close,OUTPUT);
  pinMode(Door3open,OUTPUT);
  pinMode(Door3close,OUTPUT);
  pinMode(Door4open,OUTPUT);
  pinMode(Door4close,OUTPUT);

//signals for controling the UNO
  pinMode(AutoIR_Sensor_1,OUTPUT);
  pinMode(AutoIR_Sensor_2,OUTPUT);
  pinMode(AutoIR_Sensor_3,OUTPUT);
  pinMode(Select_Arm_1,OUTPUT);
  pinMode(Select_Arm_2,OUTPUT);
  pinMode(Select_Arm_3,OUTPUT);
  pinMode(Select_Arm_4,OUTPUT);
//Mega outputs - TTL signals
  pinMode(TRIAL_START,OUTPUT);
  pinMode(TRIAL_END,OUTPUT);


  //digitalWrite(Select_Arm_1,LOW);
  //digitalWrite(Select_Arm_2,LOW);
  //digitalWrite(Select_Arm_3,LOW);
  //digitalWrite(Select_Arm_4,LOW);
  //digitalWrite(AutoIR_Sensor_1,LOW);
  //digitalWrite(AutoIR_Sensor_2,LOW);
  //digitalWrite(AutoIR_Sensor_3,LOW);



  //digitalWrite(TRIAL_START,LOW);
  //digitalWrite(TRIAL_END,LOW);
}

void loop() {
  // read door button state
  Door1StateOpen = digitalRead(ButtonDoor1Open);
  Door1StateClosed = digitalRead(ButtonDoor1Closed);
  Door2StateOpen = digitalRead(ButtonDoor2Open);
  Door2StateClosed = digitalRead(ButtonDoor2Closed);
  Door3StateOpen = digitalRead(ButtonDoor3Open);
  Door3StateClosed = digitalRead(ButtonDoor3Closed);
  Door4StateOpen = digitalRead(ButtonDoor4Open);
  Door4StateClosed = digitalRead(ButtonDoor4Closed);
// activate door states for each door
  if (Door1StateOpen == HIGH && Door1StateClosed == LOW){
    digitalWrite(Door1open,HIGH);
    digitalWrite(Door1close,LOW);
  }
   if (Door1StateOpen == LOW && Door1StateClosed == HIGH){
    digitalWrite(Door1open,LOW);
    digitalWrite(Door1close,HIGH);
  }
  if (Door2StateOpen == HIGH && Door2StateClosed == LOW){
    digitalWrite(Door2open,HIGH);
    digitalWrite(Door2close,LOW);
  }
   if (Door2StateOpen == LOW && Door2StateClosed == HIGH){
    digitalWrite(Door2open,LOW);
    digitalWrite(Door2close,HIGH);
  }
   if (Door3StateOpen == HIGH && Door3StateClosed == LOW){
    digitalWrite(Door3open,HIGH);
    digitalWrite(Door3close,LOW);
  }
   if (Door3StateOpen == LOW && Door3StateClosed == HIGH){
    digitalWrite(Door3open,LOW);
    digitalWrite(Door3close,HIGH);
  }
   if (Door4StateOpen == HIGH && Door4StateClosed == LOW){
    digitalWrite(Door4open,HIGH);
    digitalWrite(Door4close,LOW);
  }
   if (Door4StateOpen == LOW && Door4StateClosed == HIGH){
    digitalWrite(Door4open,LOW);
    digitalWrite(Door4close,HIGH);
  }
  
}
