#include "gfx.h"
#include <graphx.h>

int listcur;

/* Takes a list of strings terminated by (char *) NULL and displays them on the
 * screen. 
 */
void g_list(const char *s[], int x, int y) {
	gfx_SetTextFGColor(BLACK);
	for (int i = 0; s[i] != NULL; ++i) {
		gfx_PrintStringXY(s[i], x, y + i * CHAR_HEIGHT);
	}
}

/* An arrow is printed before the ith row stored in listcur */
void g_sel(int x, int y) {
	extern int listcur;
	gfx_SetTextFGColor(BLACK);
	gfx_PrintStringXY("->", x, y + listcur * CHAR_HEIGHT);
}

/* scales a sprite by 4 times without allocating anything else*/
void g_blit_sprite4x(const gfx_sprite_t *sprite, int startx, int starty) {
	char *sprite_c = (char *) sprite; // we're reading raw, yahh
	unsigned char w, h;
	w = sprite_c[0];
	h = sprite_c[1];
	int size = w*h+2;

	for (int i = 2; i < size; ++i) {
		int x = ((i-2)%w)<<2;
		int y = ((i-2)/w)<<2;
		gfx_SetColor(sprite_c[i]);
		gfx_FillRectangle(x+startx, y+starty, 4, 4);
	}
}