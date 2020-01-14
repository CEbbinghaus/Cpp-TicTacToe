#include <iostream>
#include<fstream>


#include "FileIO.h"

using namespace std;

//Reads contents of File into a Buffer
bool FileIO::read(stringstream& buffer) {
	ifstream fin(name, ios::in | ios::binary);
	if (fin.good()) {
		buffer << fin.rdbuf();
		fin.close();
		return true;
	}
	else
		fin.close();
	return false;
}

//Writes a Buffer to file
bool FileIO::write(stringstream& data) {
	ofstream fout(name, ios::out | ios::binary);
	if (fout.good()) {
		fout << data.rdbuf();
		fout.close();
		return true;
	}
	else
		fout.close();
	return false;
}

//Inserts a Interger at a given Position
bool FileIO::sign(const int pos, unsigned int date) {
	ofstream fed(name, ios::in | ios::out | ios::binary);
	if (fed.good()) {
		fed.seekp(pos, ios::beg);
		fed.write((char*)&date, sizeof(int));
		return true;
	}
	else
		return false;
}