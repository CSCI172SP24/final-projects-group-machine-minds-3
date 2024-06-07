/************************************************
* Anna Wilson, Chase Lafaurie, Gracy Conrad
* Jonah Duncan, Sam Richardson, Vincent Tang 
* 
* 05/20/24 - 06/07/24
*
* Machine Minds Group Project
************************************************/

/* Libraries */
#include <IRremote.h>

/* IR Receiver Pins/Logic */
#define IR_RECV_PIN 10       // IR receiver Signal pin connect -> pin D10
IRrecv irrecv(IR_RECV_PIN);  // Create an instance of the IRrecv class with the specified pin
decode_results IRresults;

/* Movement Pins */
#define RIGHT_SPD_PIN   9  // Right PWM pin               -> MODEL-X ENA
#define RIGHT_DIR_PIN_1 12 // Right Motor direction pin 1 -> MODEL-X IN1
#define RIGHT_DIR_PIN_2 11 // Right Motor direction pin 2 -> MODEL-X IN2
#define LEFT_SPD_PIN    6  // Left PWM pin                -> MODEL-X ENB
#define LEFT_DIR_PIN_1  7  // Left Motor direction pin 1  -> MODEL-X IN3
#define LEFT_DIR_PIN_2  8  // Left Motor direction pin 1  -> MODEL-X IN4

/* IR Button Pins */
#define IR_BTN_UP     0x00FF18E7 //  "▲" button
#define IR_BTN_DOWN   0x00FF4AB5 //  "▼" button
#define IR_BTN_RIGHT  0x00FF5AA5 //  ">" button
#define IR_BTN_LEFT   0x00FF10EF //  "<" button
#define IR_BTN_OK     0x00FF38C7 // "OK" button
#define IR_BTN_HASH   0x00FFB04F //  "#" button
#define IR_BTN_STAR   0x00FF6897 //  "*" button
#define IR_BTN_1      0x00FFA25D //  "1" button
#define IR_BTN_2      0x00FF629D //  "2" button
#define IR_BTN_3      0x00FFE21D //  "3" button
#define IR_BTN_4      0x00FF22DD //  "4" button
#define IR_BTN_5      0x00FF02FD //  "5" button
#define IR_BTN_6      0x00FFC23D //  "6" button
#define IR_BTN_7      0x00FFE01F //  "7" button
#define IR_BTN_8      0x00FFA857 //  "8" button
#define IR_BTN_9      0x00FF906F //  "9" button
#define IR_BTN_0      0x00FF9867 // "10" button

/* Other Sensor Pins */
#define ECHO_PIN          4  // Ultrasonic sensor echo pin     -> pin D4
#define TRIG_PIN          5  // Ultrasonic sensor trigger pin  -> pin D5
#define RIGHT_IR_PAIR_PIN 2  // IR sensor pair yellow wire     -> pin D2
#define LEFT_IR_PAIR_PIN  3  // IR sensor pair green wire      -> pin D3
#define BUZZER_PIN        A5

/* Line Following Pins */
#define LINE_SENSOR_0 A0
#define LINE_SENSOR_1 A1
#define LINE_SENSOR_2 A2
#define LINE_SENSOR_3 A3
#define LINE_SENSOR_4 A4

/* Constants */
#define HALF_SPD_OF_SOUND 0.01715
#define MOVE_SPD 90
#define TURN_SPD 170

/* Other Variables */
bool lockFlag = LOW;
int leftIRPairValue = 0;
int rightIRPairValue = 0;
bool irPairsFollowObjectFlag = LOW;
bool lineFollowFlag = LOW;
String last_line_sensor;

