#include "WorldManager.h"

bool runGraphics = true;
bool runAudio = true;

pthread_t gameThread;
pthread_t outputThread;


bool startWorld(int graphics, int audio) {
	srand(time(NULL));
	initDirections();

	initCore();

	initGame();

	initTermInput();
	if (graphics > 0) {
		initScreen();
	}
	if (audio > 0) {
		initAudio();
	}
	addRenderFunction(formRender);
	runGraphics = graphics;
	runAudio = audio;
	pauseFunc = pauseSet;

	return true;
}

void runWorld() {
	gameLoop = &formLoop;
	resizeScreen = &screenChanged;
	gameThread = createThread(runGame, NULL, false);
	outputThread = 0;
	if (runGraphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}
	coreLoop();
}


void formLoop(float delta) {
	parseAudioEvents();
}

void formRender() {
	if (runGraphics) {
		renderView();
		renderWorld();
	}
}

void screenChanged(int x, int y) {
	if (runGraphics) {
		setNewRender();
	}
}

bool endWorld() {
	freeWorld();
	pthread_join(gameThread, NULL);
	if (runGraphics > 0) {
		pthread_join(outputThread, NULL);
	}
	
	exitCore();
	closeGame();
	exitTermInput();
	if (runGraphics > 0) {
		exitScreen();
	}
	if (runAudio > 0) {
		endAudio();
	}
	return true;
}

void pauseSet(bool value) {
	pauseAudioEvents(value);
}

void asciiRenderForm(Form *f, uint8_t r, uint8_t g, uint8_t b) {
	RenderCommand reco = {
		.type = 0,
		.index = -1,
		.layer = 0,
		.pos = {
			.x = worldXToScreenX(f->pos[0]),
			.y = worldYToScreenY(f->pos[1]),
		},
	};
	Color col = {
		.rgb = {r, g, b},
	};
	memcpy(reco.data, &col, sizeof(Color));
	addRenderCommand(reco);
}

