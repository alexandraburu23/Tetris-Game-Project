#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h" // need the library

const int pinSW = A0;
const int pinX = A2;
const int pinY = A1;

bool swState;
bool lastSwState;
int xValue = 0;
int yValue = 0;
bool joyMovedX = false;
bool joyMovedY = false;

int menu = 0;
int timer = 0;
int minThreshold = 400;
int maxThreshold = 600;
bool selected = false;

boolean startUp = true;
int startUpTime = 2000;

const int buzzer = 13;

unsigned int startingDifficultyValue = 1;
unsigned int highScore = 0;
unsigned int LCDBrightness = 128;
unsigned int matrixBrightness = 16;
unsigned int LCDContrast = 90;
unsigned int chIndex = 0;
unsigned int character = 65;
int option = 0;
const int LCDContrastPin = 9; // PWN in loc de POTENTIOMETRU
const int LCDBrightnessPin = 10;
const int RS = 2;
const int enable = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
String playerName = "USER";
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(12, 11, 8, 1);
int matrix[8][8] = {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};
int prevX = 0;
int prevY = 0;
int posX = 0;
int posY = 3;
int winX = 7;
int winY = 7;
bool movedX = false, movedY = false;
long randNumber;
int lives = 3;
long long blinkTime;
unsigned int dropPieceDelay = 2000;
unsigned int scorePoints = 10;
int leftM,centerM,rightM;
bool moveDelay = true;
bool stopSignal = false;
bool pointsWinned = false;
bool exitScreen1 = false;
bool exitScreen2 = false;
String player1 = "", player2 = "", player3 = "";
int highScore1=0,highScore2=0,highScore3=0;
int lastOption = 0;
bool changed = false;
String pieces[7][3] = {
  {"011",
   "110",
   "000"},
   
  {"110",
   "011",
   "000"},
   
  {"001",
   "111",
   "000"},
   
  {"100",
   "111",
   "000"},
   
  {"110",
   "110",
   "000"},
   
  {"010",
   "111",
   "000"},
   
  {"010",
   "010",
   "010"}
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

String piece[3];
int leftBorder,rightBorder;    


void setup(){
  // put your setup code here, to run once:
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  /* PWN in loc de POTENTIOMETRU */
  pinMode(LCDContrastPin, OUTPUT); 
  /* PWN in loc de POTENTIOMETRU */
  pinMode(LCDBrightnessPin, OUTPUT); 
  pinMode(buzzer,OUTPUT);  
  Serial.begin(9600);
  /* turn off power saving, enables display */
  lc.shutdown(0, false); 
  /* initializing LCD and matrix settings with the values from EEPROM */
  LCDBrightness = readStringFromEEPROM(100).toInt();
  LCDContrast = readStringFromEEPROM(200).toInt();
  matrixBrightness = readStringFromEEPROM(300).toInt();
  analogWrite(LCDBrightnessPin, LCDBrightness);
  analogWrite(LCDContrastPin, LCDContrast);
  lc.setIntensity(0,matrixBrightness);
  /* initializing the game before playing */
  initializeGame();  
}

void loop() {
  /* message for startup */
  if(startUp == true){
    lcd.setCursor(0, 0);
    lcd.print("Tetris Mania");
    lcd.setCursor(0,1);
    lcd.print("Game by Buru");
    tone(buzzer,261); 
  }
  /* if startUpTime is up, then we can enter in the menu */
  if(millis() - timer > startUpTime){
    startUp = false;
    lcd.clear();
  }
  /*  menu scroll - depending on the joystick value,
   *  a menu option is displayed and we can select it 
   *  or scroll to another
   */
  if(startUp == false){
    noTone(buzzer); 
    lcd.setCursor(0, 0);
    lcd.print("Tetris Main Menu");
    xValue = analogRead(pinX);   
    menu = updateJoystick(menu);
    swState = digitalRead(pinSW);
  /*  when we want to select a category of the menu 
   *  we push the joystick button, modifying the selected value 
   */
    if(swState != lastSwState && swState == LOW){           
        selected = !selected;
        option = 0;
        delay(10);
      }
    /* we are calling the menu-displaying function */
    displayLCD(menu);
    lastSwState = swState; 
  }
  delay(100);
}
/* function for incrementing of the option value */
int updateJoystick(int counter){
  /* when the joystick is moved on the Ox axis the menu option is changing */
  /* selected value needs to be false because we did not select any sub-category of the menu yet */
  if(xValue < minThreshold && joyMovedX == false && !selected){
    if(counter < 3){
      counter++;
    }else{
      counter = 0;
    }
    joyMovedX = true;
  }
  if(xValue > maxThreshold &&joyMovedX == false && !selected){
    if(counter > 0){
      counter--;
    }else{
      counter = 3;
    }
    joyMovedX = true;
  }
  if(xValue >= minThreshold && xValue <= maxThreshold && !selected){
    joyMovedX = false;
  }
  return counter;
}

void displayLCD(int menu){
  /* a sub-category of the menu was not selected yet, we can scroll through them */ 
  if(!selected){
    displayMenu(menu);
  }
  /* a sub-category of the menu was selected, we have to display the right one */
  if(selected){
    switch(menu){
      case 0:
        displayGame();
        break;
      case 1:
        option = 0;
        displayHighscore();
        break;
      case 2:
        option=0;
        displaySettings(option);
        break;
      case 3:
        option=0;
        displayAbout(option);
        break;
    }
  }
  /* if the option changes, then the LCD has to be cleared */
  if(lastOption != menu){
    changed = !changed;
    changeLCD(changed);
    lastOption = menu;
  }
}

/* writing values in EEPROM function */
void writeStringToEEPROM(int addrOffset, const String &strToWrite){
  int len = strToWrite.length();
  /* erasing the value stored at the address before */
  for (int i = addrOffset; i < addrOffset + len; i++) {
    EEPROM.write(i, 0);
  }
  /* writing in EEPROM */
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++){
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

/* reading values from EEPROM function */
String readStringFromEEPROM(int addrOffset){
  /* reading the length of the message from EEPROM */
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen];
  /* reading the string from EEPROM */
  for (int i = 0; i < newStrLen; i++){
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  return String(data);
}

/* function for setting difficulty level */
void setDifficulty (int startingDifficultyValue){
  /* increased difficulty determines an increased speed at which tetris pieces fall */
  dropPieceDelay = 1500 - (startingDifficultyValue - 1) * 250;
  /* increased difficulty determines an increased score that the player wins when he completes a full line*/ 
  scorePoints = 10 * startingDifficultyValue;
}

/* function for clearing the display */
void changeLCD(bool changed){
  if(changed){
    lcd.clear();
    changed=!changed;
  }
}

/* function for displaying the menu options */
void displayMenu(int menu){
  switch(menu){
    case 0: 
      lcd.setCursor(0,1);
      lcd.print("Start Game");
      break;
    case 1: 
      lcd.setCursor(0,1);
      lcd.print("Highscore");
      break;
    case 2: 
      lcd.setCursor(0,1);
      lcd.print("Settings");
      break;
    case 3:
      lcd.setCursor(0,1);
      lcd.print("About");
      break;
  }
}
/* function for Highscore sub-category */
void displayHighscore(){
  /* clearing the matrix for a new animation */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
  /* drawing the animation for highscore on the matrix */
  lc.setLed(0,0,0,true);
  lc.setLed(0,0,1,true);
  lc.setLed(0,0,2,true);
  lc.setLed(0,0,3,true);
  lc.setLed(0,0,4,true);
  lc.setLed(0,0,5,true);
  lc.setLed(0,0,6,true);
  lc.setLed(0,0,7,true);
  lc.setLed(0,1,0,true);
  lc.setLed(0,1,7,true);
  lc.setLed(0,2,1,true);
  lc.setLed(0,2,6,true);
  lc.setLed(0,3,2,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,3,4,true);
  lc.setLed(0,3,5,true);
  lc.setLed(0,4,3,true);
  lc.setLed(0,4,4,true);
  lc.setLed(0,5,3,true);
  lc.setLed(0,5,4,true);
  lc.setLed(0,6,2,true);
  lc.setLed(0,6,3,true);
  lc.setLed(0,6,4,true);  
  lc.setLed(0,6,5,true);
  lc.setLed(0,7,2,true);
  lc.setLed(0,7,3,true);
  lc.setLed(0,7,4,true);  
  lc.setLed(0,7,5,true);
  String nume="";
  /* reading the no. 1 player */
  nume = readStringFromEEPROM(10);
  /* editing the name in order to avoid showing weird characters */
  nume = nume.substring(0,4);
  player1.remove(0,4);
  player1 += nume;
  /* reading the no.1 score */
  highScore1 = readStringFromEEPROM(20).toInt();
  /* reading the no. 2 player */
  nume = readStringFromEEPROM(30);
  /* editing the name in order to avoid showing weird characters */
  nume = nume.substring(0,4);
  player2.remove(0,4);
  player2 += nume;
  /* reading the no.2 score */
  highScore2 = readStringFromEEPROM(40).toInt();
  /* reading the no. 3 player */
  nume = readStringFromEEPROM(50);
  /* editing the name in order to avoid showing weird characters */
  nume = nume.substring(0,4);
  player3.remove(0,4);
  player3 += nume;
  /* reading the no.3 score */
  highScore3 = readStringFromEEPROM(60).toInt();
  /* displaying the right score when scrolling on the Oy axis */
  while(true){
    delay(100);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
    /* modifying the option when scrolling with the joystick */
    if(yValue > maxThreshold && joyMovedY == false){
      if(option < 2){
        option++;
      }else{
        option = 0;
      }
      joyMovedY = true;
    }
    if(yValue < minThreshold && joyMovedY == false){
      if(option > 0){
       option--;
      }else{
        option = 2;
      }
      joyMovedY = true;
    }
    if(yValue >= minThreshold && yValue <= maxThreshold){
      joyMovedY = false;
    }
    /* clearing the display */
    if(lastOption != option){
      lcd.clear();
    }
    /* switch-case for displaying the right score from top 3*/
    switch(option){
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Highscore ");
        lcd.setCursor(0,1);
        lcd.print("1.");
        lcd.print(player1);
        lcd.print(": ");
        lcd.print(highScore1);
        break;
      case 1: 
        lcd.setCursor(0, 0);
        lcd.print("Highscore ");
        lcd.setCursor(0,1);
        lcd.print("2.");
        lcd.print(player2);
        lcd.print(": ");
        lcd.print(highScore2);
        break;
      case 2: 
        lcd.setCursor(0, 0);
        lcd.print("Highscore ");
        lcd.setCursor(0,1);
        lcd.print("3.");
        lcd.print(player3);
        lcd.print(": ");
        lcd.print(highScore3);
        break;
    }
    /* switching back to the main menu when pushing the joystick button */
    if(swState != lastSwState && swState == LOW){
      selected = !selected;
      break;
    } 
   lastOption = option;
   lastSwState = swState;
 }
  /* clearing the matrix for a new animation */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
}
/* function for Settings sub-category */
void displaySettings(int option){
  /* clearing the matrix for a new animation */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
  /* drawing a new animation for settings */
  lc.setLed(0,0,3,true);
  lc.setLed(0,1,2,true);
  lc.setLed(0,1,3,true);
  lc.setLed(0,1,4,true);
  lc.setLed(0,2,1,true);
  lc.setLed(0,2,2,true);
  lc.setLed(0,2,3,true);
  lc.setLed(0,3,0,true);
  lc.setLed(0,3,1,true);
  lc.setLed(0,3,2,true);
  lc.setLed(0,4,1,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,4,4,true);
  lc.setLed(0,5,5,true);
  lc.setLed(0,6,6,true);
  /* scrolling through the settings sub-category */
  while(true){
    delay(100);
    /* the option value is changed when we are moving the joystick on the Oy axis */ 
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
    if(yValue > maxThreshold && joyMovedY == false){
      if(option < 5){
        option++;
      }else{
        option = 0;
      }
      joyMovedY = true;
    }
    if(yValue < minThreshold && joyMovedY == false){
      if(option > 0){
       option--;
      }else{
        option = 5;
      }
      joyMovedY = true;
    }
    if(yValue >= minThreshold && yValue <= maxThreshold ){
      joyMovedY = false;
    }
    /* switch case for displaying the right text */
    switch(option){
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Show name");
        break;
      case 1: 
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Enter name");
        break;
      case 2:    
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Set difficulty");
        break;
      case 3:  
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Set LCD bright");
        break;
      case 4: 
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Set LCD contrast");
        break;
      case 5:  
        lcd.setCursor(0, 0);
        lcd.print("Settings Menu");
        lcd.setCursor(0,1);
        lcd.print("Set matrix light");
        break;
    }
    /* when wanting to select a settings option we push the joystick button */
    if(swState != lastSwState && swState == LOW){
      break;
    } 
    lastSwState = swState;
    /* clearing the LCD */
    if(lastOption != option){
      lcd.clear();
    }
    delay(100);
    lastOption = option;
  }
  /* reading current player name from EEPROM */
  String nume="";
  nume = readStringFromEEPROM(0);
  /* editing name to avoid displaying weird characters */
  nume = nume.substring(0,4);
  playerName.remove(0,4);
  playerName += nume;
  lcd.clear();
  /* switch case for showing and executing the right settings option */
  switch(option){
    /* case 0 - Show name - displaying current player name */
    case 0:
      while(true){
        delay(100);
        swState = digitalRead(pinSW);
        /* displaying playerName read from EEPROM */
        lcd.setCursor(0, 0);
        lcd.print("Your name");
        lcd.setCursor(0,1);
        lcd.print(playerName);
        /* exiting the settings menu */
        if( swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
      }
      break;
    /* case 1 -  Edit name - editing current player name and saving it in EEPROM */
    case 1: 
      while(true){
        delay(100);
        xValue = analogRead(pinX);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        /* printing the current name first */
        lcd.setCursor(0, 0);
        lcd.print("Enter your name");
        lcd.setCursor(0, 1);
        lcd.print(playerName);
        /* joystick moving on the Oy axis is scrolling left-right between name characters */
        if(yValue > maxThreshold && chIndex < 3 && joyMovedY == false){
          chIndex++;
          joyMovedY = true;
        }
        if(yValue < minThreshold && chIndex > 0 && joyMovedY == false){
          chIndex--;
          joyMovedY = true;
        }
        if(yValue >= minThreshold && yValue <= maxThreshold){
          joyMovedY = false;
        }
        character = (int)(playerName[chIndex]);
        /* joystick moving on the Ox axis is scrolling up-down between alphabet characters (A-Z) */
        if(xValue > maxThreshold && character > 65 && joyMovedX == false){
          lcd.clear();
          character--;
          joyMovedX = true;
        }
        if(xValue < minThreshold && character < 90 && joyMovedX == false){
          lcd.clear();
          character++;
          joyMovedX = true;
        }
        if(xValue >= minThreshold && xValue <= maxThreshold){
          joyMovedX = false;
        }
        /* displaying the new character in the name */
        playerName[chIndex] = character;
        lcd.setCursor(chIndex, 1);
        lcd.print((char)(character));
        /* when exiting to the main menu we save the new name in EEPROM */
        if(swState != lastSwState && swState == LOW){
          writeStringToEEPROM(0,playerName);
          selected = !selected;
          option = 0;
          break;
          }
          delay(100);
          lastSwState = swState;
       }
     break;
    /* case 2 - setting difficulty level */
    case 2: 
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        /* moving the joystick on the Oy axis is increasing or decreasing startingDifficultyLevel */
        if(yValue > maxThreshold && joyMovedY == false && startingDifficultyValue < 5){
          startingDifficultyValue++;
          joyMovedY = true;
        }
        if(yValue < minThreshold && joyMovedY == false && startingDifficultyValue > 1){
          startingDifficultyValue--;
          joyMovedY = true;
        }
        if(yValue >= minThreshold && yValue <= maxThreshold ){
          joyMovedY = false;
        }
        /* modifying the game values that are depending on the difficulty level */
        setDifficulty(startingDifficultyValue);
        /* displaying the difficulty level */
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Level ");
        lcd.print(startingDifficultyValue);
        /* exiting to the main menu*/
        if(swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
      }
      break;
    /* case 3 - changing LCD brightness */
    case 3:
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        /* reading previous value from EEPROM */
        LCDBrightness = readStringFromEEPROM(100).toInt();
        /* moving the joystick on the Oy axis is increasing or decreasing the LCDBrightness value (values between 0 and 255) */
        if(yValue > maxThreshold && LCDBrightness < 255){
          LCDBrightness++;
        }
        if(yValue < minThreshold && LCDBrightness > 0){
          LCDBrightness--;
        }
        /* writing the new value in EEPROM */
         writeStringToEEPROM(100, String(LCDBrightness));
        /* setting the new value to the LCD */
         analogWrite(LCDBrightnessPin, LCDBrightness);
        /* displaying the value on the screen */
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Brightness ");
         lcd.print(LCDBrightness);
        /* exiting to the main menu */
         if(swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          break;
       }
       lastSwState = swState;
      }
      break;
    case 4:
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        /* reading previous value from EEPROM */
        LCDContrast = readStringFromEEPROM(200).toInt();
        /* moving the joystick on the Oy axis is increasing or decreasing the LCDContrast value (values between 0 and 255) */
        if(yValue > maxThreshold && LCDContrast < 255){
          LCDContrast++;
        }
        if(yValue < minThreshold && LCDContrast > 0){
          LCDContrast--;
        }
        /* writing the new value in EEPROM */
        writeStringToEEPROM(200, String(LCDContrast));
        /* setting the new value to the LCD */
        analogWrite(LCDContrastPin, LCDContrast);
        /* displaying the value on the screen */
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Contrast ");
        lcd.print(LCDContrast);
        /* exiting to the main menu */
        if(swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          break;
         }
         lastSwState = swState;
      }
      break;
    case 5:
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        /* reading previous value from EEPROM */
        matrixBrightness = readStringFromEEPROM(300).toInt();
        /* moving the joystick on the Oy axis is increasing or decreasing the matrixBrightness value (values between 1 and 16) */
        if(yValue > maxThreshold && matrixBrightness < 16){
          matrixBrightness++;
        }
        if(yValue < minThreshold && matrixBrightness > 1){
          matrixBrightness--;
        }
        /* writing the new value in EEPROM */
        writeStringToEEPROM(300,String(matrixBrightness));
        /* setting the new value to the LCD */
        lc.setIntensity(0, matrixBrightness);
        /* displaying the value on the screen */
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Matrix ");
        lcd.print(matrixBrightness);
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            lc.setLed(0, i, j, true);
         }
        }
        /* exiting to the main menu */
        if(swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          for(int i = 0; i < 8;i++){
            for(int j = 0; j < 8; j++){
              lc.setLed(0,i,j,false);
            }
          }
          break;
        }
        lastSwState = swState;
      }
    break;
  }
  /* erasing the animation on the matrix */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
}
/* function for About sub-category */
void displayAbout(int option){
  /* clearing the matrix for a new animation */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
  /* drawing a new animation for about */
  lc.setLed(0,0,2,true);
  lc.setLed(0,0,3,true);
  lc.setLed(0,0,4,true);
  lc.setLed(0,0,5,true);
  lc.setLed(0,1,1,true);
  lc.setLed(0,1,6,true);
  lc.setLed(0,2,0,true);
  lc.setLed(0,2,7,true);
  lc.setLed(0,3,0,true);
  lc.setLed(0,3,7,true);
  lc.setLed(0,4,0,true);
  lc.setLed(0,4,7,true);
  lc.setLed(0,5,0,true);
  lc.setLed(0,5,7,true);
  lc.setLed(0,6,1,true);
  lc.setLed(0,6,6,true);
  lc.setLed(0,7,2,true);
  lc.setLed(0,7,3,true);
  lc.setLed(0,7,4,true);
  lc.setLed(0,7,5,true);
  lc.setLed(0,2,3,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,4,3,true);
  lc.setLed(0,5,3,true);
  lc.setLed(0,2,4,true);
  lc.setLed(0,3,4,true);
  lc.setLed(0,4,4,true);
  lc.setLed(0,5,4,true);

  while(true){
    delay(100);
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
    /* scrolling through the information when moving the joystick on the Oy axis */
    if(yValue > maxThreshold && joyMovedY == false){
      if(option > 0){
       option--;
      }else{
       option = 2;
      }
      joyMovedY = true;
    }
    if(yValue < minThreshold && joyMovedY == false){
      if(option < 2){
       option++;
      }else{
        option = 0;
      }
      joyMovedY = true;
    }
    if(yValue >= minThreshold && yValue <= maxThreshold ){
      joyMovedY = false;
    }
    /* switch case for the option display */
    switch(option){
      /* case 0 - the name of the game */
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("About Name");
        lcd.setCursor(0,1);
        lcd.print("Tetris Mania");
        break;
      /* case 1 - the author of the game */
      case 1:  
        lcd.setCursor(0, 0);
        lcd.print("About Author");
        lcd.setCursor(0,1);
        lcd.print("@alexandraburu23");
        break;
      /* case 2 - the link to the github repository */
      case 2:  
        lcd.setCursor(0, 0);
        lcd.print("About Github");
        lcd.setCursor(0,1);
        lcd.print("bit.ly/3EAxc9p");
        break;
    }
    /* exiting to the main menu */
    if(swState != lastSwState && swState == LOW){
      selected = !selected;
      option = 0;
      delay(10);
      break;
    }
    /* clearing the LCD */
    if(lastOption != option){
      lcd.clear();
    }
    lastOption = option;
    lastSwState = swState;
  }
  /* clearing the animation on the matrix */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
}
/* function for StartGame sub-category */
void displayGame(){
  /* initialize the value of the new score */
  int newScore = 0;
  /* reading the current player name from EEPROM */
  String player0 = readStringFromEEPROM(0);
  /* reading the top 3 score + name from EEPROM */
  String player1 = readStringFromEEPROM(10);
  int highScore1 = readStringFromEEPROM(20).toInt();
  String player2 = readStringFromEEPROM(30);
  int highScore2 = readStringFromEEPROM(40).toInt();
  String player3 = readStringFromEEPROM(50);
  int highScore3 = readStringFromEEPROM(60).toInt();
  /* displaying no. of lives, score and difficulty level on the LCD */
  lcd.clear();
  lcd.createChar(1, heart);
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.print(":3 Score:0");
  lcd.setCursor(0,1);
  lcd.print("Difficulty:");
  lcd.print(startingDifficultyValue);
  lives = 3;
  /* the new score will be returned at the end of the game when the player lost all of his lives */
  newScore = matrixGame();
  swState = digitalRead(pinSW);
  /* rearranging the top 3 scores */
  /* in case the new score is worthy of the first place */
  if(newScore > highScore1){
    /* saving the new values on the first place */
    writeStringToEEPROM(10,player0);
    writeStringToEEPROM(20,String(newScore));
    /* the former first place score is now on the second place */
    writeStringToEEPROM(30,player1);
    writeStringToEEPROM(40,String(highScore1));
    /* the former second place score is now on the third place */
    writeStringToEEPROM(50,player2);
    writeStringToEEPROM(60,String(highScore2));
    highScore3 = highScore2;
    player3 = player2;
    highScore2 = highScore1;
    player2 = player1;
    highScore1 = newScore;
    player1 = player0;
    highScore = newScore;
  }
  /* in case the new score is worthy of the second place */
  else if(newScore > highScore2){
    writeStringToEEPROM(30,player0);
    writeStringToEEPROM(40,String(newScore));
    writeStringToEEPROM(50,player2);
    writeStringToEEPROM(60,String(highScore2));
    highScore3 = highScore2;
    player3 = player2;
    highScore2 = newScore;
    player2 = player0;
  }
  /* in case the new score is worthy of the third place */
  else if(newScore > highScore3){
    highScore3 = newScore;
    player3 = player0;
    writeStringToEEPROM(50,player0);
    writeStringToEEPROM(60,String(newScore));
  }
  selected = !selected;
}
void loseLife(){
  /* specific losing sound */
  tone(buzzer,150); 
  /* no. of lives decreases */
  lives--;
  /* the position of the piece is the initial one */
  posX=0;
  posY=3;
  /* erasing the matrix */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
  delay(10);
  /* showing the big X symbol */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(i == j || i + j == 7){
        lc.setLed(0, i, j, true);
      }
    }
  }
  delay(1000);
  /* erasing the matrix */
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      lc.setLed(0, i, j, false);
    }
  }
  /* changing the no. of lives displayed on the LCD screen */
  lcd.createChar(1, heart);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.print(":");
  lcd.print(lives);
}
void winPoints(int score, int lives){
  /* specific winning sound */
  tone(buzzer,261); 
  /* showing the same value for no. of lives */
  lcd.clear();
  lcd.createChar(1, heart);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.print(":");
  lcd.print(lives);
  /* showing the new value for the score */
  lcd.print(" Score:");
  lcd.print(score);
  lcd.setCursor(0, 1);
  /* showing the same value for difficulty */
  lcd.print("Difficulty:");
  lcd.print(startingDifficultyValue);
}
/* function used to mix the order of the pieces that will fall */
long randomizePiece(){
  long randNumber = random(7);
  return randNumber;
}
/* functions used to erase the previous position of the piece */
void erasePiece(int prevX, int prevY, String piece[3]){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      /* the piece containing '1' means that a LED was lit in that position */
      if(piece[i][j] == '1'){
         lc.setLed(0, prevX + i, prevY + j, false);
      }
    }
  }
}
/* functions used to display the current position of the piece */
void showPiece(int posX, int posY, String piece[3]){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      /* the piece containing '1' means that a LED needs to be lit in that position */
      if(piece[i][j] == '1'){
        lc.setLed(0, posX + i, posY + j, true);
      }else{
        lc.setLed(0, posX + i, posY + j, false);
      }
    }
  }
}
/* function used to rotate the piece counter clockwise */
void rotatePiece(int posX, int posY, String piece[3]){
  int pieceIndex = 0;
  /* we determine which of the pieces from the list is the current one */
  for(int i = 0; i < 7; i++){
    if(pieces[i][0] == piece[0] && pieces[i][1] == piece[1] && pieces[i][2] == piece[2]){
      pieceIndex = i; 
      break;
    }
  }
  /* we are modifying in an additional piece to avoid messing up the original piece */
  String auxPiece[3] = {"000", "000", "000"};
  /* we are going through the values rotating the matrix to the left */
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
        auxPiece[i][j] = piece[2 - j][i];
    }
  }
  /* our piece gets the value of the additional piece */
  piece[0] = auxPiece[0];
  piece[1] = auxPiece[1];
  piece[2] = auxPiece[2];
  /* the rotated piece is saved in the list, replacing the other one */
  pieces[pieceIndex][0] = auxPiece[0];
  pieces[pieceIndex][1] = auxPiece[1];
  pieces[pieceIndex][2] = auxPiece[2];
}
/* this function is used to put the piece in the value-matrix */
void assignMatrix(int posX, int posY, String piece[3]){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(piece[i][j] == '1'){
        matrix[posX + i][posY + j] = 1;
      }
    }
  }
}
/* this function calculates the lowest point in the left column of the piece */
int limitLeft(String piece[3]){
  for(int i = 2; i >= 0; i--){
    if(piece[i][0] == '1'){
      return i;
    }
  }
  return -999;
}
/* this function calculates the lowest point in the center column of the piece */
int limitCenter(String piece[3]){
  for(int i = 2; i >= 0; i--){
    if(piece[i][1] == '1'){
      return i;
    }
  }
  return -999;
}
/* this function calculates the lowest point in the right column of the piece */
int limitRight(String piece[3]){
  for(int i = 2; i >= 0; i--){
    if(piece[i][2] == '1'){
      return i;
    }
  }
  return -999;
}
/* this function calculates the point of the piece that is the furthest to the right */
int borderRight(String piece[3]){
  for(int j = 2; j >= 0; j--){
    for(int i = 0; i < 3; i++){
      if(piece[i][j] == '1'){
        return j;
      }
    }
  }
}
/* this function calculates the point of the piece that is the furthest to the left */
int borderLeft(String piece[3]){
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 3; i++){
      if(piece[i][j] == '1'){
        return j;
      }
    }
  }
}
/* initializing game */
void initializeGame(){
  randomSeed(analogRead(0));
  /* a random piece is chosen */
  int index = randomizePiece();
  /* we copy the piece from the list */
  for(int i = 0; i < 3; i++){
      piece[i] = pieces[index][i];
  }
  /* we call the functions to find out limits left, right and below */
  leftM = limitLeft(piece);
  centerM = limitCenter(piece);
  rightM = limitRight(piece);
  rightBorder = borderRight(piece);
  leftBorder = borderLeft(piece);
}
/* function that in fact is managing the game */
int matrixGame(){
  /* initially the score is 0 and the LED matrix is empty */
  int score = 0;
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      lc.setLed(0, i, j, false);
  
  while(true){
    swState = digitalRead(pinSW);
    noTone(buzzer);
    /* the game is played as long as there are lives left */
    if(lives > 0){
      xValue = analogRead(pinX);
      yValue = analogRead(pinY);
      /* whatever pieces were placed before are lit up */
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          if(matrix[i][j] == 1){
            lc.setLed(0, i, j, true); 
          }
        }
      }
      /* move the piece left or right with the joystick */
      if(yValue < minThreshold){
        if(joyMovedY == false && matrix[posX][posY + leftBorder - 1] != 1 && posY + leftBorder > 0){
          prevY = posY;
          posY--;
          joyMovedY = true;
        }
      }
      else{
        if(yValue > maxThreshold){
          if(joyMovedY == false && matrix[posX][posY + rightBorder + 1] != 1 && posY + rightBorder < 7){
            prevY = posY;
            posY++;
            joyMovedY = true;
          }
        }else{
          joyMovedY = false;
        }
      }
      /* move the piece down with the joystick */
      if(xValue < minThreshold && joyMovedX == false){
        if((leftM < 7 && matrix[leftM + 1][posY] != 1) && 
          (centerM < 7 && matrix[centerM + 1][posY + 1] != 1) && 
          (rightM < 7 && matrix[rightM + 1][posY + 2] != 1)){
            prevX = posX;
            posX++;
            leftM++;
            centerM++;
            rightM++;
            joyMovedX = true;
          }
      }else if(xValue <= maxThreshold && xValue >= minThreshold){
          joyMovedX = false;
      }
      /* rotating the piece */
      if(swState != lastSwState && swState == LOW){
        rotatePiece(prevX, prevY, piece);
        leftM = limitLeft(piece) + posX;
        centerM = limitCenter(piece) + posX;
        rightM = limitRight(piece) + posX;
        rightBorder = borderRight(piece);
        leftBorder = borderLeft(piece);
      }
      /* erasing previous position */
      erasePiece(prevX, prevY, piece);
      /* lighting the current position */
      showPiece(posX, posY, piece);
      /* registering the last position */
      if(posX != prevX || posY != prevY){
        prevX = posX;
        prevY = posY;
      }
      /* making the piece fall */
      if(moveDelay){
        blinkTime = millis();
        moveDelay = !moveDelay;
      }
      /* the piece falls after a time determined by the difficulty level */
      if(millis() - blinkTime > dropPieceDelay){
        posX++;
        leftM++;
        centerM++;
        rightM++;
        moveDelay = !moveDelay;
      } 
      /* piece hitting bottom */
      if(leftM == 7 || matrix[leftM + 1][posY] == 1){
        stopSignal = true;
      }
      if(centerM == 7 || matrix[centerM + 1][posY + 1] == 1){
        stopSignal = true; 
      }
      if(rightM == 7 || matrix[rightM + 1][posY + 2] == 1){
        stopSignal = true;
      }
      /* the piece has stopped at the lowest position posible for it */
      if(stopSignal){
        /* erasing previous position */
        erasePiece(prevX, prevY, piece);
        /* assigning the piece in the matrix of values now that it stopped */
        assignMatrix(posX, posY, piece);
        /* the current position is the initial one */
        posX = 0;
        posY = 3;
        tone(buzzer,300); 
        stopSignal = !stopSignal;
        /* a new piece is prepared */
        int index = randomizePiece();
        for(int i = 0; i < 3; i++){
          piece[i] = pieces[index][i];
        }
        leftM = limitLeft(piece);
        centerM = limitCenter(piece);
        rightM = limitRight(piece);
        rightBorder = borderRight(piece);
        leftBorder = borderLeft(piece);
      }
      /* losing case - when the pieces reach the first row */
      for(int k = 0; k < 8; k++){
        if(matrix[0][k] == 1){
          showPiece(posX, posY, piece);
          loseLife();
          delay(500);
          /* every piece is deleted from the matrix of values starting clean with the new life */
          for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
              matrix[i][j] = 0;
              lc.setLed(0, i, j, false);
            }
          }
          break;
        }
      }
      /* winning points */
      /* we are going through every line in the matrix because multiple lines can be completed at the same time */
      for(int i = 0; i < 8; i++){
        /* counting if the line is full */
        int cont = 0;
        for(int j = 0; j < 8; j++){
          if(matrix[i][j] == 1){
            cont++;
          }
        }
        /* when a line is fully occupied, then the player increases his score and the line dissapears */
        if(cont == 8){
          pointsWinned = true;
          for(int j = 0; j < 8; j++){
            lc.setLed(0, i, j, false);
            matrix[i][j] = 0;
            }
          for(int k = i; k > 0; k--){
            for(int j = 0; j < 8; j++){
              if(k > 0){
                matrix[k][j] = matrix[k-1][j];
              }
              else{
                matrix[k][j] = 0;
              }
            }
          }
         score += scorePoints;
         winPoints(score, lives);
        }
      }
      /* if points have been awarded then we have to light up the matrix again because at least one line has been changed */
      if(pointsWinned){
        for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
            if(matrix[i][j] == 1){
              lc.setLed(0, i, j, true);
            }else{
              lc.setLed(0, i, j, false);
            }
          }
        }
         pointsWinned = !pointsWinned;
      }
  }else{
    /* we are left without lives so the game ended */
    /* a sad face appeared because the game ended */
    lc.setLed(0, 1, 2, true);
    lc.setLed(0, 2, 2, true);
    lc.setLed(0, 3, 2, true);
    lc.setLed(0, 1, 5, true);
    lc.setLed(0, 2, 5, true);
    lc.setLed(0, 3, 5, true);
    lc.setLed(0, 6, 2, true);
    lc.setLed(0, 5, 3, true);
    lc.setLed(0, 5, 4, true);
    lc.setLed(0, 6, 5, true);
    /* we are showing the first exit screen */
    exitScreen1 = displayExitScreen1(score);
    /* we are showing the second exit screen */
    exitScreen2 = displayExitScreen2(score);
    /* after the second exit screen we are back to the main menu*/
    if(exitScreen2){
      break;
    }
  }
  lastSwState = swState;
  }
  return score;
}
/* function for the first exit screen */
bool displayExitScreen1(int score){
  /* displays a congratulatory text with the no. of points awarded to the player */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Yey! You won");
  lcd.setCursor(0, 1);
  lcd.print(score);
  lcd.print(" points");
  while(true){
    swState = digitalRead(pinSW);
    /* escaping the first exit screen when the joystick button is pressed */
    if(swState != lastSwState && swState == LOW){
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          lc.setLed(0, i, j, false);
        }
      }
      break;
    }
    lastSwState = swState;
  }
  return true;
}
/* function for the first exit screen */
bool displayExitScreen2(int score){
  /* displaying the score again */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
  highScore1 = readStringFromEEPROM(20).toInt();
  /* the player is announced if the score is bigger than the first highscore */
  if(score > highScore1){
    lcd.setCursor(0, 1);
    lcd.print("New highscore");
  }
  while(true){
    delay(100);
    swState = digitalRead(pinSW);
    /* escaping the second exit screen when the joystick button is pressed */
    if(swState != lastSwState && swState == LOW){
      for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
          lc.setLed(0, i, j, false);
        }
      }
      break;
    }
    lastSwState = swState;
  }
  return true;
}
