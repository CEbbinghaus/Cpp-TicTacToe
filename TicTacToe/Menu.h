#pragma once
#include <iostream>
#include <vector>
#include "Scaffold.h"
#include "KeyHandler.h"

using namespace std;

//Menu Field. using a Name and function
struct field {
	string name;
	function<void()> func;
};

//Field Interface with functions for name and task
struct IField {
	function<string()> name;
	function<void()> func;
	IField(function<string()> a_name, function<void()> a_func) {
		name = a_name;
		func = a_func;
	}
	IField(field origin) {
		func = origin.func;
		name = [=]() {return origin.name;};
	}
};

//Menu Class that creates draws and navigates a menu
class Menu : public BaseStructure {

	//Bool checked when a option is selected to either quit afterwards to infinitely loop
	bool canExit = false;

	//Cursor Position
	int cursor = 0;

	//Key handler Dedicated to User Input handling
	KeyHandler handler;

	//Field array for drawing and selecting
	vector<IField> options;

	//amount of Fields
	int size = 0;

public:
	//Adds a Field and converts it to its Interrface
	void addField(field o);

	//Adds a Interface into the array
	void addField(IField o);

	//executes the function at a certain index
	void select(int index);

	//Starts the Menu with its Exit Condition
	void start(bool exit = true);
	
	//Stops the Menu 
	void end();

	//Moves the cursor to a certain location
	void setCursor(int pos);

	//Initializes all values
	void init();

	//Draws the Menu
	void draw();


	//Clamps the cursor and draws the update
	void update();
};