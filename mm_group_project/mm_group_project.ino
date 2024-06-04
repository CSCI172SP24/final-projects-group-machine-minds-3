/************************************************
* Anna Wilson, Chase Lafaurie, Gracy Conrad
* Jonah Duncan, Sam Richardson, Vincent Tang 
* 
* 05/20/24
*
* Machine Minds Group Project
************************************************/

/* Libraries */
#include <IRremote.h>

/* IR Receiver Pins/Logic */
#define IR_RECV_PIN  10     // IR receiver Signal pin connect -> pin D10
IRrecv irrecv(IR_RECV_PIN); // Create an instance of the IRrecv class with the specified pin
decode_results IRresults;

/* Movement Pins */
#define RIGHT_SPD_PIN    9  // Right PWM pin               -> MODEL-X ENA
#define RIGHT_DIR_PIN_1  12 // Right Motor direction pin 1 -> MODEL-X IN1 
#define RIGHT_DIR_PIN_2  11 // Right Motor direction pin 2 -> MODEL-X IN2
#define LEFT_SPD_PIN     6  // Left PWM pin                -> MODEL-X ENB
#define LEFT_DIR_PIN_1   7  // Left Motor direction pin 1  -> MODEL-X IN3
#define LEFT_DIR_PIN_2   8  // Left Motor direction pin 1  -> MODEL-X IN4

/* IR Button Pins */
#define IR_BTN_UP     0x00FF18E7 // "▲" button
#define IR_BTN_DOWN   0x00FF4AB5 // "▼" button
#define IR_BTN_RIGHT  0x00FF5AA5 // ">" button
#define IR_BTN_LEFT   0x00FF10EF // "<" button
#define IR_BTN_OK     0x00FF38C7 // "OK" button
#define IR_BTN_HASH   0x00FFB04F // "#" button
#define IR_BTN_STAR   0x00FF6897 // "*" button
#define IR_BTN_1	    0x00FFA25D // "1" button
#define IR_BTN_2	    0x00FF629D // "2" button
#define IR_BTN_3	    0x00FFE21D // "3" button
#define IR_BTN_4	    0x00FF22DD // "4" button
#define IR_BTN_5	    0x00FF02FD // "5" button
#define IR_BTN_6	    0x00FFC23D // "6" button
#define IR_BTN_7	    0x00FFE01F // "7" button
#define IR_BTN_8	    0x00FFA857 // "8" button
#define IR_BTN_9	    0x00FF906F // "9" button
#define IR_BTN_0	    0x00FF9867 // "10" button

/* Other Sensor Pins */
#define ECHO_PIN          4 // Ultrasonic sensor echo pin     -> pin D4
#define TRIG_PIN          5 // Ultrasonic sensor trigger pin  -> pin D5
#define RIGHT_IR_PAIR_PIN 2 // IR sensor pair yellow wire     -> pin D2
#define LEFT_IR_PAIR_PIN  3 // IR sensor pair green wire      -> pin D3
#define BUZZER_PIN        A0

/* Constants */
#define HALF_SPD_OF_SOUND  0.01715
#define MOVE_SPD           90
#define TURN_SPD           170

/* Other Variables */
bool lockFlag = LOW;
int leftIRPairValue = 0;
int rightIRPairValue = 0;
bool irPairsFollowObjectFlag = LOW;

/* Movement Functions */
void moveForward(){
  digitalWrite(RIGHT_DIR_PIN_1, HIGH);
  digitalWrite(RIGHT_DIR_PIN_2, LOW);
  digitalWrite(LEFT_DIR_PIN_1, HIGH);
  digitalWrite(LEFT_DIR_PIN_2, LOW);
  moveSetSpd(MOVE_SPD, MOVE_SPD);
}
void moveLeft(){
  digitalWrite(RIGHT_DIR_PIN_1, HIGH);
  digitalWrite(RIGHT_DIR_PIN_2, LOW);
  digitalWrite(LEFT_DIR_PIN_1, LOW);
  digitalWrite(LEFT_DIR_PIN_2, HIGH);
  moveSetSpd(0, TURN_SPD);
}
void moveRight(){
  digitalWrite(RIGHT_DIR_PIN_1, LOW);
  digitalWrite(RIGHT_DIR_PIN_2, HIGH);
  digitalWrite(LEFT_DIR_PIN_1, HIGH);
  digitalWrite(LEFT_DIR_PIN_2, LOW);
  moveSetSpd(TURN_SPD, 0);
}
void moveBack(){
  digitalWrite(RIGHT_DIR_PIN_1, LOW);
  digitalWrite(RIGHT_DIR_PIN_2, HIGH);
  digitalWrite(LEFT_DIR_PIN_1, LOW);
  digitalWrite(LEFT_DIR_PIN_2, HIGH);
  moveSetSpd(MOVE_SPD, MOVE_SPD);
}
void moveBackLeft(){
  digitalWrite(RIGHT_DIR_PIN_1, LOW);
  digitalWrite(RIGHT_DIR_PIN_2, HIGH);
  digitalWrite(LEFT_DIR_PIN_1, HIGH);
  digitalWrite(LEFT_DIR_PIN_2, LOW);
  moveSetSpd(0, TURN_SPD);
}
void moveBackRight(){
  digitalWrite(RIGHT_DIR_PIN_1, HIGH);
  digitalWrite(RIGHT_DIR_PIN_2, LOW);
  digitalWrite(LEFT_DIR_PIN_1, LOW);
  digitalWrite(LEFT_DIR_PIN_2, HIGH);
  moveSetSpd(TURN_SPD, 0);
}
void moveStop(){
  digitalWrite(RIGHT_DIR_PIN_1, LOW);
  digitalWrite(RIGHT_DIR_PIN_2, LOW);
  digitalWrite(LEFT_DIR_PIN_1, LOW);
  digitalWrite(LEFT_DIR_PIN_2, LOW);
  moveSetSpd(0, 0);
}
void moveSetSpd(int left_spd,int right_spd){
  analogWrite(RIGHT_SPD_PIN, right_spd); 
  analogWrite(LEFT_SPD_PIN, left_spd);   
}

