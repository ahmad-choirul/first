/*
 * Obstacle avoiding robot.
 * dc motor 5 volt.
 * motor kiri = m3
 * motor kanan = m4
 * ultrasonic sensor = trigger pin 3, echo pin 11 
 * servo motor = pin 9/servo 2
 * by Ahmad Choirul jul 2016
 */

#include <AFMotor.h>
#include <Servo.h>

#define trigPin  3 
#define echoPin  11
const int scanservopin = 6;
const int jarakbatas = 20; //in cm
AF_DCMotor motor1(3, MOTOR12_64KHZ); //mendeklarasikan m4 menjadi motor 1, 64KHz
AF_DCMotor motor2(4, MOTOR12_64KHZ); //mendeklarasikan m3 menjadi motor 2, 64KHz
Servo myservo;  // membuat variabel servo dengan nama myservo 

void setup() {
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
myservo.attach(9);
myservo.write(90);
motor1.setSpeed(255); //Set kecepatan motor 255 (Range 0-255)
motor2.setSpeed(255); //Set kecepatan motor 255 (Range 0-255)
Serial.begin(9600);
delay(5000);
go();
}

void loop() {
  // put your main code here, to run repeatedly:
go();
int jarak = ping();
  if (jarak<jarakbatas){
    stopp();//berhenti dan malkukan pengecekan jarak
    char turndir = scan();
        switch (turndir){
            case 'l'://belok kiri
            belokkanan();
            break;

            case 'r'://belok kanan
            belokkiri();
            break;

            case 'b'://balik arah
            balik();
            break;
            }
  }
  delay(100);
}

int ping(){

  long duration, jarak;
  //Send Pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  //Read Echo
  duration = pulseIn(echoPin, HIGH);
  // convert duration menjadi distance
  jarak = (duration/2) / 29.1;
 Serial.println(jarak);
  return jarak;
}

void go(){
  Serial.print("maju");
motor1.run(FORWARD);      // motor 1 maju
motor2.run(FORWARD);      // motor 2 maju
}

void belokkiri(){
  Serial.print("kiri");
 motor1.run(BACKWARD);     // motor 1 mundur
 motor2.run(FORWARD);      //motor 2 maju
  delay(400); // run motors this way for 400ms

  motor1.run(FORWARD);      // turn it on going forward
  motor2.run(FORWARD);      // turn it on going forward
}

void belokkanan(){
  Serial.print("kanan");
motor1.run(FORWARD);      // turn motor 1 forward
  motor2.run(BACKWARD);     // turn motor 2 backward
  delay(400); // run motors this way for 400ms

  motor1.run(FORWARD);      // set both motors back to forward
  motor2.run(FORWARD); 
}
void balik(){
Serial.print("balik");
motor1.run(FORWARD);      // turn motor 1 forward
  motor2.run(BACKWARD);     // turn motor 2 backward
  delay(800); // run motors this way for 400ms

}
void stopp(){
  Serial.print("berhenti");
motor1.run(RELEASE);      // set both motors back to forward
  motor2.run(RELEASE); 
delay(1000);
}

char scan(){

  int scankiri, scantengah, scankanan;
  char pilih;

  //lihat kiri
  myservo.write(160);
  Serial.println("hadap kiri ");
  delay(300);
  scankiri = ping();

  //lihat kanan
  myservo.write(15);
  Serial.println("hadap kanan ");
  delay(700);
  scankanan = ping();

  //tengah
  myservo.write(90);
  Serial.println("hadap tengah ");
  delay(500);
  scantengah = ping();

  if ((scankiri>scankanan) && (scankiri>scantengah)){
  pilih = 'l';
  }

  else if ((scankanan>scankiri) && (scankanan>scantengah)){
  pilih = 'r';
  }

  else{
  pilih = 'b';
  }

  return pilih;

}
