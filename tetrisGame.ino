#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h" // need the library

const int pinSW = A0;
const int pinX = A2;
const int pinY = A1;
int swState;
int lastSwState;
int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;
int menu = 0;
int timer = 0;
int minThreshold = 400;
int maxThreshold = 600;
bool dpState = false;
bool selected = false;

boolean startUp = true;
int startUpTime = 2000;

unsigned int startingDifficultyValue = 1;
unsigned int highScore = 0;
unsigned int LCDBrightness = 128;
unsigned int matrixBrightness = 16;
unsigned int LCDContrast = 90;
int option = 0;
const int LCDContrastPin = 9; // PWN in loc de POTENTIOMETRU
const int LCDBrightnessPin = 10;
const int RS = 2;
const int enable = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

char incomingByte = 0; // for incoming serial data
String inputString = ""; // a String to hold incoming data
bool stringComplete = false;

LedControl lc = LedControl(12, 11, 8, 1);
int matrix[8][8] = {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};
int prevX = 0, prevY = 0, posX = 0, posY = 3;
int winX = 7, winY = 7;
int valX, valY;
boolean movedX = false, movedY = false;
long randNumber;
int lives = 3;
long long blinkTime;
int leftM,centerM,rightM;
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

String piece[3];
int leftBorder,rightBorder;    
boolean moveDelay = true,stopSignal=false,pointsWinned=false;
bool exitScreen1 = false;
bool exitScreen2 = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  pinMode(LCDContrastPin, OUTPUT); // PWN in loc de POTENTIOMETRU
  analogWrite(LCDContrastPin, 90); // PWN in loc de POTENTIOMETRU
  pinMode(LCDBrightnessPin, OUTPUT); // PWN in loc de POTENTIOMETRU
  analogWrite(LCDBrightnessPin, 128); // PWN in loc de POTENTIOMETRU
  Serial.begin(9600);
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
//  lc.setLed(0,posX,posY,true);
  
  initializeGame();  
}

void loop() {
  /*message for startup*/
  if(startUp == true){
    lcd.setCursor(0, 0);
    lcd.print("Tetris Mania");
    lcd.setCursor(0,1);
    lcd.print("Game by Buru");
  }
  if(millis() - timer > startUpTime){
    startUp = false;
  }
  if(startUp == false){
    lcd.setCursor(0, 0);
    lcd.print("Tetris Main Menu");
  
    xValue = analogRead(pinX);   
    menu = updateJoystick(menu);
    swState = digitalRead(pinSW);
  
    if(swState != lastSwState && swState == LOW){
        dpState = !dpState;
        selected = !selected;
        option = 0;
        //Serial.println("pushed");
        delay(10);
      }
      displayLCD(menu);
      delay(100);
      lastSwState = swState; 
  }
}

int updateJoystick(int counter){
      
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
    if(!selected){
        displayMenu(menu);
    }
    if(selected){
        switch(menu){
        case 0:
          displayGame();
          break;
        case 1:
          displayHighscore();
          break;
        case 2:
          delay(100);
          option=0;
          displaySettings(option);
          break;
        case 3:
          delay(100);
          option=0;
          displayAbout(option);
          break;
        }
    }
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  int len = strToWrite.length();
  Serial.println(len);
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  //data[newStrLen] = '\0'; // 
  return String(data);
}

void displayMenu(int opt)
{

  switch(opt){
    case 0: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tetris Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Start Game");
      break;
    case 1: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tetris Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Highscore");
      break;
    case 2: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tetris Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Settings");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tetris Main Menu");
      lcd.setCursor(0,1);
      lcd.print("About");
      break;
    
  }
}

