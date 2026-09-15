#pragma once
#include <FormEngine.h>

#define CONTENT_SIZE FORMS_PER_CELL * 40
#define TOOL_SIZE FORMIDS * 10
typedef struct {
	bool on;
	Pos cursor;

	Menu *contextMenu;
	int curCheck;
	int inspector; //textbox
	Menu *toolBar;
	int curForm;
} Editor;

Editor *makeEditor();
void makeEditorPlayer(Editor *editor);
void renderEditor();
void renderCursor(Editor *e);
void renderContextMenu(Editor *e);
void renderToolBar(Editor *e);
void renderInspector(Editor *e);

void setEditMode(Editor *e, bool on);
void freeEditor();
void moveCursor(Editor *e, int direction);
void checkForForms(Editor *e);
void editorSpawnForm(Editor *e);
void switchInspect(Editor *e);
void switchRecipe(Editor *e);
void removeInspected(Editor *editor);

