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

#define MAX_AUDIO_EVENTS 256
int addTimedEvent(void (*func)(void *), void *data, double frequency);
void pauseSet(bool value);
void clearTimedEvents();

void asciiRenderForm(Form *f, uint8_t r, uint8_t g, uint8_t b);

