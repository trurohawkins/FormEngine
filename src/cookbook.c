#include "cookbook.h"

CookBook cookBook = {
	.ids = 0,
	.recipes = 0,
	.nubs = 0,
};

void initCookBook(int ids) {
	cookBook.recipes = calloc(sizeof(FormRecipe), ids);
	cookBook.ids = ids;
}

void destroyForm(void *form) {
	Form *f = form;
	if (cookBook.recipes) {
		if (f->id < cookBook.ids) {
			cookBook.recipes[f->id].delete(f);
			return;
		}
	}
	freeForm(form);
}

void freeCookBook() {
	if (cookBook.recipes != 0) {
		free(cookBook.recipes);
		cookBook.recipes = 0;
	}
}


void writeWorld(char *file) {
	FILE *fptr = fopen(file, "wb");
	World *w = getWorld();
	int sizes[3] = {w->x, w->y, FORMS_PER_CELL};
	fwrite(sizes, sizeof(int), 3, fptr);
	int empty = -1;
	for (int i = 0; i < w->x * w->y; i++) {
		Cell c = w->map[i];
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c.within[i]) {
				Form *f = c.within[i];
				fwrite(&f->id, sizeof(int), 1, fptr);
			} else {
				fwrite(&empty, sizeof(int), 1, fptr);
			}
		}
	}
	fclose(fptr);
}

bool loadWorld(char *file) {
	debugWrite("loading world\n");
	FILE *fptr = fopen(file, "rb");
	if (fptr != NULL) {
		int sizes[3];// = readBinaryInt(fptr, 3);
		fread(sizes, sizeof(int), 3, fptr);
		if (sizes[2] > FORMS_PER_CELL) {
			debugWrite("Forms per cell mismatch");
			return false;
		}
		World *w = getWorld();
		if (w->map) {
			freeWorld();
		}
		makeWorld(sizes[0], sizes[1]);
		w = getWorld();
		for (int y = 0; y < w->y; y++) {
			for (int x = 0; x < w->x; x++) {
				//make block big enough for max Forms
				int idBlock[FORMS_PER_CELL];
				//only read the given amount
				fread(idBlock, sizeof(int), sizes[2], fptr);
				for (int i = 0; i < sizes[2]; i++) {
					if (idBlock[i] >= 0 && idBlock[i] < cookBook.ids) {
						FormRecipe r = cookBook.recipes[idBlock[i]];
						Form *f = cookBook.recipes[idBlock[i]].spawn(x, y);
					}
				}
			}
		}
		fclose(fptr);
		return true;
	}
	return false;
}

