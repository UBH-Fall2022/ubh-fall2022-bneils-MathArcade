#include <tice.h>
#include <graphx.h>

#include "gfx.h"

static inline void palette_init();

const char *list_items[] = {
	"sudoku",
	"sokoban",
	NULL
};

int main(void) {
	gfx_Begin();
	gfx_SetDrawBuffer();
	palette_init();

	gfx_FillScreen(WHITE);
	g_list(list_items);
	gfx_SwapDraw();

	while (!os_GetCSC())
		;

	gfx_End();
}

/* Initializes the palette using the pre-generated array created by convimg.
 * The colors will be defined using an enum.
 */
static inline void palette_init() {
	gfx_SetPalette(global_palette, sizeof_global_palette,
		sprites_palette_offset);
	gfx_SetTextTransparentColor(TRANSPARENT);
	gfx_SetTransparentColor(TRANSPARENT);
	gfx_SetTextBGColor(TRANSPARENT);
}
