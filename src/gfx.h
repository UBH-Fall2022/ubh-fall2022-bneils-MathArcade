#ifndef SRC_GFX_H
#define SRC_GFX_H

#include <graphx.h>
#include "sprites/gfx.h"

// In my experience, characters tend to be 8 pixels high.
#define CHAR_HEIGHT 8

enum Colors {
	TRANSPARENT = 0,
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE,
	GRAY1,
	GRAY2,
	G2048_2,
	G2048_4,
	G2048_8,
	G2048_16,
	G2048_32,
	G2048_64,
	G2048_128,
	G2048_256,
	G2048_512,
	G2048_1024,
	G2048_2048,
	G2048_4096,
};

void g_list(const char *s[], int x, int y);
void g_sel(int x, int y);
void g_blit_sprite4x(const gfx_sprite_t *sprite, int, int);
#endif // SRC_GFX_H
