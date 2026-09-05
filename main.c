#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#include "constants.h"
#include "move.c"
#include "guy.c"
Form *(*spawnKit[FORMIDS])(void);
#include "cookbook.h"
#include "editor.c"

Editor *e = 0;

void renderThis() {
	formRender();
	if (e && e->on) {
		renderEditor(e);
	}
}

int main(int argc, char **argv) {
	spawnKit[GUY] = makeGuy;
	spawnKit[BLOCK] = makeBlock;

	startWorld(true, true);
	renderFunc = &renderThis;

	makeWorld(worldX, worldY);
	setViewDimension(worldX, worldY);
	int spawnPos[2] = {worldX/2, worldY/2};
	setViewPosition(spawnPos[0], spawnPos[1]);
	setBackgroundColor(60, 50, 60);
	setRenderStride(2, 1);

	e = makeEditor();
	e->cursor.x = spawnPos[0];
	e->cursor.y = spawnPos[1];
	Form *guy = makeGuy();
	placeForm(guy, spawnPos[0], spawnPos[1]);

	Form *block = makeBlock();
	placeForm(block, 15, 10);
	runWorld();

	freeEditor(e);

	endWorld();
	return 0;
}
