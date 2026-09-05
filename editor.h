#pragma once
typedef struct {
	Form *self;
	bool on;
} Editor;

Editor *makeEditor();
void *renderCursor(void *data);
void setEditMode(Editor *e, bool on);
void freeEditor(Editor *e);
void toggleEditMode(void *e, float val);
void cursorUp(void *e, float val);
void cursorLeft(void *e, float val);
void cursorDown(void *e, float val);
void cursorRight(void *e, float val);

