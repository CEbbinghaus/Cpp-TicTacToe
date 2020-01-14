#pragma once
#include <string>
#include <sstream>

///FilkeIO
//- Writes and Reads from a file
class FileIO
{
public:
	// Name of the File
	const char name[9] = "Settings";

	//bool Write(StringStream& Buffer) - writes the stream to file. Returns True if successfull
	bool write(std::stringstream& buffer);

	//bool Read(StringStream& Bufer) - Reads the files contents and writes it to the buffer. Returns True if Successfull
	bool read(std::stringstream& buffer);

	//bool Sign(int position, int data) - Writes Data to file at the pecified Position. Returns True if successfull
	bool sign(const int position, unsigned int date);
};

