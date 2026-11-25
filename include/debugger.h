#pragma once
#include "FormEngine.h"

typedef struct {
	bool on;
	int textBox;
	float pos[2];
	//temperary need to refactor render commands
	char *text;
} Debugger;

Debugger *makeDebugger(float uix, float uiy);
void setDebugger(bool state);
void debuggerText(char *text);
void renderDebugger();