void displayHighscore()
{

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Highscore ");
    lcd.setCursor(0,1);
    String nume = readStringFromEEPROM(1);
    lcd.print(nume);
    lcd.print(": ");
    lcd.print(highScore);
    Serial.println("okay");

}
void displaySettings(int option)
{
  Serial.println("okay");
  while(true)
   {
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
    if(yValue > maxThreshold &&joyMovedY == false){
        if(option > 0){
          option--;
        }else{
          option = 5;
        }
        joyMovedY = true;
      }
      if(yValue <minThreshold &&joyMovedY ==false){
        if(option < 5){
         option++;
        }else{
          option = 0;
        }
        joyMovedY = true;
      }
      if(yValue >= minThreshold && yValue <=maxThreshold ){
        joyMovedY = false;
      }
      switch(option){
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Show name");
          break;
        case 1: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Enter name");
          break;
        case 2: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Set difficulty");
          break;
        case 3: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Set LCD bright");
          break;
        case 4: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Set LCD contrast");
          break;
        case 5: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Set matrix bright");
          break;
          
      
      }
      delay(100);
      if( swState != lastSwState && swState == LOW){
        dpState = !dpState;
//        //selected = !selected;
//        //Serial.println("pushed");
//        delay(10);
        break;
        } 
        //lcd.clear();
        lastSwState = swState;
        //Serial.println(option);
      }
    switch(option){
      case 0:
      while(true){
        delay(100);
        swState = digitalRead(pinSW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your name");
        lcd.setCursor(0,1);
        String nume = readStringFromEEPROM(1);
        Serial.println(nume);
        lcd.print(nume);
        if( swState != lastSwState && swState == LOW){
          //dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
      }
      case 1: 
      while(true)
      {
        swState = digitalRead(pinSW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter your name");
        lcd.setCursor(0,1);
        lcd.print("in serial");
        
        if( swState != lastSwState && swState == LOW){
          while (Serial.available() > 0) {
          // read the incoming byte:
          incomingByte = (char)Serial.read();
          inputString += incomingByte;
          Serial.print("I received: ");
          Serial.println(incomingByte);

          if (incomingByte == '\n') {
            stringComplete = true;
            Serial.println(inputString);
            //writeStringToEEPROM(1,inputString);
            break;
            }
            
          }
          writeStringToEEPROM(1,inputString);
          dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
          }
          delay(100);
          lastSwState = swState;
      }
      break;
      case 2: 
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        if(yValue > maxThreshold &&joyMovedY == false){
          startingDifficultyValue--;
          joyMovedY = true;
        }
        if(yValue <minThreshold &&joyMovedY ==false){
          startingDifficultyValue++;
          joyMovedY = true;
        }
        if(yValue >= minThreshold && yValue <=maxThreshold ){
          joyMovedY = false;
        }
        
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Level ");
         lcd.print(startingDifficultyValue);
         
         if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
      }
      break;
      case 3:
        while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        if(yValue > maxThreshold && LCDBrightness > 0){
          LCDBrightness--;
        }
        if(yValue <minThreshold && LCDBrightness < 255){
          LCDBrightness++;
        }
         analogWrite(LCDBrightnessPin, LCDBrightness);
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Brightness ");
         lcd.print(LCDBrightness);

         if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
         }
        }
    
      case 4:
      while(true){
        delay(100);
       yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        if(yValue > maxThreshold && LCDContrast > 0){
          LCDContrast--;
        }
        if(yValue < minThreshold && LCDContrast < 255){
          LCDContrast++;
        }
         analogWrite(LCDContrastPin, LCDContrast);
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Contrast ");
         lcd.print(LCDContrast);

         
         if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
         }
      }
      case 5:
      while(true){
        delay(100);
        yValue = analogRead(pinY);
        swState = digitalRead(pinSW);
        if(yValue > maxThreshold && matrixBrightness > 0){
          matrixBrightness--;
        }
        if(yValue < minThreshold && matrixBrightness < 16){
          matrixBrightness++;
        }
         lc.setIntensity(0,matrixBrightness);
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Matrix ");
         lcd.print(matrixBrightness);
         for(int i = 0; i < 8;i++){
          for(int j = 0; j < 8; j++){
            lc.setLed(0,i,j,true);
          }
         }
         
         if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          for(int i = 0; i < 8;i++){
          for(int j = 0; j < 8; j++){
            lc.setLed(0,i,j,false);
          }
         }
          break;
         }
      }
    }
  }


void displayAbout(int option)
{
  Serial.println("okay");
  while(true)
   {
    delay(100);
    yValue = analogRead(pinY);
    swState = digitalRead(pinSW);
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
      switch(option){
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("About Name");
          lcd.setCursor(0,1);
          lcd.print("Tetris Mania");
          break;
        case 1: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("About Author");
          lcd.setCursor(0,1);
          lcd.print("@alexandraburu23");
          break;
        case 2: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("About Github");
          lcd.setCursor(0,1);
          lcd.print("bit.ly/3EAxc9p");
          break;
      }
     if( swState != lastSwState && swState == LOW){
          //dpState = !dpState;
          selected = !selected;
          option = 0;
          //Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
        }
        lastSwState = swState;
    }
}


