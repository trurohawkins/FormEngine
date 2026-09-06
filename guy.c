#include "guy.h"
Form *makeGuy() {
	Form *guy = makeForm(GUY);

	Nub *ren = growRenderNub(guy);
	RenderObject *rob = ren->data;
	rob->data = guy;
	rob->render = renderGuy;

	MoveVars *mv = makeMover(guy);
	//mv->curMove[1] = 1;

	Actor *actor = makeFormActor(guy);
	Action *action = makeAction(0, guyAction, guy);
	addAction(actor, action);
	addActor(actor);

	Player *player = checkPlayer(1);
	if (player == 0) {
		player = makePlayer(guy, 1 ,0);
		addPlayer (player);
		addKeyControl(player, 'W', moveUp);
		addKeyControl(player, 'A', moveLeft);
		addKeyControl(player, 'S', moveDown);
		addKeyControl(player, 'D', moveRight);
	}

	return guy;
}

int guyAction(void *data, Action *a, float delta) {
	Form *guy = data;
	MoveVars *mv = getMove(guy);
	if (mv) {
		debugWrite("guy action\n");
		if (mv->curMove[0] != 0 || mv->curMove[1] != 0) {
			bool canMove = true;
			Form *m = data;
			int dest[2] = {m->pos[0] + mv->curMove[0], m->pos[1] + mv->curMove[1]};
			Cell *c = getCell(dest[0], dest[1]);
			if (c) {
				for (int i = 0; i < FORMS_PER_CELL; i++) {
					Form *f = c->within[i];
					if (f) {
						if (getMove(f)) {
							canMove = false;
						}
						if (f->id == GOAL) {
							//nextlevel
							loadNextLevel();
							return 0;
						}
					}
				}
			}
			if (canMove) {
				removeForm(guy, guy->pos[0], guy->pos[1]);
				placeForm(guy, dest[0], dest[1]);
			}
			memset(mv->curMove, 0, sizeof(int) * 2);
		}
	}
}

void *renderGuy(void *data) {
	asciiRenderForm(data, 40, 223, 184);
}

void freeGuy(void *form) {
	Player *p = checkPlayer(1);
	if (p) {
		removePlayer(p);
		freePlayer(p);
	}
	freeForm(form);
}

Form *makeBlock() {
	Form *block = makeForm(BLOCK);
	
	Nub *ren = growRenderNub(block);
	RenderObject *rob = ren->data;
	rob->data = block;
	rob->render = renderBlock;

	MoveVars *mv = makeMover(block);

	return block;
}

void *renderBlock(void *data) {
	asciiRenderForm(data, 180, 70, 40);
}

Form *makeGoal() {
	Form *goal = makeForm(GOAL);
	Nub *ren = growRenderNub(goal);
	RenderObject *rob = ren->data;
	rob->data = goal;
	rob->render = renderGoal;
	return goal;
}

void *renderGoal(void *data) {
	asciiRenderForm(data, 20, 255, 20);
}


