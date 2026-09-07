#pragma once
#include "form.h"
#include "cell.h"
typedef struct {
	int x;
	int y;
	Cell *map;
} World;


void makeWorld(int x, int y);
World *getWorld();
void freeWorld();
bool placeForm(Form *f, int x, int y);
bool removeForm(Form *f, int x, int y);
bool checkCellFull(int x, int y);
bool moveForm(Form *f, int xd, int y);
Cell *getCell(int x, int y);
Form *checkFormID(int x, int y, int id);

typedef struct {
	char *type;
	Form *(*spawn)(int, int);
	void (*delete)(void*);
} FormRecipe;

typedef struct {
	int ids;
	FormRecipe *recipes;
} CookBook;
extern CookBook cookBook;

void initCookBook(int ids);
void freeCookBook();
void writeWorld(char *file);
bool loadWorld(char *file);
void destroyForm(void *f);
