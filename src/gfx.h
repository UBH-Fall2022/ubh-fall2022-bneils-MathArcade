#ifndef SRC_GFX_H
#define SRC_GFX_H

#include "sprites/gfx.h"

// In my experience, characters tend to be 8 pixels high.
#define CHAR_HEIGHT 8

enum Colors {
	TRANSPARENT = 0,
	BLACK,
	WHITE,
};

void g_list(const char *s[]);

#endif // SRC_GFX_H
