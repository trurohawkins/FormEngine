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
	int (*inspect)(Nub*, char*, int);
} NubInfo;

typedef struct {
	int ids;
	FormRecipe *recipes;
	int nubs;
	NubInfo *infos;
} CookBook;
extern CookBook cookBook;

//nubs should include the 3 basics nubs + whatever else you created
void initCookBook(int ids, int nubs);
void freeCookBook();
void writeWorld(char *file);
bool loadWorld(char *file);

void destroyForm(void *f);
int inspectNub(Nub *nub, char *buff, int capacity);
int inspectStats(Nub *stats, char *buff, int capacity);

