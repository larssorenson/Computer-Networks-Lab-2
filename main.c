#include "myreminder.h"

#ifndef Testing
int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("You must supply a file name to read from!\r\n");
	}
	
	int resp = setAlarmHandler();
	if(resp)
	{
		printf("Error! Could not set Signal Alarm Handler!\r\n");
	}
	
	if(parse(argv[1]))
	{
		writeTimeOfDay();
		write(2, "Reminders set!\r\n", 16);
		alarm(currentAlarm->time);		
		while(currentAlarm){}	
	}
	
	return 0;
}
#endif
