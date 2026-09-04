#pragma once
#include "world.h"
typedef struct {
	int pos[2];
	int dim[2];
	Color col;
} View;

View *getView();
void setRenderStride(int x, int y);
void setViewDimension(int x, int y);
void setViewPosition(int x, int y);
void setBackgroundColor(int r, int g, int b);
void moveView(int xd, int yd);
int worldXToScreenX(int wx);
int worldYToScreenY(int wy);
void renderView();
void renderWorld();

