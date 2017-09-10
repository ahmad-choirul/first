#include <AFMotor.h>
#include <Servo.h>
#include <Ultrasonic.h>
typedef enum {
  RADAR_LEFT = 0, RADAR_CENTER = 60, RADAR_RIGHT = 120 } RadarDirection;
Servo radarServo;  // create servo object to control a servo
int RADAR_INPUT_PIN = 9;
int RADAR_MOVEMENT_DELAY = 250;
int RADAR_DISTANCE_MINIMAL_IN_CM = 80; // centimeter
int RADAR_DISTANCE_UNKNOWN_IN_CM = 0; // centimeter
int RADAR_DISTANCE_UNDEFINED = -1;
Ultrasonic radarUltrasonic(A5,A4);
AF_DCMotor motorLeft(3, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motorRight(1, MOTOR12_64KHZ); // create motor #1, 64KHz pwm
int MOTOR_SPEED = 200;// 150;
int MOTOR_TURN_SPEED = 200; //250;
int MOTOR_DELAY = 150;
int MOTOR_TURN_DELAY = 500;
int MOTOR_BACKWARD_DELAY = 1000;
struct RobotState {
  int leftMotorDirection;
  int rightMotorDirection;
  int leftMotorSpeed;
  int rightMotorSpeed;
  int motorDelay;
  boolean isBackwardState;
};
struct RadarDistances {
  int left;
  int center;
  int right;
};
RobotState forwardState = {
  FORWARD,FORWARD,MOTOR_SPEED,MOTOR_SPEED,MOTOR_DELAY,false};
RobotState backwardState = {
  BACKWARD,BACKWARD,MOTOR_SPEED,MOTOR_SPEED,MOTOR_BACKWARD_DELAY,true};
RobotState backwardUturnState = {
  FORWARD,BACKWARD,MOTOR_SPEED,MOTOR_SPEED,MOTOR_TURN_DELAY,true};
RobotState turnLeftState = {
  RELEASE,FORWARD,MOTOR_SPEED,MOTOR_TURN_SPEED,MOTOR_TURN_DELAY,false};
RobotState turnRightState = {
  FORWARD,RELEASE,MOTOR_TURN_SPEED,MOTOR_SPEED,MOTOR_TURN_DELAY,false};
RobotState stopState = {
  RELEASE,RELEASE,MOTOR_SPEED,MOTOR_SPEED,MOTOR_BACKWARD_DELAY,false};
RobotState currentState = forwardState;
RadarDistances currentDistances = {
  RADAR_DISTANCE_UNDEFINED,RADAR_DISTANCE_UNDEFINED,RADAR_DISTANCE_UNDEFINED};
RadarDistances lastDistances = {
  RADAR_DISTANCE_UNDEFINED,RADAR_DISTANCE_UNDEFINED,RADAR_DISTANCE_UNDEFINED};
void setup() {
  initTrace();
  initRadar();
  initMotors();
}
void initTrace(){
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("My First Robot");
}
void initRadar(){
  radarServo.attach(RADAR_INPUT_PIN);  // attaches the servo on pin RADAR_INPUT_PIN to the servo object
  radarServo.write(RADAR_CENTER);
}
void initMotors(){
  motorLeft.setSpeed(MOTOR_SPEED);     // set the speed to 200/255
  motorRight.setSpeed(MOTOR_SPEED);     // set the speed to 200/255
}
void loop() {
  calculateRobotState();
  makeDistancesSnapshot();
  go();
}
void calculateRobotState(){
  measureDistanceAhead();
  if(
    isCenterDistanceUndefinded() ||
    isNoObstacleInPathAhead())
  {
//    if(isInCorner()){
//      currentState = backwardState; 
//      return;
//    }
    currentState = forwardState; 
    return;
  }
  measureDistanceLeft();
  measureDistanceRight();
  if(greater(currentDistances.left,currentDistances.right)){
    currentState = turnLeftState; 
    return;
  }
  if(greater(currentDistances.right,currentDistances.left)){
    currentState = turnRightState; 
    return;
  }
  if(eq(currentDistances.left,currentDistances.right)){
    currentState = backwardState; 
    return;
  }
  currentState = stopState;
}
void go(){
  Serial.print("Go : { ");
  Serial.print(currentState.leftMotorDirection);
  Serial.print(" , ");
  Serial.print(currentState.rightMotorDirection);
  Serial.println(" }");
  Serial.print("with : { left:");
  Serial.print(currentDistances.left);
  Serial.print(" , center:");
  Serial.print(currentDistances.center);
  Serial.print(" , right:");
  Serial.print(currentDistances.right);
  Serial.println(" }");
  motor();
}
void motor(){
  motorLeft.setSpeed(currentState.leftMotorSpeed);
  motorRight.setSpeed(currentState.rightMotorSpeed);
  motorLeft.run(currentState.leftMotorDirection);
  motorRight.run(currentState.rightMotorDirection);
  if(currentState.isBackwardState){
    delay(currentState.motorDelay);
    motorLeft.setSpeed(backwardUturnState.leftMotorSpeed);
    motorRight.setSpeed(backwardUturnState.rightMotorSpeed);
    motorLeft.run(backwardUturnState.leftMotorDirection);
    motorRight.run(backwardUturnState.rightMotorDirection);
    delay(backwardUturnState.motorDelay);
  }
}
void measureDistanceLeft(){
  moveRadarFastTo(RADAR_LEFT);
  currentDistances.left = internalMeasure();
}
void measureDistanceRight(){
  moveRadarFastTo(RADAR_RIGHT);
  currentDistances.right = internalMeasure();
}
void measureDistanceAhead(){
  moveRadarFastTo(RADAR_CENTER);
  currentDistances.center = internalMeasure();
}
int internalMeasure(){
  long microsec = radarUltrasonic.timing();
  return radarUltrasonic.convert(microsec, Ultrasonic::CM);
}
void moveRadarFastTo(int radarServoPosition){
  radarServo.write(radarServoPosition);              // tell servo to go to position in variable 'pos'
  delay(RADAR_MOVEMENT_DELAY);
}
boolean isCenterDistanceUndefinded(){
  return currentDistances.center == RADAR_DISTANCE_UNKNOWN_IN_CM;
}
boolean isNoObstacleInPathAhead(){
  return currentDistances.center >= RADAR_DISTANCE_MINIMAL_IN_CM;
}
boolean isInCorner(){
  boolean left = eq(currentDistances.left,lastDistances.left);
  boolean center = eq(currentDistances.center,lastDistances.center);
  boolean right = eq(currentDistances.right,lastDistances.right);
  return left && center && right;
}
boolean eq(int a, int b){
  int epsilon = 5;
  int diff = a - b;
  diff = diff < 0 ? (-1)*diff : diff;
  return diff <= epsilon ? true : false;
}
boolean greater(int a, int b){
  int epsilon = 5;
  int diff = a - b;
  if(diff > 0){
    diff = diff - epsilon;
  }
  return diff > 0;
}
void makeDistancesSnapshot(){
  lastDistances.left = currentDistances.left;
  lastDistances.center = currentDistances.center;
  lastDistances.right = currentDistances.right;
} 
