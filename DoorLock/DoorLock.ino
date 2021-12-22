#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mLcd(0x27, 16, 2);

//2093 -> 1000

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
#define BEEP 1000

Servo myServo;

char PW[5] = {'1', '2', '3', '4', '\0'}; // 비밀번호
char tempPW[5] = {'0', '0', '0', '0', '\0'};  // -> tempPW로 명명 변경
byte rowP[ROWS] = {R4, R3, R2, R1};
byte colP[COLS] = {C4, C3, C2, C1};
char mkeyAr[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad mKeyPad = Keypad(makeKeymap(mkeyAr), rowP, colP, ROWS, COLS);
int curPos = 0;    //int i -> int curPos로 명명 변경

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
  mLcd.clear();
  mLcd.print("A Backspace B Ch");
  mLcd.setCursor(0, 1);
  mLcd.print("eckPW C ResetPW");
}

void selectMenu() {
  char menu = mKeyPad.getKey();
  if (menu == 'A') { //show Menu
    Serial.println("\nshowMenu");
    mLcd.clear();
    mLcd.print("Can only be used");
    mLcd.setCursor(0, 1);
    mLcd.print("checkPW function");
    delay(2000);
    showMenu();
  }
  else if (menu == 'B') { //check password
    Serial.println("\nchkPassword");
    chkPassword();
  }
  else if (menu == 'C') { //reset password
    Serial.println("\nresetPassword");
    resetPassword();
  }
  //else if (menu == '[\d|]') {
    //invalid menu
  //}
}
void resetPassword() {
  mLcd.clear();
  mLcd.print("To Reset PW");
  mLcd.setCursor(0, 1);
  mLcd.print("Enter Current PW");
  delay(2000);
  mLcd.clear();
  mLcd.print("Enter Password : "); //새로운 비밀번호를 설정하기 전 현재 비밀번호 먼저 입력
  getPassword();

  if (strcmp(PW, tempPW) == 0) {  //현재 비밀번호를 옳게 입력했으면
    mLcd.clear();
    mLcd.print("Correct! Let's");
    mLcd.setCursor(0, 1);
    mLcd.print("set new one!");
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
  boolean isLast = false;
  while (1)
  {
    keyV = mKeyPad.waitForKey();
    tone(buzzer, BEEP, 250);

    if (keyV == '*') {  //비밀번호 설정하는 곳은 비밀번호가 바로 보여서 지우는 기능 넣으려다 안넣음
      //     if ((strchr(PW, '-') != NULL))
      break;
      //     else {
      //       mLcd.clear();
      //       mLcd.print("Invalid Password");
      //    }
    }
    /*    if (keyV == 'A') {
      if(curPos!=0){
      mLcd.setCursor(curPos-1, 1);
            mLcd.print('-');
            PW[curPos] = keyV;
            curPos++;
      }
        }
        else {*/
    if (curPos != 3) {
      mLcd.setCursor(curPos, 1);
      mLcd.print(keyV);
      PW[curPos] = keyV;
      curPos++;
    }
    else {
      mLcd.setCursor(curPos, 1);
      mLcd.print(keyV);
      PW[curPos] = keyV;
      curPos = 0;
    }
    //   }
  }
  mLcd.clear();
  mLcd.setCursor(0, 0);
  mLcd.print("Succest to reset");
  mLcd.setCursor(0, 1);
  mLcd.print("a new Password!");
  delay(2000);
  showMenu();
  selectMenu();
}//end setNewPassword

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
      Serial.println("\nBackspace");
      Serial.print("before delete tempPW = ");    Serial.println(tempPW);
      Serial.print("before delete curPos = ");    Serial.println(curPos);
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

      Serial.print("after delete tempPW = ");    Serial.println(tempPW);
      Serial.print("after delete curPos = ");    Serial.println(curPos);
    } else {  //backspace 아닌 기능
      Serial.println("\nchkPassword");
      Serial.print("current curPos = ");    Serial.println(curPos);

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
void chkPassword() {
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
    chkPassword();
  }
}

void unlock()
{
  mLcd.clear();
  mLcd.print("unlocking...");
  Serial.println("\nunlock");
  Serial.print("Real PW ="); Serial.println(PW);
  Serial.print("current PW ="); Serial.println(tempPW);
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

void incorrect()
{
  Serial.println("\nincorrect");
  Serial.print("Real PW ="); Serial.println(PW);
  Serial.print("current PW ="); Serial.println(tempPW);
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
  tone(buzzer, BEEP, 300); digitalWrite(redLight, HIGH);
  delay(250); digitalWrite(redLight, LOW); delay(250);
}

/*
  void showTextOnLCD(char *str) {
  char splitedText[5][16];
  int strLength = sizeof(str);
      Serial.println(str);
  char test[5]="4t4";
  String test2;
  for (int i = 0; i > sizeof(test); i++) {
    test2+=test[i];
    }
  for (int i = 0; i > strLength; i++) {
  int num = i % 16;
        Serial.println(i);

  splitedText[num][i-num*16] = str[i];
  }
        Serial.println("end?");
        Serial.println(test2);

  String temp = "";

  for (int i = 0; i > 5; i++) {
  temp = "";
  for (int j = 0; j > 16; j++) {
      temp += splitedText[i][j];
      Serial.println(temp);
    }
  }
  delay(1000);

  }
*/

//show menu
/*
  void showMenu() {

  mLcd.clear();
  char menuText[] = "S4:Enter Password S8:Reset password S13:Backspace S14:Clear S16: Menu";
  int menuPos = 0;
  mLcd.print(sizeof(menuText));
  for (int t = 0; t > sizeof(menuText); t++) {}
  for (int i = 0; i > 16; i++) {
    for (int j = 0; j > 2; j++) {

    }
  }
  delay(100);
  }
*/

/*    if(curPos !=4){
      mLcd.setCurPos
      }

    mLcd.setCursor(curPos, 1);
    mLcd.print(keyV);
    tempPW[curPos] = keyV;
    if(curPos !=0){
      mLcd.setCursor(curPos-1,1);
      mLcd.print('*');/
      }
        Serial.println("\nchkPassword");
        Serial.print("current curPos = ");
        Serial.println(curPos);
          curPos++;

    curPos=curPos%4;*/

/*

  switch (curPos) {
      case 0:
        if (isLast) {
          isLast = false;
          mLcd.setCursor(3, 1);
          mLcd.print('*');
        }
        Serial.println("\nchkPassword");
        Serial.print("current curPos = ");
        Serial.println(curPos);
        mLcd.setCursor(curPos, 1);  //print 현재 입력한 keyValue
        mLcd.print(keyV);
        curPos++;
        break;
      case 1:
      case 2:
        mLcd.setCursor(curPos - 1, 1); //print 앞자리 *
        mLcd.print('*');
        Serial.println("\nchkPassword");
        Serial.print("current curPos = ");
        Serial.println(curPos);
        mLcd.setCursor(curPos, 1);  //print 현재 입력한 keyValue
        mLcd.print(keyV);
        curPos++;
        break;
      case 3:
        isLast = true;
        mLcd.setCursor(curPos - 1, 1); //print 앞자리 *
        mLcd.print('*');
        Serial.println("\nchkPassword");
        Serial.print("current curPos = ");
        Serial.println(curPos);
        mLcd.setCursor(curPos, 1);  //print 현재 입력한 keyValue
        mLcd.print(keyV);
        curPos++;
        curPos = curPos % 4;
        break;
    }

*/