/* IR Button-Command Logic */
bool irMoveCMD(int received){
  switch(received){
    case IR_BTN_UP:{
      if(lockFlag == LOW){
        moveForward();
      }
      break;
    }
    case IR_BTN_DOWN:{
      if(lockFlag == LOW){
        moveBack();
      }
      break;
    }
    case IR_BTN_RIGHT:{
      if(lockFlag == LOW){
        moveRight();
      }
      break;
    }
    case IR_BTN_LEFT:{
      if(lockFlag == LOW){
        moveLeft();
      }
      break;
    }
    case IR_BTN_OK:{
      if(lockFlag == LOW){
        moveStop();
      }
      break;
    }
    case IR_BTN_STAR:{
      lockFlag = HIGH;
      enableBuzzer();
      return lockFlag;
      break;
    }
    case IR_BTN_HASH:{
      lockFlag = LOW;
      enableBuzzer();
      delay(200);
      enableBuzzer();
      return lockFlag;
      break;
    }
    case IR_BTN_0:{
      if(lockFlag == LOW){
        irPairsFollowObjectFlag = !irPairsFollowObjectFlag;
        return irPairsFollowObjectFlag;
      }
      break;
    }
    /*
    case IR_BTN_1:{
      break;
    }
    case IR_BTN_2:{
      break;
    }
    case IR_BTN_3:{
      break;
    }
    case IR_BTN_4:{
      break;
    }
    case IR_BTN_5:{
      break;
    }
    case IR_BTN_6:{
      break;
    }
    case IR_BTN_7:{
      break;
    }
    case IR_BTN_8:{
      break;
    }
    case IR_BTN_9:{
      break;
    }
    */
    default:{
      break;
    }
  }
}

void enableBuzzer(){
  for(int b = 0; b < 10; b++){
   digitalWrite(BUZZER_PIN, LOW);
   delay(2);
   digitalWrite(BUZZER_PIN, HIGH);
   delay(2);
  }
}

void disableBuzzer(){
  digitalWrite(BUZZER_PIN, HIGH);
}

/* Sensor Logic */
long getUltrasonicDistance(){
  // Send a 10 microsecond pulse to the Trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure the duration of the pulse on the Echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance based on the duration of the pulse
  long distance = duration * HALF_SPD_OF_SOUND;
  
  // Return the distance in centimeters  
  return distance;
}

void ultrasonicAvoidObject(long distance){
  if((distance < 10) && (lockFlag == LOW)){
    enableBuzzer();
    moveStop();
    moveBack();
    delay(250);
    moveStop();
  }
}

void irPairsFollowObject(int leftIRPairValue, int rightIRPairValue){
  if((leftIRPairValue == LOW) && (rightIRPairValue == LOW)){ 
    moveBack();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == HIGH)){
    moveStop();
  }
  else if((leftIRPairValue == LOW) && (rightIRPairValue == HIGH)){ 
    moveBackLeft();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == LOW)){ 
    moveBackRight();
  }
}

void irPairsAvoidObject(int leftIRPairValue, int rightIRPairValue){
  if((leftIRPairValue == LOW) && (rightIRPairValue == LOW) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
   enableBuzzer();
   moveStop();
   moveForward();
   delay(250);
   moveStop();
  }
  else if((leftIRPairValue == LOW) && (rightIRPairValue == HIGH) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
   enableBuzzer();
   moveStop();
   moveForward();
   delay(250);
   moveStop();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == LOW) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
   enableBuzzer();
   moveStop();
   moveForward();
   delay(250);
   moveStop();
  }
}

void setup(){

  /* Stop Buzzing ASAP */
  pinMode(BUZZER_PIN, OUTPUT);
  disableBuzzer();
  
  /* Start the receiver */
  irrecv.enableIRIn();
  
  /* Set Pin Modes*/
  pinMode(RIGHT_DIR_PIN_1, OUTPUT); 
  pinMode(RIGHT_DIR_PIN_2, OUTPUT); 
  pinMode(RIGHT_SPD_PIN, OUTPUT);  
  pinMode(LEFT_DIR_PIN_1, OUTPUT);
  pinMode(LEFT_DIR_PIN_2, OUTPUT); 
  pinMode(LEFT_SPD_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(RIGHT_IR_PAIR_PIN, INPUT);
  pinMode(LEFT_IR_PAIR_PIN, INPUT);
}

void loop(){
  long distance = getUltrasonicDistance();
  rightIRPairValue = digitalRead(RIGHT_IR_PAIR_PIN);
  leftIRPairValue = digitalRead(LEFT_IR_PAIR_PIN);

  if((irPairsFollowObjectFlag == HIGH) && (lockFlag == LOW)){
    irPairsFollowObject(leftIRPairValue, rightIRPairValue);
  }

  irPairsAvoidObject(leftIRPairValue, rightIRPairValue);

  ultrasonicAvoidObject(distance);

  if(irrecv.decode(&IRresults)){     // Check if an IR signal has been received
    int received = IRresults.value;  // Get the received IR command
    irrecv.resume();                 // Continue receiving IR commands
    irMoveCMD(received);             // Move the robot received IR command
  delay(20);  // Wait for 20 ms
  }
}