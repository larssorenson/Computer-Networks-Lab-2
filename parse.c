#include "myreminder.h"

Alarm* parse(char* file)
{
	if(file)
	{
		char buffer[2];
		buffer[1] = '\0';
		int fd = open(file, O_RDONLY);
		int c = 1;
		int resp;
		int timeFlag = 0;
		int msgCount = 0;
		if(fd > 1)
		{
			currentAlarm = (Alarm*)malloc(sizeof(Alarm));
			currentAlarm->prev = NULL;
			
			while(c != 0)
			{
				c = read(fd, buffer, 1);
				if(c <= 0)
					continue;
				
				if(buffer[0] == '\0')
				{
					currentAlarm = currentAlarm->prev;
					currentAlarm->next->prev = NULL;
					if(currentAlarm->next->reminder)
						free(currentAlarm->next->reminder);
					free(currentAlarm->next);
					currentAlarm->next = NULL;
					c = 0;
					continue;
				}
				
				#ifdef Debug
					write(2, "Read: ", 6);
					write(2, buffer, 1);
					write(2, "\r\n", 2);
				#endif
				
				if(!timeFlag)
				{
					if(buffer[0] == ' ')
					{
						timeFlag = 1;
						
						#ifdef Debug
							char* currentTime = timeToString(currentAlarm->time);
							if(currentTime)
							{
								write(2, "Done parsing time! Final: ", 26);
								write(2, currentTime, strlen(currentTime));
								write(2, "\r\n", 2);
								free(currentTime);
							}
							
						#endif
						currentAlarm->reminder = (char*)malloc(sizeof(char)*1024);
						memset(currentAlarm->reminder, 0, 1024);
					}
					
					else
					{
						resp = addTimeToAlarm(buffer[0]);
						if(resp)
						{
							write(2, "Error! Failed to update alarm's time! Check input of file\r\n", 59);
						}
						
					}
					
				}
				
				else
				{
					if(buffer[0] != '\r' && buffer[0] != '\n' && buffer[0] != '\0' && msgCount < 1024)
					{
						strcat(currentAlarm->reminder, buffer);
						msgCount++;
					}
					
					else
					{
						timeFlag = 0;
						#ifdef Debug
							if(currentAlarm->reminder)
							{
								write(2, "Done parsing line! Result: ", 27);
								write(2, currentAlarm->reminder, strlen(currentAlarm->reminder));
								write(2, "\r\n", 2);
							}
							
							else
							{
								write(2, "Error reading Reminder!\r\n", 25);
							}
						#endif
						currentAlarm->next = (Alarm*)malloc(sizeof(Alarm));
						currentAlarm->next->prev = currentAlarm;
						currentAlarm = currentAlarm->next;
					}
					
				}
				
			}
			
			currentAlarm->next = NULL;
			while(currentAlarm->prev != NULL)
			{
				if(currentAlarm->next && !currentAlarm->next->reminder)
				{
					currentAlarm->next->prev = NULL;
					free(currentAlarm->next);
					currentAlarm->next = NULL;
				}
				
				currentAlarm = currentAlarm->prev;
			}
			
			return currentAlarm;
		}
		
	}
	
	return NULL;
}

#ifdef Testing
int main(int argc, char** argv)
{
		if(argc > 0 && argv[1])
		{
			parse(argv[1]);
			if(currentAlarm)
			{
				while(currentAlarm->next != NULL)
				{
					char* currentTime = timeToString(currentAlarm->time);
					if(currentTime)
					{
						write(2, currentTime, strlen(currentTime));
						write(2, ": ", 2);
						write(2, currentAlarm->reminder, strlen(currentAlarm->reminder));
						write(2, "\r\n", 2);
						free(currentTime);
					}
					
					currentAlarm = currentAlarm->next;
				}
			
			}
			
		}
	
	return 1;
}
#endif
