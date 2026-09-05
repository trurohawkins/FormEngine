#pragma once

#define CONTENT_SIZE FORMS_PER_CELL * 40
#define TOOL_SIZE FORMIDS * 10
typedef struct {
	bool on;
	Pos cursor;

	Menu *contextMenu;
	int curCheck;
	Menu *toolBar;
	int curForm;
} Editor;

Editor *makeEditor();
void *renderEditor(void *data);
void renderCursor(Editor *e);
void renderContextMenu(Editor *e);
void renderToolBar(Editor *e);

void setEditMode(Editor *e, bool on);
void freeEditor(Editor *e);
void toggleEditMode(void *e, float val);
void cursorUp(void *e, float val);
void cursorLeft(void *e, float val);
void cursorDown(void *e, float val);
void cursorRight(void *e, float val);
void checkForForms(Editor *e);
void useTool(void *e, float val);
void switchRecipe(void *editor, float val);
void switchRemove(void *editor, float val);
void pullForm(void *editor, float val);

