#include "GameCore.h"
#include "form.h"
#include "cell.h"
#include "world.h"
#include "view.h"

View curView = {
	.pos = {0, 0},
	.dim = {0, 0}
};

int renderStride[2] = {1, 1};
View *getView() {
	return &curView;
}

int worldXToScreenX(int wx) {
	return wx + screenX/(2 * renderStride[0]) - curView.dim[0]/2;
}

int worldYToScreenY(int wy) {
	return wy + screenY/(2 * renderStride[1]) - curView.dim[1]/2;
}

//used for ascii render
void setRenderStride(int x, int y) {
	renderStride[0] = x;
	setTapestryStride(renderStride[0]);
	renderStride[1] = y;
}

void setViewDimension(int x, int y) {
	curView.dim[0] = x;
	curView.dim[1] = y;
	setNewRender();
}

void setBackgroundColor(int r, int g, int b) {
	curView.col.rgb[0] = r;
	curView.col.rgb[1] = g;
	curView.col.rgb[2] = b;
}

void setViewPosition(int x, int y) {
	int oldX = curView.pos[0];
	int oldY = curView.pos[1];
	curView.pos[0] = clamp(x - curView.dim[0]/2, 0, theWorld.x - curView.dim[0]);
	curView.pos[1] = clamp(y - curView.dim[1]/2, 0, theWorld.y - curView.dim[1]);
	if (oldX != curView.pos[0] || oldY != curView.pos[1]) {
		setNewRender();
	}
}

void moveView(int xd, int yd) {
	setViewPosition(curView.pos[0] + curView.dim[0]/2  + xd, curView.pos[1] + curView.dim[1] / 2 + yd);
}

void renderView() {
	//set xp and adjust for stride and even or odd number frame
	// only necesarry with stride of 2 for ascii squares
	int xp = curView.pos[0] + screenX/2;
	if (curView.dim[0] % 2 == 1) {
		xp += renderStride[0] - 1;
	}
	RenderCommand bg = {
		.type = 4,
		.pos = {
			.x = xp,
			.y = worldYToScreenY(curView.pos[1] + curView.dim[1]/2),
		},
		.layer = 0,
	};
	int boxData[5] = {curView.dim[0]*renderStride[0], curView.dim[1], curView.col.rgb[0], curView.col.rgb[1], curView.col.rgb[2]};
	memcpy(bg.data, boxData, sizeof(int) * 5);
	addRenderCommand(bg);

}

void renderWorld() {
	if (!theWorld.map) {
		return;
	}
	static int visit = 0;
	visit++;
	for (int y = 0; y < curView.dim[1]; y++) {
		for (int x = 0; x < curView.dim[0]; x++) {
			int xp = x + curView.pos[0];
			int yp = y + curView.pos[1];
			if (xp >= 0 && yp >= 0 && xp < theWorld.x && yp < theWorld.y) {
				int w = yp * theWorld.x + xp;
				Cell c = theWorld.map[w];
				for (int i = 0; i < FORMS_PER_CELL; i++) {
					if (c.within[i]) {
						Nub *skin = findNub(c.within[i], 1);
						if (skin && skin->data) {
							RenderObject *rob = skin->data;
							if (rob->render && rob->lastRender < visit) {
								rob->lastRender = visit;
								rob->render(rob->data);
							}
						}
					}						
				}
			}
		}
	}
}

