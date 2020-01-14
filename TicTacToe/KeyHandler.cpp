#pragma once
#include"KeyHandler.h"

//Starts waiting for Keyboard Events
void KeyHandler::start() {
	handling = true;
	while (handling) {
		handleInput();
	}
}

//Awaits a key input and executes a function accordingly
void KeyHandler::handleInput() {
	char character = getch();
	if (keys.find(character) != keys.end()) {
		keys[character]();
	}
	if (keys.find(7) != keys.end()) {
		keys[7]();
	}
}

//Stops the Handler from processing Inputs
void KeyHandler::stop() {
	handling = false;
}

//Subscribes to the Key Handler Event
void KeyHandler::on(char character, function<void()> handler) {
	keys[character] = handler;
}

//Emits a "Fake" Event. Used for manually Triggering code
void KeyHandler::emit(char character) {
	if (keys.find(character) != keys.end()) {
		keys[character]();
	}
}

//Utility function to get a key
char KeyHandler::subscribeToEvent() {
	return getch();
}