#include "cell.h"

bool addToCell(Form *form, Cell *cell) {
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (cell->within[i] == 0) {
			cell->within[i] = form;
			return true;
		}
	}
	return false;
}

Form *removeFromCell(Form *form, Cell *cell) {
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (cell->within[i] == form) {
			cell->within[i] = 0;
			return form;
		}
	}
	return NULL;
}

Form *removeIndexCell(Cell *cell, int index) {
	Form *f = indexCell(cell, index);
	if (f) {
		cell->within[index] = 0;
	}
	return f;
}

Form *indexCell(Cell *cell, int index) {
	if (index >= 0 && index < FORMS_PER_CELL) {
		return cell->within[index];
	}
	return NULL;
}


bool cellFull(Cell *c) {
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (!c->within[i]) {
			return false;
		}
	}
	return true;
}

void printCell(Cell c) {
	printf("printing CELL %i\n", c);
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		printf("[%i] %i\n", i, c.within[i]);
	}
	printf("\n");
}
