// Rolly Mk. II Robot
// APC magazine - June 2014 issue
// Darren Yates - 30 March 2014

#include <AFMotor.h>
#include <Servo.h> 
#include <NewPing.h>

#define TRIG_PIN A5
#define ECHO_PIN A4
#define MAX_DISTANCE 400
#define MAX_SPEED 255
#define MAX_SPEED_OFFSET -8
#define COLL_DIST 20
#define TURN_DIST COLL_DIST+10
#define ACT_TIME 250
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motorR(3, MOTOR12_1KHZ); // create motor #1, 1kHz PWM
AF_DCMotor motorL(4, MOTOR12_1KHZ); // create motor #2, 1kHz PWM

Servo myservo;  // create servo object to control a servo 
String motorSet = "";

int curDist = 0;
int pos;
int speedSet = 0;

//-------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(90); // face servo forward
  delay(2000);
  motorSet = "FORWARD";
  moveForward();
  
}
//-------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  
  checkPath();
  
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkPath() {
  
  int curLeft = 0; int curRight = 0; int curFront = 0;
  curDist = 0;
  
  checkForward();
  myservo.write(35);
  delay(100);
  for (pos = 135; pos >= 45; pos -= 45) {
    myservo.write(pos);
    delay(170);
    curDist = readPing();
    
    if (curDist < COLL_DIST) { checkCourse(); break; }
    if (curDist < TURN_DIST) { changePath(); } 
  
  }    
}  
//-------------------------------------------------------------------------------------------------------------------------------------
int readPing() {
  int cm = 0;
  while (cm < 2) {
    int uS = sonar.ping();
    cm = uS/US_ROUNDTRIP_CM;
  }
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkForward() { 
  if (motorSet=="FORWARD") { motorR.run(FORWARD); motorL.run(FORWARD); } 
}     // turn it on going forward
//-------------------------------------------------------------------------------------------------------------------------------------
void changePath() {
  if (pos < 90) { veerLeft(); } 
  if (pos > 90) { veerRight(); }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void veerRight() {
  motorR.run(BACKWARD);  motorL.run(FORWARD); 
  delay(ACT_TIME); 
  motorR.run(FORWARD);   motorL.run(FORWARD);
  motorSet = "FORWARD";
}
//-------------------------------------------------------------------------------------------------------------------------------------
void veerLeft() {
  motorL.run(BACKWARD);   motorR.run(FORWARD); 
  delay(ACT_TIME); 
  motorL.run(FORWARD);   motorR.run(FORWARD);
  motorSet = "FORWARD";
}
//-------------------------------------------------------------------------------------------------------------------------------------
void checkCourse() {
  moveBackward();
  delay(ACT_TIME);
  moveStop();
  setCourse();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void setCourse() {
  if (pos < 90) { turnRight(); } 
  if (pos > 90) { turnLeft(); }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
  motorSet = "BACKWARD";
  motorR.run(BACKWARD);      
  motorL.run(BACKWARD);     
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)
  {
    motorL.setSpeed(speedSet);
    motorR.setSpeed(speedSet+MAX_SPEED_OFFSET);
    delay(5);
  }
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  motorSet = "FORWARD";
  checkForward();
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2)  {
    motorL.setSpeed(speedSet);
    motorR.setSpeed(speedSet+MAX_SPEED_OFFSET);
    delay(4);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() { motorR.run(RELEASE); motorL.run(RELEASE); }
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  motorR.run(FORWARD);      // turn it on going forward
  motorL.run(BACKWARD);     // turn it on going forward
  delay(ACT_TIME);
  motorSet = "FORWARD";
  checkForward();
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  motorR.run(BACKWARD);      // turn it on going backward
  motorL.run(FORWARD);     // turn it on going forward
  delay(ACT_TIME);
  motorSet = "FORWARD";
  checkForward();
}  
//-------------------------------------------------------------------------------------------------------------------------------------

