#pragma once

typedef struct {
	int curMove[2];
} MoveVars;

MoveVars *makeMover(Form *f);
int moveAction(void *data, Action *a, float delta);
//gets movenub from form and sets it curMove on the axis to value
void setMove(Form *f, int axis, int val);

