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
	Action *action = makeAction(0, moveAction, mv);
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

void *renderGuy(void *data) {
	Form *guy = data;	
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = 0,
		.pos = {
			.x = worldXToScreenX(guy->pos[0]),
			.y = worldYToScreenY(guy->pos[1]),
		},
	};
	Color col = {
		.rgb = {
			255, 255, 255,
		},
	};
	memcpy(reco.data, &col, sizeof(Color));
	addRenderCommand(reco);
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

