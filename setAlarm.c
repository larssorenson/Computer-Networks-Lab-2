#include "myreminder.h"

void setAlarm()
{
	if(currentAlarm)
	{
		if(currentAlarm->time - currentAlarm->prev->time <= 0)
			writeAlarm(0);
		else
			alarm(currentAlarm->time - currentAlarm->prev->time);
	}
}