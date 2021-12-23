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

```setup()``` - pinMode declaration and initializing of the contrast and brightness values, and also initializing the game before playing

```loop()``` - the first thing that is displayed is the start-up message, after which we can select the menu option (calling the menu-displaying function and using a variable called ```selected```)

```updateJoystick()``` - this function is updating the ```counter``` value when scrolling through the menu, ```counter``` has values between 0 and 3

```displayLCD()``` - this function is the one that helps us display the right thing on the LCD; if ```selected``` is false, then we have the flexibility to scroll through the menu with the help of ```displayMenu()``` to show the right text, if ```selected``` is true, then we entered a sub-category of the menu and we have to display the right one (START-GAME - ```displayGame()```, HIGHSCORE - ```displayHighscore()```, SETTINGS - ```displaySettings()```, ABOUT - ```displayAbout()```)