void displayGame()
{
    int newScore = 0;
    lcd.clear();
    int mesaj1 = 0;
    while(mesaj1 == 0)
    { 
    
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Lives:3 Score:0");
      lcd.setCursor(0,1);
      lcd.print("Difficulty:");
      lcd.print(startingDifficultyValue);
      lives = 3;
      newScore = matrixGame();
      swState = digitalRead(pinSW);
      if( swState != lastSwState && swState == LOW){
        dpState = !dpState;
        selected = !selected;
        option = 0;
        Serial.println("pushed");
        delay(10);
        break;
        }  
      }
      if(newScore > highScore){
        highScore = newScore;
      }
      Serial.println("okay");
}

void loseLife(){
  lives--;
  posX=0;
  posY=3;

  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      lc.setLed(0,i,j,false);
  delay(10);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(i==j || i+j == 7)
        lc.setLed(0,i,j,true);
  delay(1000);
  
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      lc.setLed(0,i,j,false);

  lcd.setCursor(0, 0);
  lcd.print("Lives:");
  lcd.print(lives);
}
void winPoints(int score, int lives){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lives:");
  lcd.print(lives);
  lcd.print(" Score:");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Difficulty:");
  lcd.print(startingDifficultyValue);
}
void displayWin(){
  posX=0;
  posY=0;

  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      lc.setLed(0,i,j,false);
  delay(10);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(j>3 && i+j == 9)
        lc.setLed(0,i,j,true);
      else if(j<=3 && i-j == 3 && j!=0)
        lc.setLed(0,i,j,true);
  delay(1000);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      lc.setLed(0,i,j,false);
  delay(10);

}


long randomizePiece() {
  long randNumber = random(7);
  return randNumber;
}

void erasePiece(int prevX, int prevY, String piece[3]){
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      if(piece[i][j]=='1')
         lc.setLed(0, prevX+i, prevY+j, false);
}

void showPiece(int posX, int posY,String piece[3]){
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      {
        if(piece[i][j]=='1')
          lc.setLed(0,posX+i,posY+j,true);
        else
          lc.setLed(0,posX+i,posY+j,false);
      }
}

void assignMatrix(int posX,int posY, String piece[3])
{
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      {
        if(piece[i][j]=='1')
          matrix[posX+i][posY+j]=1;
      }
}
int limitLeft(String piece[3]){
  for(int i=2;i>=0;i--)
    if(piece[i][0]=='1')
      return i;

  return -999;
}

int limitCenter(String piece[3]){
  for(int i=2;i>=0;i--)
    if(piece[i][1]=='1')
      return i;

  return -999;
}

int limitRight(String piece[3]){
  for(int i=2;i>=0;i--)
    if(piece[i][2]=='1')
      return i;

  return -999;
}

int borderRight(String piece[3]){
  for(int j=2;j>=0;j--)
    for(int i=0;i<3;i++)
      if(piece[i][j]=='1')
        return j;

}

