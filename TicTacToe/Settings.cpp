#pragma once

#include<iostream>
#include <time.h>

#include"Settings.h"

using namespace std;

///--------------------------------SETTINGS
//starts the Settings Menu
void Settings::start(){
	draw();
}
//Initializes the Settings. Calls Load
void Settings::init() {
	load();
}
//Saves the settings
void Settings::save() {

	stringstream buffer;

	//Signed with Copyright and Current Date/Time
	buffer.write(copyright, sizeof(char) * copyrightSize);
	unsigned int t = 0;
	buffer.write((const char*)&t, sizeof(int));

	//Writes every field to the Buffer
	for (int i = 0; i < fields.size(); i++) {
		fields[i]->write(buffer);
	}

	//Tries to Write and Sign the file. If failed it will Print a Error
	if (file.write(buffer) && file.sign(copyrightSize, time(0))) {
		cout << "Successfully Saved" << endl;
	}
	else {
		cout << "Failed to Save. Try restarting the game with Administrator Permissions" << endl;
	}

	cout << "Output Called";
	return;
}

//Loads the Data from file
void Settings::load() {
	stringstream buffer;

	//Reads the file content to buffer
	if (file.read(buffer)) {
		buffer.seekg(0, ios::beg);
		buffer.clear();

		//Retrieves Copyright and Signature
		char cpr[copyrightSize];
		buffer.get(cpr, copyrightSize, '\0');
		buffer.seekg(1, ios::cur);

		if (strcmp(cpr, copyright) != 0){
			cls();
			cout << "INVALID DATA SIGNATURE";
			_sleep(2000);
			exit(0);
		}
		cout << "DATA VERIFIED";
		_sleep(200);

		unsigned int date = 999999;
		char data[sizeof(int) + 1];
		buffer.get(data, sizeof(int) + 1);
 		memcpy(&date, data, sizeof(int));

		//Loops over settings and loads Data
		buffer.clear();
		for (int i = 0; i < fields.size(); i++) {
			bool passed = fields[i]->read(buffer);
			if (!passed) {
				cls();
				cout << "Corrupted or Invalid Data" << endl;
				//
				if (remove(file.name) == 0) {
					cout << "Successfully Reset Data. Reloading in:" << endl;
					for (int i = 3; i > 0; i--) {
						cout << i << endl;
						Sleep(500);
					}
					Sleep(500);
					cls();
					cout << "Please Reopen the Game" << endl;
					Sleep(1000);
					exit(0);
				}
				else {
					cout << "Failed to Remove File. Delete Settings File Manually";
				}
				Sleep(2000);
				exit(404);
			}
		}
	}
	else {
		cls();
		cout << "Failed to Load Settings" << endl;
		Sleep(200);
		cout << "Saving to File" << endl;
		save();
		Sleep(2000);
	}
}

//Draws Settings Menu
void Settings::draw() {
	Menu m;
	for (int i = 0; i < fields.size(); i++) {
		auto& o = fields[i];
		m.addField({ [&]() {return o->draw(); }, [&]() {o->configure(); } });
	}
	m.addField({ "Save Settings", [&]() {
		m.end();
		save();
		
		cls();
		cout << "Saved";
		Sleep(1000);
	}});
	m.start(false);
}
///------------------------------------------------------OptionData

//Destructs all Options
OptionData::~OptionData() {
	for (auto i : fields) {
		delete i;
	}
}

//Returns a Option by its Name
Option* OptionData::get(string s) {
	for (Option* field : fields) {
		if (field->name == s)return field;
	}
	return nullptr;
}

///------------------------------------------------------Options
//-----------------------IOption
IOption::IOption(string a_name, int a_value, int a_min, int a_max) {
	type = VType::Int;
	name = a_name;
	value = new int(a_value);
	min = a_min;
	max = a_max;
}

IOption::~IOption() {
	delete value;
	value = nullptr;
}

void* IOption::Value() {
	return value;
}

void IOption::write(stringstream& buffer) {
	buffer.write((const char*)(value), sizeof(int));
}

bool IOption::read(stringstream& buffer) {
	buffer.clear();
	char data[5];
	buffer.get(data, 5);
	memcpy(value, data, 4);
	if (*value < min || *value > max)return false;
	return true;
}

string IOption::draw() {
	return name + ": " + to_string(*value);

}

