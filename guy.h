#pragma once 

Form *placeGuy(int x, int y);
Form *makeGuy();
void *renderGuy(void *data);
int guyAction(void *data, Action *a, float delta);
void freeGuy(void *form);
Form *placeBlock(int x, int y);
Form *makeBlock();
void *renderBlock(void *data);
Form *placeGoal(int x, int y);
Form *makeGoal();
void *renderGoal(void *data);
