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
	asciiRenderForm(data, 40, 223, 184);
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

