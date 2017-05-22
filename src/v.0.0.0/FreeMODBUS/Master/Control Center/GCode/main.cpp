// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ctype.h>

#include "gcode.h"

int main(int argc, char* argv[])
{
	LPLIST lpRootGProgramList = NULL, temp;

	gcode_loadstr( "G1 X300\n\r(123)\n\rG01 X100\n\r", &lpRootGProgramList );

	temp = lpRootGProgramList;
	while( temp ) {
		gcode *lpGc = (gcode*)temp->lpData;

		printf("G0%d X%4.1f Y%4.1f Z%4.1f\n", lpGc->G, lpGc->X, lpGc->Y, lpGc->Z );

		temp = temp->next;
	}

	freeList( &lpRootGProgramList );

	return 0;
}
