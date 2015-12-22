#include "Logger.h"

#include <stdio.h>

int logging [] = {1, 1, 1}; // 0->main, 1->CodeGen, 2->Tree

void disableLogging(int id) { logging[id%3] = 0; }

void enableLogging(int id) { logging[id%3] = 1; }

void debugInfo(char* message, int id)
{
	if ( logging[id%3] == 1)
		printf("%s\n", message);
}
