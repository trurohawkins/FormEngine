#include "GameCore.h"
#include "form.h"
#include "cell.h"
#include "world.h"

World theWorld = {
	.x = 0,
	.y = 0,
	.map = 0
};

Frame curFrame = {
	.pos = {0, 0},
	.dim = {0, 0}
};

int renderStride[2] = {1, 1};

void makeWorld(int x, int y) {
	theWorld.x = x;
	theWorld.y = y;
	theWorld.map = calloc(x * y, sizeof(Cell));
}

World *getWorld() {
	return &theWorld;
}

Frame *getFrame() {
	return &curFrame;
}

//used for ascii render
void setRenderStride(int x, int y) {
	renderStride[0] = x;
	setTapestryStride(renderStride[0]);
	renderStride[1] = y;
}

void setFrameDimension(int x, int y) {
	curFrame.dim[0] = x;
	curFrame.dim[1] = y;
	setNewRender();
}

void setFramePosition(int x, int y) {
	int oldX = curFrame.pos[0];
	int oldY = curFrame.pos[1];
	curFrame.pos[0] = clamp(x - curFrame.dim[0]/2, 0, theWorld.x - curFrame.dim[0]);
	curFrame.pos[1] = clamp(y - curFrame.dim[1]/2, 0, theWorld.y - curFrame.dim[1]);
	if (oldX != curFrame.pos[0] || oldY != curFrame.pos[1]) {
		setNewRender();
	}
}

void moveFrame(int xd, int yd) {
	setFramePosition(curFrame.pos[0] + curFrame.dim[0]/2  + xd, curFrame.pos[1] + curFrame.dim[1] / 2 + yd);
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

int worldXToScreenX(int wx) {
	return wx + screenX/(2 * renderStride[0]) - curFrame.dim[0]/2;
}

int worldYToScreenY(int wy) {
	wy = curFrame.dim[1] - wy;
	return wy + screenY/(2 * renderStride[1]) - curFrame.dim[1]/2;
}

void renderWorld() {
	if (!theWorld.map) {
		return;
	}
	static int visit = 0;
	visit++;
	for (int y = 0; y < curFrame.dim[1]; y++) {
		for (int x = 0; x < curFrame.dim[0]; x++) {
			int xp = x + curFrame.pos[0];
			int yp = y + curFrame.pos[1];
			int w = yp * theWorld.x + xp;
			Cell c = theWorld.map[w];
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				if (c.within[i]) {
					Nub *skin = findNub(c.within[i], 1);
					if (skin && skin->data) {
						RenderObject *rob = skin->data;
						if (rob->render && rob->lastRender < visit) {
							rob->lastRender = visit;
							rob->render(rob->data);
						}
					}
				}						
			}
		}
	}
}

