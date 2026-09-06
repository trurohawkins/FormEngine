#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

#include "constants.h"
#include "move.c"
#include "guy.c"

int main(int argc, char **argv) {
	initCookBook(formIDs);
	//FormRecipe guy = {"Guy", makeGuy, freeGuy};
	cookBook.recipes[0] = (FormRecipe){
		"Guy", makeGuy, freeGuy
	};
	cookBook.recipes[1] = (FormRecipe){"Block", makeBlock, freeForm};
	cookBook.recipes[2] = (FormRecipe){"Goal", makeGoal, freeForm};
	Level *lvl0 = makeLevel("lvl00.bin", 0);
	makeLevel("lvl01.bin", 0);
	startWorld(true, true);
	addRenderFunction(renderEditor);//renderThis;

	int spawnPos[2] = {worldX/2, worldY/2};
	if (!loadLevel(lvl0)) {
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

	Editor *e = makeEditor();
	e->cursor.x = spawnPos[0];
	e->cursor.y = spawnPos[1];


	
	runWorld();
	//writeWorld("world.bin");

	freeEditor();
	freeCookBook();
	deleteList(&levels, freeLevel);

	endWorld();
	return 0;
}
