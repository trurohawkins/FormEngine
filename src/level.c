#include "level.h"

linkedList *levels = 0;
int curLevel = -1;

void setLevel(int level) {
	curLevel = level;
}

Level *makeLevel(char *file, void (*func)(void)) {
	Level *lvl = calloc(1, sizeof(Level));
	int fLen = strlen(file);
	if (fLen > 0) {
		lvl->file = calloc(1, fLen+1);
		memcpy(lvl->file, file, fLen+1);
	}
	lvl->func = func;
	lvl->id = 0;
	for (linkedList *cur = levels; cur; cur = cur->next) {
		lvl->id++;
	}
	addToList(&levels, lvl);
	return lvl;
}

bool loadNextLevel() {
	Level *next = findLevel(curLevel+1);
	if (next) {
		endLevel();
		curLevel += 1;
		return loadLevel(next);
	}
	return false;
}

bool reloadLevel() {
	Level *cur = findLevel(curLevel);
	if (cur) {
		endLevel();
		return loadLevel(cur);
	}
	return false;
}

Level *findLevel(int id) {
	Level *lvl = 0;
	for (linkedList *cur = levels; cur; cur = cur->next) {
		Level *tmp = cur->data;
		if (tmp->id == id) {
			lvl = tmp;
			break;
		}
	}
	return lvl;
}

bool loadLevel(Level *lvl) {
	if (lvl) {
		if (loadWorld(lvl->file)) {
			if (lvl->func) {
				lvl->func();
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

void freeLevel(void *level) {
	Level *lvl = level;
	free(lvl->file);
	free(lvl);
}
