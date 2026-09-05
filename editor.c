#include "editor.h"

Editor *makeEditor() {
	Editor *editor = calloc(1, sizeof(Editor));
	
	editor->on = false;
	Form *cursor = makeForm(CURSOR);
	editor->cursor = cursor;

	editor->contextMenu = createTextBox(24, 40, "");

	Nub *ren = growRenderNub(cursor);
	RenderObject *rob = ren->data;
	rob->data = editor;
	rob->render = renderEditor;


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

void *renderEditor(void *data) {
	Editor *e = data;
	RenderCommand reco = {
		.type = 3,
		.layer = 0,
		.pos = {
			.x = worldXToScreenX(e->cursor->pos[0]),
			.y = worldYToScreenY(e->cursor->pos[1]),
		},
	};
	Tint tint = {
		.rgb = {100, 100, 0},
		.value = 40,
		.strength = 50,
	}; 
	memcpy(reco.data, &tint, sizeof(Tint));
	addRenderCommand(reco);
	
	reco.type = 1;
	reco.index = e->contextMenu;

	reco.cmd = 2;
	World *w = getWorld();
	Cell *cell = getCell(e->cursor->pos[0], e->cursor->pos[1]);
	int size = FORMS_PER_CELL*20;
	char content[size];
	int written = 0;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		Form *f = cell->within[i];
		if (f && f != e->cursor) {
			written += sprintf(content + written, "%p\n[%i] Type: %i\n",f, i, f->id);
		}
	}
	debugWrite(content);
	/*
	int len = strlen(content);
	char buff[100];
	sprintf(buff, "size: %i %i written and %i length\n", size, written, len);
	debugWrite(buff);
	*/
	memset(reco.data, 0, RENDER_BUFFER_SIZE);
	if (written != 0) {
		memcpy(reco.data, content, min(size, written));
	}
	addRenderCommand(reco);


	reco.cmd = 0;
	reco.pos.x = screenX * 0.87;
	reco.pos.y = screenY * 0.5;;
	addRenderCommand(reco);
}

void moveCursor(Editor *e, int direction) {
	if (e->on) {
		int *dir = getDir4(direction);
		int dest[2] = {e->cursor->pos[0] + dir[0], e->cursor->pos[1] + dir[1]};
		World *w = getWorld();
		if (dest[0] >= 0 && dest[1] >= 0 && dest[0] < w->x && dest[1] < w->y) {
			removeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
			placeForm(e->cursor, dest[0], dest[1]);
		}
	}
}

void setEditMode(Editor *e, bool on) {
	e->on = on;
	if (on) {
		placeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
	} else {
		removeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
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
		freeForm(e->cursor);
	}
	free(e);
}
