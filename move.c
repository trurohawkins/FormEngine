#include "move.h"

MoveVars *makeMover(Form *f) {
	Nub *moveNub = growNub(f);
	moveNub->type = MOVENUB;
	moveNub->owned = true;

	MoveVars *mv = calloc(1, sizeof(MoveVars));
	moveNub->data = mv;

	return moveNub->data;
}

int moveAction(void *data, Action *a, float delta) {
	MoveVars *mv = a->data;
	if (mv->curMove[0] != 0 || mv->curMove[1] != 0) {
		Form *m = data;
		int dest[2] = {m->pos[0] + mv->curMove[0], m->pos[1] + mv->curMove[1]};
		if (checkMove(m, dest[0], dest[1])) {
			removeForm(m, m->pos[0], m->pos[1]);
			placeForm(m, dest[0], dest[1]);
		}
		mv->curMove[0] = 0;
		mv->curMove[1] = 0;
	}
}

bool checkMove(Form *f, int destX, int destY) {
	World *w = getWorld();
	if (destX >= 0 && destY >= 0 && destX < w->x && destY < w->y) {
		Cell *c = &w->map[(destY * w->y) + destX];
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i]) {
				Nub *moveNub = findNub(f, MOVENUB);
				if (moveNub) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

//gets movenub from form and sets it curMove on the axis to value
void setMove(Form *f, int axis, int val) {
	if (f) {
		//get the move nub from form
		Nub *moveNub = findNub(f, MOVENUB);
		if (moveNub) {
			MoveVars *mv = moveNub->data;
			//get movevars and check if proper axis
			if (axis >= 0 && axis < 2) {
				//cur move locked into -1, 0, 1
				mv->curMove[axis] = sign(val);
			}
		}
	}
}

void moveUp(void *g, float val) {
	if (val == 1) {
		setMove(g, 1, 1);
	}
}

void moveLeft(void *g, float val) {
	if (val == 1) {
		setMove(g, 0, -1);
	}
}

void moveDown(void *g, float val) {
	if (val == 1) {
		setMove(g, 1, -1);
	}
}

void moveRight(void *g, float val) {
	if (val == 1) {
		setMove(g, 0, 1);
	}
}

