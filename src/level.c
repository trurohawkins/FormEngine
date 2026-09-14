#include "level.h"

Level *levels = 0;
int numLevels = 0;
int maxLevels = 0;
int curLevel = -1;

void initLevels(int num) {
	if (levels == 0) {
		levels = calloc(num, sizeof(Level));
		for (int i = 0; i < num; i++) {
			levels[i].file = 0;
		}
		maxLevels = num;
	}
}

void setLevel(int level) {
	curLevel = level;
}

int makeLevel(char *file, void (*func)(void)) {
	if (numLevels >= maxLevels) {
		return -1;
	}
	Level *lvl = &levels[numLevels];//calloc(1, sizeof(Level));
	int fLen = strlen(file);
	if (fLen > 0) {
		lvl->file = calloc(1, fLen+1);
		memcpy(lvl->file, file, fLen+1);
	}
	lvl->func = func;
	numLevels++;
	return numLevels;
}

bool loadNextLevel() {
	if (curLevel + 1 < maxLevels) {
		if (levels[curLevel+1].file != 0) {
			endLevel();
			curLevel++;
			return loadLevel(curLevel);
		}
	}
	return false;
}

bool reloadLevel() {
	if (curLevel >= 0 && curLevel < maxLevels) {
		endLevel();
		return loadLevel(curLevel);
	}
	return false;
}

bool loadLevel(int lvl) {
	if (lvl < numLevels) {
		Level level = levels[lvl];
		if (loadWorld(level.file)) {
			if (level.func) {
				level.func();
			}
			return true;
		}
	}
	return false;
}

void endLevel() {
	clearTimedEvents();
	freeWorld();
	//deleteActorLists();
}

void freeLevels() {
	for (int i = 0; i < numLevels; i++) {
		if (levels[i].file != 0) {
			free(levels[i].file);
		}
	}
}
