#include <Servo.h>

#define SERVO    3     // Servo Motor Control Pin

#define C1 8
#define C2 9
#define C3 10
#define C4 11
#define R1 7
#define R2 6
#define R3 5
#define R4 4
#define greenLight A0
#define redLight A1
#define buzzer 13

Servo myServo;

void setup( ) {
Serial.begin(9600);
myServo.attach(SERVO);
pinMode(greenLight, OUTPUT);
pinMode(redLight, OUTPUT);
digitalWrite(greenLight, LOW);
digitalWrite(redLight, LOW);
pinMode(C1, INPUT_PULLUP);
pinMode(C2, INPUT_PULLUP);
pinMode(C3, INPUT_PULLUP);
pinMode(C4, INPUT_PULLUP);
pinMode(R1, OUTPUT); digitalWrite(R1, 1);
pinMode(R2, OUTPUT); digitalWrite(R2, 1);
pinMode(R3, OUTPUT); digitalWrite(R3, 1);
pinMode(R4, OUTPUT); digitalWrite(R4, 1);
}
void loop( ) {
digitalWrite(R1, 0);
if (digitalRead(C1) == 0) {Serial.println("S16"); digitalWrite(greenLight, HIGH);}
if (digitalRead(C2) == 0) {Serial.println("S15"); digitalWrite(greenLight, LOW);}
if (digitalRead(C3) == 0) {Serial.println("S14"); digitalWrite(redLight, HIGH);}
if (digitalRead(C4) == 0) {Serial.println("S13"); digitalWrite(redLight, LOW);}
digitalWrite(R1, 1);
digitalWrite(R2, 0);
if (digitalRead(C1) == 0) {Serial.println("S12");tone(buzzer, 262, 100);delay(100);tone(buzzer, 330, 100);delay(100);tone(buzzer, 392, 100); } //열림
if (digitalRead(C2) == 0) {Serial.println("S11");tone(buzzer, 392, 100);delay(100);tone(buzzer, 330, 100);delay(100);tone(buzzer, 262, 100); } //잠김
if (digitalRead(C3) == 0) {Serial.println("S10");    myServo.write(1450);
    delay(3000);
    myServo.write(500);
    delay(3000);}
if (digitalRead(C4) == 0) {Serial.println("S9");}
digitalWrite(R2, 1);
digitalWrite(R3, 0);
if (digitalRead(C1) == 0) {Serial.println("S8");}
if (digitalRead(C2) == 0) {Serial.println("S7");}
if (digitalRead(C3) == 0) {Serial.println("S6");}
if (digitalRead(C4) == 0) {Serial.println("S5");}
digitalWrite(R3, 1);
digitalWrite(R4, 0);
if (digitalRead(C1) == 0) {Serial.println("S4");}
if (digitalRead(C2) == 0) {Serial.println("S3");}
if (digitalRead(C3) == 0) {Serial.println("S2");}
if (digitalRead(C4) == 0) {Serial.println("S1");}
digitalWrite(R4, 1);
}
