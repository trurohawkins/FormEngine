#pragma once

typedef struct {
	int curMove[2];
} MoveVars;

MoveVars *makeMover(Form *f);
int moveAction(void *data, Action *a, float delta);
//gets movenub from form and sets it curMove on the axis to value
void setMove(Form *f, int axis, int val);
bool checkMove(Form *f, int destX, int destY);
void moveUp(void *g, float val);
void moveLeft(void *g, float val);
void moveDown(void *g, float val);
void moveRight(void *g, float val);


