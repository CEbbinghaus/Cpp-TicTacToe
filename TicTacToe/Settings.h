#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include "FileIO.h"
#include "Menu.h"
#include "util.h"


using namespace std;


enum VType {
	UDef = 0,
	Str = 1,
	Int = 2,
	Flt = 3,
	Bll = 4
};

//Option Class to act likea base
class Option:public BaseStructure{
public:
	int type = VType::UDef;
	string name;
	virtual ~Option() = default;
	virtual void configure() = 0;
	virtual void* Value() = 0;
	virtual void write(stringstream& buffer) = 0;
	virtual bool read(stringstream& buffer) = 0;
	virtual string draw() = 0;
};

//String option for multiple choice or manual entry
class SOption : public Option {
public:
	char* value;
	vector<string> possibilities;
	SOption(string a_name, const char* a_value, vector<string> options);
	~SOption();
	void* Value();
	void write(stringstream& buffer);
	bool read(stringstream& buffer);
	string draw();;
	void configure();
};

//Interger Option for Sliders with Whole numbers
class IOption : public Option {
public:
	int* value;
	int min;
	int max;
	IOption(string a_name, int a_value, int a_min, int a_max);
	~IOption();
	void* Value();
	void write(stringstream& buffer);
	bool read(stringstream& buffer);
	string draw();
	void configure();
};

//Float Option for sliders with decimal numbers
class FOption : public Option {
public:
	float* value;
	float min;
	float max;
	FOption(string a_name, float a_value, float min, float max);
	~FOption();
	void* Value();
	string draw();
	void write(stringstream& buffer);
	bool read(stringstream& buffer);
	void configure();
};

//Boolean Option for Toggles
class BOption : public Option {
public:
	bool* value;
	BOption(string a_name, float a_value);
	~BOption();
	void* Value();
	string draw();
	void write(stringstream& buffer);
	bool read(stringstream& buffer);
	void configure();
};

//Option Data containing everything written to file
class OptionData {
public:

	//Size of the CopyRight string
	static const unsigned int copyrightSize = 48;

	//Copyright String
	const char copyright[copyrightSize] = {"Copyright Christopher-Robin Ebbinghaus, 3132019"};

	//Option Fields to populate the Settings Menu
	vector<Option*> fields = {
		new SOption("PlayerOneName", "Circle", {}),
		new SOption("PlayerTwoName", "Cross", {}),
		new IOption("FieldSize", 3, 2, 10),
		new BOption("WinConditionAmount==FieldSize", true),
		new IOption("WinConditionAmount", 3, 2, 10)
	};

	//Destructor
	~OptionData();

	//Method to retrieve a single Option by its name
	Option* get(string s);
};

//Settings class Implementing the Data and File Loading/Saving
class Settings : public BaseStructure, public OptionData {
	//File to read from and write to
	FileIO file;

	//Meny to navigate for individual settings
	Menu menu;
public:
	//Starts the Menu and Settings
	void start();

	//Initializes the Values
	void init();

	//Loads Data from File
	void load();

	//Saves Data to file
	void save();

	//Renders all Options with their Value
	void draw();
};
/*

Game{
	Setting s;
	int* Volume = s.get("Volume").init();
}

*/