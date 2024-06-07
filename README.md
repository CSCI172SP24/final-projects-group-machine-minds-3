# Machine Minds: Safe Cat Car - Group Project

## Link to Slides
https://docs.google.com/presentation/d/1hcUatwnTb_Jt8qYY_E0cmD1dE1vI0Ov38E-occ2F1qs/edit?usp=sharing

## Link to Demonstration
https://drive.google.com/drive/folders/1sIG0-v08_aeYgSkFfhgbS_N7AI5cES7p?usp=drive_link

## Inspiration
Coconut: 

![coconut](https://github.com/CSCI172SP24/final-projects-group-machine-minds-3/assets/5532247/231b37ad-ce5d-4516-a867-2fdd06567f1d)
+ cat
+ beeps
+ scampers
+ avoidant

## Problem
We want to solve the problem of robot safety with our safe cat car. For safety it avoids obstacles, locks/unlocks, and can scream. It can also can move around and follow objects/lines (very well-behaved).

## Objectives - Safe Cat Car
+ Front object avoidance via ultrasonic sensor (distance indicates closeness -> avoid)
+ Rear object avoidance via IR pair sensors (any non-zero output indicates closeness -> avoid)
+ Object following via IR pair sensors (specific non-zero output indicates closeness -> follow)
+ Buzzer to interface with user (can be applied in many scenarios to mean different things)
+ Lock/unlock functionality via IR remote (simplest form of controlled user input)
+ Basic robot movement via IR remote (simplest form of controlled user input)
+ Line following via line following sensor (only way to accurately measure color on ground)

## Group Plan
05/25/2024 - Robot movement initialization code shall be completed. Robot shall be fully constructed. \
05/26/2024 - IR control initialization code and controls shall be completed. \
**05/29/2024** - **Initial Group Presentation:** Robot movement and IR controls shall work for demo. \
06/01/2024 - Obstacle avoidance shall be implemented. \
06/02/2024 - Buzzer sensor logic shall be implemented. \
06/03/2024 - Object following shall be implemented. \
**06/05/2024** - **Final Group Presentation:** Almost robot features shall be implemented. \
06/06/2024 - Add line following functionality. \
**06/09/2024** - **Project Due:** All documentation and code shall be submitted/complete.

## Anna Wilson: Group Role and Tasks
*Tasks*: Implement IR remote control logic. \
*Finished*: ✔ \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Interacting with global variables (use returns).
### IR Reception - Receiver/Remote Interaction
A typical IR remote consists of buttons that send hex data to an IR transmitter (looks like clear LED). Each button has a unique hex code that can be decoded by an IR receiver to mean whatever the user desires, including simply the label printed on the buttons of the IR remote.
### IR Reception - Command Logic
+ Maps "▲" to move the robot forward
+ Maps "▼" to move the robot backwards
+ Maps ">" to move the robot right
+ Maps "<" to move the robot left
+ Maps "OK" to stop robot movement
+ Maps "*" to lock the robot preventing further commands until unlocked and beep once
+ Maps "#" to unlock the robot enabling further commands until locked and beeps twice
+ Maps "0" to toggle object following
+ Maps “8” to make the cat car scream
```c++
void irMoveCMD(int received){
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
    default:{
      break;
    }
  }
}
```

## Chase Lafaurie: Group Role and Tasks
*Tasks*: Initialization Code and code review. \
*Finished*: ✔ \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Sourcing IR remote codes (google). Naming conventions (group discussion).
### Initialization Code - IR Remote Button Declarations
Initialize the preprocessor commands mapped to the buttons on our IR remote.
```c++
#define IR_BTN_UP     0x00FF18E7  //  "▲" button
#define IR_BTN_DOWN   0x00FF4AB5  //  "▼" button
#define IR_BTN_RIGHT  0x00FF5AA5  //  ">" button
#define IR_BTN_LEFT   0x00FF10EF  //  "<" button
#define IR_BTN_OK     0x00FF38C7  //  "OK" button
#define IR_BTN_HASH   0x00FFB04F  //  "#" button
#define IR_BTN_STAR   0x00FF6897  //  "*" button
#define IR_BTN_1      0x00FFA25D  //  "1" button
#define IR_BTN_2      0x00FF629D  //  "2" button
#define IR_BTN_3      0x00FFE21D  //  "3" button
#define IR_BTN_4      0x00FF22DD  //  "4" button
#define IR_BTN_5      0x00FF02FD  //  "5" button
#define IR_BTN_6      0x00FFC23D  //  "6" button
#define IR_BTN_7      0x00FFE01F  //  "7" button
#define IR_BTN_8      0x00FFA857  //  "8" button
#define IR_BTN_9      0x00FF906F  //  "9" button
#define IR_BTN_0      0x00FF9867  // "10" button
```
### Initialization Code - Pin and Variable Initialization
+ Declare the pin our IR receiver is connected to, digital pin 10.
+ Create an instance of the IRrecv class with the specified IR receiver pin, digital pin 10.
+ Create a variable to store the decoded IR results.
```c++
#include <IRremote.h>
#define IR_RECV_PIN  10
IRrecv irrecv(IR_RECV_PIN);
decode_results IRresults;
```
### Initialization Code - Other Declarations
Declare pins for robot motor control, pins for sensor control, preprocessor commands for constants, and global variables.
```c++
#define RIGHT_SPD_PIN    9  // Right PWM pin               -> MODEL-X ENA
#define RIGHT_DIR_PIN_1  12 // Right Motor direction pin 1 -> MODEL-X IN1
#define RIGHT_DIR_PIN_2  11 // Right Motor direction pin 2 -> MODEL-X IN2
#define LEFT_SPD_PIN     6  // Left PWM pin                -> MODEL-X ENB
#define LEFT_DIR_PIN_1   7  // Left Motor direction pin 1  -> MODEL-X IN3
#define LEFT_DIR_PIN_2   8  // Left Motor direction pin 1  -> MODEL-X IN4

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
```
### Initialization Code - Setup Function
Start IR reception and set pin modes of all used pins.
```c++
void setup(){
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
}
```

## Gracy Conrad: Group Role and Tasks
*Tasks*: Robot Assembly and movement functions. \
*Finished*: ✔ \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Servo displacing ultrasonic sensor (disconnect it).
### Robot Movement - Functions
Declare functions to move the robot forward, left, right, back,back left, back right.
```c++
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
```

### Robot Movement - Functions Cont.
Declare functions to stop robot movement and to set the speed of the motors.
```c++
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
```

## Jonah Duncan: Group Role and Tasks
*Tasks*: Obstacle avoidance with ultrasonic sensor, documentation, and main loop. Line following (done after presentation). \
*Finished*: ✔ (line following done after presentation) \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Obstacle avoidance logic (trial & error). Keeping loop function clean (group discussion).
### Object Avoidance - Ultrasonic Sensor
Get distance from ultrasonic sensor.
```c++
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
```
### Object Avoidance - Ultrasonic Sensor Cont.
Use distance from ultrasonic sensor to keep car safe if too close to wall.
```c++
void ultrasonicAvoidObject(long distance){
  if((distance < 10) && (lockFlag == LOW)){
    frontAvoidReaction();
  }
}
```
### Loop Logic - Assorted
+ Avoid objects if not in follow mode (“0”) and not in lock mode
+ Follow objects if in follow mode (“0”) and not in lock mode
```c++
void loop(){
  long distance = getUltrasonicDistance();
  rightIRPairValue = digitalRead(RIGHT_IR_PAIR_PIN);
  leftIRPairValue = digitalRead(LEFT_IR_PAIR_PIN);

  if((irPairsFollowObjectFlag == HIGH) && (lockFlag == LOW)){
    irPairsFollowObject(leftIRPairValue, rightIRPairValue);
  }

  irPairsAvoidObject(leftIRPairValue, rightIRPairValue);

  ultrasonicAvoidObject(distance);
```
### Loop Logic - IR reception
+ Check if an IR signal has been received.
+ Store the received IR command.
+ Continue receiving IR commands.
+ Move the robot based on received IR command.
+ Delay processing by 20 ms to let Arduino catch up.
```c++
  if(irrecv.decode(&IRresults)){     // Check if an IR signal has been received
    int received = IRresults.value;  // Get the received IR command
    irrecv.resume();                 // Continue receiving IR commands
    irMoveCMD(received);             // Move the robot received IR command
  delay(20);  // Wait for 20 ms
  }
```
## Jonah Duncan: Post Presentation Task
These are code snippets since the code is added in assorted places.
### Line Following - Initialization
I needed to keep track of the mode and the last line sensor value for upcoming logic. The rest is to cover all cases of sensor outputs.
```c++
/* Other Variables */
bool lineFollowFlag = LOW;
String last_line_sensor;
```
```c++
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
```
### Line Following - Function
The following code converts inverted line sensor digital reads into a string and then compares them against the 32 cases we made, reacting differently based on the case.
```c++
void lineSensorFollowLine(){
  /* Line sensor values come in inverted. So, we need to invert them to read them correctly. */
  String line_sensor = (String)(!digitalRead(LINE_SENSOR_0)) + (String)(!digitalRead(LINE_SENSOR_1)) + (String)(!digitalRead(LINE_SENSOR_2)) + (String)(!digitalRead(LINE_SENSOR_3)) + (String)(!digitalRead(LINE_SENSOR_4)) + '\0';

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
```

### Line Following - Loop Code
Continuously follow line updating while updating last line value. Mode checking implemented.
```c++
void loop(){
  if((lineFollowFlag == HIGH) && (irPairsFollowObjectFlag == LOW) && (lockFlag == LOW)){
    lineSensorFollowLine();
  }
}
```

## Sam Richardson: Group Role and Tasks
*Tasks*: Buzzer Sensor and user interaction. \
*Finished*: ✔ \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Determining when/how to buzz (group discussion, trial & error).
### Buzzer - Functionality
Declare functions to enable the buzzer (and buzz) and to disable to buzzer.
```c++
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
```
### Buzzer - Avoidance Reactions
Declare functions for buzzer/avoidance reactions.
```c++
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
```

## Vincent Tang: Group Role and Tasks
*Tasks*: Object following/avoidance with IR distance Sensor. \
*Finished*: ✔ \
*Contribution*: 5/5 \
*Communication*: 5/5 \
*Challenges*: Covering edge cases (trial & error). Setting follow reaction (trial & error).
### Object Avoidance - IR Sensors
Check different states of left/right IR sensor pairs and react accordingly.
```c++
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
```
### Object Avoidance - Object Following
Check different states of left/right IR sensor pairs and react accordingly.
```c++
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
```

## Thank you for your reading!
![man](https://github.com/CSCI172SP24/final-projects-group-machine-minds-3/assets/5532247/32e8c436-f273-4b95-88d6-ff4d4f74bcf7)

## Contributors
<table>
<tbody>
<!---
<tr>
<td align="center">
  <a>
    Some Role
  </a>
</td>
<td align="center">
  <a >
    Some Role
  </a>
</td>
<td align="center">
  <a>
    Some Role
  </a>
</td>
<td align="center">
  <a>
    Some Role
  </a>
</td>
<td align="center">
  <a>
    Some Role
  </a>
</td>
<td align="center">
  <a>
    Some Role
  </a>
</td>
</tr>
-->
<tr>
<td align="center">
  <a>
    Anna Wilson
  </a>
</td>
<td align="center">
  <a >
    Chase Lafaurie
  </a>
</td>
<td align="center">
  <a>
    Gracy Conrad
  </a>
</td>
<td align="center">
  <a>
    Jonah Duncan
  </a>
</td>
<td align="center">
  <a>
    Sam Richardson
  </a>
</td>
<td align="center">
  <a>
    Vincent Tang
  </a>
</td>
</tr>
</tbody>
</table>
