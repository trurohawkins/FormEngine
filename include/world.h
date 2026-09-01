#ifndef WORLD
#define WORLD

typedef struct {
	int x;
	int y;
	Cell *map;
} World;

typedef struct {
	int pos[2];
	int dim[2];
} Frame;

extern World theWorld;

void makeWorld(int x, int y);
World *getWorld();
Frame *getFrame();
void setRenderStride(int x, int y);
void setFrameDimension(int x, int y);
void setFramePosition(int x, int y);
void moveFrame(int xd, int yd);
void freeWorld();
bool placeForm(Form *f, int x, int y);
bool removeForm(Form *f, int x, int y);
bool moveForm(Form *f, int xd, int y);
Cell *getCell(int x, int y);
Form *checkFormID(int x, int y, int id);
int worldXToScreenX(int wx);
int worldYToScreenY(int wy);
void renderWorld();
#endif
