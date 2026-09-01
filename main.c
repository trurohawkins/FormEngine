#include <FormEngine.h>
#include <TUI.h>
#include <AudioMan.h>

int main(int argc, char **argv) {
	startWorld(true, true);
	runWorld();
	endWorld();
	return 0;
}
