#pragma once
#include <map>
#include <vector>
#include "Scaffold.h"
#include "Settings.h"
#include "Rendering.h"
#include "util.h"

using namespace std;
class GameRenderer;

///Game
//Main Game class running the Game
class Game: BaseStructure
{
	//Possible Field States
	enum state
	{
		cross = 'X',
		circle = 'O',
		undefined = ' '
	};

	//Output Messages. Used for easy access to long messages
	map<string, string> messages =
	{
		{"input", " Select a Field"},
		{"taken", "This Field Has Already Been Picked Please Pick Another One"}
	};

	//Names of the Players. Used to customize names
	map<int, char*> playerTitles;
	void Deconstruct();


	int oldFieldSize = 0;
public:
	//Size of the Grid. n x n size
	int* gridSize;

	//max Grid Size
	int maxGridSize;

	//the amount of neighbouring fields you need to win
	int* winSize;

	//the 2d playing field array
	int** playingField = nullptr;

	//the first player to play
	state currentPlayer = circle;

	//the message to print after the field
	string outputMessage;

	//the Key Handler that takes the input and translates it to input
	KeyHandler handler;

	//the Rendering engine that prints the field to screen
	GameRender* render;

	//Settings class storing all permanent data/settings
	Settings* settings;

	//Location of the Curson on the playing field
	map<char, int> cursor =
	{
		{'x', 0},
		{'y', 0}
	};

	//Game constructor requirng a settings argument
	Game(Settings* options);

	//Game Deconstructor. Calls Deconstruct
	~Game();

	//Deletes and creates a new playing field with the current sizes
	void genPlayingField();

	//empties the playing field and sets all values to 0
	void clearPlayingField();

	//Starts the game and sets all processes in motion
	void start();

	//checks if the game has ended through a draw or a winner
	bool hasEnded();

	//advances the game and makes sure the cursor stays within the playing field
	void tick();

	//initializes important values that must be current with th gridSize
	void init();

	//Update checks is the game has ended and draws the updates
	void update();

	//Loops over the array elements checking if a player has won
	bool isWinner(); 

	//checks if field has a empty spot or not
	bool isFilled();

	//Draws the Field to Console using the Render
	void draw();
};