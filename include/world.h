#pragma once
#include "form.h"
#include "cell.h"
typedef struct {
	int x;
	int y;
	Cell *map;
} World;

extern World theWorld;

void makeWorld(int x, int y);
World *getWorld();
void freeWorld();
bool placeForm(Form *f, int x, int y);
bool removeForm(Form *f, int x, int y);
bool moveForm(Form *f, int xd, int y);
Cell *getCell(int x, int y);
Form *checkFormID(int x, int y, int id);
