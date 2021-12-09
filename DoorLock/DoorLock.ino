#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mLcd(0x27, 16, 2);

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
#define ROWS  4
#define COLS  4

Servo myServo;

char PW[5] = {'1','2','3','4','\0'}; // 비밀번호
byte rowP[ROWS] = {R4,R3,R2,R1};
byte colP[COLS] = {C4,C3,C2,C1};
char mkeyAr[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}};
Keypad mKeyPad = Keypad(makeKeymap(mkeyAr),rowP, colP,ROWS,COLS);

void setup( ) {
Serial.begin(9600);
myServo.attach(SERVO);
myServo.write(700);
pinMode(greenLight, OUTPUT);
pinMode(redLight, OUTPUT);
digitalWrite(greenLight, LOW);
digitalWrite(redLight, LOW);
mLcd.init();
mLcd.backlight();
mLcd.print("----");
mLcd.setCursor(0, 0);
}

void loop( ) {
  char keyV = mKeyPad.getKey();
  char pwAr[5] = {'0', '0', '0', '0', '\0'};
  int i = 0;
  while(1)
  {
    keyV = mKeyPad.waitForKey();
    if(keyV == '*')
      break;
    tone(buzzer, 2093, 250);
    if(i != 0)
    {
      mLcd.setCursor(i-1, 0);
      mLcd.print('*');
    }
    mLcd.setCursor(i, 0);
    mLcd.print(keyV);
    pwAr[i] = keyV;
    i++;
  }
  if(strcmp(PW, pwAr)==0)
  {
    unlock();
  }
  else
  {
    incorrect();
  }
}

void unlock()
{
  digitalWrite(greenLight, HIGH);
  tone(buzzer, 262, 100); delay(100);
  tone(buzzer, 330, 100); delay(100);
  tone(buzzer, 392, 100);
  myServo.write(1650);
  mLcd.clear();
  delay(3000);
  digitalWrite(greenLight, LOW);
  tone(buzzer, 392, 100); delay(100);
  tone(buzzer, 330, 100); delay(100);
  tone(buzzer, 262, 100);
  myServo.write(700);
  mLcd.print("----");
  mLcd.setCursor(0, 0);
  delay(1000);
}

void incorrect()
{
  tone(buzzer, 2093, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, 2093, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, 2093, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  mLcd.clear();
  mLcd.print("----");
  mLcd.setCursor(0, 0);
}
