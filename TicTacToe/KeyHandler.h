#pragma once
#include<iostream>
#include<conio.h>
#include<functional>
#include<map>
#include "Scaffold.h"

using namespace std;

//KeyCode Identifiers
struct keyCode
{
	static const int up = 72;
	static const int down = 80;
	static const int left = 75;
	static const int right = 77;
	static const int enter = 13;
	static const int space = 32;
	static const int esc = 27;
	static const int bspc = 8;
};

//KeyHandler that detects Key Inputs
class KeyHandler : BaseStructure
{
	//Map of all keys and their respective functions
	map<char, function<void()>> keys;

	//Used to check if it should continue quering for input
	bool handling = false;

	//Awaits User input and reacts accordingly
	void handleInput();
public:
	//The KeyCode for Upate. Ascii Bell Symbol
	static const int update = 7;

	//Starts the Key Handling
	void start();

	//Stops the key Handling. Only stops before the next key Handle
	void stop();

	//Adds a function to the map with the character code so when a user presses a button the corresponding function will be executed
	void on(char character, function<void()> handler);

	//emits a "fake" call of the function in which it executes the function of the key code
	void emit(char character);

	//Utility function that waits for the next key pressed to proceed
	char subscribeToEvent();
};