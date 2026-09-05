#include "editor.h"

Editor *makeEditor() {
	Form *cursor = makeForm(CURSOR);

	MoveVars *mv = makeMover(cursor);
	Nub *ren = growRenderNub(cursor);
	RenderObject *rob = ren->data;
	rob->data = cursor;
	rob->render = renderCursor;
	
	Actor *actor = makeFormActor(cursor);
	Action *action = makeAction(0, moveAction, mv);
	addAction(actor, action);
	addActor(actor);

	Editor *editor = calloc(1, sizeof(Editor));
	editor->self = cursor;
	editor->on = false;

	Player *player = checkPlayer(0);
	if (player == 0) {
		player = makePlayer(editor, 0, 0);
		player->ignorePause = true;
		addPlayer (player);
		addKeyControl(player, ' ', toggleEditMode);
		addKeyControl(player, 'W', cursorUp);
		addKeyControl(player, 'A', cursorLeft);
		addKeyControl(player, 'S', cursorDown);
		addKeyControl(player, 'D', cursorRight);
	}

	return editor;
}

void *renderCursor(void *data) {
	Form *c = data;
	RenderCommand reco = {
		.type = 3,
		.layer = 0,
		.pos = {
			.x = worldXToScreenX(c->pos[0]),
			.y = worldYToScreenY(c->pos[1]),
		},
	};
	Tint tint = {
		.rgb = {100, 100, 0},
		.value = 40,
		.strength = 50,
	}; 
	memcpy(reco.data, &tint, sizeof(Tint));
	addRenderCommand(reco);
}

void moveCursor(Editor *e, int direction) {
	if (e->on) {
		int *dir = getDir4(direction);
		int dest[2] = {e->self->pos[0] + dir[0], e->self->pos[1] + dir[1]};
		World *w = getWorld();
		if (dest[0] >= 0 && dest[1] >= 0 && dest[0] < w->x && dest[1] < w->y) {
			removeForm(e->self, e->self->pos[0], e->self->pos[1]);
			placeForm(e->self, dest[0], dest[1]);
		}
	}
}

void setEditMode(Editor *e, bool on) {
	e->on = on;
	if (on) {
		placeForm(e->self, e->self->pos[0], e->self->pos[1]);
	} else {
		removeForm(e->self, e->self->pos[0], e->self->pos[1]);
	}
}

void toggleEditMode(void *e, float val) {
	if (val == 1) {
		Editor *editor = e;
		toggleGamePause();
		setEditMode(editor, !editor->on);
	}
}

void cursorUp(void *e, float val) {
	if (val == 1) {
		moveCursor(e, 0);
	}
}

void cursorLeft(void *e, float val) {
	if (val == 1) {
		moveCursor(e, 1);
	}
}

void cursorDown(void *e, float val) {
	if (val == 1) {
		moveCursor(e, 2);
	}
}

void cursorRight(void *e, float val) {
	if (val == 1) {
		moveCursor(e, 3);
	}
}


void freeEditor(Editor *e) {
	if (!e->on) {
		freeForm(e->self);
	}
	free(e);
}
