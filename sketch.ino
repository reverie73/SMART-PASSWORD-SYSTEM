#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include<Password.h>

#define servoPin 12
Servo servo;
LiquidCrystal_I2C lcd(0x27,16,2);
const byte ROWS =4;
const byte COLS =4;

char keys[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
byte rowPins[ROWS]= {23,19,18,5};
byte colPins[COLS]={17,16,4,2};
bool isDoorLocked = true;
// door starts locked
Password password = Password("1234");

byte maxPasswordLength=6;
byte currentPasswordLength = 0;
Keypad keypad = Keypad (makeKeymap(keys),rowPins,colPins,ROWS,COLS);
void setup(){
  Serial.begin(115200);
  servo.attach(servoPin);
  servo.write(10); //initial position (locked)
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("DOOR LOCK SYSTEM");
  delay(2000);
  lcd.clear();
  }

void loop(){
  lcd.setCursor(3,0);
  isDoorLocked?lcd.print("DOOR LOCKED"): lcd.print("DOOR OPEN");
  char key = keypad.getKey();
  Serial.println(key);
if (key!=NO_KEY){
  delay(100);
  if (key=='#'){
    resetPassword();
  } else if (key== '*'){
    //toggle lock status only if password is correct
    if(password.evaluate()){
      isDoorLocked=!isDoorLocked; //toggle door state
      isDoorLocked?doorLocked():doorOpen();
    }else{
      displayMessage("WRONG PASSWORD","TRY AGAIN");
    }
  } else {
    processNumberKey(key);
  }
}
}
void processNumberKey( char key){
  if(currentPasswordLength < maxPasswordLength){
    lcd.setCursor(currentPasswordLength+5,1);
    lcd.print("*");
    password.append(key);
    currentPasswordLength++;
  }
}
void doorLocked(){
  servo.write(10); //lock door (servo position 10 degrees)
  displayMessage("Password Correct","LOCKED");
}
void doorOpen(){
  servo.write(100); // unlock door (servo position 100 degrees)
  displayMessage("Password Correct","UNLOCKED");
}
void resetPassword(){
  password.reset();
  currentPasswordLength= 0;
  lcd.clear();
  lcd.setCursor(0,0);
}
void displayMessage (const char *line1,const char *line2){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(3,1);
  lcd.print(line2);
  delay(3000);
  lcd.clear();
  }

