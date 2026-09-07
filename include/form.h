#ifndef FORM
#define FORM
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GameCore.h"

// standard Nub types
#define RENDERNUB 1
#define CONTROLNUB 2
	// 0 - no type
	// 1 - rendering nub
	// 2 - actor/player nub
typedef struct Nub {
	int type;
	void *data;
	// if true free data when deleting, default = false
	bool owned;
	struct Nub *nub;
} Nub;

typedef struct {
	int id;
	int pos[2];
	Nub *nub;
} Form;

typedef struct {
	Actor *actor;
	Player *player;
} Control;


Form *makeForm(int id);
Nub *growNub(Form *f);
Nub *findNub(Form *f, int type);

Nub *growRenderNub(Form *f, void *data, void *(*renderFunc)(void*));
// attaches nub that knows actor
Control *makeFormControl(Form *f);

void freeForm(void *form);
void freeNub(void *nub);

void printForm(Form *f);

#endif
