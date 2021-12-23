# Tetris-Game-Project

## Game description

Everybody knows (and maybe loves) the game of Tetris. It was created in 1984 by Alexey Pajitnov and it became very popular in the '90s. 
Its popularity made the game one of the classics.

I chose this game for my robotics project because it was one of my favourites growing up and I played it so much in my attempt to kill the boredom in school.

## How to play

Tetris is primarily composed of a field of play in which pieces of different geometric forms, called "tetriminos", descend from the top of the field. 

During this descent, the player can move the pieces laterally and rotate them until they touch the bottom of the field or land on a piece that had been placed before it. The player can neither slow down the falling pieces nor stop them, but can accelerate them. 

The objective of the game is to use the pieces to create as many horizontal lines of blocks as possible. 

When a line is completed, it disappears, and the blocks placed above fall one rank, while also granting the player points. 

If the player cannot make the blocks disappear quickly enough, the field will start to fill, and when the pieces reach the top of the field and prevent the arrival of additional pieces, the game ends.

At the end of each game, the player receives a score based on the number of lines that have been completed.

The game never ends with the player's victory; the player can only complete as many lines as possible before an inevitable loss.

My version of the game grants the player 3 lives at the start of the game and the score is calculated adding all the points won in those 3 lives.

Also, the higher you choose the difficulty, the faster the pieces will fall, but you can win more points this way.

## How to use the menu

When you power-up the board, a starting message will appear for a few seconds and then you can scroll through the menu.

Menu Options: 

* Start Game - will start the game with the settings you selected previously
* Highscore - will display top 3 scores of all times with the names of the players that received them
* Settings - here you have more options 
  - Show name - displays the name of the current player
  - Edit name - modifies the name of the current player - the user is able to choose a 4-character name 
  - Set difficulty - the player can choose the difficulty level for the game (values between 1 and 5)
  - Set LCD bright - the player can change the brightness of the LCD, which will be saved in EEPROM (values between 0 and 255)
  - Set LCD contrast - the player can change the contrast of the LCD, which will be saved in EEPROM (values between 0 and 255)
  - Set matrix light - the player can change the brightness of the LED matrix, which will be saved in EEPROM (values between 1 and 16)
* About - will display the name of the game, the author and a link to this repository


## Components

* 8x8 led matrix
* matrix driver
* joystick
* pasive buzzer
* LCD1602 Module
* 10 uF 50V capacitor
* 104 ceramic capacitor
* connection cables
* Arduino Uno Board
* 1 big breadboard
* a lot of patience (must have!)

## Setup

