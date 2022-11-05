#include <tice.h>
#include <graphx.h>

#include "gfx.h"

/* Initializes the palette using the pre-generated array created by convimg.
 * The colors will be defined using an enum.
 */
#define palette_init() gfx_SetPalette(global_palette, \
				sizeof_global_palette, sprites_palette_offset);

int main(void) {
	gfx_Begin();
	gfx_SetDrawBuffer();
	palette_init();

	gfx_FillScreen(WHITE);
	gfx_SetColor(BLACK);
	gfx_FillRectangle(20, 20, 20, 20);

	gfx_SwapDraw();

	while (!os_GetCSC())
		;

	gfx_End();
}

