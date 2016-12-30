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

#include "stdafx.h"

#include "stdlib.h"
#include "llist.h"

unsigned int addToList(LPLIST *root, void *data, void (*freeData)(void *lpData))
{
	LPLIST last, temp;
	unsigned int id = 0;

	temp = (LPLIST)malloc(sizeof(LIST));
	temp->next = NULL;
	temp->lpData = data;
	temp->freeData = freeData;

	if(!*root) {
//		printf("ID: %d\n", id);
		temp->id = id;
		*root = temp;
		return id;
	}

	last = (LPLIST)(*root);
	while(last) {
		if(id == last->id) { id++; last = (LPLIST)(*root); }
		else last = last->next;
	}

//	printf("ID: %d\n", id);

	last = (LPLIST)(*root);
	while(last->next) last = last->next;
	last->next = temp;
	last->next->id = id;

	return id;
}

void removeFromList(LPLIST *root, unsigned int id)
{
	LPLIST temp, current, previous;

	if(NULL == (LPLIST)(*root)) return;

	temp = (LPLIST)(*root);
	current = NULL;

	do {
		if(id == temp->id) current = temp;
		else previous = temp;

		temp = temp->next;
	} while(temp && current==NULL);

	if(NULL == current) return;

	if(*root == current) *root = current->next;
	else previous->next = current->next;

	if(current->freeData) current->freeData(current->lpData);
	free(current->lpData);
	free(current);
}

void freeList(LPLIST *root) // +
{
	while(*root) {
		LPLIST current = *root;
		removeFromList(root, current->id);
	}

	*root = NULL;
}