![setup](https://github.com/alexandraburu23/Tetris-Game-Project/blob/main/setup.jpg)

## Demo

Here you can see the [demo](https://drive.google.com/file/d/1FP7AEnCBzxuFjnsAiDPh-IIQVWkuzQiu/view?usp=drivesdk) .

## About the code

The thought behind the code architecture was to have a division in modules. That's why ```setup()``` and ```loop()``` are short functions.

```setup()``` - pinMode declaration and initializing of the contrast and brightness values, and also initializing the game before playing.

```loop()``` - the first thing that is displayed is the start-up message, after which we can select the menu option (calling the menu-displaying function and using a variable called ```selected```).

```updateJoystick(int counter)``` - this function is updating the ```counter``` value when scrolling through the menu, ```counter``` has values between 0 and 3.

```displayLCD(int menu)``` - this function is the one that helps us display the right thing on the LCD; if ```selected``` is false, then we have the flexibility to scroll through the menu with the help of ```displayMenu()``` to show the right text, if ```selected``` is true, then we entered a sub-category of the menu and we have to display the right one (START-GAME - ```displayGame()```, HIGHSCORE - ```displayHighscore()```, SETTINGS - ```displaySettings()```, ABOUT - ```displayAbout()```).

```displayMenu(int menu)``` - this function contains a switch-case block that helps the display of the right text of the menu option, before selecting a sub-category.

```writeStringToEEPROM(int addrOffset, const String &strToWrite)``` - this function frees the space of the address and saves a new string in that place.

```readStringFromEEPROM(int addrOffset)``` - this function reads the string saved at the address.

```setDifficultyLevel(int startingDifficultyValue)``` - this function changes the speed at which the tetris pieces fall and the amount of points won by the player when he completes a full line, all of these depending on the difficulting level that was set in the Settings menu.

```changeLCD(bool changed)``` - this function is called to clear the LCD screen when the ```changed``` variable is True.

```displayHighscore()``` - this function is used for displaying the Highscore sub-category; a new animation is lit on the LED matrix, the names and scored for top 3 are read from EEPROM and displayed; the joystick scroll is made on Oy axis; when we want to go back to the main menu, we push the joystick button.

```displaySettings()``` - this function is used for displaying the Settings sub-category; 
  - a new animation is lit on the LED matrix;
  - moving the joystick on the Oy axis is scrolling through the Settings option, that are displayed in a switch case block;
  - before displaying the selected option in Settings, the current name of the player is read from EEPROM and edited to leave out any weird characters;
    - Case 0 - Show name - displaying current player name; exiting to the main menu when joystick button is pressed;
    - Case 1 - Edit name - displaying current player name and editing it; Oy joystick movement - scrolling through the name characters left or right; Ox joystick movement - scrolling through the alphabet characters up or down (A-Z), exiting to the main menu when joystick button is pressed;
    - Case 2 - Set difficulty level - Oy joystick movement - modifying the ```startingDifficultyLevel``` with values between 1 and 5; calling the ```setDifficultyLevel``` function to make the game changes; exiting to the main menu when joystick button is pressed;
    - Case 3 - Set LCD bright - Oy joystick movement - modifying ```LCDBrightness``` value (0 - 255); changing the brightness of the LCD with this value; saving this value in EEPROM; diplaying the value on the screen; exiting to the main menu when joystick button is pressed
    - Case 4 - Set LCD contrast - Oy joystick movement - modifying ```LCDContrast``` value (0 - 255); changing the contrast of the LCD with this value; saving this value in EEPROM; diplaying the value on the screen; exiting to the main menu when joystick button is pressed;
    - Case 5 - Set matrix light - Oy joystick movement - modifying ```matrixBrightness``` value (1 - 16); changing the brightness of the LED matrix with this value; saving this value in EEPROM; diplaying the value on the screen; exiting to the main menu when joystick button is pressed.
 
```displayAbout()``` - this function is used for displaying the About sub-category; a new animation is lit on the LED matrix, moving the joystick on the Oy axis is scrolling through the About information; exiting to the main menu when the joystick button is pressed;
  - Case 0 - Game name;
  - Case 1 - Author name;
  - Case 2 - Github link.

```displayGame()``` - this function is used for displaying the Game sub-category; top 3 scores and names are read from EEPROM before the start of the game; no. of lives, score and difficulty level are written on the LCD; the game starts when we call the function ```matrixGame()``` that returns the value of the new score; this value is compared with the top 3 scores and included in the ranking if it is the case.

```loseLife()``` - this function is called when the player loses a life in the game; no. of lives is decreased, the position of the piece is the initial one in the center of the first line, a specific tone is played by the buzzer and on the matrix is drawn a big X symbol.

```winPoints()``` - this function is used to display the values of no. of lives, score and difficulty after a new score has been calculated.

```randomizePiece()``` - this function returns a random number that will represent the piece that will fall next in the game.

```erasePiece(int prevX, int prevY, String piece[3])``` - this function is used to erase the previous position of the piece.

```showPiece(int posX, int posY, String piece[3])``` - this function is used to display the current position of the piece.

```rotatePiece(int posX, int posY, String piece[3])``` - this function is used to rotate the piece counter clockwise.

```assignMatrix()``` - this function is used to assign the piece to the matrix of values that is used later to calculate the scores and limits.

```limitLeft(String piece[3])```, ```limitCenter(String piece[3])```, ```limitRight(String piece[3])``` - these functions calculate the lowest point in the left column, center column and also the right column of the piece; these limits help us determine when the piece has hit the bottom or another piece.

```borderRight(String piece[3])```, ```borderLeft(String piece[3])``` - these functions calculate the point of the piece that is the furthest to the right or to the left, these limits help us determine how much we can move the piece left or right.

```initializeGame()``` - this function is executing the first steps before the fall of the piece, a random piece is chosen and saved from the list, also the limits have been determined.

```matrixGame()``` - this function is, in fact, managing the game; the game can be played as long as there are lives left; the matrix of values is displayed in the LED matrix, in case there have been pieces placed previosly; with the help of the joystick, a piece can be moved left, right or down, in addition of the fact that the pieces fall one position at a certain period of time; when the button of the joystick is pressed, the piece is rotated; when the piece has hit the bottom or the pieces previously placed on the matrix, a ```stopSignal``` value becomes true, which means a new piece is prepared to fall; everytime a piece stops, we are counting if there are fully completed lines; if that's the case, the score increases and the line in question disappeares; there could be multiple lines completed at the same time; a life is lost when the pieces reach the first row; the matrix is erased between rounds; when all the lives have been lost, a new animation is shown on the LED matrix and the game displayes two exit screens.

```displayExitScreen1(int score)``` - this function is used for the first exit screen, that displayes a congratulatory message with the no. of points awarded to the player; the screen is exited when the joystick button is pressed.

```displayExitScreen2(int score)``` - this function is used for the second exit screen, that displayes the score and a message that lets the player know if he beat the first highscore; the screen is exited when the joystick button is pressed.

