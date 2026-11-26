#include "debugger.h"

Debugger *debug;

Debugger *makeDebugger(float uix, float uiy) {
	Debugger *db = calloc(1, sizeof(Debugger));
	db->on =  false;
	db->size[0] = 32;
	db->size[1] =  10;
	db->textBox = createTextBox(db->size[0], db->size[1], "debug");
	db->pos[0] = uix;
	db->pos[1] = uiy;
	addRenderFunction(renderDebugger);
	debug = db;
	debug->capacity = (db->size[0] - 4) * (db->size[1] - 4);
	debug->text = calloc(debug->capacity, sizeof(char));
	memcpy(debug->text, "debug\n", sizeof(char) * 6);
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
	int len = strlen(debug->text);
	printfFile("filling debug text: old len %i and new len %i\n", len, strlen(text));
	int lines = countLines(debug->text, debug->size[0]);
	int newLines = countLines(text, debug->size[0]);
	if (lines + newLines < debug->size[1] - 1) {
		int roomLeft = debug->capacity - len - 1;
		memcpy(debug->text + len, text, min(strlen(text), roomLeft));
		//printfFile(debug->text);
	} else {
		//memcpy(debug->text, text, min(strlen(text), debug->capacity));
	}
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
		//memcpy(reco.data, debug->text, min(strlen(debug->text), debug->capacity));
		printfFile("debug len: %i\n", strlen(debug->text));
		copyRenderData(&reco, debug->text, min(strlen(debug->text), debug->capacity));
		addRenderCommand(reco);
		reco.cmd = 0;
		addRenderCommand(reco);
	}
}

