#pragma once
#include "FormEngine.h"
typedef struct {
	int x;
	int y;
	Cell *map;
} World;


void makeWorld(int x, int y);
World *getWorld();
void freeWorld();
bool placeForm(Form *f, int x, int y);
Form *removeForm(Form *f, int x, int y);
bool checkCellFull(int x, int y);
bool moveForm(Form *f, int xd, int y);
Cell *getCell(int x, int y);
Form *checkFormID(int x, int y, int id);
