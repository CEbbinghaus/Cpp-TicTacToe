#pragma once

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

//Returns the Width of the Console window for Percentages
static int ConsoleWidth() {
	#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns, rows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return rows;
	#else
		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		return size.ws_row;
	#endif // _WIN32

}

//Clamp function to ensure a value in in a certain range
template <typename T>
static int clamp(T& value, T min, T max) {
	return (value = (value < min) ? min : (value > max) ? max: value);
}