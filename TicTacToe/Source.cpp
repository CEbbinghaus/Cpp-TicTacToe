#include <iostream>
#include <string>
#include <cmath>
#include <map>

using namespace std;

#define SIZE 3
#define MAX_SIZE (SIZE * SIZE) + (SIZE + 1)

int playField[SIZE][SIZE];
char renderingField[MAX_SIZE][MAX_SIZE];


enum state {
	cross = 'X',
	circle = 'O',
	undefined = ' '
};
int currentPlayer = circle;

string Message = "";

string joinArray(char arr[], int length, char spacer = 0) {
	int size = length;
	string result = "";
	for (int i = 0; i < size; i++) {
		result += arr[i];
		if(i != size - 1)
			result += spacer;
	}
	return result;
}
map<string, string> messages = {
	{"str_error", "Sorry That input was Invalid. Try Again"},
	{"input", " Please Input a Number Corresponding Number to a field"},
	{"taken", "This Field Has Already Been Picked Please Pick Another One"}
};
map<int, char> stateIdentifiers = {
	{circle, 'O'},
	{cross, 'X'},
	{undefined, 0}
};
map<int, string> playerTitles = {
	{circle, "Player One"},
	{cross, "Player Two"}
};
map<string, char> tiles = {
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

void clearPlayingField() {
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			playField[x][y] = undefined;
		}
	}
}
string getPositionString(int size, int x, int y) {
	string positionString = "";
	if (renderingField[x][y] != 'W')return positionString;
	if (!(y == 0) && renderingField[x][y - 1] == 'W')positionString += "U";
	if (!(y == size - 1) && renderingField[x][y + 1] == 'W')positionString += "D";
	if (!(x == 0) && renderingField[x - 1][y] == 'W')positionString += "L";
	if (!(x == size - 1) && renderingField[x + 1][y] == 'W')positionString += "R";
	return positionString;
}
string playingFieldText(string prePendString) {
	system("CLS");
	std::string log = "";
	int localSize = SIZE;
	int max = MAX_SIZE;
	for(int x = 0; x < max; x++){
		for(int y = 0; y < max; y++){
			renderingField[x][y] = ' ';//254
			bool isWall = (x == 0 || x == max - 1 || y == 0 || y == max - 1);
			bool isSeperator = isWall ? false : (x % (1 + localSize) == 0 || y % (1 + localSize) == 0);
			bool isCenter = isWall ? false : ((2 + x) % (1 + localSize) == 0 && (2 + y) % (1 + localSize) == 0);
			if (isWall || isSeperator) {
				renderingField[x][y] = 'W';
				continue;
			}
			if (isCenter) {
				char tile = playField[(x + 2) / (localSize + 1) - 1][(y + 2) / (localSize + 1) - 1];
				int index = ((x + 2) / (localSize + 1) - 1) * SIZE + ((y + 2) / (localSize + 1) - 1) + 1;
				renderingField[x][y] =  tile == ' ' ? 48 + index : tile;
			}
		}
	}
	char drawField[MAX_SIZE][MAX_SIZE];
	for (int x = 0; x < max; x++) {
		for (int y = 0; y < max; y++) {
			string tile = getPositionString(max, x, y);
			drawField[x][y] = (tile != "") ? tiles[tile] : renderingField[x][y];
			//cout << tile << endl;
		}
		bool isSeperator = (x % (1 + localSize) == 0);
		log += prePendString + joinArray(drawField[x], max, isSeperator ? tiles["UD"] : ' ') + "\n";
	}
	return log;
}
void draw(){
	string tabs = "		";
	cout << endl << endl << endl;
	cout << playingFieldText(tabs) << endl << endl;
	cout << tabs + Message << endl;
}

bool isFilled() {
	bool result = true;
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			result = playField[x][y] == undefined ? false : result;
		}
	}
	if (result)Message = "The Game ends in a Draw";
	return result;
}
bool isWinner() {
	for (int x = 0; x < SIZE; x++) {
		int xs = 0;
		int ys = 0;
		for (int y = 0; y < SIZE; y++) {
			xs += playField[x][y] == undefined ? 0 : playField[x][y] == circle ? 1 : -1;
			ys += playField[y][x] == undefined ? 0 : playField[y][x] == circle ? 1 : -1;
		}
		if (xs == 3 || ys == 3) {
			Message = "Circle Wins";
			return true;
		}
		if (xs == -3|| ys == -3) {
			Message = "Cross Wins";
			return true;

		}
	}
	int DR = 0;
	int DL = 0;
	for (int xy = 0; xy < SIZE; xy++) {
		char FDR = playField[xy][xy];
		DR += FDR == undefined ? 0 : FDR == circle ? 1 : -1;
		char FDL = playField[(SIZE - 1) - xy][xy];
		DL += FDL == undefined ? 0 : FDL == circle ? 1 : -1;
	}
	if (DR == 3 || DL == 3) {
		Message = "Circle Wins";
		return true;
	}
	if (DR == -3 || DL == -3) {
		Message = "Cross Wins";
		return true;

	}
	return false;
}
void update() {
	int fieldID = 0;
	cin >> fieldID;
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cout << fieldID;
	if (fieldID <= 0 || fieldID > 9 || cin.fail()) {
		Message = messages["str_error"];
		draw();
		return update();
	}
	fieldID--;	
	int y = floor(fieldID / SIZE);
	int x = fieldID % SIZE;
	if (playField[y][x] != undefined) {
		Message = messages["taken"];
		return update();
	}
	playField[y][x] = currentPlayer;
	currentPlayer = (currentPlayer == circle) ? cross : circle;
	Message = playerTitles[currentPlayer] + messages["input"];
	draw();
	if (isWinner()) {
		draw();
		system("pause");
		return;
	}
	if (isFilled()) {
		draw();
		system("pause");
		return;
	}
	return update();
}
void startGame() {
	Message = playerTitles[currentPlayer] + messages["input"];
	clearPlayingField();
	draw();
	update();
}
void drawMenu() {
	system("CLS");
	cout << "Press 1 to Play or 2 to Exit" << endl;
	int choice = 0;
	cin >> choice;
	if (choice < 1 || choice > 2) {
		return drawMenu();
	}
	if (choice == 1) {
		startGame();
		return drawMenu();
	}
	else return;
}
int main() {
	drawMenu();
	return 0;
}