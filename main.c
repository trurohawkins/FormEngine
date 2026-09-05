#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#include "constants.h"
#include "move.c"
#include "guy.c"
FormRecipe CookBook[FORMIDS] = {
	{"Guy", makeGuy, freeGuy},
	{"Block", makeBlock, freeForm},
};


#include "editor.c"

Editor *e = 0;

void renderThis() {
	formRender();
	if (e && e->on) {
		renderEditor(e);
	}
}

int main(int argc, char **argv) {
	startWorld(true, true);
	renderFunc = &renderThis;

	int spawnPos[2] = {worldX/2, worldY/2};
	if (!loadWorld("world.bin")) {
		makeWorld(worldX, worldY);
		Form *guy = makeGuy();
		placeForm(guy, spawnPos[0], spawnPos[1]);

		Form *block = makeBlock();
		placeForm(block, 15, 10);
	}

	setViewDimension(worldX, worldY);
	setViewPosition(spawnPos[0], spawnPos[1]);
	setBackgroundColor(60, 50, 60);
	setRenderStride(2, 1);
	//loadWorld("world.bin");

	e = makeEditor();
	e->cursor.x = spawnPos[0];
	e->cursor.y = spawnPos[1];


	
	runWorld();
	writeWorld("world.bin");

	freeEditor(e);

	endWorld();
	return 0;
}
