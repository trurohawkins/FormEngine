#include "world.h"

World theWorld = {
	.x = 0,
	.y = 0,
	.map = 0
};

void makeWorld(int x, int y) {
	theWorld.x = x;
	theWorld.y = y;
	theWorld.map = calloc(x * y, sizeof(Cell));
}

World *getWorld() {
	return &theWorld;
}

void freeWorld() {
	if (theWorld.map) {
		linkedList *forms = 0;
		for (int i = 0; i < theWorld.x * theWorld.y; i++) {
			Cell c = theWorld.map[i];
			for (int j = 0; j < FORMS_PER_CELL; j++) {
				if (c.within[j] != 0) {
					addToListSingle(&forms, c.within[j]);
				}
			}
		}
		if (forms) {
			deleteList(&forms, &freeForm);
		}
		free(theWorld.map);
		theWorld.map = 0;
	}
}

bool placeForm(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		Cell *c = &theWorld.map[(y*theWorld.x) + x];
		if (addToCell(f, c)) {
			f->pos[0] = x;
			f->pos[1] = y;
			setNewRender();
			return true;
		}
	}
	return false;
}

bool removeForm(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		Cell *c = &theWorld.map[(y*theWorld.x) + x];
		if (removeFromCell(f, c)) {
			setNewRender();
		}
	}
	return false;
}

bool moveForm(Form *f, int xd, int yd) {
	int xp = f->pos[0] + xd;
	int yp = f->pos[1] + yd;
	if (xp >= 0 && yp >=0 && xp < theWorld.x && yp < theWorld.y) {
		removeForm(f, f->pos[0], f->pos[1]);
		placeForm(f, xp, yp);
		return true;
	}
	return false;
}

Cell *getCell(int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		return &theWorld.map[(y * theWorld.x) + x];
	} else {
		return 0;
	}
}

Form *checkFormID(int x, int y, int id) {
	Cell *c = getCell(x, y);
	if (c) {
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i] && c->within[i]->id == id) {
				return c->within[i];
			}
		}
	}
	return NULL;
}

bool checkCellFull(int x, int y) {
	Cell *c = getCell(x, y);
	if (c) {
		return cellFull(c);
	}
	return true;
}

