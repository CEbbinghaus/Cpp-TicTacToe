#pragma once
#include<string>
#include <map>
#include <vector>

#include "Scaffold.h"

using namespace std;

class Game;

//Game Rendere respnible for printing the field to console
class GameRender : BaseStructure{
public:
	//Differemt Tile kinds
	map<string, char> tiles =
	{
		{"UDL", 202},  //T-Intersection Left -|
		{"UDR", 203},  //T-Intersection Right |-
		{"ULR", 185},  //T-Intersection Top _|_
		{"DLR", 204},  //T-Intersection Bottom ⁻|⁻
		{"LR", 186},  //Straight Vertical |
		{"UD", 205},  //Straight Horizontal _
		{"DL", 200}, //LCorner Bottom Left ⁻|
		{"UL", 188}, //LCorner Top Left _|
		{"UR", 187}, //LCorner Top Right |_
		{"DR", 201}, //LCorner Bottom Right |⁻
		{"UDLR", 206}    //X Intersection -|-
	};

	// 2D vector of chars representing the rendering field
	vector<vector<char>> renderingField;

	//instance of the game class
	Game* game;

	//constructor
	GameRender(Game *a_game) {
		game = a_game;
	};

	//Starts the Rendering Process
	void start();

	//Initializes all Values
	void init();

	//Draws the Array to screen
	void draw();

	//Joins an array with a spacer character
	string joinArray(vector<char> arr, char spacer = 0);

	//Writes the Playing field to Console
	string playingFieldText(string prePendString);

	//Gets the corresponding tile to a certain x, y position
	string getPositionString(int size, int x, int y);
};