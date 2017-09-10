#include <AFMotor.h>
char val;
AF_DCMotor motor1(4, MOTOR12_64KHZ); //Jadikan motor 1, 64KHz
AF_DCMotor motor2(3, MOTOR12_64KHZ); //Jadikan motor 2, 64KHz

void setup() {
  motor1.setSpeed(240); //Set kecepatan motor 240 (Range 0-255)
  motor2.setSpeed(240); //Set kecepatan motor 240 (Range 0-255)
  Serial.begin(9600); //Kecepatan komunikasi serial
}

void loop() {
  if( Serial.available() >0 )     
  {
    //Variable val untuk menyimpan sementara hasil dari bluetooth
    val = Serial.read();
    Serial.println(val);
  }
  if( val == 'F' ) {     //Motor Maju
    motor1.run(FORWARD); //Motor kiri maju
    motor2.run(FORWARD); //Motor kanan maju
  }
   if( val == 'B') {      //Motor Mundur
    motor1.run(BACKWARD); //Motor kiri mundur
    motor2.run(BACKWARD); //Motor kanan mundur
  }
   if( val == 'R' ) {     //Motor Berbelok kanan
    motor1.run(FORWARD);  //Motor kiri maju
    motor2.run(BACKWARD); //Motor kanan mundur
  }
   if( val == 'L' ) {     //Motor Berbelok kiri
    motor1.run(BACKWARD); //Motor kiri mundur
    motor2.run(FORWARD);  //Motor kanan maju
  }
    if( val == 'S' ) {     //Motor Berhenti
    motor1.run(RELEASE);   //Motor kiri berhenti
    motor2.run(RELEASE);   //Motor kanan berhenti
  }
  delay(100);

    if( val == '1' ) {     //Motor Serong Kiri depan
    motor1.run(RELEASE);   //Motor kiri berhenti
    motor2.run(FORWARD);   //Motor kanan maju
  }
    if( val == '2' ) {     //Motor Serong Kanan depan
    motor1.run(FORWARD);   //Motor kiri maju
    motor2.run(RELEASE);   //Motor kanan berhenti
  }
    if( val == '3' ) {     //Motor Serong Kiri belakang
    motor1.run(BACKWARD);  //Motor kiri mundur
    motor2.run(RELEASE);   //Motor kanan berhenti
  }
    if( val == '4' ) {     //Motor Serong Kanan belakang
    motor1.run(RELEASE);   //Motor kiri berhenti
    motor2.run(BACKWARD);  //Motor kanan mundur
  }
}  
