#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <conio.h>
#include <windows.h>


void setColor(int i) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}

using namespace std;

#define SIZE 3
#define MAX_SIZE (SIZE * SIZE) + (SIZE + 1)

int playField[SIZE][SIZE];
char renderingField[MAX_SIZE][MAX_SIZE];

enum keyCodes {
	kc_up = 72,
	kc_down = 80,
	kc_left = 75,
	kc_right = 77,
	kc_enter = 13
};
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
		if (i != size - 1)
			result += spacer;
	}
	return result;
}
map<string, string> messages = {
	{"input", " Select a Field"},
	{"taken", "This Field Has Already Been Picked Please Pick Another One"}
};
map<char, int> cursor = {
	{'x', 0},
	{'y', 0}
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
	cursor['x'] = cursor['y'] = 0;
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
string formatText(string text, int color = 31) {
	return "\033[1;" + to_string(color) + "m" + text + "\033[0m";
}
string playingFieldText(string prePendString) {
	system("CLS");
	std::string log = "";
	int localSize = SIZE;
	int maxSize = MAX_SIZE;
	for (int x = 0; x < maxSize; x++) {
		for (int y = 0; y < maxSize; y++) {
			renderingField[x][y] = ' ';//254
			bool isWall = (x == 0 || x == maxSize - 1 || y == 0 || y == maxSize - 1);
			bool isSeperator = isWall ? false : (x % (1 + localSize) == 0 || y % (1 + localSize) == 0);
			bool isCenter = isWall ? false : ((2 + x) % (1 + localSize) == 0 && (2 + y) % (1 + localSize) == 0);
			if (isWall || isSeperator) {
				renderingField[x][y] = 'W';
				continue;
			}
			if (isCenter) {
				char tile = playField[(x + 2) / (localSize + 1) - 1][(y + 2) / (localSize + 1) - 1];
				int fx = ((x + 2) / (localSize + 1) - 1);
				int fy = ((y + 2) / (localSize + 1) - 1);
				bool isCursor = (cursor['x'] == fx && cursor['y'] == fy);
				bool isTile = !(tile == ' ');
				renderingField[x][y] = isTile ? isCursor ? 176 : tile : (!isCursor ? ' ' : 219);
			}
		}
	}
	char drawField[MAX_SIZE][MAX_SIZE];
	for (int x = 0; x < maxSize; x++) {
		for (int y = 0; y < maxSize; y++) {
			string tile = getPositionString(maxSize, x, y);
			drawField[x][y] = (tile != "") ? tiles[tile] : renderingField[x][y];
			//cout << tile << endl;
		}
		bool isSeperator = (x % (1 + localSize) == 0);
		log += prePendString + joinArray(drawField[x], maxSize, isSeperator ? tiles["UD"] : ' ') + "\n";
	}
	return log;
}
void draw() {
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
	int winCount = SIZE;
	int DR = 0;
	int DL = 0;
	for (int x = 0; x < SIZE; x++) {
		char FDR = playField[x][x];
		DR += FDR == undefined ? 0 : FDR == circle ? 1 : -1;

		char FDL = playField[(SIZE - 1) - x][x];
		DL += FDL == undefined ? 0 : FDL == circle ? 1 : -1;


		int xs = 0;
		int ys = 0;
		for (int y = 0; y < SIZE; y++) {
			xs += playField[x][y] == undefined ? 0 : playField[x][y] == circle ? 1 : -1;
			ys += playField[y][x] == undefined ? 0 : playField[y][x] == circle ? 1 : -1;
		}
		if (xs == winCount || ys == winCount || DR == winCount || DL == winCount) {
			Message = "Circle Wins";
			return true;
		}
		if (xs == -winCount || ys == -winCount || DR == -winCount || DL == -winCount) {
			Message = "Cross Wins";
			return true;

		}
	}
	return false;
}
bool update() {
	if (isWinner()) {
		draw();
		system("pause");
		return true;
	}
	if (isFilled()) {
		draw();
		system("pause");
		return true;
	}
	return false;
}
void handleInput() {
	//if (kbhit())
	{
		char ch = getch();
		char c = ' ';
		switch (ch)
		{
			case kc_up:
				if (cursor['x'] != 0)cursor['x']--;
				break;
			case kc_down:
				if (cursor['x'] < SIZE - 1)cursor['x']++;
				break;
			case kc_left:
				if (cursor['y'] != 0)cursor['y']--;
				break;
			case kc_right:
				if (cursor['y'] < SIZE - 1)cursor['y']++;
				break;
			case kc_enter:
				if (playField[cursor['x']][cursor['y']] != undefined) {
					Message = messages["taken"];
					update();
					return;
				}
				playField[cursor['x']][cursor['y']] = currentPlayer;
				currentPlayer = (currentPlayer == circle) ? cross : circle;
				Message = playerTitles[currentPlayer] + messages["input"];

		}
	}
}
void tick() {
	while (true) {
		draw();
		handleInput();
		if (update()) {
			return;
		}
		//_sleep(30);
	}
}
void startGame() {
	Message = playerTitles[currentPlayer] + messages["input"];
	clearPlayingField();
	draw();
	tick();
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
	//if (!(SIZE % 2))return 0;
	drawMenu();
	return 0;
}