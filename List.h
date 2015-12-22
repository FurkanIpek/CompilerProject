#pragma once

typedef struct Symbols {
	char* code;
	struct Symbols* next;
} Symbols_t;

void getNextCode(Symbols_t*, char* code, int reset);
Symbols_t* findLastEntry(Symbols_t* head);
void addEntry(Symbols_t* head, char* code);
Symbols_t* initList();
void destroyList(Symbols_t* head);
