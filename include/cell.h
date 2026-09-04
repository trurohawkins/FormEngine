#ifndef CELL
#define CELL
#include <stdio.h>
#include "form.h"

#define FORMS_PER_CELL 8
typedef struct {
	Form *within[FORMS_PER_CELL];
} Cell;

bool addToCell(Form *form, Cell *cell);
bool removeFromCell(Form *form, Cell *cell);
void printCell(Cell c);

#endif
