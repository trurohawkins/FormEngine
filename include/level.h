#pragma once
#include "FormEngine.h"

typedef struct {
	int id;
	char *file;
	void (*func)(void);
} Level;

extern linkedList *levels;

void setLevel(int level);
Level *makeLevel(char *file, void (*func)(void));
Level *findLevel(int id);
bool loadLevel(Level *lvl);
bool loadNextLevel();
bool reloadLevel();
void endLevel();
void freeLevel(void *level);
