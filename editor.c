#include "editor.h"

Editor *makeEditor() {
	Editor *editor = calloc(1, sizeof(Editor));
	
	editor->on = false;
	/*
	Form *cursor = makeForm(CURSOR);
	editor->cursor = cursor;
	
	Nub *ren = growRenderNub(cursor);
	RenderObject *rob = ren->data;
	rob->data = editor;
	rob->render = renderEditor;
	*/
	
	editor->contextMenu = createTextBox(24, 40, "");


	Player *player = checkPlayer(0);
	if (player == 0) {
		player = makePlayer(editor, 0, 0);
		player->ignorePause = true;
		addPlayer (player);
		addKeyControl(player, 'P', toggleEditMode);
		addKeyControl(player, 'W', cursorUp);
		addKeyControl(player, 'A', cursorLeft);
		addKeyControl(player, 'S', cursorDown);
		addKeyControl(player, 'D', cursorRight);

		addKeyControl(player, ' ', useTool);
	}

	return editor;
}

void *renderEditor(void *data) {
	Editor *e = data;
	RenderCommand reco = {
		.type = 3,
		.layer = 0,
		.pos = {
			.x = worldXToScreenX(e->cursor.x),
			.y = worldYToScreenY(e->cursor.y),
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
	Cell *cell = getCell(e->cursor.x, e->cursor.y);
	char content[CONTENT_SIZE];
	int written = 0;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		Form *f = cell->within[i];
		if (f) {
	char buff[100];
	sprintf(buff, "written: %i vs size: %i\n", written, CONTENT_SIZE);
	debugWrite(buff);
			written += snprintf(content + written, CONTENT_SIZE-written, "%p\n[%i] Type: %i\n",f, i, f->id);
		}
	}

	memset(reco.data, 0, RENDER_BUFFER_SIZE);
	if (written != 0) {
		memcpy(reco.data, content, min(CONTENT_SIZE, written));
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
		int dest[2] = {e->cursor.x + dir[0], e->cursor.y + dir[1]};
		World *w = getWorld();
		if (dest[0] >= 0 && dest[1] >= 0 && dest[0] < w->x && dest[1] < w->y) {
			/*
			removeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
			placeForm(e->cursor, dest[0], dest[1]);
			*/
			e->cursor.x = dest[0];
			e->cursor.y = dest[1];
			screenChanged(0, 0);
		}
	}
}

void setEditMode(Editor *e, bool on) {
	e->on = on;
	screenChanged(0, 0);

	if (on) {
		//placeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
	} else {
		//removeForm(e->cursor, e->cursor->pos[0], e->cursor->pos[1]);
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

void useTool(void *editor, float val) {
	if (val == 1) {
		Editor *e = editor;
		if (e->on) {
			if (!checkCellFull(e->cursor.x, e->cursor.y)) {
				Form *f = spawnKit[1]();
				placeForm(f, e->cursor.x, e->cursor.y);
			}
		}
	}
}


void freeEditor(Editor *e) {
	if (!e->on) {
		//freeForm(e->cursor);
	}
	free(e);
}
