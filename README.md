# Group Project - Final - Machine Minds

## Link to slides
https://docs.google.com/presentation/d/1hcUatwnTb_Jt8qYY_E0cmD1dE1vI0Ov38E-occ2F1qs/edit?usp=sharing

## IR Reception - Receiver/Remote Interaction
A typical IR remote consists of buttons that send hex data to an IR transmitter (looks like clear LED). Each button has a unique hex code that can be decoded by an IR receiver to mean whatever the user desires, including simply the label printed on the buttons of the IR remote.

## IR Reception - Pin and Variable Initialization
+ Declare the pin our IR receiver is connected to, digital pin 10.
+ Create an instance of the IRrecv class with the specified IR receiver pin, digital pin 10.
+ Create a variable to store the decoded IR results.
```c++
#include <IRremote.h>
#define IR_RECV_PIN  10
IRrecv irrecv(IR_RECV_PIN);
decode_results IRresults;
```

## IR Reception - IR Remote Button Preprocessor Commands
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

## IR Reception - Command Logic
+ Maps "▲" to move the robot forward
+ Maps "▼" to move the robot backwards
+ Maps ">" to move the robot right
+ Maps "<" to move the robot left
+ Maps "OK" to stop robot movement
+ Maps "*" to move the robot backwards and to the left
+ Maps "#" to move the robot backwards and to the right
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
      moveBackLeft();
      break;
    }
    case IR_BTN_HASH:{
      moveBackRight();
      break;
    }
    default:{
      break;
    }
  }
}
```

## Robot Movement - Setup
Declare pins for robot motor control and preprocessor commands for constants.
```c++
#define RIGHT_SPD_PIN    9   // Right PWM pin               -> MODEL-X ENA
#define RIGHT_DIR_PIN_1  12  // Right Motor direction pin 1 -> MODEL-X IN1
#define RIGHT_DIR_PIN_2  11  // Right Motor direction pin 2 -> MODEL-X IN2
#define LEFT_SPD_PIN     6   // Left PWM pin                -> MODEL-X ENB
#define LEFT_DIR_PIN_1   7   // Left Motor direction pin 1  -> MODEL-X IN3
#define LEFT_DIR_PIN_2   8   // Left Motor direction pin 1  -> MODEL-X IN4
#define HALF_SPD_OF_SOUND  0.01715
#define MOVE_SPD           90
#define TURN_SPD           170
```

## Robot Movement - Functions
Declare functions to move the robot forward and left.
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
```

Declare functions to move the robot right and back.
```c++
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
```

Declare functions to move the robot back left and back right.
```c++
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

## Main - Setup Function
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

## Main - Loop Function
+ Check if an IR signal has been received.
+ Store the received IR command.
+ Continue receiving IR commands.
+ Move the robot based on received IR command.
+ Delay processing by 20 ms to let Arduino catch up.
```c++
void loop(){
  if(irrecv.decode(&IRresults)){
    int received = IRresults.value;
    irrecv.resume();
    irMoveCMD(received);
  }
  delay(20);
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
