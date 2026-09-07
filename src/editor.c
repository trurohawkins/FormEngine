#include "editor.h"

Editor *editor = 0;

Editor *makeEditor() {
	if (editor != 0) {
		return editor;
	}
	editor = calloc(1, sizeof(Editor));

	editor->on = false;
	Menu *toolBar = makeMenu(1, cookBook.ids, 15, 3);
	toolBar->pos[0] = 0.13;
	toolBar->pos[1] = 0.5;
	for (int i = 0; i < cookBook.ids; i++) {
		Button *butt = getButton(toolBar, 0, i);
		TextBox *tBox = getTextBox(butt->textBox);
		int len = strlen(cookBook.recipes[i].type);
		snprintf(tBox->string, len+6, "[%i] %s", i, cookBook.recipes[i].type);
	}
	editor->toolBar = toolBar;
	editor->curForm = 0;

	Menu *con = makeMenu(1, FORMS_PER_CELL, 25, 5);
	con->pos[0] = 0.87;
	con->pos[1] = 0.1;
	setMenuSpacing(con, 1, 5);
	editor->contextMenu = con;
	editor->curCheck = 0;


	Player *player = addPlayer(editor);
	player->ignorePause = true;
	addKeyControl(player, 'P', toggleEditMode);
	addKeyControl(player, 'W', cursorUp);
	addKeyControl(player, 'A', cursorLeft);
	addKeyControl(player, 'S', cursorDown);
	addKeyControl(player, 'D', cursorRight);

	addKeyControl(player, 'C', useTool);
	addKeyControl(player, 'F', switchRecipe);
	addKeyControl(player, 'R', switchRemove);
	addKeyControl(player, 'X', pullForm);
	addKeyControl(player, 'M', saveMap);
	addRenderFunction(renderEditor);//renderThis;

	return editor;
}

void renderEditor() {
	if (editor->on) {
		renderCursor(editor);
		renderContextMenu(editor);
		//addMenu(e->contextMenu);
		addMenu(editor->toolBar);
	}
}

void renderContextMenu(Editor *e) {
	RenderCommand reco = {
		.type = 1,
		.cmd = 2,
	};

	World *w = getWorld();
	Cell *cell = getCell(e->cursor.x, e->cursor.y);
	char content[CONTENT_SIZE];
	int size = 40;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		Button *butt = getButton(e->contextMenu, 0, i);
		reco.index = butt->textBox;
		Form *f = cell->within[i];
		char entry[40];
		int written = 0;
		if (f) {
			written = snprintf(entry, 40, "%p\n[%i] Type: %i",f, i, f->id);
		} else {
			written = snprintf(entry, 40, "[%i] -- ", i);
		}
		memset(reco.data, 0, RENDER_BUFFER_SIZE);
		memcpy(reco.data, entry, written);
		addRenderCommand(reco);
	}
	/*
		 memset(reco.data, 0, RENDER_BUFFER_SIZE);
		 if (written != 0) {
		 memcpy(reco.data, content, min(CONTENT_SIZE, written));
		 }

		 reco.cmd = 0;
		 reco.pos.x = screenX * 0.87;
		 reco.pos.y = screenY * 0.5;
		 addRenderCommand(reco);
		 */
	addMenu(e->contextMenu);
}

void renderCursor(Editor *e) {
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
}

void moveCursor(Editor *e, int direction) {
	if (e->on) {
		int *dir = getDir4(direction);
		int dest[2] = {e->cursor.x + dir[0], e->cursor.y + dir[1]};
		World *w = getWorld();
		if (dest[0] >= 0 && dest[1] >= 0 && dest[0] < w->x && dest[1] < w->y) {
			e->cursor.x = dest[0];
			e->cursor.y = dest[1];
			checkForForms(e);
			screenChanged(0, 0);
		}
	}
}

void setEditMode(Editor *e, bool on) {
	e->on = on;
	screenChanged(0, 0);
}

void toggleEditMode(void *e, float val) {
	if (val == 1) {
		Editor *editor = e;
		toggleGamePause();
		editor->on = !editor->on;
		screenChanged(0, 0);
		debugWrite("toggled on\n");
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
				if (e->curForm >= 0 && e->curForm < cookBook.ids) {
					Form *f = cookBook.recipes[e->curForm].spawn(e->cursor.x, e->cursor.y);
					if (f) {
						checkForForms(e);
					}
				}
			}
		}
	}
}

void switchRemove(void *editor, float val) {
	if (val == 1) {
		Editor *e = editor;
		if (e->on) {
			e->curCheck = (e->curCheck + 1) % FORMS_PER_CELL;	
			checkForForms(e);
			screenChanged(0,0);
		}
	}
}

void checkForForms(Editor *e) {
	Cell *c = getCell(e->cursor.x, e->cursor.y);
	bool gotOne = false;
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (c->within[e->curCheck]) {
			gotOne = true;
			break;
		}
		e->curCheck = (e->curCheck + 1) % FORMS_PER_CELL;
	}
	if (gotOne) {
		selectButton(e->contextMenu, 0, e->curCheck);
	} else {
		deselectButton(e->contextMenu);
		e->curCheck = -1;
	}
}

void switchRecipe(void *editor, float val) {
	if (val == 1) {
		Editor *e = editor;
		if (e->on) {
			e->curForm = (e->curForm + 1) % cookBook.ids;
			selectButton(e->toolBar, 0, e->curForm);
			screenChanged(0, 0);
		}
	}
}

void pullForm(void *editor, float val) {
	if (val == 1) {
		Editor *e = editor;
		if (e->on) {
			if (e->curCheck >= 0) {
				Cell *c = getCell(e->cursor.x, e->cursor.y);
				Form *f = removeIndexCell(c, e->curCheck);
				if (f->id >= 0 && f->id < cookBook.ids) {
					cookBook.recipes[f->id].delete(f);
					checkForForms(e);
					screenChanged(0, 0);
				}
			}
		}
	}
}

void saveMap(void *editor, float val) {
	if (val == 1) {
		Editor *e = editor;
		if (e->on) {
			writeWorld("level00.bin");
		}
	}
}

void freeEditor() {
	deleteMenu(editor->toolBar);
	deleteMenu(editor->contextMenu);
	free(editor);
}
