#pragma once
#include <FormEngine.h>
#define STATNUB 2
#define EMPTYSTAT -42069
#define ID_MAX 8
typedef struct {
	int id;
	float value;
} Stat;

typedef struct {
	Stat *stats;
	int num;
} StatBlock;

Nub *initStats(Form *f, int numStats);
bool addStat(Form *f, int id, float value);
float *getStat(Form *f, int stat);
bool setStat(Form *f, int stat, float value);
Stat *getStatBlock(Form *f);

Form *checkStat(int x, int y, int stat);
