#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

extern unsigned int __heap_start;
extern void *__brkval;

#include "MemoryFree.h"

int freeMemory()
{
	int free_memory;
	if ((int)__brkval == 0)
	{
		free_memory = ((int)&free_memory) - ((int)&__heap_start);
	}
	else
	{
		free_memory = ((int)&free_memory) - ((int)__brkval);
	}
	return free_memory;
}
