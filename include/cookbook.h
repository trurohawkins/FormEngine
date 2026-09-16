#pragma once
#include "FormEngine.h"

typedef struct {
	char *type;
	Form *(*spawn)(int, int);
	Form *(*remove)(Form*,int, int);
	void (*delete)(void*);
} FormRecipe;

typedef struct {
	char *type;
	void (*inspect)(Nub*);
} NubInfo;

typedef struct {
	int ids;
	FormRecipe *recipes;
	NubInfo *nubs;
} CookBook;
extern CookBook cookBook;

void initCookBook(int ids);
void freeCookBook();
void writeWorld(char *file);
bool loadWorld(char *file);

void destroyForm(void *f);

