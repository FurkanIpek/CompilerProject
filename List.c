#include "List.h"

#include <stdlib.h>
#include <string.h>

void getNextCode(Symbols_t* head, char* code, int reset)
{
	static Symbols_t* cur = NULL;

	if (reset == 1) cur = head->next;

	if (cur != NULL)
	{
		code = cur->code;
		cur = cur->next;
	}
}

Symbols_t* findLastEntry(Symbols_t *head)
{
	Symbols_t *ptr = head->next;

	while ( ptr->next != NULL )
		ptr = ptr->next;

	return ptr;
}

void addEntry(Symbols_t* head, char* code)
{
	Symbols_t* ptr = head->next;

	Symbols_t* newNode = malloc(sizeof(Symbols_t));
	newNode->code = strdup(code);
	newNode->next = NULL;

	if ( ptr == NULL )
	{
		head->next = newNode;
	}
	else
	{
		ptr = findLastEntry(head);
		ptr->next = newNode;
	}
}

Symbols_t* initList()
{
	Symbols_t* head = malloc(sizeof(Symbols_t));
	head->next = NULL;
	head->code = "";

	return head;
}

void destroyList(Symbols_t* head)
{
	if (head != NULL)
	{
		Symbols_t *ptr = head->next;

		free(head); head = ptr;

		while (ptr != NULL)
		{
			head = head->next;
			free(ptr->code); free(ptr);
			ptr = head;
		}
	}
}
