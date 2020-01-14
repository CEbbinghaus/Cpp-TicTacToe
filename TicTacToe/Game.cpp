#pragma once
#include <iostream>
#include"Game.h"

using namespace std;

//Constructs the game and sets everything up. Settings are required for many values
Game::Game(Settings* options) {
	//Sets up the Pointers to the Renderer and Settings
	render = new GameRender(this);
	settings = options; 

	//Initializes the Values with the ones in the settings
	gridSize = (int*)settings->get("FieldSize")->Value();
	winSize = (int*)settings->get("WinConditionAmount")->Value();
	playerTitles =
	{
		{circle, (char*)settings->get("PlayerOneName")->Value()},
		{cross, (char*)settings->get("PlayerTwoName")->Value()}
	};

	playingField = nullptr;
	
	//Setting up the Key Handler Inputs
	handler.on(keyCode::up, [&]() {cursor['x']--; });
	handler.on(keyCode::down, [&]() {cursor['x']++; });
	handler.on(keyCode::left, [&]() {cursor['y']--; });
	handler.on(keyCode::right, [&]() {cursor['y']++; });
	handler.on(keyCode::space, [&]() {tick(); });
	handler.on(keyCode::enter, [&]() {tick(); });
	handler.on(handler.update, [&]() {update(); });
	init();
}

//Rebuilds the Playing Field Before the game Begins and resets all values
void Game::start() {
	init();
	genPlayingField();
	clearPlayingField();

	//Sets states to default values
	currentPlayer = state::cross;
	outputMessage = playerTitles[currentPlayer] + messages["input"];

	update();
	handler.start();
}

//Generates Playing Field
void Game::genPlayingField() {
	if(playingField != nullptr)Deconstruct();
	oldFieldSize = *gridSize;
	playingField = new int*[*gridSize];
	for (int i = 0; i < *gridSize; ++i)
	{
		playingField[i] = new int[*gridSize];
	}
}

//Initializes The Renderer
void Game::init() {
	maxGridSize = (*gridSize * 3) + (*gridSize + 1);
	render->init();
}

//Clears the Playing Field by setting it all to undefined
void Game::clearPlayingField() {
	cursor['x'] = cursor['y'] = 0;
	for (int x = 0; x < *gridSize; x++) {
		for (int y = 0; y < *gridSize; y++) {
			playingField[x][y] = undefined;
		}
	}
}

//Selects the current tile and updates the game
void Game::tick() {
	int* field = &playingField[cursor['x']][cursor['y']];
	if (*field != state::undefined) {
		outputMessage = messages["taken"];
		return;
	}
	*field = currentPlayer;
	if (hasEnded()) {
		cls();
		cout << playerTitles[currentPlayer] << " Has Won";
		handler.stop();
	}
	currentPlayer = currentPlayer == state::circle ? state::cross : state::circle;
	outputMessage = playerTitles[currentPlayer] + messages["input"];
}

//Clamps the cursor and Draws the field
void Game::update() {
	clamp<int>(cursor['x'], 0, *gridSize - 1);
	clamp<int>(cursor['y'], 0, *gridSize - 1);
	draw();
}

//Checks for either a Win or a draw
bool Game::hasEnded() {
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

//Loops through every Field abd Checks if its filled
bool Game::isFilled() {
	bool result = true;
	for (int x = 0; x < *gridSize; x++) {
		for (int y = 0; y < *gridSize; y++) {
			result = playingField[x][y] == undefined ? false : result;
		}
	}
	if (result)outputMessage = "The Game ends in a Draw";
	return result;
}

//Checks if a player has won
bool Game::isWinner() {
	bool* wingrid = (bool*)settings->get("WinConditionAmount==FieldSize")->Value();
	int winCount = (*wingrid) ? *gridSize : (((*winSize) > (*gridSize)) ? *gridSize : *winSize);
	int DR = 0;
	int DL = 0;
	//Loops over every field in every order to find matches for horizontal/vertical winners
	for (int x = 0; x < *gridSize; x++) {
		char FDR = playingField[x][x];
		DR += FDR == undefined ? 0 : FDR == circle ? 1 : -1;

		char FDL = playingField[(*gridSize - 1) - x][x];
		DL += FDL == undefined ? 0 : FDL == circle ? 1 : -1;


		int xs = 0;
		int ys = 0;
		for (int y = 0; y < *gridSize; y++) {
			xs += playingField[x][y] == undefined ? 0 : playingField[x][y] == circle ? 1 : -1;
			ys += playingField[y][x] == undefined ? 0 : playingField[y][x] == circle ? 1 : -1;
		}
		bool PlayerOneWon = (xs == winCount || ys == winCount || DR == winCount || DL == winCount);
		bool PlayerTwoWon = (xs == -winCount || ys == -winCount || DR == -winCount || DL == -winCount);
		if (PlayerOneWon || PlayerTwoWon) {
			outputMessage = playerTitles[currentPlayer];
			outputMessage += " Wins";
			return true;

		}
	}
	return false;
}
//Destroys the Playing Field
void Game::Deconstruct() {
	for (int i = 0; i < oldFieldSize; ++i) {
		if (playingField[i] == nullptr)continue;
		delete playingField[i];
		playingField[i] = nullptr;
	}
	delete[] playingField;
	playingField = nullptr;
}

//Deconstructs the Game
Game::~Game() {
	Deconstruct();
	delete render;
}

//Draws the field
void Game::draw() {
	render->draw();
}