int borderLeft(String piece[3]){
  for(int j=0;j<3;j++)
    for(int i=0;i<3;i++)
      if(piece[i][j]=='1')
        return j;

}
void initializeGame(){
  randomSeed(analogRead(0));
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

int matrixGame(){
  int score = 0;
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      lc.setLed(0,i,j,false);
  
  while(true){
  swState = digitalRead(pinSW);
  if(lives > 0){
      valX = analogRead(pinX);
      valY = analogRead(pinY);
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
          if(matrix[i][j] == 1)
            lc.setLed(0,i,j,true); 
      //move piece
      if(valY < minThreshold)
      {
        if(movedY == false && matrix[posX][posY+leftBorder-1] != 1 && posY+leftBorder > 0)
        {
          prevY = posY;
          posY--;
          movedY = true;
        }
      }
      else
      {
        if(valY > maxThreshold)
        {
          if(movedY == false && matrix[posX][posY+rightBorder+1] != 1 && posY+rightBorder < 7)
          {
            prevY = posY;
            posY++;
            movedY = true;
          }
        }
        else movedY = false;
      }
      
      erasePiece(prevX,prevY,piece);
      showPiece(posX,posY,piece);
      
      if(posX!=prevX || posY!=prevY){
          prevX=posX;
          prevY=posY;}

      //making the piece fall
      if(moveDelay){
        blinkTime = millis();
        moveDelay = !moveDelay;
      }
      if(millis() - blinkTime > 1000){
        posX++;
        leftM++;
        centerM++;
        rightM++;
        moveDelay = !moveDelay;
        }
        
      //piece hitting bottom
      if(leftM == 7 || matrix[leftM + 1][posY]==1){
        stopSignal = true;
      }

      if(centerM == 7 || matrix[centerM + 1][posY + 1]==1){
        stopSignal = true; 
      }

      if(rightM == 7 || matrix[rightM + 1][posY + 2]==1){
        stopSignal = true;
      }
      
      if(stopSignal){
        erasePiece(prevX,prevY,piece);
        assignMatrix(posX,posY,piece);
        posX=0;
        posY=3;
        stopSignal=!stopSignal;
        int index = randomizePiece();
        for(int i=0;i<3;i++)
            piece[i]=pieces[index][i];
        leftM = limitLeft(piece);
        centerM = limitCenter(piece);
        rightM = limitRight(piece);
        rightBorder = borderRight(piece);
        leftBorder = borderLeft(piece);
      }
      
      //losing case
      for(int k=0;k<8;k++)
        if(matrix[0][k]==1){
        showPiece(posX,posY,piece);
        loseLife();
        delay(500);
        for(int i=0;i<8;i++)
          for(int j=0;j<8;j++)
            {
              matrix[i][j]=0;
              lc.setLed(0,i,j,false);
            }
        break;
      }
      
      //winning points
      for(int i=0;i<8;i++){
        int cont=0;
        for(int j=0;j<8;j++)
          if(matrix[i][j]==1)
            cont++;
        if(cont==8){
          pointsWinned=true;
          for(int j=0;j<8;j++)
            {lc.setLed(0,i,j,false);
            matrix[i][j]=0;}
          for(int k=i;k>0;k--)
            {for(int j=0;j<8;j++)
                if(k>0){
                  matrix[k][j]=matrix[k-1][j];
                }
                else{
                  matrix[k][j]=0;
                }
            }
           score += 10;
           winPoints(score,lives);
        }
      }

      if(pointsWinned){
        for(int i=0;i<8;i++)
          for(int j=0;j<8;j++)
            if(matrix[i][j] == 1)
              lc.setLed(0,i,j,true);
             else
              lc.setLed(0,i,j,false);
         pointsWinned = !pointsWinned;
      }
      
  }else{

      lc.setLed(0,1,2,true);
      lc.setLed(0,2,2,true);
      lc.setLed(0,3,2,true);
  
      lc.setLed(0,1,5,true);
      lc.setLed(0,2,5,true);
      lc.setLed(0,3,5,true);
  
      lc.setLed(0,6,2,true);
      lc.setLed(0,5,3,true);
      lc.setLed(0,5,4,true);
      lc.setLed(0,6,5,true);

      exitScreen1 = displayExitScreen1(score);
      exitScreen2 = displayExitScreen2(score);

      if(exitScreen2){
        break;
      }
    }
  
//  Serial.print(posX);
//  Serial.print(" ");
//  Serial.print(posY);
//  Serial.print(" ");
//  Serial.print(swState);
//  Serial.print(" ");
//  Serial.println(lastSwState);
//  //Serial.println(bombDelay);    
//  delay(10);
  lastSwState = swState;
  }
  return score;
}

bool displayExitScreen1 (int score){
  while(true){
    delay(100);
    swState = digitalRead(pinSW);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Yey! You won");
    lcd.setCursor(0, 1);
    lcd.print(score);
    lcd.print(" points");
    
    if(swState!=lastSwState && swState == LOW){
      Serial.println("pushed");
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
          lc.setLed(0,i,j,false);
      break;
    }
    lastSwState = swState;
  }
  return true;
}
bool displayExitScreen2(int score){
  while(true){
    delay(100);
    swState = digitalRead(pinSW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.print(score);
    if(score > highScore){
      lcd.setCursor(0, 1);
      lcd.print("New highscore");
    }
    if(swState!=lastSwState && swState == LOW){
      Serial.println("pushed");
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
          lc.setLed(0,i,j,false);
      break;
    }
    lastSwState = swState;
  }
  return true;
}
