#pragma once
#include "Scaffold.h"
#include "Game.h"
#include "Menu.h"
#include "Settings.h"


//Base Process that runs everything
struct Process : public BaseStructure {

	//Menu class for Main Menu
	Menu* menu;

	//Game Class for Game
	Game* game;

	//Settings class for FileIO and options
	Settings* settings;

	//Destructor
	~Process();

	//STarts the Process
	void start();

	//Inititalizes the Game and Menu
	void init();
};