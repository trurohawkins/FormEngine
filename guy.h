#pragma once 

Form *makeGuy();
void *renderGuy(void *data);
int guyAction(void *data, Action *a, float delta);
void freeGuy(void *form);
Form *makeBlock();
void *renderBlock(void *data);
Form *makeGoal();
void *renderGoal(void *data);
