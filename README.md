# Machine Minds: Safe Cat Car - Group Project

## Link to slides
https://docs.google.com/presentation/d/1hcUatwnTb_Jt8qYY_E0cmD1dE1vI0Ov38E-occ2F1qs/edit?usp=sharing

## Objectives - Safe Cat Car
+ Front object avoidance via ultrasonic sensor
+ Rear object avoidance via IR pair sensors
+ Object following via IR pair sensors
+ Lock/unlock functionality via IR remote
+ Buzzer to interface with user
+ Basic robot movement via IR remote

## Group Plan
05/25/2024 - Robot movement initialization code shall be completed. Robot shall be fully constructed. \
05/26/2024 - IR control initialization code and controls shall be completed. \
**05/29/2024** - **Initial Group Presentation:** Robot movement and IR controls shall work for demo. \
06/01/2024 - Obstacle avoidance shall be implemented. \
06/02/2024 - Buzzer sensor logic shall be implemented. \
06/03/2024 - Object following shall be implemented. \
**06/05/2024** - **Final Group Presentation:** All robot features shall be implemented. \
06/06/2024 - Add speed/turn controls to IR remote logic (optional). \
**06/09/2024** - **Project Due:** All documentation and code shall be submitted.

## Anna Wilson: Group Role and Tasks
*Tasks*: Implement IR remote control logic. \
*Finished*: ✔ \
*Contribution*: ★★★ \
*Communication*: ★★★
### IR Reception - Receiver/Remote Interaction
A typical IR remote consists of buttons that send hex data to an IR transmitter (looks like clear LED). Each button has a unique hex code that can be decoded by an IR receiver to mean whatever the user desires, including simply the label printed on the buttons of the IR remote. \
### IR Reception - Command Logic
+ Maps "▲" to move the robot forward
+ Maps "▼" to move the robot backwards
+ Maps ">" to move the robot right
+ Maps "<" to move the robot left
+ Maps "OK" to stop robot movement
+ Maps "*" to lock the robot preventing further commands until unlocked and beep once
+ Maps "#" to unlock the robot enabling further commands until locked and beeps twice
+ Maps "0" to toggle object following
+ Default that does nothing to catch any other codes
```c++
void irMoveCMD(int received){
  switch(received){
    case IR_BTN_UP:{
      moveForward();
      break;
    }
    case IR_BTN_DOWN:{
      moveBack();
      break;
    }
    case IR_BTN_RIGHT:{
      moveRight();
      break;
    }
    case IR_BTN_LEFT:{
      moveLeft();
      break;
    }
    case IR_BTN_OK:{
      moveStop();
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
    default:{
      break;
    }
  }
}
```

## Chase Lafaurie: Group Role and Tasks
*Tasks*: Initialization Code and code review. \
*Finished*: ✔ \
*Contribution*: ★★★ \
*Communication*: ★★★

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
*Contribution*: ★★★ \
*Communication*: ★★★
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
*Tasks*: Obstacle avoidance with ultrasonic sensor, documentation, and main loop. \
*Finished*: ✔ \
*Contribution*: ★★★ \
*Communication*: ★★★
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
    enableBuzzer();
    moveStop();
    moveBack();
    delay(250);
    moveStop();
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

## Sam Richardson: Group Role and Tasks
*Tasks*: Buzzer Sensor and user interaction. \
*Finished*: ✔ \
*Contribution*: ★★★ \
*Communication*: ★★★
### Buzzer Functionality
Declare functions to stop robot movement and to set the speed of the motors.
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

## Vincent Tang: Group Role and Tasks
*Tasks*: Object following/avoidance with IR distance Sensor. \
*Finished*: ✔ \
*Contribution*: ★★★ \
*Communication*: ★★★
### Object Avoidance - IR Sensors
Get distance from ultrasonic sensor.
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
Use distance from ultrasonic sensor to keep car safe if too close to wall.
```c++
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
```

<!---
## Robot Introduction
#### Physical Programming Framework 
+ Four wheels
+ Four motors
+ Arduino UNO3
+ Acrylic chassis
+ Sensors

## Purpose of Robot
#### Experience Gain:
+ Sensors
+ Electronics
+ Programming
#### Physical Programming Framework
+ The physicality of the robot forces us to adapt our code to work in real life scenarios

## Hardware Components Descriptions
<table>
    <tr>
        <td>Arduino UNO3 board</td>
        <td>Controls motor driver and logic of robot</td>
    </tr>
    <tr>
        <td>MODEL-X Motor driver</td>
        <td>Sets/toggles motor speed/direction</td>
    </tr>
    <tr>
        <td>Four DC motors</td>
        <td>Move the wheels of the robot</td>
    </tr>
    <tr>
        <td>Four wheels</td>
        <td>Move the robot based on the motor driver</td>
    </tr>
    <tr>
        <td>Jumper wires</td>
        <td>Connect peripherals, drivers, and arduino board</td>
    </tr>
    <tr>
        <td>Battery pack</td>
        <td>Powers the robot</td>
    </tr>
    <tr>
        <td>Chassis</td>
        <td>Holds all components in place</td>
    </tr>
    <tr>
        <td>Mounting hardware</td>
        <td>Affixes components to chassis</td>
    </tr>
</table>

## Code Summary and Functions
+ Our code moves our robot in a square over the duration of about 6 seconds.
+ We had to change timing on turning and moving to account for our testing environment.
<table>
    <tr>
        <td>go_Advance()</td>
        <td>Moves robot forward</td>
    </tr>
    <tr>
        <td>stop_Stop()</td>
        <td>Stops robot movement</td>
    </tr>
    <tr>
        <td>go_Back()</td>
        <td>Moves robot backwards for t ms</td>
    </tr>
    <tr>
        <td>turn_Left(int t)</td>
        <td>Moves robot left for t ms</td>
    </tr>
    <tr>
        <td>turn_Right(int t)</td>
        <td>Moves robot right for t ms</td>
    </tr>
    <tr>
        <td>set_Motorspeed(int speed_L, int speed_R)</td>
        <td>Sets left/right motors speeds</td>
    </tr>
</table>

```c++
void move_in_square(){          // move robot in square in ~6 sec
  set_Motorspeed(SPEED, SPEED); // set speed to 190
  for(int i = 0; i < 4; i++){   // loop following code 4 times
    go_Advance();               // move forward
    delay(MOVE_TIME);           // keep moving forward for 1 sec
    turn_Right(TURN_TIME);      // turn right for 600 ms
    stop_Stop();                // stop robot movement each time
  }
}
```

## Demo
See _demo.mp4_

## Added Functionalities
+ We added code that spins the robot in a circle for about 5 seconds.
+ The loops and turn times are arbitrarily set to visually see the robot spin.
+ Spinning in a circle can be used to gather 360° perception data.
+ This could also allow the robot to draw a circle (if we attached a marker).

```c++
void move_in_circle(){          // spin robot for ~5 sec
  set_Motorspeed(SPEED, SPEED); // set speed to 190
  for(int i = 0; i < 4; i++){   // loop following code 4 times
    turn_Right(TURN_TIME*8);    // turn right for ~5 sec
  }
  stop_Stop();                  // stop robot movement after loop
}
```

-->

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
