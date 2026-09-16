#include "editor.h"

Editor *editor = 0;
void toggleEditMode(void *e, float val);
void cursorUp(void *e, float val);
void cursorLeft(void *e, float val);
void cursorDown(void *e, float val);
void cursorRight(void *e, float val);
void pressEditorSpawn(void *e, float val);
void pressSwitchRecipe(void *editor, float val);
void pressSwitchInspect(void *editor, float val);
void pressRemoveInspected(void *editor, float val);
void pressSaveMap(void *editor, float val);

Editor *makeEditor() {
	if (editor != 0) {
		return editor;
	}
	editor = calloc(1, sizeof(Editor));

	editor->on = false;
	Menu *toolBar = makeMenu(cookBook.ids, 1, 15, 3);
	toolBar->pos[0] = 0.5 - (cookBook.ids/2 * 0.1);
	toolBar->pos[1] = 0.9;
	for (int i = 0; i < cookBook.ids; i++) {
		Button *butt = getButton(toolBar, i, 0);
		TextBox *tBox = getTextBox(butt->textBox);
		int len = strlen(cookBook.recipes[i].type);
		char buff[len+7];
		snprintf(buff, len+6, "[%i] %s", i, cookBook.recipes[i].type);
		fillText(tBox->text, buff);
	}
	editor->toolBar = toolBar;
	editor->curForm = 0;

	Menu *con = makeMenu(1, FORMS_PER_CELL, 20, 4);
	con->pos[0] = 0.87;
	con->pos[1] = 0.1;
	setMenuSpacing(con, 1, 5);
	editor->contextMenu = con;
	editor->curCheck = 0;
	editor->inspector = createTextBox(24, 30, "");

	addRenderFunction(renderEditor);//renderThis;


	return editor;
}

void makeEditorPlayer(Editor *editor) {
	Player *player = addPlayer(editor);
	player->ignorePause = true;
	addKeyControl(player, 'P', toggleEditMode);
	addKeyControl(player, 'W', cursorUp);
	addKeyControl(player, 'A', cursorLeft);
	addKeyControl(player, 'S', cursorDown);
	addKeyControl(player, 'D', cursorRight);

	addKeyControl(player, 'C', pressEditorSpawn);
	addKeyControl(player, 'F', pressSwitchRecipe);
	addKeyControl(player, 'R', pressSwitchInspect);
	addKeyControl(player, 'X', pressRemoveInspected);
	addKeyControl(player, 'M', pressSaveMap);
}

void renderEditor() {
	if (editor->on) {
		renderCursor(editor);
		renderContextMenu(editor);
		renderInspector(editor);
		//addMenu(e->contextMenu);
		addMenu(editor->toolBar);
	}
}

void renderInspector(Editor *e) {
	RenderCommand reco = {
		.type = 1,
		.index = editor->inspector,
		.cmd = 2,
		.pos = {
			.x = screenX * 0.14,
			.y = screenY/2,
		},
	};
	if (e->curCheck >= 0 && e->curCheck < FORMS_PER_CELL) {
		Cell *c = getCell(e->cursor.x, e->cursor.y);
		if (c) {
			Form *f = c->within[e->curCheck];
			if (f) {
				int capacity = 23 * 29;
				char buff[capacity];
				int written = 0;
				if (f->id < cookBook.ids) {
					written += snprintf(buff, capacity - written, "%s\n", cookBook.recipes[f->id].type);
					Stat *stats = getStatBlock(f);
					if (stats) {
						int num = stats[0].id;
						for (int i = 1; i < num; i++) {
							written += snprintf(buff + written, capacity - written, "%i: %f\n", stats[i].id, stats[i].value);
						}
					}
					memcpy(reco.data, buff, written);
					addRenderCommand(reco);
				}
			}
		}
	}
	reco.cmd = 0;
	addRenderCommand(reco);
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
			written = snprintf(entry, 40, "[%i] Type: %i\n%p", i, f->id, f);
		} else {
			written = snprintf(entry, 40, "[%i] ------       ", i);
		}
		memset(reco.data, 0, RENDER_BUFFER_SIZE);
		memcpy(reco.data, entry, written);
		addRenderCommand(reco);
	}
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
	setGamePause(e->on);
	screenChanged(0, 0);
}

void toggleEditMode(void *e, float val) {
	if (val == 1) {
		Editor *editor = e;
		toggleGamePause();
		editor->on = !editor->on;
		screenChanged(0, 0);
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

void editorSpawnForm(Editor *e) {
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

void switchInspect(Editor *e) {
	if (e->on) {
		e->curCheck = (e->curCheck + 1) % FORMS_PER_CELL;	
		checkForForms(e);
		screenChanged(0,0);
	}
}

void pressEditorSpawn(void *editor, float val) {
	if (val == 1) {
		editorSpawnForm(editor);
	}
}

void pressSwitchInspect(void *editor, float val) {
	if (val == 1) {
		switchInspect(editor);
	}
}

void checkForForms(Editor *e) {
	Cell *c = getCell(e->cursor.x, e->cursor.y);
	bool gotOne = false;
	e->curCheck = clamp(e->curCheck, 0, FORMS_PER_CELL);
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

void switchRecipe(Editor *e) {
	if (e->on) {
		e->curForm = (e->curForm + 1) % cookBook.ids;
		selectButton(e->toolBar, e->curForm, 0);
		screenChanged(0, 0);
	}
}

void pressSwitchRecipe(void *editor, float val) {
	if (val == 1) {
		switchRecipe(editor);
	}
}

void removeInspected(Editor *e) {
	if (e->on) {
		if (e->curCheck >= 0) {
			Cell *c = getCell(e->cursor.x, e->cursor.y);
			Form *f = indexCell(c, e->curCheck);
			if (f && (f->id >= 0 && f->id < cookBook.ids)) {
				Form *unmake = cookBook.recipes[f->id].remove(f, e->cursor.x, e->cursor.y);
				if (unmake) {
					cookBook.recipes[f->id].delete(unmake);
				}
				checkForForms(e);
				screenChanged(0, 0);
			}
		}
	}
}

void pressRemoveInspected(void *editor, float val) {
	if (val == 1) {
		removeInspected(editor);
	}
}

void pressSaveMap(void *editor, float val) {
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
