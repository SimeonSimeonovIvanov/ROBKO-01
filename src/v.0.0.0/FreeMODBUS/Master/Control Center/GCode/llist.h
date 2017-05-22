/*
		The^day^of^DooM

		Borland C++ 3.1
			  &&
		  Open Watcom

	Create Date:	26.12.2007
	Last Update:    28.12.2007
	Last Update:    26.01.2008 - Deklaraciqta na LPLIST e premestena w "src/TYPE.H"
	Last Update:    12.07.2008
*/

#ifndef __LIST_H__
#define __LIST_H__
#include "stdlib.h"

typedef struct list {
	void *lpData;
	unsigned char id;
	struct list *next;
	void (*freeData)(void *lpData);
} LIST, *LPLIST;

unsigned int addToList(LPLIST *root, void *data, void (*freeData)(void *lpData));
void removeFromList(LPLIST *root, unsigned int id);
void freeList(LPLIST *root);

#endif