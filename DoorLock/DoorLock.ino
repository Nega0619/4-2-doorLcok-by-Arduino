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
#define BEEP 2093     //2093 -> 1000

Servo myServo;

int curPos = 0;
char PW[5] = {'1', '2', '3', '4', '\0'}; 
char tempPW[5] = {'0', '0', '0', '0', '\0'}; 
byte rowP[ROWS] = {R4, R3, R2, R1};
byte colP[COLS] = {C4, C3, C2, C1};
char mkeyAr[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad mKeyPad = Keypad(makeKeymap(mkeyAr), rowP, colP, ROWS, COLS);

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
  mLcd.setCursor(0, 0);
  showMenu();
}

void loop( ) {
  selectMenu();
}

void showMenu() {
  showMsgOnLCD("A Backspace B Ch", "eckPW C ResetPW");
}

void selectMenu() {
  char menu = mKeyPad.getKey();
  if (menu == 'A') { //show Menu
    showMsgOnLCD("Can only be used", "checkPW function");
    delay(2000);
    showMenu();
  }
  else if (menu == 'B') { //check password
    checkPassword();
  }
  else if (menu == 'C') { //reset password
    resetPassword();
  }
}

void resetPassword() {
  showMsgOnLCD("To Reset PW", "Enter Current PW");
  delay(2000);
  mLcd.clear();
  mLcd.print("Enter Password : "); //새로운 비밀번호를 설정하기 전 현재 비밀번호 먼저 입력
  getPassword();

  if (strcmp(PW, tempPW) == 0) {  //현재 비밀번호를 옳게 입력했으면
    showMsgOnLCD("Correct! Let's", "set new one!");
    delay(2000);
    setNewPassword();
  }
  else {
    incorrect();
    resetPassword();
  }
}

void setNewPassword() {
  mLcd.clear();
  mLcd.print("Enter new PW :");
  curPos = 0;
  mLcd.setCursor(curPos, 1);
  char keyV = mKeyPad.getKey();
  boolean isPWsetted[4] = {false, false, false, false};
  while (1)
  {
    keyV = mKeyPad.waitForKey();
    tone(buzzer, BEEP, 250);

    if (keyV == '*') {  //비밀번호 설정하는 곳은 비밀번호가 바로 보여서 지우는 기능 넣으려다 안넣음
      if (isPWsetted[0]&isPWsetted[1]&isPWsetted[2]&isPWsetted[3])      {
        showMsgOnLCD("Succest to reset", "a new Password!");
        delay(2000);
        break;
      }
      else {
        showMsgOnLCD("Invalid PW", "Try Again");
        delay(2000);
        setNewPassword();
        break;
      }
    }
    if (keyV == 'A') {
      showMsgOnLCD("SORRY! Can't use", "A for Password");
      delay(2000);
      setNewPassword();
      break;
    }
    if (curPos != 3 && keyV != 'A') {
      isPWsetted[curPos] = true;
      mLcd.setCursor(curPos, 1);
      mLcd.print(keyV);
      PW[curPos] = keyV;
      curPos++;
    }
    else if (curPos == 3 && keyV != 'A') {
      isPWsetted[curPos] = true;
      mLcd.setCursor(curPos, 1);
      mLcd.print(keyV);
      PW[curPos] = keyV;
      curPos = 0;
    }
  }
  showMenu();
  selectMenu();
}

void getPassword() {
  curPos = 0;
  char keyV = mKeyPad.getKey();
  boolean isLast = false;
  while (1)
  {
    keyV = mKeyPad.waitForKey();
    tone(buzzer, BEEP, 250);

    if (keyV == '*') {
      break;
    }
    if (keyV == 'A') {
      switch (curPos) {
        case 0:
          if (isLast) { //마지막 비밀번호 자리에서 backspace사용시
            isLast = false;
            mLcd.setCursor(3, 1);
            mLcd.print('-');
            tempPW[3] = '-';
            mLcd.setCursor(2, 1);
            mLcd.print(tempPW[2]);
            curPos = 3;
          }
          else {
            mLcd.setCursor(0, 1);
            curPos = 0;
          }
          break;
        case 1:
          mLcd.setCursor(0, 1);
          mLcd.print('-');
          tempPW[0] = '-';
          curPos = 0;
          break;
        case 2:
        case 3:
          mLcd.setCursor(--curPos, 1);
          mLcd.print('-');
          tempPW[curPos] = '-';
          mLcd.setCursor(--curPos, 1);
          mLcd.print(tempPW[curPos]);
          curPos = ++curPos;
          break;
      }
    } else {  //backspace 아닌 기능
      mLcd.setCursor(curPos, 1);  //print 현재 입력한 keyValue
      mLcd.print(keyV);

      tempPW[curPos] = keyV;  //tempPW에 keyValue저장

      switch (curPos) {
        case 0:
          if (isLast) {
            isLast = false;
            mLcd.setCursor(3, 1);
            mLcd.print('*');
          }
          break;
        case 1:
        case 2:
          mLcd.setCursor(curPos - 1, 1); //앞자리 *로 표시
          mLcd.print('*');
          break;
        case 3:
          isLast = true;
          mLcd.setCursor(curPos - 1, 1); //앞자리 *로 표시
          mLcd.print('*');
          break;
      }
      curPos++;
      curPos = curPos % 4;
    }
  }
}

void checkPassword() {
  mLcd.clear();
  mLcd.print("Enter Password : ");
  getPassword();
  if (strcmp(PW, tempPW) == 0) {
    unlock();
    showMenu();
    selectMenu();
  }
  else {
    incorrect();
    checkPassword();
  }
}

void unlock(){
  mLcd.clear();
  mLcd.print("unlocking...");
  digitalWrite(greenLight, HIGH);
  tone(buzzer, 262, 100); delay(100);
  tone(buzzer, 330, 100); delay(100);
  tone(buzzer, 392, 100);
  myServo.write(1650);
  delay(3000);

  mLcd.clear();
  mLcd.print("locking...");
  digitalWrite(greenLight, LOW);
  tone(buzzer, 392, 100); delay(100);
  tone(buzzer, 330, 100); delay(100);
  tone(buzzer, 262, 100);
  myServo.write(700);
  delay(1000);
}

void incorrect(){
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
}

void showMsgOnLCD(String str1, String str2) {
  mLcd.clear();
  mLcd.print(str1);
  mLcd.setCursor(0, 1);
  mLcd.print(str2);
}
