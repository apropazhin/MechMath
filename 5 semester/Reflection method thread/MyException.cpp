#include <stdlib.h>
#include <string.h>
#include "MyException.h"


MyException::MyException(int _id, const char *msg)
{
	strncpy(message, msg, 255);
	message[255] = 0;
	id = _id;
}

MyException::MyException(const MyException &ex)
{
	strncpy(message, ex.message, 255);
	message[255] = 0;
	id = ex.id;
}
