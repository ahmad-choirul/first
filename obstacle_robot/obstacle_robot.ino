#include <AFMotor.h>
#include <Servo.h>
#include <NewPing.h>
 
#define TRIG_PIN A4 // Pin A4 on the Motor Drive Shield connected to the ultrasonic sensor
#define ECHO_PIN A5 // Pin A5 on the Motor Drive Shield connected to the ultrasonic sensor
#define MAX_DISTANCE_POSSIBLE 1000 // sets maximum useable sensor measuring distance to 1000cm
#define MAX_SPEED 120 // sets speed of DC traction motors to 120/256 or about 47% of full speed - to reduce power draining.
#define MOTORS_CALIBRATION_OFFSET 3 // this sets offset to allow for differences between the two DC motors
#define COLL_DIST 20 // sets distance at which robot stops and reverses to 10cm
#define TURN_DIST COLL_DIST+10 // sets distance at which robot looks away from object (not reverse) to 20cm (10+10)
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE_POSSIBLE); // sets up sensor library to use the correct pins to measure distance.
 
AF_DCMotor leftMotor(1, MOTOR12_64KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor rightMotor(4, MOTOR12_64KHZ); // create motor #2, using M2 output, set to 1kHz PWM frequency
Servo neckControllerServoMotor;  // create servo object to control a servo
 
int pos = 0; // this sets up variables for use in the sketch (code)
int maxDist = 0;
int maxAngle = 0;
int maxRight = 0;
int maxLeft = 0;
int maxFront = 0;
int course = 0;
int curDist = 0;
String motorSet = "";
int speedSet = 0;
int gt=0;
 
//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  neckControllerServoMotor.attach(9);  // attaches the servo on pin 9 (SERVO_2 on the Motor Drive Shield to the servo object
  neckControllerServoMotor.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  delay(2000); // delay for two seconds
  checkRoute(); // run the CheckRoute routine to find the best Route to begin travel
  motorSet = "FORWARD"; // set the director indicator variable to FORWARD
  neckControllerServoMotor.write(90); // ensure servo is still facing forward
  moveForward(); // run function to make robot move forward
}
//------------------------------------------------------------------------------------------------------------------------------------
 
//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  checkForward(); // check that if the robot is supposed to be moving forward, that the drive motors are set to move forward - this is needed to overcome some issues with only using 4 AA NiMH batteries
  checkRoute(); // set ultrasonic sensor to scan for any possible obstacles
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkRoute() {
  int curLeft = 0;
  int curFront = 0;
  int curRight = 0;
  int curDist = 0;
  neckControllerServoMotor.write(144); // set servo to face left 54-degrees from forward
  delay(120); // wait 120milliseconds for servo to reach position
  for (pos = 144; pos & gt = 36; pos -= 18) // loop to sweep the servo (& sensor) from 144-degrees left to 36-degrees right at 18-degree intervals.
  {
    neckControllerServoMotor.write(pos);  // tell servo to go to position in variable 'pos'
    delay(90); // wait 90ms for servo to get to position
    checkForward(); // check the robot is still moving forward
    curDist = readPing(); // get the current distance to any object in front of sensor
    if (curDist & lt; COLL_DIST) { // if the current distance to object is less than the collision distance
      checkCourse(); // run the checkCourse function
      break; // jump out of this loop
    }
    if (curDist & lt; TURN_DIST) { // if current distance is less than the turn distance
      changeRoute(); // run the changeRoute function
    }
    if (curDist & gt; curDist) {
      maxAngle = pos;
    }
    if (pos & gt; 90 & amp; & curDist & gt; curLeft) {
      curLeft = curDist;
    }
    if (pos == 90 & amp; & curDist & gt; curFront) {
      curFront = curDist;
    }
    if (pos & lt; 90 & amp; & curDist & gt; curRight) {
      curRight = curDist;
    }
  }
  maxLeft = curLeft;
  maxRight = curRight;
  maxFront = curFront;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void setCourse() { // set direction for travel based on a very basic distance map, simply which direction has the greatest distance to and object - turning right or left?
  if (maxAngle & lt; 90) {
    turnRight();
  }
  if (maxAngle & gt; 90) {
    turnLeft();
  }
  maxLeft = 0;
  maxRight = 0;
  maxFront = 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkCourse() { // we're about to hit something so move backwards, stop, find where the empty Route is.
  moveBackward();
  delay(500);
  moveStop();
  setCourse();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void changeRoute() {
  if (pos & lt; 90) {
    lookLeft(); // when current position of sensor is less than 90-degrees, it means the object is on the right hand side so look left
  }
  if (pos & gt; 90) {
    lookRight(); // when current position of sensor is greater than 90-degrees, it means the object is on the left hand side so look right
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
 
int readPing() { // read the ultrasonic sensor distance
  delay(70);
  unsigned int uS = sonar.ping();
  int cm = uS / US_ROUNDTRIP_CM;
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkForward() {
  if (motorSet == "FORWARD") {
    leftMotor.run(FORWARD);  // ensure motors are going forward
    rightMotor.run(FORWARD);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkBackward() {
  if (motorSet == "BACKWARD") {
    leftMotor.run(BACKWARD);  // ensure motors are going backward
    rightMotor.run(BACKWARD);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
 
// In some cases, the Motor Drive Shield may just stop if the supply voltage is too low (due to using only four NiMH AA cells).
// The above functions simply remind the Shield that if it's supposed to go forward, then ensure it is going forward and vice versa.
 
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {
  leftMotor.run(RELEASE);  // stop the motors.
  rightMotor.run(RELEASE);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);      // turn it on going forward
  rightMotor.run(FORWARD);      // turn it on going forward
  for (speedSet = 0; speedSet & lt; MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor.setSpeed(speedSet + MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
  motorSet = "BACKWARD";
  leftMotor.run(BACKWARD);      // turn it on going forward
  rightMotor.run(BACKWARD);     // turn it on going forward
  for (speedSet = 0; speedSet & lt; MAX_SPEED; speedSet += 2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor.setSpeed(speedSet + MOTORS_CALIBRATION_OFFSET);
    rightMotor.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  leftMotor.run(FORWARD);      // turn motor 1 forward
  rightMotor.run(BACKWARD);     // turn motor 2 backward
  delay(400); // run motors this way for 400ms
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);      // set both motors back to forward
  rightMotor.run(FORWARD);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  leftMotor.run(BACKWARD);     // turn motor 1 backward
  rightMotor.run(FORWARD);      // turn motor 2 forward
  delay(400); // run motors this way for 400ms
  motorSet = "FORWARD";
  leftMotor.run(FORWARD);      // turn it on going forward
  rightMotor.run(FORWARD);      // turn it on going forward
}
//-------------------------------------------------------------------------------------------------------------------------------------
void lookRight() {
  rightMotor.run(BACKWARD);  // looking right? set right motor backwards for 400ms
  delay(400);
  rightMotor.run(FORWARD);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void lookLeft() {
  leftMotor.run(BACKWARD);  // looking left? set left motor backwards for 400ms
  delay(400);
  leftMotor.run(FORWARD);
}