void IOption::configure() {
	int width = (int)round(ConsoleWidth() / 0.75);
	float step = (float)(1.0f / (float)width);
	int cursor = floor((float)((float)(*value - min) / (float)(max - min)) * (float)width);
	KeyHandler h;
	h.on(keyCode::left, [&]() {cursor--; });
	h.on(keyCode::right, [&]() {cursor++; });
	h.on(keyCode::esc, [&]() {h.stop(); });
	h.on(keyCode::bspc, [&]() {h.stop(); });
	h.on(keyCode::enter, [&]() {
		*value = round(min + (float)(max - min) * (float)(cursor * step));
		h.stop();
	});
	h.on(h.update, [&]() {
		cls();
		cursor = (cursor < 0) ? 0 : (cursor > width) ? width : cursor;
		for (int b = 0; b < round(width * 0.125); b++) {
			cout << ' ';
		}
		for (int i = 0; i <= width; i++) {
			cout << (char)((i == cursor) ? 178 : '-');
		}
		cout << endl;
		for (int b = 0; b < round(width * 0.5); b++) {
			cout << ' ';
		}
		cout << round(min + (float)(max - min) * (float)(cursor * step)) << endl;
	});
	h.emit(h.update);
	h.start();
}

//----------------------FOption

FOption::FOption(string a_name, float a_value, float a_min, float a_max) {
	type = VType::Flt;
	name = a_name;
	value = new float(a_value);
	min = a_min;
	max = a_max;
}

FOption::~FOption() {
	delete value;
	value = nullptr;
}

void* FOption::Value() {
	return value;
}

bool FOption::read(stringstream& buffer) {
	buffer.clear();
	char data[5];
	buffer.get(data, 5);
	memcpy(value, data, 4);
	if (*value < min || *value > max)return false;
	return true;
}

void FOption::write(stringstream& buffer) {
	buffer.write((const char*)(value), sizeof(float));
}

string FOption::draw() {
	return name + ": " + to_string(*value);
}

void FOption::configure() {
	int width = (int)round(ConsoleWidth() / 0.75);
	float step = (float)(1.0f / (float)width);
	int cursor = floor((float)((float)(*value - min) / (float)(max - min)) * (float)width);
	KeyHandler h;
	h.on(keyCode::left, [&]() {cursor--; });
	h.on(keyCode::right, [&]() {cursor++; });
	h.on(keyCode::esc, [&]() {h.stop(); });
	h.on(keyCode::bspc, [&]() {h.stop(); });
	h.on(keyCode::enter, [&]() {
		*value = min + (float)(max - min) * (float)(cursor * step);
		h.stop();
	});
	h.on(h.update, [&]() {
		cls();
		cursor = (cursor < 0) ? 0 : (cursor > width) ? width : cursor;
		for (int b = 0; b < round(width * 0.125); b++) {
			cout << ' ';
		}
		for (int i = 0; i <= width; i++) {
			cout << (char)((i == cursor) ? 178 : '-');
		}
		cout << endl;
		for (int b = 0; b < round(width * 0.5); b++) {
			cout << ' ';
		}
		cout << min + (float)(max - min) * (float)(cursor * step) << endl;
	});
	h.emit(h.update);
	h.start();
}

//-----------------------SOption

SOption::SOption(string a_name, const char* a_value, vector<string> options) {
	type = VType::Str;
	name = a_name;
	possibilities = options;
	value = new char[32];
	strcpy(value, a_value);
}

SOption::~SOption() {
	delete value;
	value = nullptr;
}

void* SOption::Value() {
	return value;
}

void SOption::write(stringstream& buffer) {
	buffer << value;
	buffer << '\0';
}

bool SOption::read(stringstream& buffer) {
	buffer.clear();
	buffer.get(value, 32, '\0');
	buffer.seekg(1, ios::cur);
	cout << value;
	return true;
}

string SOption::draw() {
	return name + ": " + value;
}

void SOption::configure() {
	if (possibilities.size() > 0) {
		Menu m;
		for (string s : possibilities) {
			m.addField({ s, [=]() {strcpy(value, s.c_str()); } });
		}

		auto currentValue = find(possibilities.begin(), possibilities.end(), string(value));
		if (currentValue != possibilities.end())
			m.setCursor(distance(possibilities.begin(), currentValue));
		m.draw();
		Sleep(200);
		m.start();
	}
	else {
		cls();
		cout << "Please Enter the Desired Value: " << endl;
		cin.get(value, 32);
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}
}

//----------------------BOption

BOption::BOption(string a_name, float a_value) {
	type = VType::Bll;
	name = a_name;
	value = new bool(a_value);
}

BOption::~BOption() {
	delete value;
	value = nullptr;
}

void* BOption::Value() {
	return value;
}

string BOption::draw() {
	return name + ": " + (*value ? "true" : "false");

}

void BOption::write(stringstream& buffer) {
	buffer.write((const char*)(value), sizeof(boolean));
	//buffer << *value;
}

bool BOption::read(stringstream& buffer) {
	buffer.clear();
	char data[2];
	buffer.get(data, 2);
	memcpy(value, data, 1);
	if ((int*)*value != (int*)true && (int*)*value != (int*)false)return false;
	return true;
}

void BOption::configure() {
	*value = !*value;
}