#pragma once

class MyException
{
private:
	char message[256];
	int id;

public:
	MyException(int _id, const char *msg);
	MyException(const MyException &ex);
	const char* Message() { return message; }
	int ID() { return id; }
}; 
