#include "gfx.h"
#include <graphx.h>

// Takes a list of strings terminated by (char *) NULL and displays them on the
// screen.
void g_list(const char *s[]) {
	gfx_SetTextFGColor(BLACK);
	for (int y = 0; *s != NULL; y += CHAR_HEIGHT, ++s) {
		gfx_PrintStringXY(*s, 5, y);
	}
}
