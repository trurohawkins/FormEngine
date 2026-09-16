#include "stat.h"

Stat empty = {
	.id = EMPTYSTAT,
	.value = EMPTYSTAT,
};

Nub *initStats(Form *f, int numStats) {
	Nub *stat = growNub(f);
	stat->type = STATNUB;
	stat->owned = true;
	Stat *stats = calloc(numStats + 1, sizeof(Stat));
	stats[0].value = numStats + 1;
	for (int i = 1; i < stats[0].value; i++) {
		stats[i] = empty;
	}
	stat->data = stats;
	return stat;
}

bool isEmpty(Stat s) {
	return s.id == EMPTYSTAT && equal(s.value, EMPTYSTAT);
}

bool addStat(Form *f, int id, float value) {
	Nub *stat = findNub(f, STATNUB);
	if (stat) {
		Stat *stats = stat->data;
		for (int i = 1; i < stats[0].value; i++) {
			if (isEmpty(stats[i])) {
				stats[i].id = id;
				stats[i].value = value;
				return true;
			}
		}
	}
	return false;
}

float *getStat(Form *f, int id) {
	Nub *stat = findNub(f, STATNUB);
	if (stat) {
		Stat *stats = stat->data;
		for (int i = 1; i < stats[0].value; i++) {
			if (!isEmpty(stats[i])) {
				if (stats[i].id == id) {
					return &stats[i].value;
				}
			}
		}
	}
	return 0;
}

bool setStat(Form *f, int id, float value) {
	float *stat = getStat(f, id);
	if (stat) {
		*stat = value;
		return true;
	} else {
		return false;
	}
}

Stat *getStatBlock(Form *f) {
	Nub *stat = findNub(f, STATNUB);
	if (stat) {
		return stat->data;
	}
	return 0;
}

Form *checkStat(int x, int y, int id) {
	World *w = getWorld();
	if (x >= 0 && y >= 0 && x < w->x && y < w->y) {
		Cell *c = &w->map[(y * w->x) + x];
		for (int i = 0; i < FORMS_PER_CELL; i++) {
			if (c->within[i]) {
				float *stat = getStat(c->within[i], id);
				if (stat) {
					return c->within[i];
				}
			}
		}
	}
	return NULL;
}
