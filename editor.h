#pragma once

#define CONTENT_SIZE FORMS_PER_CELL * 20
typedef struct {
	bool on;
	Form *cursor;

	int contextMenu;
} Editor;

Editor *makeEditor();
void *renderEditor(void *data);
void setEditMode(Editor *e, bool on);
void freeEditor(Editor *e);
void toggleEditMode(void *e, float val);
void cursorUp(void *e, float val);
void cursorLeft(void *e, float val);
void cursorDown(void *e, float val);
void cursorRight(void *e, float val);

