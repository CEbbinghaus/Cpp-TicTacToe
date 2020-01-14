#pragma once
#include"Rendering.h"
#include "Game.h"

using namespace std;

//Empty Start function
void GameRender::start() {}

//Initializes the Rendere. Resizes Vector and sets values
void GameRender::init(){
	renderingField.clear();
 	int maxlength = (*game).maxGridSize;
	if (maxlength <= 0)return;
	while (renderingField.size() < maxlength) {
		vector<char> tmp;
		for (int i = 0; i < maxlength; i++) {
			tmp.push_back(0);
		}
		renderingField.push_back(tmp);
	}
}

//Draws the playing field to Screen
string GameRender::playingFieldText(string prePendString) {
	std::string log = "";
	int boxSize = 3;
	int maxSize = (*game).maxGridSize;

	//Loops over every "theoretical" point in the rendering field and decides if its a wall, seperator or Center point. Replaces Draw field with character accordingly
	for (int x = 0; x < maxSize; x++) {
		for (int y = 0; y < maxSize; y++) {
			renderingField[x][y] = ' ';//254
			bool isWall = (x == 0 || x == maxSize - 1 || y == 0 || y == maxSize - 1);
			bool isSeperator = isWall ? false : (x % (1 + boxSize) == 0 || y % (1 + boxSize) == 0);
			bool isCenter = isWall ? false : ((2 + x) % (1 + boxSize) == 0 && (2 + y) % (1 + boxSize) == 0);
			if (isWall || isSeperator) {
				renderingField[x][y] = 'W';
				continue;
			}
			if (isCenter) {
				int fx = (floor((x + 2) / (float)(boxSize + 1)) - 1);
				int fy = (floor((y + 2) / (float)(boxSize + 1)) - 1);

				char tile = (*game).playingField[fx][fy];
				
				int cx = (*game).cursor['x'];
				int cy = (*game).cursor['y'];
				bool isCursor = (cx == fx && cy == fy);

				bool isTile = !(tile == ' ');
				renderingField[x][y] = isTile ? (isCursor ? 176 : tile) : (isCursor ? 219 : ' ');
			}
		}
	}
	
	//Creates a Copy of the Rendering field to draw to console with proper character replacements
	vector<vector<char>> drawField(renderingField);
	for (int x = 0; x < maxSize; x++) {
		for (int y = 0; y < maxSize; y++) {
			string tile = getPositionString(maxSize, x, y);
			drawField[x][y] = (tile != "") ? tiles[tile] : renderingField[x][y];
			//cout << tile << endl;
		}
		bool isSeperator = (x % (1 + boxSize) == 0);
		log += prePendString + joinArray(drawField[x], isSeperator ? tiles["UD"] : ' ') + "\n";
	}
	return log;
}

//Draws the Entire Field
void GameRender::draw() {
	cls();
	string tabs = "		";
	cout << endl << endl << endl;
	cout << playingFieldText(tabs) << endl << endl;
	cout << tabs + (*game).outputMessage << endl;
}

//Joins an array seperated with a char and returns it as a string
string GameRender::joinArray(vector<char> arr, char spacer) {
	string result = "";
	for (int i : arr) {
		result += i;
		result += spacer;
	}
	return result.substr(0, result.length() - 1);
}

//Checks the Walls around a given Field and decides on a Character to insert 
string GameRender::getPositionString(int size, int x, int y) {
	string positionString = "";
	if (renderingField[x][y] != 'W')return positionString;
	if (!(y == 0) && renderingField[x][y - 1] == 'W')positionString += "U";
	if (!(y == size - 1) && renderingField[x][y + 1] == 'W')positionString += "D";
	if (!(x == 0) && renderingField[x - 1][y] == 'W')positionString += "L";
	if (!(x == size - 1) && renderingField[x + 1][y] == 'W')positionString += "R";
	return positionString;
}