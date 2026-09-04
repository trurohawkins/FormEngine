#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#include "constants.h"
#include "move.c"
#include "guy.c"

int main(int argc, char **argv) {
	startWorld(true, true);

	makeWorld(worldX, worldY);
	setViewDimension(worldX, worldY);
	int spawnPos[2] = {worldX/2, worldY/2};
	setViewPosition(spawnPos[0], spawnPos[1]);
	setBackgroundColor(60, 50, 60);
	setRenderStride(2, 1);

	Form *guy = makeGuy();
	placeForm(guy, spawnPos[0], spawnPos[1]);
	runWorld();
	endWorld();
	return 0;
}
