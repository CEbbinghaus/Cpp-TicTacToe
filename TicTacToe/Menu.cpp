#pragma once
#include<cmath>
#include"Menu.h"
#include "KeyHandler.h"


//Adds a Field to the List of menu Items using a String and function
void Menu::addField(field o) 
{
	IField f(o);
	size++;
	options.push_back(o);
}

//Adds a Field Interface to the list with Two functions
void Menu::addField(IField o)
{
	size++;
	options.push_back(o);
}

//Changes the current cursor position (only takes effect on the next draw event)
void Menu::setCursor(int pos) {
	cursor = pos;
	cursor = cursor < 0 ? 0 : cursor > size - 1 ? size - 1 : cursor;
}

//Selects Menu Element
void Menu::select(int index) 
{
	if (canExit)
		handler.stop();
	options[index].func();
	//update();
}

//Starts the Menu Process
void Menu::start(bool exit) {
	this->canExit = exit;
	draw();
	init();
}

//Ends the Menu
void Menu::end(){
	canExit = true;
	handler.stop();
}

//Clamps the Cursor and calls draw
void Menu::update() {
	cursor = cursor < 0 ? 0 : cursor > size - 1 ? size - 1 : cursor;
	draw();
}

//Initializes all Keyhandler events
void Menu::init(){
	handler.on(keyCode::down, [&]() {cursor++; });
	handler.on(keyCode::up, [&]() {cursor--; });
	handler.on(keyCode::enter, [&]() {select(cursor); });
	handler.on(keyCode::space, [&]() {select(cursor); });
	handler.on(keyCode::esc, [&]() {handler.stop(); });
	handler.on(keyCode::bspc, [&]() {handler.stop(); });
	handler.on(handler.update, [&]() {update(); });
	handler.start();
}

//Draws the Menu
void Menu::draw() {
	cls();
	cout << "Menu" << endl;
	for (auto& i : options) {
		bool selected = &options[cursor] == &i;
		cout << "[" << (selected ? (char)178 : ' ') << ']';
		cout << i.name().c_str() << endl;
	}
}