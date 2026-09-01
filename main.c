#include "FormEngine.h"
#include "TUI.h"
#include "AudioMan.h"

int worldX = 40;
int worldY = 50;

typedef struct {
	Form *self;
	int moveX;
	int moveY;

	int speed;
	int speedCounter;
} moveVar;

int sound0;
int playerStamp = -1;

void moveGuy(void *move) {
	moveVar *mv = move;
	Form *f = mv->self;
	if (mv->moveX != 0 || mv->moveY != 0) {
		moveForm(f, mv->moveX, mv->moveY);
		setFramePosition(f->pos[0], f->pos[1]);
	}
}

int move(void *form, Action *act, float delta) {
	return 0;
	moveVar *mv = act->data;
	Form *f = form;
	if (mv->speedCounter >= mv->speed) {
		if (mv->moveX != 0 || mv->moveY != 0) {
			moveForm(f, mv->moveX, mv->moveY);
			setFramePosition(f->pos[0], f->pos[1]);
		}
		mv->speedCounter = 0;
	} else {
		mv->speedCounter++;
	}
	return 0;
}

void moveUp(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveY = 1;
		} else {
			//mv->moveY = 0;
		}
	}
}

void moveLeft(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveX = -1;
		} else {
			//mv->moveX = 0;
		}
	}
}

void moveDown(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveY = -1;
		} else {
			//mv->moveY = 0;
		}
	}
}

void moveRight(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveX = 1;
		} else {
			//mv->moveX = 0;
		}
	}
}

void moveFrameUp(void *actor, float val) {
	if (val == 1) {
		moveFrame(0, 1);
	}
}
void moveFrameLeft(void *actor, float val) {
	if (val == 1) {
		moveFrame(-1, 0);
	}
}
void moveFrameDown(void *actor, float val) {
	if (val == 1) {
		moveFrame(0, -1);
	}
}
void moveFrameRight(void *actor, float val) {
	if (val == 1) {
		moveFrame(1, 0);
	}
}


void *renderForms(void *data) {
	Form *f = data;
	RenderCommand reco = {
		.type = 0,
		. pos = {
			.x = worldXToScreenX(f->pos[0]),
			.y = worldYToScreenY(f->pos[1]),
		}
	};
	if (f->id == 0) {
		reco.index = playerStamp;
		Color col = {
			.rgb = {128, 128, 128},
		};
		memcpy(reco.data, &col, sizeof(Color));
	} else {
		reco.index = -1;
		Color col = {
			.rgb = {0, 0, 0},
		};
		
		if (f->id == 1) {
			col.rgb[1] = 64;
		} else if (f->id == 2) {
			col.rgb[2] = 64;
		}
		memcpy(reco.data, &col, sizeof(Color));
	}
	addRenderCommand(reco);
}


int main() {
	debugWrite("caca\n");
	startWorld(true, true);
	sound0 = processAudioFile("resources/a1.wav", false);
	playerStamp = createStamp("<", "3");
	//scheduleAudio(sound0, 1.0);
	
	makeWorld(worldX, worldY);
	setFrameDimension(20, 10);
	setFramePosition(worldX/2, worldY/2);

	Form *f = makeForm(0);
	Nub *ren = growRenderNub(f);
	RenderObject *rob = ren->data;
	rob->data = f;
	rob->render = renderForms;

	Actor *actor = makeFormActor(f);
	addActor(actor);
	moveVar mv = {
		.self = f,
		.moveX = 0,
		.moveY = 0,
		.speed = 10,
		.speedCounter = 0,
	};
	Action *m = makeAction(0, &move, &mv);
	addAction(actor, m);
	scheduleEvent(moveGuy, &mv, 3.0);
	Player *p = makePlayer(actor, 0, 0);
	addControl(p, "K0W", moveUp);
	addControl(p, "K0A", moveLeft);
	addControl(p, "K0S", moveDown);
	addControl(p, "K0D", moveRight);
	addControl(p, "K0I", moveFrameUp);
	addControl(p, "K0J", moveFrameLeft);
	addControl(p, "K0K", moveFrameDown);
	addControl(p, "K0L", moveFrameRight);
	addPlayer(p);

	placeForm(f, worldX/2, worldY/2);
	Form *checker0 = makeForm(1);
	ren = growRenderNub(checker0);
	rob = ren->data;
	rob->data = checker0;
	rob->render = renderForms;
	Form *checker1 = makeForm(2);
	ren = growRenderNub(checker1);
	rob = ren->data;
	rob->data = checker1;
	rob->render = renderForms;
	for (int x = 0; x < worldX; x++) {
		for (int y = 0; y < worldY; y++) {
			if (randPercent() > 0.5) {
				placeForm(checker0, x ,y);
			} else {
				placeForm(checker1, x, y);
			}
		}
	}
	//printForm(f);
	runWorld();
	endWorld();
	return 0;
}

