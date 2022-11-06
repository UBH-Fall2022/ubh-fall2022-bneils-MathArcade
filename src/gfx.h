#ifndef SRC_GFX_H
#define SRC_GFX_H

#include "sprites/gfx.h"

// In my experience, characters tend to be 8 pixels high.
#define CHAR_HEIGHT 8

enum Colors {
	TRANSPARENT = 0,
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE
};

void g_list(const char *s[], int x, int y);
void g_sel(int x, int y);

#endif // SRC_GFX_H
