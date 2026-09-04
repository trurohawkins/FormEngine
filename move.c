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
		World *w = getWorld();
		int dest[2] = {m->pos[0] + mv->curMove[0], m->pos[1] + mv->curMove[1]};
		if (dest[0] >= 0 && dest[1] >= 0 && dest[0] < w->x && dest[1] < w->y) {
			removeForm(m, m->pos[0], m->pos[1]);
			placeForm(m, dest[0], dest[1]);
		}
		mv->curMove[0] = 0;
		mv->curMove[1] = 0;
	}
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