/* Line Sensor Variations - 32 Total (2^5) */
#define LINE_NONE_CASE_A          "00000"
#define LINE_RIGHT                "00001"
#define LINE_SLIGHT_RIGHT_CASE_A  "00010"
#define LINE_SLIGHT_RIGHT_CASE_B  "00011"
#define LINE_CENTER_CASE_A        "00100"
#define LINE_SLIGHT_RIGHT_CASE_C  "00101"
#define LINE_SLIGHT_RIGHT_CASE_D  "00110"
#define LINE_SLIGHT_RIGHT_CASE_E  "00111"
#define LINE_SLIGHT_LEFT_CASE_A   "01000"
#define LINE_SLIGHT_RIGHT_CASE_F  "01001"
#define LINE_SLIGHT_RIGHT_CASE_G  "01010"
#define LINE_SLIGHT_RIGHT_CASE_H  "01011"
#define LINE_SLIGHT_LEFT_CASE_B   "01100"
#define LINE_SLIGHT_RIGHT_CASE_I  "01101"
#define LINE_CENTER_CASE_B        "01110"
#define LINE_SLIGHT_RIGHT_CASE_J  "01111"
#define LINE_LEFT                 "10000"
#define LINE_NONE_CASE_B          "10001"
#define LINE_SLIGHT_LEFT_CASE_C   "10010"
#define LINE_NONE_CASE_C          "10011"
#define LINE_SLIGHT_LEFT_CASE_D   "10100"
#define LINE_NONE_CASE_D          "10101"
#define LINE_NONE_CASE_E          "10110"
#define LINE_NONE_CASE_F          "10111"
#define LINE_NONE_CASE_G          "11000"
#define LINE_NONE_CASE_H          "11001"
#define LINE_SLIGHT_LEFT_CASE_E   "11010"
#define LINE_NONE_CASE_I          "11011"
#define LINE_SLIGHT_LEFT_CASE_F   "11100"
#define LINE_NONE_CASE_J          "11101"
#define LINE_NONE_CASE_K          "11110"
#define LINE_ALL                  "11111"

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
void moveSetSpd(int left_spd, int right_spd){
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
        enableBuzzer();
        return irPairsFollowObjectFlag;
      }
      break;
    }
    case IR_BTN_8:{
      // Scream
      if(lockFlag == LOW){
        for(int b = 0; b < 25; b++){
          enableBuzzer();
        }
      }
      break;
    }
    case IR_BTN_5:{
      // Line Follow
      lineFollowFlag = !lineFollowFlag;
      if(lockFlag == LOW){
        enableBuzzer();
        return lineFollowFlag;
      }
      break;
    }
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

void frontAvoidReaction(){
  enableBuzzer();
  moveStop();
  moveBack();
  delay(250);
  moveStop();
}

void backAvoidReaction(){
  enableBuzzer();
  moveStop();
  moveForward();
  delay(250);
  moveStop();
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
    frontAvoidReaction();
  }
}

void irPairsFollowObject(int leftIRPairValue, int rightIRPairValue){
  if((leftIRPairValue == LOW) && (rightIRPairValue == LOW)){
    // Object detected - follow it
    moveBack();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == HIGH)){
    // No object detected - stop
    moveStop();
  }
  else if((leftIRPairValue == LOW) && (rightIRPairValue == HIGH)){
    // Left detected object - follow it
    moveBackLeft();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == LOW)){
    // Right detected object - follow it
    moveBackRight();
  }
}

void irPairsAvoidObject(int leftIRPairValue, int rightIRPairValue){
  if((leftIRPairValue == LOW) && (rightIRPairValue == LOW) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
    backAvoidReaction();
  }
  else if((leftIRPairValue == LOW) && (rightIRPairValue == HIGH) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
    backAvoidReaction();
  }
  else if((leftIRPairValue == HIGH) && (rightIRPairValue == LOW) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
    backAvoidReaction();
  }
}

