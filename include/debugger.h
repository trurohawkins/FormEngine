#pragma once
#include "FormEngine.h"

typedef struct {
	bool on;
	int textBox;
	float pos[2];
	int size[2];
	int capacity;
	//temperary need to refactor render commands
	char *text;
} Debugger;

Debugger *makeDebugger(float uix, float uiy);
void setDebugger(bool state);
void debuggerText(char *text);
void renderDebugger();
