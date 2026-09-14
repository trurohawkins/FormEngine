#pragma once
#include "FormEngine.h"

typedef struct {
	char *file;
	void (*func)(void);
} Level;

extern Level *levels;

void initLevels(int num);
void setLevel(int level);
int makeLevel(char *file, void (*func)(void));
bool loadLevel(int lvl);
bool loadNextLevel();
bool reloadLevel();
void endLevel();
void freeLevels();
