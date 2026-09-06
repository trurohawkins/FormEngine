#pragma once
#include "FormEngine.h"

typedef struct {
	int id;
	char *file;
	void (*func)(void);
} Level;

extern linkedList *levels;

Level *makeLevel(char *file, void (*func)(void));
Level *findLevel(int id);
bool loadLevel(Level *lvl);
void loadNextLevel();
void endLevel();
void freeLevel(void *level);
