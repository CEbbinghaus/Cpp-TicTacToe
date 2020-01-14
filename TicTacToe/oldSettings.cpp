//#pragma once
//#include <iostream>
//#include <string>
//#include <sstream>
//
//#include "FileIO.h"
//#include "Menu.h"
//
//
//using namespace std;
//
//
//enum VType {
//	UDef = 0,
//	Str = 1,
//	Int = 2,
//	Flt = 3,
//	Bll = 4
//};
//
//class Option {
//public:
//	int type = VType::UDef;
//	string name;
//	virtual void configure() = 0;
//	virtual friend ostream &operator << (ostream &buffer, const Option &D) {
//
//	}
//};
//
//class SOption : public Option {
//public:
//	string* value;
//	vector<string> possibilities;
//	SOption(string a_name, string a_value, vector<string> options) {
//		type = VType::Str;
//		name = a_name;
//		possibilities = options;
//		value = new string(a_value);
//	}
//	SOption(char* data[]) {
//
//	}
//	~SOption() {
//		delete value;
//		value = nullptr;
//	}
//	string* init() {
//		return value;
//	}
//	friend ostream &operator << (ostream &buffer, const SOption &D) {
//		buffer << (*D.value).c_str();
//		for (string s : D.possibilities) {
//			buffer << s.c_str();
//		}
//	}
//
//	void configure() {
//		Menu m;
//		for (string s : possibilities) {
//			m.addField({ s, [&]() {*value = s; } });
//		}
//		m.start();
//	}
//};
//
//class IOption : public Option {
//public:
//	int* value;
//	int min;
//	int max;
//	IOption(string a_name, int a_value, int min, int max) {
//		type = VType::Int;
//		name = a_name;
//		value = new int(a_value);
//	}
//	IOption(char* data[]) {
//
//	}
//	~IOption() {
//		delete value;
//		value = nullptr;
//	}
//	int* init() {
//		return value;
//	}
//	friend ostream &operator << (ostream &buffer, const IOption &D) {
//		buffer << *D.value
//			<< D.min
//			<< D.max;
//	}
//	void configure() {}
//};
//
//class FOption : public Option {
//public:
//	float* value;
//	float min;
//	float max;
//	FOption(string a_name, float a_value, float min, float max) {
//		type = VType::Flt;
//		name = a_name;
//		value = new float(a_value);
//	}
//	FOption(char* data[]) {
//
//	}
//	~FOption() {
//		delete value;
//		value = nullptr;
//	}
//	float* init() {
//		return value;
//	}
//	friend ostream &operator << (ostream &buffer, const FOption &D) {
//		buffer << *D.value
//			<< D.min
//			<< D.max;
//	}
//	void configure() {}
//};
//
//class BOption : public Option {
//public:
//	bool* value;
//	BOption(string a_name, float a_value) {
//		type = VType::Bll;
//		name = a_name;
//		value = new bool(a_value);
//	}
//	BOption(char* data[]) {
//
//	}
//	~BOption() {
//		delete value;
//		value = nullptr;
//	}
//	bool* init() {
//		return value;
//	}
//	friend ostream &operator << (ostream &buffer, const BOption &D) {
//		buffer << *D.value;
//	}
//	void configure() {}
//};
//
//class OptionData {
//public:
//	vector<Option*> fields = {
//		new SOption("StringTest", "test", {"test", "tets1", "test2", "test3"}),
//		new IOption("IntTest", 0, 0, 100),
//		new FOption("FloatTest", 0.0f, 0.0f, 100.0f),
//		new BOption("BooleanTest", false)
//	};
//
//	friend ostream &operator<<(ostream &output, const OptionData &D) {
//		stringstream s;
//		for (Option* i : D.fields) {
//			s << i << 23 << 23;
//		}
//		output << s.str().c_str();
//		return output;
//	}
//
//	friend istream &operator>>(istream  &input, OptionData &D) {
//		return input;
//	}
//};
//
//class Settings : public BaseStructure, public OptionData {
//	FileIO file;
//	Menu menu;
//public:
//	void start();
//	void init();
//	void load();
//	void save();
//	void draw() {
//		//Menu m;
//		//for (Option *o : fields) {
//		//	m.addField({ o->name, o->configure });
//		//}
//		//m.addField({ "Save Settings", [&]() {cls(); cout << "Saved"; _sleep(2000); } });
//		//m.start();
//	}
//};
///*
//
//Game{
//	Setting s;
//	int* Volume = s.get("Volume").init();
//}
//
//*/