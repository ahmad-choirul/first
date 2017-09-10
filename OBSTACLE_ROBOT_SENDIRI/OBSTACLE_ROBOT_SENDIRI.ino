/*
 * Obstacle avoiding robot.
 * dc motor 5 volt.
 * motor kiri = m3
 * motor kanan = m4
 * ultrasonic sensor = trigger pin a4, echo pin a5
 * servo motor = pin 6
 * by Ahmad Choirul jul 2016
 */

#include <AFMotor.h>
#include <Servo.h>

#include <NewPing.h>
Servo scanservo;   //Ping Sensor Servo

#define trigPin  3
#define echoPin  11
const int scanservopin = 6;
const int distancelimit = 10; //in cm
AF_DCMotor motor1(3, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor motor2(4, MOTOR12_1KHZ); // create motor #2, using M2 output, set to 1kHz PWM frequency
Servo myservo;  // create servo object to control a servo 
void setup() {
  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
myservo.attach(9);
myservo.write(90);
Serial.begin(9600);
delay(2000);      // wait two seconds
go();
}

void loop() {
  // put your main code here, to run repeatedly:
go();
int jarak = ping();
  if (jarak<distancelimit){
    stopp();
    char turndir = scan();
        switch (turndir){
            case 'l':
            turnleft();
            break;

            case 'r':
            turnright();
            break;

            case 'b':
            turnaround();
            break;
            }
  }
}

int ping(){

  long duration, distance;

  //Send Pulse

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  //Read Echo
  duration = pulseIn(echoPin, HIGH);
  // convert the time into a distance
  distance = (duration/2) / 29.1;
 Serial.print(distance);
  return distance;

}

void go(){
  Serial.print("maju");
motor1.run(FORWARD);      // turn it on going forward
    motor2.run(FORWARD);      // turn it on going forward
}
void turnleft(){
  Serial.print("kiri");
 motor1.run(BACKWARD);     // turn motor 1 backward
  motor2.run(FORWARD);      // turn motor 2 forward
  delay(400); // run motors this way for 400ms

  motor1.run(FORWARD);      // turn it on going forward
  motor2.run(FORWARD);      // turn it on going forward
}

void turnright(){
  Serial.print("kanan");
motor1.run(FORWARD);      // turn motor 1 forward
  motor2.run(BACKWARD);     // turn motor 2 backward
  delay(400); // run motors this way for 400ms

  motor1.run(FORWARD);      // set both motors back to forward
  motor2.run(FORWARD); 
}
void turnaround(){
Serial.print("balik");
motor1.run(FORWARD);      // turn motor 1 forward
  motor2.run(BACKWARD);     // turn motor 2 backward
  delay(400); // run motors this way for 400ms

  motor1.run(BACKWARD);      // set both motors back to forward
  motor2.run(FORWARD); 
  delay(400);
}
void stopp(){
  Serial.print("berhenti");
motor1.run(RELEASE);      // set both motors back to forward
  motor2.run(RELEASE); 
delay(1000);
}

char scan(){

  int leftscanval, centerscanval, rightscanval;
  char choice;

  //Look left
  scanservo.write(178);
  delay(300);
  leftscanval = ping();

  //Look right
  scanservo.write(4);
  delay(1000);
  rightscanval = ping();

  //center scan servo
  scanservo.write(83);
  delay(500);
  centerscanval = ping();
 

  if ((leftscanval>rightscanval) && (leftscanval>centerscanval)){
  choice = 'l';
  }

  else if ((rightscanval>leftscanval) && (rightscanval>centerscanval)){
  choice = 'r';
  }

  else{
  choice = 'b';
  }

  return choice;

}
