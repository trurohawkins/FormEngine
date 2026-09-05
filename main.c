#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#include "constants.h"
#include "move.c"
#include "guy.c"
#include "editor.c"

int main(int argc, char **argv) {
	startWorld(true, true);

	makeWorld(worldX, worldY);
	setViewDimension(worldX, worldY);
	int spawnPos[2] = {worldX/2, worldY/2};
	setViewPosition(spawnPos[0], spawnPos[1]);
	setBackgroundColor(60, 50, 60);
	setRenderStride(2, 1);

	Editor *e = makeEditor();
	Form *guy = makeGuy();
	placeForm(guy, spawnPos[0], spawnPos[1]);

	Form *block = makeBlock();
	placeForm(block, 15, 10);
	runWorld();

	freeEditor(e);

	endWorld();
	return 0;
}