void lineSensorFollowLine(){
  /* Line sensor values come in inverted. So, we need to invert them to read them correctly. */
  String line_sensor = (String)(!digitalRead(LINE_SENSOR_0)) + (String)(!digitalRead(LINE_SENSOR_1)) + (String)(!digitalRead(LINE_SENSOR_2)) + (String)(!digitalRead(LINE_SENSOR_3)) + (String)(!digitalRead(LINE_SENSOR_4));

  if(line_sensor == LINE_CENTER_CASE_A || line_sensor == LINE_CENTER_CASE_B){
    moveForward();
  }

  if(line_sensor == LINE_LEFT){
    moveLeft();
  }
  if(line_sensor == LINE_SLIGHT_LEFT_CASE_A || line_sensor == LINE_SLIGHT_LEFT_CASE_B || line_sensor == LINE_SLIGHT_LEFT_CASE_C || line_sensor == LINE_SLIGHT_LEFT_CASE_D || line_sensor == LINE_SLIGHT_LEFT_CASE_E || line_sensor == LINE_SLIGHT_LEFT_CASE_F){
    moveForward();
    delay(100);
    moveLeft();
    moveStop();
  }
  if(line_sensor == LINE_RIGHT){
    moveRight();
  }
  if(line_sensor == LINE_NONE_CASE_A || line_sensor == LINE_ALL){
    if(last_line_sensor == LINE_RIGHT || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_A || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_B || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_C || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_D || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_E || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_F || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_G || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_H || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_I || last_line_sensor == LINE_SLIGHT_RIGHT_CASE_J){
      moveRight();
    }
    if(last_line_sensor == LINE_LEFT || last_line_sensor == LINE_SLIGHT_LEFT_CASE_A || last_line_sensor == LINE_SLIGHT_LEFT_CASE_B || last_line_sensor == LINE_SLIGHT_LEFT_CASE_C || last_line_sensor == LINE_SLIGHT_LEFT_CASE_D || last_line_sensor == LINE_SLIGHT_LEFT_CASE_E || last_line_sensor == LINE_SLIGHT_LEFT_CASE_F){
      moveLeft();
    }
  }
  if(line_sensor == LINE_SLIGHT_RIGHT_CASE_A || line_sensor == LINE_SLIGHT_RIGHT_CASE_B || line_sensor == LINE_SLIGHT_RIGHT_CASE_C || line_sensor == LINE_SLIGHT_RIGHT_CASE_D || line_sensor == LINE_SLIGHT_RIGHT_CASE_E || line_sensor == LINE_SLIGHT_RIGHT_CASE_F || line_sensor == LINE_SLIGHT_RIGHT_CASE_G || line_sensor == LINE_SLIGHT_RIGHT_CASE_H || line_sensor == LINE_SLIGHT_RIGHT_CASE_I || line_sensor == LINE_SLIGHT_RIGHT_CASE_J){
    moveForward();
    delay(100);
    moveLeft();
    moveStop();
  }
  if(line_sensor == LINE_NONE_CASE_B || line_sensor == LINE_NONE_CASE_C || line_sensor == LINE_NONE_CASE_D || line_sensor == LINE_NONE_CASE_E || line_sensor == LINE_NONE_CASE_F || line_sensor == LINE_NONE_CASE_G || line_sensor == LINE_NONE_CASE_H || line_sensor == LINE_NONE_CASE_I || line_sensor == LINE_NONE_CASE_J || line_sensor == LINE_NONE_CASE_K){
    moveStop();
  }
  last_line_sensor = line_sensor;
}

void setup(){
  /* Stop Buzzing ASAP */
  pinMode(BUZZER_PIN, OUTPUT);
  disableBuzzer();

  /* Start the receiver */
  irrecv.enableIRIn();

  /* Set Pin Modes */
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

  if((irPairsFollowObjectFlag == HIGH) && (lineFollowFlag == LOW) && (lockFlag == LOW)){
    irPairsFollowObject(leftIRPairValue, rightIRPairValue);
  }

  if((lineFollowFlag == HIGH) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
    lineSensorFollowLine();
  }

  irPairsAvoidObject(leftIRPairValue, rightIRPairValue);

  ultrasonicAvoidObject(distance);

  if(irrecv.decode(&IRresults)){   // Check if an IR signal has been received
    int received = IRresults.value;  // Get the received IR command
    irrecv.resume();                 // Continue receiving IR commands
    irMoveCMD(received);             // Move the robot received IR command
    delay(20);                       // Wait for 20 ms
  }
}