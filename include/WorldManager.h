#pragma once
#include "GameCore.h"
#include "TUI.h"
#include "AudioMan.h"

#include "view.h"

bool startWorld(int graphics, int audio);
void runWorld();
void formLoop(float delta);
void formRender();
void screenChanged(int x, int y);
bool endWorld();

void pauseSet(bool value);

void asciiRenderForm(Form *f, uint8_t r, uint8_t g, uint8_t b);

