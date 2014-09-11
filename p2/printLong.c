#include "mychecksum.h"

void printLong(long number)
{
	char buffer[2];
	buffer[0] = 0;
	buffer[1] = 0;
	while(number != 0)
	{
		buffer[0] = (char)((number - (((long)(number/10))*10))+48);
		write(2, buffer, 1);
		number /= 10;
	}
}
