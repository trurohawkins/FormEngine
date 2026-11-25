#include "debugger.h"

Debugger *debug;

Debugger *makeDebugger(float uix, float uiy) {
	Debugger *db = calloc(1, sizeof(Debugger));
	db->on =  false;
	db->textBox = createTextBox(15, 5, "debug");
	db->pos[0] = uix;
	db->pos[1] = uiy;
	addRenderFunction(renderDebugger);
	debug = db;
	debug->text = calloc(100, sizeof(char));
	memcpy(debug->text, "debug", sizeof(char) * 6);
	return db;
}

void setDebugger(bool state) {
	debug->on = state;
	if (debug->on) {
		setDebugFunc(&debuggerText);
	} else {
		resetDebugFunc();
	}
}

void debuggerText(char *text) {
	memcpy(debug->text, text, min(strlen(text), 100));
}

void renderDebugger() {
	if (debug->on) {
		RenderCommand reco = {
			.type = 1,
			.cmd = 2,
			.index = debug->textBox,
			.layer = 50,
			.pos = {
				.x = screenX * debug->pos[0],
				.y = screenY * debug->pos[1],
			}
		};
		memcpy(reco.data, debug->text, min(strlen(debug->text), 100));
		addRenderCommand(reco);
		reco.cmd = 0;
		addRenderCommand(reco);
	}
}

